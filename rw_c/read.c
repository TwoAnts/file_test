#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <error.h>
#include <errno.h>
#include "util.h"

#define MAX_TARGET_FILES 10
#define MAX_SIZES_NUM 1000
#define BUF_SIZE 1024

static char *fs = NULL;
static char *fo = NULL;
static int ft_num = 0;
static char *ft[MAX_TARGET_FILES];
static int rq_size_num = 0;
static int rq_size[MAX_SIZES_NUM]; //KB

void usage(){
    char *str = \
"Usage:\n\
    read <target file> as sizes in <sizes file>.\n\
    read -f <sizes file> [<target file>, ...]\n\
    read -o <result file>\n\
Options:\n\
    -f <sizes file> file of sizes.\n\
    -o <result file> file of result.\n\
    -h print this message.\n\
";
    printf("%s", str);
}

int args_parse(int argc, char* argv[])
{
    char opt;
    while((opt = getopt(argc, argv, "f:o:h")) != -1) 
    {
        switch(opt)
        {
            case 'f':
                fs = optarg;
                break;
            case 'o':
                fo = optarg;
                break;
            case 'h':
            default:
                return 0;
        }
    }

    while(optind < argc && ft_num < MAX_TARGET_FILES)
    {
        char c0 = argv[optind][0];
        if(c0 == '-' || c0 == '+') { optind++; continue;} 
        ft[ft_num++] = argv[optind++];
    }
    return 1;
}


static void * read_thread(void *arg)
{
    char *target = (char *)arg;
    int fd = open(target, O_RDONLY);
    if(!fd) {error(0, errno, "open %s failed!", target); return NULL;}
    int i, size;
    char *buffer = (char *)malloc(BUF_SIZE);
    ssize_t count;
    struct timespec ts1, ts2;
    struct timespec * ts_diff = (struct timespec *)malloc(sizeof(struct timespec));
    clock_gettime(CLOCK_REALTIME, &ts1);
    for(i = 0;i < rq_size_num;i++)
    {
        size = rq_size[i]; 
        printf("[read %5dKB] %s\n", size, target);
        for(;size > 0;size--)
        {
            count = read(fd, buffer, BUF_SIZE);
            if(count != BUF_SIZE) 
            {   
                error(0, errno, "read %s count %d", target, count);
                return NULL;
            }
        } 
    } 
    clock_gettime(CLOCK_REALTIME, &ts2);
    free(buffer);
    close(fd);
    
    timespec_diff(&ts1, &ts2, ts_diff);
    
    return ts_diff;
}




int main(int argc, char* argv[])
{
    if(!args_parse(argc, argv)) {usage(); return 0;}
    if(!fs || !fo || !ft_num){ usage(); return 0;}    
    int i, s;
    
    FILE *fsizes = fopen(fs, "r");
    int size = 0;
    while((fscanf(fsizes, "%d", &size) != -1) && (rq_size_num < MAX_SIZES_NUM)) 
    {
        rq_size[rq_size_num++] = size;
        
    } 
    fclose(fsizes);
    
    printf("create %d threads.\n", ft_num);

    pthread_t thrs[ft_num];
    for(i = 0;i < ft_num;i++)
    {
        s = pthread_create(&(thrs[i]), NULL, read_thread, (void *)ft[i]);
        if(s)
        {
            error(-1, s, "create thread failed for %s", ft[i]);
        }
    }
    
    FILE *fout = fopen(fo, "a+");
    if(!fout){
        error(-1, errno, "open %s failed!", fo);
    }

    struct timespec *ts;
    for(i = 0;i < ft_num;i++)
    {
        s = pthread_join(thrs[i], (void *)&ts);
        if(s)
        {
            error(-1, s, "join thread failed for %s", ft[i]); 
        }
        fprintf(fout, "%s %lld.%.9ld\n", ft[i], (long long)ts->tv_sec, ts->tv_nsec);
        free(ts);
        ts = NULL;
    }

    fclose(fout);

    return 0; 
}
