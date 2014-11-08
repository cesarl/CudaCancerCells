# Multi-core programming homework 2
## César Leblic

### Langage version and libraries used

#### C++11

I used C++11 to write the program. This choice was motivated by mutliple reasons :
- new conding standard
- Lambda functions

#### FreeGlut

I used Freeglut instead of Glut :
- Glut is not maintained since a lot of time
- Glut do not permit to exit the game loop properly. In fact you cannot dealocate memory or destroy objects at the end of the program, it just autorize you to `exit()` the program.

#### Imgui

I used Imgui ( https://github.com/ocornut/imgui ) which permit you to display easily a GUI.

#### TBB

I used Intel TBB ( Intel Threading Building Blocks). That's a framework that lets you easily write parallel C++ programs that take full advantage of multicore performance.

### Architecture

#### Class descrition

Architecture is relatively simple, it count a small number of class :
- `Display` which is nothing more than a array of `char`. Pixel colors are writed in it and data are send to the GPU with `glDrawPixels` for the rendering.

- `GridBuffer` is one of the more important class of the program. It contain two buffers of `enum Celltype` (each `CellType` have the same size that an `unsigned char`), one for the reading, one for the writing. So that threads, when computing new state will read in the `read` buffer and save the solution in the `write` buffer. Then for the next step, buffer will be swapped so that `read` buffer will be used as `write` buffer and vice versa.
In addition `GridBuffer` class contain all methods and rules which permit to compute the new state. Threads will call them at execution time.

#### Workflow explaination

In past implementation, using classical threads, each thread treat a part of the grid (to limit memory fragmentation and cache missed, grid of cells is a one dimensionnal array and not a double). So, for example, if you create 4 worker thread, work was divided by 4.

I used TBB in the same way as before but the difference is that I let TBB divide the work between threads. So, I use a TBB `parallel_for` to divide the work in the array of cell. I pass to that function a grain size to indicate to TBB the optimal size of the chunk to process. This grain size can be dynamically modified at run time by the user to experiment what's the best task division.
We'll speak in more detail of that aspect further.

I also used a TBB `concurent_vector` to count the total number of cell of each type. In fact, each task will compute a small part of the array and will be capable of counting cells only into that range. That's why I used a concurent vector into which tasks can push their resuls. When all tasks are one we iterate through that vector and compute the total number of cells of each types.
We'll speak in more detail of that aspect further.


### Difficulties

I encounter some difficulties doing this project :

- The first on is a performance issue due to the counting of the cells. If we manage to count cells the loop time can be less than 34ms on a 4th generation Intel I7. In the old version, the better loop time was 7.9ms.
In the old version we used `std::future` to collect chunk counter result and them additionnate them. With TBB, we multiply the compute time by more than 4.

- The second problem is a global performance issue. In fact, if we deactivate the counting of the cell which is bottleneck we still do not have so much great performance improvements in comparaison with the old version (where the counter is active) :
    - (benchmark on an 8 core CPU)
    - Grain size 1/1 : TBB 18ms < Threads 21ms
    - Grain size 1/2 : 12ms < Threads 13ms
    - Grain size 1/4 : 8.5ms < Threads 9.2ms
    - Grain size 1/6 : 8.5ms > Threads 8.3ms
    - Grain size 1/8 : 8ms > Threads 7.9ms
We can clearly see in that comparaison that more the task division is optimized for the hardware, less the performances are in TBB version in comparaison with threads one.

### Conclusion

One of the biggest problem encountered is to count the different cell types. In fact I do not manage a way to compute the total without loosing a lot of performance (8.5ms without counting versus 34ms with the counting activated). So TBB is not so good to aggregate results of tasks.

Another conclusion we can deduce from that project is that, TBB does not bring so much performance to the program in comparaison with a well designed threaded version. In fact, TBB simplified a lot the programming (hundred of lines less than in thread version), in that way it's very handy, but it do not multiply performance so much.