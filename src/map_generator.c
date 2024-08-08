#include "map_generator.h"
#include "maths.h"

CVecInt* mapgen_generate_RANDOMWALKER(Grid *grid)
{
    CVecInt *visited;
    cvec_int_init(visited);
    int area = grid->width * grid->height;

    int start_idx = maths_randbetween_int(0, area); 
    int start_x, start_y;
    grid_i2c(grid, start_idx, &start_x, &start_y);
   
     
        
}
