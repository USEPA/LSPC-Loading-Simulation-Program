/*
 * Copyright (C) 2000 Pixia Corp., All Rights Reserved.
 * Rahul Thakkar
 */

/*
 *  Description:
 *      ETA calls for a process to know how much time is left in
 *      a loop.
 *
 *		ETA *eta = NULL;
 *			.
 *		your_stuff();
 *			.
 *			.
 *		eta = ETA_create();
 *		ETA_begin(eta, start_point, end_point);
 *		while (in_some_loop) {
 *			.
 *			.
 *			.
 *			ETA_update(eta, current_point);
 *			ms_elap = ETA_get_time_elapsed(eta, &hh_elap, &mm_elap, &ss_elap);
 *			ms_eta = ETA_get_time_remaining(eta, &hh_eta, &mm_eta, &ss_eta);
 *			.
 *			.
 *			.
 *		} 
 *		ETA_end(eta);
 *		ETA_destroy(eta);
 *
 *
 */

#ifndef ETA_HAS_BEEN_INCLUDED
#define ETA_HAS_BEEN_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include <time.h>

/* There is no gettimeofday function in Windows */
#if (defined(_WIN32) || defined(_WIN64))
	#include <sys/timeb.h>
	struct __timeval {
		long    tv_sec;         /* seconds */
		long    tv_usec;        /* and microseconds */
	};

	struct __timezone { /* pace holder */
		long    tz;
	};
#else
    #include <sys/time.h>
#endif

typedef struct ETA ETA;                                             
                                                                            
struct ETA {                                                            
	int				begin_flag;

    double 			task_begin;
    double			task_end;
    double			task_current;

    double			msec_begin;
    double			msec_current;

	struct __timeval	begin_tp;
	struct __timeval	current_tp;
};                                                                      

ETA	   *ETA_create(void);
void	ETA_destroy(ETA *eta);
void	ETA_begin(ETA *eta, double task_begin, double task_end);
void	ETA_end(ETA *eta);
double	ETA_update(ETA *eta, double task_current);
double	ETA_get_time_elapsed(ETA *eta, int *hh, int *mm, int *ss);
double	ETA_get_time_remaining(ETA *eta, int *hh, int *mm, int *ss);
/*
 * returns the seconds elapsed since the first time it was called 
 * in the application or since last reset
 */
void    ETA_time_reset(void); /* resets timer */
double  ETA_time(void); /* seconds elapsed since last reset or first call */
char   *ETA_time_format(double seconds, /* from seconds to hh:mm:ss */
			int *hh, int *mm, int *ss); /* returns "%02d:%02d:%02d" */

#ifdef __cplusplus
}
#endif

#endif
/*
 * eta.h ends
 */
