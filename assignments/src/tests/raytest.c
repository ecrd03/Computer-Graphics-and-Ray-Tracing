#include <stdio.h>
#include <string.h>
#include "CUnit/Basic.h"
#include "../ray/ray.h"
#include "../surfaces/sphere.h"

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

/*	​Scenario​: Creating and querying a ray
​ 	  ​Given​ origin ← point(1, 2, 3)
​ 	    ​And​ direction ← vector(4, 5, 6)
​ 	  ​When​ r ← ray(origin, direction)
​ 	  ​Then​ r.origin = origin
​ 	    ​And​ r.direction = direction  
*/
void testRayCreation(void)
{
    Tuple origin = point(1, 2, 3);
    Tuple direction = vector(4, 5, 6);

    Ray r = ray(origin, direction);

    CU_ASSERT_TRUE( is_point(origin) );
    CU_ASSERT( r.origin.x == origin.x ); 
    CU_ASSERT( r.origin.y == origin.y ); 
    CU_ASSERT( r.origin.z == origin.z );

    CU_ASSERT_TRUE( is_vector(direction) );
    CU_ASSERT( r.direction.x == direction.x ); 
    CU_ASSERT( r.direction.y == direction.y ); 
    CU_ASSERT( r.direction.z == direction.z );
}

/*
Scenario​: Computing a point from a distance
​ 	  ​Given​ r ← ray(point(2, 3, 4), vector(1, 0, 0))
​ 	  ​Then​ position(r, 0) = point(2, 3, 4)
​ 	    ​And​ position(r, 1) = point(3, 3, 4)
​ 	    ​And​ position(r, -1) = point(1, 3, 4)
​ 	    ​And​ position(r, 2.5) = point(4.5, 3, 4)	  
*/
void testComputePointFromDistance(void)
{
    Ray r = ray( point(2, 3, 4), vector(1, 0, 0) );

    Tuple t_zero = position(r, 0);
    CU_ASSERT_TRUE( is_point(t_zero) );
    CU_ASSERT_DOUBLE_EQUAL( t_zero.x, 2, EPSILON );
    CU_ASSERT_DOUBLE_EQUAL( t_zero.y, 3, EPSILON );
    CU_ASSERT_DOUBLE_EQUAL( t_zero.z, 4, EPSILON );

    Tuple t_one = position(r, 1);
    CU_ASSERT_TRUE( is_point(t_one) );
    CU_ASSERT_DOUBLE_EQUAL( t_one.x, 3, EPSILON );
    CU_ASSERT_DOUBLE_EQUAL( t_one.y, 3, EPSILON );
    CU_ASSERT_DOUBLE_EQUAL( t_one.z, 4, EPSILON );

    Tuple t_negone = position(r, -1);
    CU_ASSERT_TRUE( is_point(t_negone) );
    CU_ASSERT_DOUBLE_EQUAL( t_negone.x, 1, EPSILON );
    CU_ASSERT_DOUBLE_EQUAL( t_negone.y, 3, EPSILON );
    CU_ASSERT_DOUBLE_EQUAL( t_negone.z, 4, EPSILON );

    Tuple t_twofive = position(r, 2.5);
    CU_ASSERT_TRUE( is_point(t_twofive) );
    CU_ASSERT_DOUBLE_EQUAL( t_twofive.x, 4.5, EPSILON );
    CU_ASSERT_DOUBLE_EQUAL( t_twofive.y, 3, EPSILON );
    CU_ASSERT_DOUBLE_EQUAL( t_twofive.z, 4, EPSILON );
}

/*
	​Scenario​: A ray intersects a sphere at two points
​ 	  ​Given​ r ← ray(point(0, 0, -5), vector(0, 0, 1))
​ 	    ​And​ s ← sphere()
​ 	  ​When​ xs ← intersect(s, r)
​ 	  ​Then​ xs.count = 2
​ 	    ​And​ xs[0] = 4.0
​ 	    ​And​ xs[1] = 6.0	  
*/
void testRayIntersectSphereTwoPoints(void)
{
    Ray r = ray( point(0, 0, -5), vector(0, 0, 1) );
    Sphere s = sphere( point(0, 0, 0), 1 );
    Isect xs = intersect(s, r);

    CU_ASSERT( xs.count == 2 ); 
    CU_ASSERT_DOUBLE_EQUAL( xs.t_vals[0], 4.0, EPSILON ); 
    CU_ASSERT_DOUBLE_EQUAL( xs.t_vals[1], 6.0, EPSILON ); 
}

