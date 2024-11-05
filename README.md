# Cow Simulator with OpenMP

This is a project for CS-309.

## Checklist
- [ ] Make grass





## Useful Code Snips

#pragma omp parallel for num_threads(4)
for(int i = 0; i < 100; i++)
{
    long id = omp_get_thread_num();
    printf("\n%d from %ld", i, id);
}

## Things I learned...

- The -mwindows flag removes the console

## Websites Visited for this Project

(Not yet compiled)