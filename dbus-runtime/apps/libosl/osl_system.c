/*
 * osl_system.c
 *
 *  Created on: 13/11/2013
 *      Author: buttonfly
 */


#include <sys/types.h>
#include <sys/sysinfo.h>
#include <sys/statvfs.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

int osl_get_system_meminfo(unsigned long* free, unsigned long* total) {

	struct sysinfo info;
	memset(&info, 0, sizeof(struct sysinfo));

	sysinfo(&info);
	*free = info.freeram;
	*total = info.totalram;
	return 0;
}

int osl_system_processor_nr() {

//	/* Return number of configured processors.  */
//	#define get_nprocs_conf() (sysconf(_SC_NPROCESSORS_CONF))
//
//	/* Return number of available processors.  */
//	#define get_nprocs() (sysconf(_SC_NPROCESSORS_ONLN))

#if 0
	return get_nprocs();
#endif
	return 0;
}

int osl_get_system_diskinfo(const char* path, unsigned long* free, unsigned long* total) {

	struct statvfs st;

	int r = statvfs(path, &st);
	if(r < 0) {
		return -1;
	}

#if 0
	fprintf(stderr, "%d\n", st.f_bavail);
	fprintf(stderr, "%d\n", st.f_bfree);
	fprintf(stderr, "%d\n", st.f_blocks);
	fprintf(stderr, "%d\n", st.f_bsize);
	fprintf(stderr, "%d\n", st.f_favail);
	fprintf(stderr, "%d\n", st.f_ffree);
	fprintf(stderr, "%d\n", st.f_frsize);
#endif

#if 0
	*free = st.f_bfree * st.f_bsize;
	*total = st.f_blocks * st.f_bsize;
#else
	/* return in MiB */
	*free = st.f_bfree / 1024 * st.f_bsize / 1024;
	*total = st.f_blocks  / 1024 * st.f_bsize / 1024;
#endif

	return 0;
}

// TODO: It can be one or more.
pid_t osl_get_pid(const char* process) {
	char line[512];
	char cmd[512] = {0};
	sprintf(cmd, "pidof %s", process);

	FILE* f = popen(cmd, "r");
	if(!f) {
		return 0;
	}
	fgets(line, sizeof(line), f);
	pid_t pid = strtoul(line, NULL, 10);
	pclose(f);
	return pid;
}

int osl_kill(pid_t pid) {
	char cmd[512] = {0};
	sprintf(cmd, "kill %d", pid);
	return system(cmd);
}

int osl_load_average(double load[3]) {
	char *cmd = "/bin/cat /proc/loadavg";

	FILE *f = popen(cmd, "r");
	if(!f) {
		return -1;
	}

	char line[512] = {0};
	const char delimiters[] = " ";
	fgets(line, sizeof(line), f);
//	printf("%s\n", line);
	int i = 0;
	for(i = 0; i  < 3; ++i) {
		char *tmp = NULL;
		if(i == 0) {
			tmp = strtok(line, delimiters);
		}
		else {
			tmp = strtok(NULL, delimiters);
		}

		if(tmp)  {
//			printf("[%d] %s\n", i, tmp);
			load[i] = atof(tmp);
//			printf("[%d] %f\n", i, load[i]);
		}
	}
	pclose(f);
}