/*	
 	​Scenario​: A ray intersects a sphere at a tangent
​ 	  ​Given​ r ← ray(point(0, 1, -5), vector(0, 0, 1))
​ 	    ​And​ s ← sphere()
​ 	  ​When​ xs ← intersect(s, r)
​ 	  ​Then​ xs.count = 2
​ 	    ​And​ xs[0] = 5.0
​ 	    ​And​ xs[1] = 5.0  
*/
void testRayIntersectSphereTangent(void)
{
    Ray r = ray( point(0, 1, -5), vector(0, 0, 1) );
    Sphere s = sphere( point(0, 0, 0), 1 );
    
    Isect xs = intersect(s, r);

    CU_ASSERT( xs.count == 2 ); 
    CU_ASSERT_DOUBLE_EQUAL( xs.t_vals[0], 5.0, EPSILON ); 
    CU_ASSERT_DOUBLE_EQUAL( xs.t_vals[1], 5.0, EPSILON ); 
}

/*
	​Scenario​: A ray misses a sphere
​ 	  ​Given​ r ← ray(point(0, 2, -5), vector(0, 0, 1))
​ 	    ​And​ s ← sphere()
​ 	  ​When​ xs ← intersect(s, r)
​ 	  ​Then​ xs.count = 0	  
*/
void testRayMissesSphere(void)
{
    Ray r = ray( point(0, 2, -5), vector(0, 0, 1) );
    Sphere s = sphere( point(0, 0, 0), 1 );
    
    Isect xs = intersect(s, r);

    CU_ASSERT( xs.count == 0 ); 
}

/*	
	​Scenario​: A ray originates inside a sphere
​ 	  ​Given​ r ← ray(point(0, 0, 0), vector(0, 0, 1))
​ 	    ​And​ s ← sphere()
​ 	  ​When​ xs ← intersect(s, r)
​ 	  ​Then​ xs.count = 2
​ 	    ​And​ xs[0] = -1.0
​ 	    ​And​ xs[1] = 1.0  
*/
void testRayOriginatesInsideSphere(void)
{
    Ray r = ray( point(0, 0, 0), vector(0, 0, 1) );
    Sphere s = sphere( point(0, 0, 0), 1 );
    
    Isect xs = intersect(s, r);

    CU_ASSERT( xs.count == 2 ); 
    CU_ASSERT_DOUBLE_EQUAL( xs.t_vals[0], -1, EPSILON ); 
    CU_ASSERT_DOUBLE_EQUAL( xs.t_vals[1], 1, EPSILON );
}

/*	
Scenario​: A sphere is behind a ray
​ 	  ​Given​ r ← ray(point(0, 0, 5), vector(0, 0, 1))
​ 	    ​And​ s ← sphere()
​ 	  ​When​ xs ← intersect(s, r)
​ 	  ​Then​ xs.count = 2
​ 	    ​And​ xs[0] = -6.0
​ 	    ​And​ xs[1] = -4.0  
*/
void testSphereBehindRay(void)
{
    Ray r = ray( point(0, 0, 5), vector(0, 0, 1) );
    Sphere s = sphere( point(0, 0, 0), 1 );
    
    Isect xs = intersect(s, r);

    CU_ASSERT( xs.count == 2 ); 
    CU_ASSERT_DOUBLE_EQUAL( xs.t_vals[0], -6, EPSILON ); 
    CU_ASSERT_DOUBLE_EQUAL( xs.t_vals[1], -4, EPSILON );
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
    if (  NULL == CU_add_test(pSuite, "test of Ray Creation", testRayCreation)
       || NULL == CU_add_test(pSuite, "testComputePointFromDistance", testComputePointFromDistance)
       || NULL == CU_add_test(pSuite, "testRayIntersectSphereTwoPoints", testRayIntersectSphereTwoPoints)
       || NULL == CU_add_test(pSuite, "testRayIntersectSphereTangent", testRayIntersectSphereTangent)
       || NULL == CU_add_test(pSuite, "testRayMissesSphere", testRayMissesSphere)
       || NULL == CU_add_test(pSuite, "testRayOriginatesInsideSphere", testRayOriginatesInsideSphere)
       || NULL == CU_add_test(pSuite, "testSphereBehindRay", testSphereBehindRay)
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
