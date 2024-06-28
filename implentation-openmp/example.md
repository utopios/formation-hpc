```c
#include <omp.h>
#include <stdio.h>

int main() {
    int n = 10;
    int local_var = 10
    #pragma omp parallel default(shared) private(local_var) for schedule(static,2)
    for (int i = 0; i < n; i++) {
        printf("Thread %d: i = %d\n", omp_get_thread_num(), i);
    }
    return 0;
}
```