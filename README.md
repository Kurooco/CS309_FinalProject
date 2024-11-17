# Cow Simulator with OpenMP

This is a project for CS-309. For a full description, see the writeup submitted through Google Classroom

## Checklist
- [x] Make grass
- [x] Make cows
- [x] Parallelize
- [x] Cleanup





## Useful Code Snips

#pragma omp parallel for num_threads(4)
for(int i = 0; i < 100; i++)
{
    long id = omp_get_thread_num();
    printf("\n%d from %ld", i, id);
}

/*
            float minDist = FLT_MAX;
            sf::Vector2f grassPosition;
            int grassInd = 0;

            //int localMinDist = INT_MAX;
            //sf::Vector2f localGrassPosition;
            //int localGrassInd = 0;
            #pragma omp parallel shared(minDist, grassPosition, grassInd, grassVector)
            {
                float localMinDist = FLT_MAX;
                sf::Vector2f localGrassPosition;
                int localGrassInd = 0;

                int iter = grassVector->size();
                #pragma omp for
                for(int i = 0; i < iter; i++)
                {
                    //printf("\nI'm %d out of %d, and my iteration is %d", omp_get_thread_num(), omp_get_num_threads(), i);
                    int xDist = 0;
                    int yDist = 0;
                    // Satisfied cows pick over the less preferable grass
                    if(food > DESPERATION_THRESHOLD && (*grassVector)[i]->isDegraded()) continue; 

                    xDist = (*grassVector)[i]->getPosition().x - position.x;
                    yDist = (*grassVector)[i]->getPosition().y - position.y;
                    float distance = sqrt(xDist * xDist + yDist * yDist);
                    if(distance < localMinDist)
                    {
                        localMinDist = distance;
                        localGrassPosition = (*grassVector)[i]->getPosition();
                        localGrassInd = i;
                        //printf("\ngot my index %d, its %f", omp_get_thread_num(), localGrassInd);
                    }
                }
                //printf("\nim out %d, %d", omp_get_thread_num(), localGrassInd);

                #pragma omp critical
                if(localMinDist < minDist)
                {
                    //int diff = minDist;
                    //printf("\ngotta change %d to my min: %d. Prev: %d", minDist, localMinDist, diff);
                    minDist = localMinDist;
                    grassPosition = localGrassPosition;
                    grassInd = localGrassInd;
                }
            }*/

                /*#pragma omp parallel for
        for(int i = 0; i < grass_num; i++) grasses[i]->update();
        for(int i = 0; i < grass_num; i++)
        {
            if(grasses[i]->canReproduce())
            {
                Grass* newGrass = grasses[i]->reproduce();
                if(newGrass != nullptr) grasses.push_back(newGrass);
            }
        }*/

## Things I Learned/Relearned...

- The -mwindows flag removes the console
- add "public" before inherited class to make inherited methods available

## Websites Visited for this Project

(Not yet compiled)