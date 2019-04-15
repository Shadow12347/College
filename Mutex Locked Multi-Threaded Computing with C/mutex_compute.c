#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct start_end
{
	int work_done;
	double num;	
	double max;
	double min;
	double total;
};

pthread_mutex_t mutt;
volatile int done = 0;
int first = 0;
double num = 0;
double max = -9999;
double min = 9999;
double total = 0;

void * thread_work(void * arg)
{
	struct start_end * se = (struct start_end *) malloc(sizeof(struct start_end));
	se = (struct start_end *) arg;
	first = 0;
	int res;
	double c;
	char t[64];
	while(done == 0)
	{
		pthread_mutex_lock(&mutt);
		if (fgets(t, 64, stdin) == NULL)
		{
			done = 1;
			pthread_mutex_unlock(&mutt);
			pthread_exit((void *)se);
		}
		sscanf(t, "%lg", &c);
		pthread_mutex_unlock(&mutt);
		if(first == 0)
		{
			se->min = c;
			se->max = c;
			first++;
			se->work_done = 1;
		}
		else
		{
			if(c < se->min)
				se->min = c;
			if(c > se->max)
				se->max = c;
		}
		se->num++;
		se->total += c;
		se->work_done = 1;
	}
	pthread_exit((void *)se);
}

int main(int argc, char ** args)
{	
	pthread_t new_threads[3] = {0};
	pthread_attr_t attr;
	int th;
	long t;
	double cur;
	char temp[64];
	int res;	
	struct start_end * se;
	se = malloc(3*sizeof(struct start_end));
	pthread_mutex_init(&mutt, NULL);

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	for (int i = 0; i < 3; i++)
	{
		(se+i)->num = 0;
		(se+i)->min = 9999;
		(se+i)->max = -9999;
		(se+i)->total = 0;
		(se+i)->work_done = 0;
		th = pthread_create(&new_threads[i], &attr, &thread_work, (void *) (se + i));
		if (th)
		{
			perror("pthread_create");
			exit(1);
		}
	}

	first = 0;

	while(done == 0)
	{
		pthread_mutex_lock(&mutt);
		if (fgets(temp, 64, stdin) == NULL)
		{
			done = 1;
			pthread_mutex_unlock(&mutt);
			break;
		}
		sscanf(temp, "%lg", &cur);
		pthread_mutex_unlock(&mutt);
		if(first == 0)
		{
			min = cur;
			max = cur;
			first++;
		}
		else
		{
			if(cur < min)
				min = cur;
			if(cur > max)
				max = cur;
		}
		num++;
		total += cur;
	}

	for (int i = 0; i < 3; i++)
	{
		th = pthread_join(new_threads[i], (void *) (se+i));
		if (th)
		{
			perror("pthread_join");
			exit(1);
		}
	}	
	
	for (int i = 0; i != 3; i++)
	{
		if ((se+i)->work_done)
		{
			if((se+i)->min < min)
				min = (se+i)->min;
			if((se+i)->max > max)
				max = (se+i)->max;
			num += (se+i)->num;
			total += (se+i)->total;
		}
	}

	pthread_attr_destroy(&attr);

	if(num == 0)
	{
		printf("No input was given!");
		exit(3);
	}
	pthread_mutex_destroy(&mutt);

	printf("max: %lg\nmin: %lg\naverage: %lg\n", max, min, total/num);
	fflush(stdout);
	return 0;
}
