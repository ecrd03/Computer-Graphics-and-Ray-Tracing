#include <stdio.h>
#include <string.h>
#include "CUnit/Basic.h"
#include "../surfaces/sphere.h"
#include "../hits/hits.h"
#include "../ray/ray.h"

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
Scenario​: An intersection object encapsulates t and a sphere
​ 	  ​Given​ s ← sphere()
​ 	  ​When​ i ← intersection(3.5, s)
​ 	  ​Then​ i.t = 3.5
​ 	    ​And​ i.object = s  
*/
void testIntersectionObject(void)
{
    Sphere s = sphere( point(1, 1, 2), 10 );
    IRec i = intersection( 3.5, s );

    CU_ASSERT_DOUBLE_EQUAL( i.t, 3.5, EPSILON ); 
    CU_ASSERT_DOUBLE_EQUAL( i.object.center.x, 1, EPSILON );
    CU_ASSERT_DOUBLE_EQUAL( i.object.center.y, 1, EPSILON );
    CU_ASSERT_DOUBLE_EQUAL( i.object.center.z, 2, EPSILON );
    CU_ASSERT_DOUBLE_EQUAL( i.object.center.w, 1, EPSILON );
    CU_ASSERT_DOUBLE_EQUAL( i.object.radius, 10, EPSILON );
}

/*	
Scenario​: Aggregating intersection objects
​ 	  ​Given​ s ← sphere()
​ 	    ​And​ i1 ← intersection(1, s)
​ 	    ​And​ i2 ← intersection(2, s)
​ 	  ​When​ xs ← intersections(i1, i2)
​ 	  ​Then​ xs.count = 2
​ 	    ​And​ xs[0].t = 1
​ 	    ​And​ xs[1].t = 2  
*/
void testAggregatingIntersectionObjects(void)
{
    Sphere s = sphere( point(0, 0, 0), 1 );
    IRec i1 = intersection( 1, s );
    IRec i2 = intersection( 2, s );

    IRecs xs = intersections( 2, i1, i2 );

    CU_ASSERT( xs.count == 2 ); 
    CU_ASSERT_DOUBLE_EQUAL( xs.records[0].t, 1, EPSILON );
    CU_ASSERT_DOUBLE_EQUAL( xs.records[1].t, 2, EPSILON );

    free(xs.records);
}

/*
	​Scenario​: Intersect sets the object on the intersection
​ 	  ​Given​ r ← ray(point(0, 0, -5), vector(0, 0, 1))
​ 	    ​And​ s ← sphere()
​ 	  ​When​ xs ← intersect(s, r)
​ 	  ​Then​ xs.count = 2
​ 	    ​And​ xs[0].object = s
​ 	    ​And​ xs[1].object = s	  
*/
void testIntersectFunctionSetsSphereOnIntersectionObj(void)
{
   
    Ray r = ray(point(0, 0, -5), vector(0, 0, 1));
    Sphere s = sphere( point(0, 0, 0), 1 );
    
    Isect xs = intersect(s, r);

    CU_ASSERT( xs.count == 2 ); 
    CU_ASSERT_DOUBLE_EQUAL( xs.object.center.x, 0, EPSILON );
    CU_ASSERT_DOUBLE_EQUAL( xs.object.center.y, 0, EPSILON );
    CU_ASSERT_DOUBLE_EQUAL( xs.object.center.z, 0, EPSILON );
    CU_ASSERT_DOUBLE_EQUAL( xs.object.center.w, 1, EPSILON );
    CU_ASSERT_DOUBLE_EQUAL( xs.object.radius, 1, EPSILON );
}

/*	
Scenario​: The hit, when all intersections have positive t
​ 	  ​Given​ s ← sphere()
​ 	    ​And​ i1 ← intersection(1, s)
​ 	    ​And​ i2 ← intersection(2, s)
​ 	    ​And​ xs ← intersections(i2, i1)
​ 	  ​When​ i ← hit(xs)
​ 	  ​Then​ i = i1  
*/
void testWhichIntersectionIsHitAllPosT(void)
{
    Sphere s = sphere( point(0, 0, 0), 1 );
    IRec i1 = intersection( 1, s );
    IRec i2 = intersection( 2, s );

    IRecs xs = intersections( 2, i1, i2 );

    IRec i = hit(xs);

    CU_ASSERT_DOUBLE_EQUAL( i.t, i1.t, EPSILON );
    CU_ASSERT_DOUBLE_EQUAL( i.object.center.x,
                            i1.object.center.x,
                            EPSILON );
    CU_ASSERT_DOUBLE_EQUAL( i.object.center.y, 
                            i1.object.center.y,
                            EPSILON );
    CU_ASSERT_DOUBLE_EQUAL( i.object.center.z,
                            i1.object.center.z, 
                            EPSILON );
    CU_ASSERT_DOUBLE_EQUAL( i.object.center.w, 
                            i1.object.center.w, 
                            EPSILON );
    CU_ASSERT_DOUBLE_EQUAL( i.object.radius,
                            i1.object.radius,
                            EPSILON );

    free(xs.records);
}

