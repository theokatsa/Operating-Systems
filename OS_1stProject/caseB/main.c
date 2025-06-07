// ΘΕΟΔΩΡΟΣ ΚΑΤΣΑΝΤΑΣ 1097459
// ΑΓΓΕΛΙΚΗ ΔΟΥΒΡΗ 1097441
// ΑΓΑΠΗ ΑΥΓΟΥΣΤΙΝΟΥ 1093327

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/sysinfo.h>
#include <math.h>
#include <semaphore.h>

size_t get_nproc() {
    struct sysinfo info;
    if (sysinfo(&info) != 0) {
        perror("sysinfo");
        exit(EXIT_FAILURE);
    }
    return (size_t)info.procs;
}

double get_wtime() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec * 1e-6;
}

unsigned short get_seed(int i, long tseed) {
    return (unsigned short)(i + tseed);
}

double f(double x) {
    return sin(cos(x));
}

int main(int argc, char *argv[]) {
    double a = 0.0;
    double b = 1.0;
    unsigned long n = 24e7; // default value
    const double h = (b - a) / n;
    const double ref = 0.73864299803689018;
    const long tseed = 10;
    double *partial_results;
    int i;
    int nproc;

    if (argc == 2) {
        n = atol(argv[1]) * 24e7; // use n * 24e7 if user provides n
    }

    nproc = get_nproc();

    sem_t *sem;
    sem = sem_open("/integral_mc_sem", O_CREAT | O_EXCL, 0666, 0);
    if (sem == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    // Initialize shared memory to zero
    int shmfd = shm_open("/integral_mc_shm", O_RDWR | O_CREAT, 0666);
    if (shmfd == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    // Set the size of shared memory to store partial results
    ftruncate(shmfd, nproc * sizeof(double));

    partial_results = mmap(NULL, nproc * sizeof(double), PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0);
    if (partial_results == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    // Close shared memory descriptor after mapping
    close(shmfd);

    // Fork processes based on the user-provided or default number of processes
    for (i = 0; i < nproc; i++) {
        if (fork() == 0) {
            // child process
            double xi;
            double res = 0.0;

            unsigned short my_seed = get_seed(i, tseed);
            srand48(my_seed);

            for (unsigned long j = i * (n / nproc); j < (i + 1) * (n / nproc); j++) {
                xi = drand48();
                res += f(xi);
            }

            sem_post(sem);  // Signal that the child process has finished its work
            sem_close(sem);

            partial_results[i] = res;
            exit(0);
        }
    }

    for (i = 0; i < nproc; i++) {
        sem_wait(sem);  // Wait for each child process to finish
    }

    sem_close(sem);
    sem_unlink("/integral_mc_sem");

    double total_result = 0.0;
    for (i = 0; i < nproc; i++) {
        total_result += partial_results[i];
    }

    total_result *= h;

    double error = fabs(total_result - ref);
    double rel_error = error / ref;

    printf("Result=%.16f Error=%e Rel.Error=%e Time=%lf seconds\n", total_result, error, rel_error, get_wtime());

    munmap(partial_results, nproc * sizeof(double));

    // Unlink shared memory object
    shm_unlink("/integral_mc_shm");

    return 0;
}
