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

## Things I Learned/Relearned...

- The -mwindows flag removes the console
- add "public" before inherited class to make inherited methods available

## Websites Visited for this Project

(Not yet compiled)