/*
	​Scenario​: The hit, when some intersections have negative t
​ 	  ​Given​ s ← sphere()
​ 	    ​And​ i1 ← intersection(-1, s)
​ 	    ​And​ i2 ← intersection(1, s)
​ 	    ​And​ xs ← intersections(i2, i1)
​ 	  ​When​ i ← hit(xs)
​ 	  ​Then​ i = i2	  
*/
void testWhichIntersectinIsHitWithSomeNegT(void)
{
    Sphere s = sphere( point(1, 0, 1), 1 );
    IRec i1 = intersection( -1, s );
    IRec i2 = intersection( 1, s );

    IRecs xs = intersections( 2, i1, i2 );

    IRec i = hit(xs);

    CU_ASSERT_DOUBLE_EQUAL( i.t, i2.t, EPSILON );

    CU_ASSERT_DOUBLE_EQUAL( i.object.center.x,
                            i2.object.center.x,
                            EPSILON );
    CU_ASSERT_DOUBLE_EQUAL( i.object.center.y, 
                            i2.object.center.y,
                            EPSILON );
    CU_ASSERT_DOUBLE_EQUAL( i.object.center.z,
                            i2.object.center.z, 
                            EPSILON );
    CU_ASSERT_DOUBLE_EQUAL( i.object.center.w, 
                            i2.object.center.w, 
                            EPSILON );
    CU_ASSERT_DOUBLE_EQUAL( i.object.radius,
                            i2.object.radius,
                            EPSILON );

    free(xs.records);
}

/*
Scenario​: The hit, when all intersections have negative t
​ 	  ​Given​ s ← sphere()
​ 	    ​And​ i1 ← intersection(-2, s)
​ 	    ​And​ i2 ← intersection(-1, s)
​ 	    ​And​ xs ← intersections(i2, i1)
​ 	  ​When​ i ← hit(xs)
​ 	  ​Then​ i is nothing	​
*/
void testWhichIntersectinIsHitWithAllNegT(void)
{
    Sphere s = sphere( point(1, 0, 1), 1 );
    IRec i1 = intersection( -2, s );
    IRec i2 = intersection( -1, s );

    IRecs xs = intersections( 2, i1, i2 );

    IRec i = hit(xs);

    CU_ASSERT_DOUBLE_EQUAL( i.t, INVALID_T, EPSILON );

    free(xs.records);
}

/*
Scenario​: The hit is always the lowest nonnegative intersection
​ 	  ​Given​ s ← sphere()
​ 	  ​And​ i1 ← intersection(5, s)
​ 	  ​And​ i2 ← intersection(7, s)
​ 	  ​And​ i3 ← intersection(-3, s)
​ 	  ​And​ i4 ← intersection(2, s)
​ 	  ​And​ xs ← intersections(i1, i2, i3, i4)
​ 	​When​ i ← hit(xs)
​ 	​Then​ i = i4
	​
*/
void testWhichIntersectinIsHitTestLowestNonnegT(void)
{
    Sphere s = sphere( point(1, 0, 1), 1 );
    IRec i1 = intersection( 5, s );
    IRec i2 = intersection( 7, s );
    IRec i3 = intersection( -3, s );
    IRec i4 = intersection( 2, s );

    IRecs xs = intersections( 4, i1, i2, i3, i4 );

    IRec i = hit(xs);

    CU_ASSERT_DOUBLE_EQUAL( i.t, i4.t, EPSILON );

    CU_ASSERT_DOUBLE_EQUAL( i.object.center.x,
                            i4.object.center.x,
                            EPSILON );
    CU_ASSERT_DOUBLE_EQUAL( i.object.center.y, 
                            i4.object.center.y,
                            EPSILON );
    CU_ASSERT_DOUBLE_EQUAL( i.object.center.z,
                            i4.object.center.z, 
                            EPSILON );
    CU_ASSERT_DOUBLE_EQUAL( i.object.center.w, 
                            i4.object.center.w, 
                            EPSILON );
    CU_ASSERT_DOUBLE_EQUAL( i.object.radius,
                            i4.object.radius,
                            EPSILON );

    free(xs.records);
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
    if (  NULL == CU_add_test(pSuite, "testIntersectionObject", testIntersectionObject)
       || NULL == CU_add_test(pSuite, "testAggregatingIntersectionObjects", testAggregatingIntersectionObjects)
       || NULL == CU_add_test(pSuite, "testIntersectFunctionSetsSphereOnIntersectionObj", testIntersectFunctionSetsSphereOnIntersectionObj)
       || NULL == CU_add_test(pSuite, "testWhichIntersectionIsHitAllPosT", testWhichIntersectionIsHitAllPosT)
       || NULL == CU_add_test(pSuite, "testWhichIntersectinIsHitWithSomeNegT", testWhichIntersectinIsHitWithSomeNegT)
       || NULL == CU_add_test(pSuite, "testWhichIntersectinIsHitWithAllNegT", testWhichIntersectinIsHitWithAllNegT)
       || NULL == CU_add_test(pSuite, "testWhichIntersectinIsHitTestLowestNonnegT", testWhichIntersectinIsHitTestLowestNonnegT)
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
