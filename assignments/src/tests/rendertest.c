#include <stdio.h>
#include <string.h>

#include "../surfaces/sphere.h"
#include "../hits/hits.h"
#include "../canvas/canvas.h"

/* pseudo-code

canvas ← canvas(canvas_pixels, canvas_pixels)
​ 	​
​ 	shape1 ← sphere( point( 1, 0, 0 ), 0.75 );
   shape2 ← sphere( point( -0.5, 0, 0 ), 1.5 );
​ 	
​ 	​# for each row of pixels in the canvas​
​ 	​for​ y ← 0 to canvas_pixels - 1
​ 	
	 ​# compute the world y coordinate (top = +half, bottom = -half)​
	 world_y ← half - pixel_size * y
​ 	
​ 	  ​# for each pixel in the row​
​ 	  ​for​ x ← 0 to canvas_pixels - 1
​ 	
​ 	    ​# compute the world x coordinate (left = -half, right = half)​
​ 	    world_x ← -half + pixel_size * x
​ 	
​ 	    ​# describe the point on the wall that the ray will target​
​ 	    position ← point(world_x, world_y, wall_z)
​ 	
​ 	    r ← ray(ray_origin, normalize(position - ray_origin))
​ 	    isect ← intersect(shape, r)

       # need to use intersection() and intersections() to find xs
       ... MUST DETERMINE yourself how to do this ...
​ 	
       # render the sphere on the left red
       # and the sphere on the right yellow
       # NOTE - the pseudo-code is incomplete below
       # and does not include the logic for selecting which
       # sphere to color red or yellow

​ 	    ​if​ hit(xs) is defined
​ 	      write_pixel(canvas, x, y, color)
​ 	    ​end​ ​if​
​ 	
​ 	  ​end​ ​for​
​ 	
​ 	​end​ ​for
*/

void testRenderSimpleNonLightedScene(void)
{
   unsigned int canvas_pixels = 100; 

   Canvas can = canvas( canvas_pixels, canvas_pixels );
   Color col = color( 1, 0, 0) ;

   Sphere shape1 = sphere( point( 1, 0, 0 ), 0.75 );
   Sphere shape2 = sphere( point( -0.5, 0, 0 ), 1.5 );

   double wall_size = 7.0;
   double pixel_size = wall_size / canvas_pixels;
   double half = wall_size / 2.0;
   double wall_z = 10;

   Tuple ray_origin = point(0, 0, -7);

   /* for each row of pixels in the canvas */
   for(unsigned int y = 0; y <= canvas_pixels - 1; y++)
   {
        /* compute the world y coordinate (top = +half, bottom = -half) */
        double world_y = half - pixel_size * y;

        // for each pixel in the row​
        for( unsigned x = 0; x <= canvas_pixels - 1; x++ )
        {
            // compute the world x coordinate (left = -half, right = half)​ 
            double world_x = -half + pixel_size * x;

            // describe the point on the wall that the ray will target​
            Tuple position = point( world_x, world_y, wall_z );
        
            Tuple dir = normalizeVector( subTuples(position, ray_origin) );
            Ray r = ray( ray_origin, dir );
           
            Isect isect1 = intersect(shape1, r);
            Isect isect2 = intersect(shape2, r);

            IRec i1 = intersection(isect1.t_vals[0], shape1);
            IRec i2 = intersection(isect1.t_vals[1], shape1);

            IRec i3 = intersection(isect2.t_vals[0], shape2);
            IRec i4 = intersection(isect2.t_vals[1], shape2);

            IRecs xs = intersections(4, i1, i2, i3, i4);

            if ( hit(xs).t > 0 ) {

                if ( hit(xs).object.center.x == shape1.center.x ) {
                    col.red = 1; col.green = 1;
                } else {
                    col.red = 1; col.green = 0;
                }

                write_pixel(can, x, y, col);
            }
        }
   }

   char* ppm = canvas_to_ppm(can);

   FILE* ppm_file_ptr = fopen("./ppm/testRenderSimpleNonLightedScene.ppm", "w+");

   fprintf(ppm_file_ptr, "%s", ppm);
   fclose(ppm_file_ptr);

   free( can.pixels );
   free( ppm );
}

int main()
{
    testRenderSimpleNonLightedScene();
}
