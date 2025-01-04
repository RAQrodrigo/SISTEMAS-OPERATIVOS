#define CANTIDAD_DE_SEMAFOROS 10

struct semaphore {
  int value;
  struct spinlock semlock;
  int init_value;
};