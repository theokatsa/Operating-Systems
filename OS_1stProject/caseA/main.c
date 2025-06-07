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
#include <sys/time.h>
#include <math.h>

double get_wtime(){
    struct timeval t;
    gettimeofday(&t,NULL);
    return (double)t.tv_sec + (double)t.tv_usec * 1.0e-6;
}

double f(double x){
    return sin(cos(x));
}

size_t get_nproc() {
    struct sysinfo info;

    if(sysinfo(&info)!= 0){
        perror("sysinfo");
        exit(EXIT_FAILURE);
    }
    return (size_t)info.procs;
}

unsigned short get_seed(int i, long tseed){
    return (unsigned short)(i+tseed);
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <number_of_processes>\n", argv[0]);
    return EXIT_FAILURE;
  }

  double a = 0.0;
  double b = 1.0;
  unsigned long n = 24e7; // or e8
  const double h = (b - a) / n;
  const double ref = 0.73864299803689018;
  const long tseed = 10;
  double *partial_results;
  int shmfd;
  int i;
  int nproc = atoi(argv[1]); // Number of processes provided by the user

  // Initialize shared memory to zero
  shmfd = shm_open("/integral_mc_shm", O_RDWR | O_CREAT, 0666);
  if (shmfd == -1) {
    perror("shm_open");
    exit(EXIT_FAILURE);
  }

  // Set the size of shared memory to store partial results
  ftruncate(shmfd, nproc * sizeof(double));

  partial_results = mmap(NULL, nproc * sizeof(double), PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0);
  if (partial_results == MAP_FAILED){
        perror("mmap");
        exit(EXIT_FAILURE);
  }

  // Close shared memory descriptor after mapping
  close(shmfd);

  // Determine loop iterations for each process
  unsigned long iterations_per_process = n / nproc;

  // Fork processes
  for (i = 0; i < nproc; i++) {
    if (fork() == 0) {
      // child process
      double xi;
      double res = 0.0;

      unsigned short my_seed = get_seed(i, tseed);
      srand48(my_seed);

      for (unsigned long j = i * iterations_per_process; j < (i + 1) * iterations_per_process; j++) {
        xi = drand48();
        res += f(xi);
      }
      res += h;

      // Update shared memory with the partial result
      partial_results[i] = res;
      exit(0);
    }
  }

  // Wait for all child processes to finish
  for (i = 0; i < nproc; i++) {
    wait(NULL);
  }

  // Calculate total result using partial results from shared memory
  double total_result = 0.0;
  for (i = 0; i < nproc; i++) {
    total_result += partial_results[i];
  }

  total_result *= h;

  printf("Result=%.16f Error=%e Rel.Error=%e Time=%lf seconds\n", total_result, fabs(total_result-ref), fabs(total_result-ref)/ref, get_wtime());

  // Cleanup: Unmap shared memory and unlink shared memory object
  munmap(partial_results, nproc * sizeof(double));
  shm_unlink("/integral_mc_shm");

  return 0;
}
