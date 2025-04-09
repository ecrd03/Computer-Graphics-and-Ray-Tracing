#include <stdio.h>
#include <string.h>
#include "CUnit/Basic.h"
#include "../tuple/tuple.h"
#include "../canvas/canvas.h"

#define EPSILON 0.00001

/* The suite initialization function.
 * Opens the temporary file used by the tests.
 * Returns zero on success, non-zero otherwise.
 */
int init_suite1(void)
{
   return 0;
}

/* The suite cleanup function.
 * Closes the temporary file used by the tests.
 * Returns zero on success, non-zero otherwise.
 */
int clean_suite1(void)
{
    return 0;
}

/*
​Scenario​: Creating a canvas
​ 	  ​Given​ c ← canvas(10, 20)
​ 	  ​Then​ c.width = 10
​ 	    ​And​ c.height = 20
​ 	    ​And​ every pixel of c is color(0, 0, 0)

*/
void testCreatingCanvas(void) {
    Canvas c = canvas( 10, 20 );

    CU_ASSERT( c.width == 10 );
    CU_ASSERT( c.height == 20 );
    
    free((void *) c.pixels); // prob not needed for app though
}


/*
	​Scenario​: Writing pixels to a canvas
​ 	  ​Given​ c ← canvas(10, 20)
​ 	    ​And​ red ← color(1, 0, 0)
​ 	  ​When​ write_pixel(c, 2, 3, red)
​ 	  ​Then​ pixel_at(c, 2, 3) = red
*/
void testWritingPixelsToCanvas(void) {
    Canvas c = canvas( 10, 20 );
    Color red = color(1, 0, 0);

    write_pixel(c, 2, 3, red);
    Color out = pixel_at(c, 2, 3);

    CU_ASSERT(out.red == 255);
    CU_ASSERT(out.green == 0);
    CU_ASSERT(out.blue == 0);
    

    free((void *) c.pixels); // prob not needed for app though
}

/*
	​Scenario​: Constructing the PPM header
​ 	  ​Given​ c ← canvas(5, 3)
​ 	  ​When​ ppm ← canvas_to_ppm(c)
​ 	  ​Then​ lines 1-3 of ppm are
​ 	    ​"""​
​ 	​    P3​
​ 	​    5 3​
​ 	​    255​
​ 	​    """
*/
void testConstructingPPMHeader(void){
    Canvas c = canvas(5, 3);
    char* ppm = canvas_to_ppm(c);

    char* line1 = strtok(ppm, "\n");
    char* line2 = strtok(NULL, "\n");
    char* line3 = strtok(NULL, "\n");

    CU_ASSERT_STRING_EQUAL(line1, "P3");
    CU_ASSERT_STRING_EQUAL(line2, "5 3");
    CU_ASSERT_STRING_EQUAL(line3, "255");
    


    free((void *) c.pixels); // prob not needed for app though
    free(ppm);
}

/*
Scenario​: Constructing the PPM pixel data
​ 	  ​Given​ c ← canvas(5, 3)
​ 	    ​And​ c1 ← color(1.5, 0, 0)
​ 	    ​And​ c2 ← color(0, 0.5, 0)
​ 	    ​And​ c3 ← color(-0.5, 0, 1)
​ 	  ​When​ write_pixel(c, 0, 0, c1)
​ 	    ​And​ write_pixel(c, 2, 1, c2)
​ 	    ​And​ write_pixel(c, 4, 2, c3)
​ 	    ​And​ ppm ← canvas_to_ppm(c)
​ 	  ​Then​ lines 4-6 of ppm are
​ 	    ​"""​
​ 	​    255 0 0 0 0 0 0 0 0 0 0 0 0 0 0​
​ 	​    0 0 0 0 0 0 0 128 0 0 0 0 0 0 0​
​ 	​    0 0 0 0 0 0 0 0 0 0 0 0 0 0 255​
​ 	​    """

*/
void testConstructingPPMPixelData(void) {
    Canvas c = canvas(5, 3);

    Color c1 = color(1.5, 0, 0);
    Color c2 = color(0, 0.5, 0);
    Color c3 = color(-0.5, 0, 1);

    write_pixel(c, 0, 0, c1);
    write_pixel(c, 2, 1, c2);
    write_pixel(c, 4, 2, c3);

    char* ppm = canvas_to_ppm(c);
    char* lines[ 8 ]; lines[ 0 ] = strtok(ppm, "\n"); 

    for ( int idx = 1; idx < 3 + 5; idx++ )
    {
        lines[ idx ] = strtok(NULL, "\n");
    }

    CU_ASSERT_STRING_EQUAL(lines[ 3 ], 
       "255 0 0 0 0 0 0 0 0 0 0 0 0 0 0");
    CU_ASSERT_STRING_EQUAL(lines[ 4 ], 
       "0 0 0 0 0 0 0 128 0 0 0 0 0 0 0");
    CU_ASSERT_STRING_EQUAL(lines[ 5 ], 
       "0 0 0 0 0 0 0 0 0 0 0 0 0 0 255");
    
    

    free( c.pixels );
    free( ppm );
}

/*
Scenario​: Splitting long lines in PPM files
​ 	  ​Given​ c ← canvas(10, 2)
​ 	  ​When​ every pixel of c is set to color(1, 0.8, 0.6)
​ 	    ​And​ ppm ← canvas_to_ppm(c)
​ 	  ​Then​ lines 4-7 of ppm are
​ 	    ​"""​
​ 	​    255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204​
​ 	​    153 255 204 153 255 204 153 255 204 153 255 204 153​
​ 	​    255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204​
​ 	​    153 255 204 153 255 204 153 255 204 153 255 204 153​
​ 	​    """
*/
void testSplittingLongLinesInPPM(void) {
    Canvas c = canvas(10, 2);
    Color col = color(1, 0.8, 0.6);

    for ( unsigned y = 0; y < 2; y++ )
    {
        for ( unsigned x = 0; x < 10; x++ )
            write_pixel(c, x, y, col);
    }

    char* ppm = canvas_to_ppm(c);

    char* lines[ 8 ];  
    lines[ 0 ] = strtok(ppm, "\n"); 

    for ( int idx = 1; idx < 8; idx++ )
    {
        lines[ idx ] = strtok(NULL, "\n");
    }

    CU_ASSERT_STRING_EQUAL(lines[ 3 ], 
       "255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204");
    CU_ASSERT_STRING_EQUAL(lines[ 4 ], 
       "153 255 204 153 255 204 153 255 204 153 255 204 153");
    CU_ASSERT_STRING_EQUAL(lines[ 5 ], 
       "255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204");
    CU_ASSERT_STRING_EQUAL(lines[ 6 ], 
       "153 255 204 153 255 204 153 255 204 153 255 204 153");


    free( c.pixels );
    free( ppm );
}

/*
	​Scenario​: PPM files are terminated by a newline character
​ 	  ​Given​ c ← canvas(5, 3)
​ 	  ​When​ ppm ← canvas_to_ppm(c)
​ 	  ​Then​ ppm ends with a newline character
*/
void testPPMTerminatedByNewlineChar(void) {
    Canvas c = canvas(5, 3);
    char* ppm = canvas_to_ppm(c);
    unsigned int len = strlen(ppm);
    
    CU_ASSERT_EQUAL(ppm[len-1], '\n');

}


/* The main() function for setting up and running the tests.
 * Returns a CUE_SUCCESS on successful running, another
 * CUnit error code on failure.
 */
int main()
{
    CU_pSuite pSuite = NULL;

    /* initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    /* add a suite to the registry */
    pSuite = CU_add_suite("Suite_1", init_suite1, clean_suite1);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* add the tests to the suite */
    /* NOTE - ORDER IS IMPORTANT */
    if (  NULL == CU_add_test(pSuite, "test of testCreatingCanvas", testCreatingCanvas)
       || NULL == CU_add_test(pSuite, "test of testWritingPixelsToCanvas", testWritingPixelsToCanvas)
       || NULL == CU_add_test(pSuite, "test of testConstructingPPMHeader", testConstructingPPMHeader)
       || NULL == CU_add_test(pSuite, "test of testConstructingPPMPixelData", testConstructingPPMPixelData)
       || NULL == CU_add_test(pSuite, "test of testSplittingLongLinesInPPM", testSplittingLongLinesInPPM)
       || NULL == CU_add_test(pSuite, "test of testPPMTerminatedByNewlineChar", testPPMTerminatedByNewlineChar)
       )
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Run all tests using the CUnit Basic interface */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}