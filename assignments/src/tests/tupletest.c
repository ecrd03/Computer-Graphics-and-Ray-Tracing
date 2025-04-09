#include <stdio.h>
#include <string.h>
#include "CUnit/Basic.h"
#include "../tuple/tuple.h"

#define EPSILON 0.00001

/* Pointer to the file used by the tests. */
static FILE* temp_file = NULL;

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

/*  Scenario​: A tuple with w=1.0 is a point
*​ 	 ​Given​ a ← tuple(4.3, -4.2, 3.1, 1.0)
​* 	  ​Then​ a.x = 4.3
​* 	    ​And​ a.y = -4.2
​* 	    ​And​ a.z = 3.1
​* 	    ​And​ a.w = 1.0
​* 	    ​And​ a is a point
​* 	    ​And​ a is not a vector
*/
void testTuplePoint(void)
{
    Tuple a;
    a.x = 4.3;
    a.y = -4.2;
    a.z = 3.1;
    a.w = 1.0;

    CU_ASSERT( a.x == 4.3 );
    CU_ASSERT( a.y == -4.2 );
    CU_ASSERT( a.z == 3.1 );
    CU_ASSERT( a.w == 1.0 );

    CU_ASSERT_TRUE( is_point(a) );
    CU_ASSERT_FALSE( is_vector(a) );
}

/*  Scenario​: A tuple with w=0 is a vector
*​ 	 ​Given​ a ← tuple(4.3, -4.2, 3.1, 0)
​* 	  ​Then​ a.x = 4.3
​* 	    ​And​ a.y = -4.2
​* 	    ​And​ a.z = 3.1
​* 	    ​And​ a.w = 1.0
​* 	    ​And​ a is not a point
​* 	    ​And​ a is a vector
*/
void testTupleVector(void)
{
    Tuple a = tuple(4.3, -4.2, 3.1, 0);

    CU_ASSERT( a.x == 4.3 );
    CU_ASSERT( a.y == -4.2 );
    CU_ASSERT( a.z == 3.1 );
    CU_ASSERT( a.w == 0 );

    CU_ASSERT_FALSE( is_point(a) );
    CU_ASSERT_TRUE( is_vector(a) );
}

/*
	​Scenario​: point() creates tuples with w=1
​ 	  ​Given​ p ← point(4, -4, 3)
​ 	  ​Then​ p = tuple(4, -4, 3, 1)
*/
void testPointCreate(void) {
    Tuple a = point(4, -4, 3);

    CU_ASSERT( a.x == 4 );
    CU_ASSERT( a.y == -4 );
    CU_ASSERT( a.z == 3 );
    CU_ASSERT( a.w == 1 );

    // not strictly needed after testing in prior unit tests...
    CU_ASSERT_TRUE( is_point(a) );
    CU_ASSERT_FALSE( is_vector(a) );
}

/*
	​Scenario​: vector() creates tuples with w=0
​ 	  ​Given​ v ← vector(4, -4, 3)
​ 	  ​Then​ v = tuple(4, -4, 3, 0)
*/
void testVectorCreate(void) {
    Tuple a = vector(4, -4, 3);

    CU_ASSERT( a.x == 4 );
    CU_ASSERT( a.y == -4 );
    CU_ASSERT( a.z == 3 );
    CU_ASSERT( a.w == 0 );

    CU_ASSERT_FALSE( is_point(a) );
    CU_ASSERT_TRUE( is_vector(a) );
}

/*
	​Scenario​: Adding two tuples
​ 	  ​Given​ a1 ← tuple(3, -2, 5, 1)
​ 	    ​And​ a2 ← tuple(-2, 3, 1, 0)
​ 	   ​Then​ a1 + a2 = tuple(1, 1, 6, 1)
*/
void testAddTuples(void) {
    Tuple a1 = tuple(3, -2, 5, 1);
    Tuple a2 = tuple(-2, 3, 1, 0);

    Tuple result = addTuples(a1, a2);

    CU_ASSERT( result.x == 1 );
    CU_ASSERT( result.y == 1 );
    CU_ASSERT( result.z == 6 );
    CU_ASSERT( result.w == 1 );
}

/*
	​Scenario​: Subtracting two points
​ 	  ​Given​ p1 ← point(3, 2, 1)
​ 	    ​And​ p2 ← point(5, 6, 7)
​ 	  ​Then​ p1 - p2 = vector(-2, -4, -6)
*/
void testSubtractPoints(void) {
    Tuple p1 = point(3, 2, 1);
    Tuple p2 = point(5, 6, 7);

    Tuple result = subTuples(p1, p2);

    CU_ASSERT( result.x == -2 );
    CU_ASSERT( result.y == -4 );
    CU_ASSERT( result.z == -6 );

    // subtracting a point from a point is a vector
    CU_ASSERT_FALSE( is_point(result) );
    CU_ASSERT_TRUE( is_vector(result) );
}

/*
Scenario​: Subtracting a vector from a point
​ 	  ​Given​ p ← point(3, 2, 1)
​ 	    ​And​ v ← vector(5, 6, 7)
​ 	  ​Then​ p - v = point(-2, -4, -6)
*/
void testSubtractVectorFromPoint(void) {
    Tuple p = point(3, 2, 1);
    Tuple v = vector(5, 6, 7);

    Tuple result = subTuples(p, v);

    CU_ASSERT( result.x == -2 );
    CU_ASSERT( result.y == -4 );
    CU_ASSERT( result.z == -6 );

    // subtracting a vector from a point yields a point
    CU_ASSERT_TRUE( is_point(result) );
    CU_ASSERT_FALSE( is_vector(result) );
}

/*
Scenario​: Subtracting two vectors
​ 	  ​Given​ v1 ← vector(3, 2, 1)
​ 	    ​And​ v2 ← vector(5, 6, 7)
​ 	  ​Then​ v1 - v2 = vector(-2, -4, -6)
*/
void testSubtractTwoVectors(void) {
    Tuple v1 = vector(3, 2, 1);
    Tuple v2 = vector(5, 6, 7);

    Tuple result = subTuples(v1, v2);

    CU_ASSERT( result.x == -2 );
    CU_ASSERT( result.y == -4 );
    CU_ASSERT( result.z == -6 );

    // subtracting a vector from a vector yields a vector
    CU_ASSERT_FALSE( is_point(result) );
    CU_ASSERT_TRUE( is_vector(result) );
}

/*
Scenario​: Subtracting a vector from the zero vector
​ 	  ​Given​ zero ← vector(0, 0, 0)
​ 	    ​And​ v ← vector(1, -2, 3)
​ 	  ​Then​ zero - v = vector(-1, 2, -3)
*/
void testSubtractVectorFromZeroVector(void) {
    Tuple zero = vector(0, 0, 0);
    Tuple v = vector(1, -2, 3);

    Tuple result = subTuples(zero, v);

    CU_ASSERT( result.x == -1 );
    CU_ASSERT( result.y ==  2 );
    CU_ASSERT( result.z == -3 );

    // subtracting a vector from a vector yields a vector
    CU_ASSERT_FALSE( is_point(result) );
    CU_ASSERT_TRUE( is_vector(result) );
}

/*
Scenario​: Negating a tuple
​ 	  ​Given​ a ← tuple(1, -2, 3, -4)
​ 	  ​Then​ -a = tuple(-1, 2, -3, 4)
*/
void testNegateTuple(void) {
    Tuple a = tuple(1, -2, 3, -4);
    a = negateTuple(a);

    CU_ASSERT( a.x == -1 );
    CU_ASSERT( a.y ==  2 );
    CU_ASSERT( a.z == -3 );
    CU_ASSERT( a.w ==  4 );
}

/*
 	​Scenario​: Multiplying a tuple by a scalar
​ 	  ​Given​ a ← tuple(1, -2, 3, -4)
​ 	  ​Then​ a * 3.5 = tuple(3.5, -7, 10.5, -14)

*/
void testMultiplyTupleByScalar(void) {
    Tuple a = tuple(1, -2, 3, -4);
    a = scalarMultTuple(3.5, a);

    CU_ASSERT( a.x == 3.5 );
    CU_ASSERT( a.y == -7 );
    CU_ASSERT( a.z == 10.5 );
    CU_ASSERT( a.w == -14 );
}

/*
	​Scenario​: Multiplying a tuple by a fraction
​ 	  ​Given​ a ← tuple(1, -2, 3, -4)
​ 	  ​Then​ a * 0.5 = tuple(0.5, -1, 1.5, -2)
*/
void testMultiplyTupleByFraction(void) {
    Tuple a = tuple(1, -2, 3, -4);
    a = scalarMultTuple(0.5, a);

    CU_ASSERT( a.x == 0.5 );
    CU_ASSERT( a.y ==  -1 );
    CU_ASSERT( a.z == 1.5 );
    CU_ASSERT( a.w ==  -2 );
}

/*
	​Scenario​: Dividing a tuple by a scalar
​ 	  ​Given​ a ← tuple(1, -2, 3, -4)
​ 	  ​Then​ a / 2 = tuple(0.5, -1, 1.5, -2)
*/
void testDivideTupleByScalar(void) {
    Tuple a = tuple(1, -2, 3, -4);
    a = scalarDivideTuple(a, 2);

    CU_ASSERT( a.x == 0.5 );
    CU_ASSERT( a.y ==  -1 );
    CU_ASSERT( a.z == 1.5 );
    CU_ASSERT( a.w ==  -2 );
}

/*
	​Scenario​: Computing the magnitude of vector(1, 0, 0)
​ 	  ​Given​ v ← vector(1, 0, 0)
​ 	  ​Then​ magnitude(v) = 1
*/
void testMagnitudeOfVector1(void) {
    Tuple v = vector(1, 0, 0);
    CU_ASSERT_DOUBLE_EQUAL(magnitudeOfVector(v), 1, EPSILON);
}

/*
​ 	​Scenario​: Computing the magnitude of vector(0, 1, 0)
​ 	  ​Given​ v ← vector(0, 1, 0)
​ 	  ​Then​ magnitude(v) = 1
*/
void testMagnitudeOfVector2(void) {
    Tuple v = vector(0, 1, 0);
    CU_ASSERT_DOUBLE_EQUAL(magnitudeOfVector(v), 1, EPSILON);
}

/*​
​ 	​Scenario​: Computing the magnitude of vector(0, 0, 1)
​ 	  ​Given​ v ← vector(0, 0, 1)
​ 	  ​Then​ magnitude(v) = 1
​*/
void testMagnitudeOfVector3(void) {
    Tuple v = vector(0, 0, 1);
    CU_ASSERT_DOUBLE_EQUAL(magnitudeOfVector(v), 1, EPSILON);
}

/*
​ 	​Scenario​: Computing the magnitude of vector(1, 2, 3)
​ 	  ​Given​ v ← vector(1, 2, 3)
​ 	  ​Then​ magnitude(v) = √14
​*/
void testMagnitudeOfVector4(void) {
    Tuple v = vector(1, 2, 3);
    CU_ASSERT_DOUBLE_EQUAL(magnitudeOfVector(v), sqrt(14), EPSILON);
}

/*
​ 	​Scenario​: Computing the magnitude of vector(-1, -2, -3)
​ 	  ​Given​ v ← vector(-1, -2, -3)
​ 	  ​Then​ magnitude(v) = √14
*/
void testMagnitudeOfVector5(void) {
    Tuple v = vector(-1, -2, -3);
    CU_ASSERT_DOUBLE_EQUAL(magnitudeOfVector(v), sqrt(14), EPSILON);
}

/*
 	​Scenario​: Normalizing vector(4, 0, 0) gives (1, 0, 0)
​ 	  ​Given​ v ← vector(4, 0, 0)
​ 	  ​Then​ normalize(v) = vector(1, 0, 0)
*/
void testNormalizeVector1(void) {
    Tuple v = vector(4, 0, 0);
    Tuple expected = vector(1, 0, 0);

    v = normalizeVector( v );

    CU_ASSERT_DOUBLE_EQUAL(v.x, expected.x, EPSILON);
    CU_ASSERT_DOUBLE_EQUAL(v.y, expected.y, EPSILON);
    CU_ASSERT_DOUBLE_EQUAL(v.z, expected.z, EPSILON);
}

/*​ 
​ 	​Scenario​: Normalizing vector(1, 2, 3)
​ 	  ​Given​ v ← vector(1, 2, 3)
​ 	                                  ​# vector(1/√14,   2/√14,   3/√14)​
​ 	  ​Then​ normalize(v) = approximately vector(0.26726, 0.53452, 0.80178)
*/

void testNormalizeVector2(void) {
    Tuple v = vector(1, 2, 3);
    Tuple expected = vector(0.26726, 0.53452, 0.80178);

    v = normalizeVector( v );

    CU_ASSERT_DOUBLE_EQUAL(v.x, expected.x, EPSILON);
    CU_ASSERT_DOUBLE_EQUAL(v.y, expected.y, EPSILON);
    CU_ASSERT_DOUBLE_EQUAL(v.z, expected.z, EPSILON);
}

/*
​ 	​Scenario​: The magnitude of a normalized vector
​ 	  ​Given​ v ← vector(1, 2, 3)
​ 	  ​When​ norm ← normalize(v)
​ 	  ​Then​ magnitude(norm) = 1
*/
void testMagnitudeOfNormalizedVector(void) {
    Tuple v = vector(1, 2, 3);
    v = normalizeVector( v );
    CU_ASSERT_DOUBLE_EQUAL(magnitudeOfVector(v), 1, EPSILON);
}

/*
	​Scenario​: The dot product of two tuples
​ 	  ​Given​ a ← vector(1, 2, 3)
​ 	    ​And​ b ← vector(2, 3, 4)
​ 	  ​Then​ dot(a, b) = 20
*/
void testDotProduct(void) {
    Tuple a = vector(1, 2, 3);
    Tuple b = vector(2, 3, 4);
    CU_ASSERT_DOUBLE_EQUAL(dotProduct(a, b), 20, EPSILON);
}


#if 0  // NOT NEEDED FOR CSC 251 raycaster project
/*
Scenario​: The cross product of two vectors
​ 	  ​Given​ a ← vector(1, 2, 3)
​ 	    ​And​ b ← vector(2, 3, 4)
​ 	  ​Then​ cross(a, b) = vector(-1, 2, -1)
​ 	    ​And​ cross(b, a) = vector(1, -2, 1)
*/
void testCrossProduct(void) {
    Tuple a = vector(1, 2, 3);
    Tuple b = vector(2, 3, 4);

    Tuple cross_a_b = crossProduct( a, b );
    Tuple cross_b_a = crossProduct( b, a );

    Tuple expected_a_b = vector(-1, 2, -1);
    Tuple expected_b_a = vector(1, -2, 1);

    CU_ASSERT_DOUBLE_EQUAL(cross_a_b.x, expected_a_b.x, EPSILON);
    CU_ASSERT_DOUBLE_EQUAL(cross_a_b.y, expected_a_b.y, EPSILON);
    CU_ASSERT_DOUBLE_EQUAL(cross_a_b.z, expected_a_b.z, EPSILON);

    CU_ASSERT_DOUBLE_EQUAL(cross_b_a.x, expected_b_a.x, EPSILON);
    CU_ASSERT_DOUBLE_EQUAL(cross_b_a.y, expected_b_a.y, EPSILON);
    CU_ASSERT_DOUBLE_EQUAL(cross_b_a.z, expected_b_a.z, EPSILON);
}
#endif

/*
Scenario​: Colors are (red, green, blue) tuples
​ 	  ​Given​ c ← color(-0.5, 0.4, 1.7)
​ 	  ​Then​ c.red = -0.5
​ 	    ​And​ c.green = 0.4
​ 	    ​And​ c.blue = 1.7

*/
void testColors(void) {
    Color c = color(-0.5, 0.4, 1.7);

    CU_ASSERT_DOUBLE_EQUAL(c.red, -0.5, EPSILON);
    CU_ASSERT_DOUBLE_EQUAL(c.green, 0.4, EPSILON);
    CU_ASSERT_DOUBLE_EQUAL(c.blue, 1.7, EPSILON);
}

/*
	​Scenario​: Adding colors
​ 	  ​Given​ c1 ← color(0.9, 0.6, 0.75)
​ 	    ​And​ c2 ← color(0.7, 0.1, 0.25)
​ 	   ​Then​ c1 + c2 = color(1.6, 0.7, 1.0)
*/
void testAddingColors(void) {
    Color c1 = color(0.9, 0.6, 0.75);
    Color c2 = color(0.7, 0.1, 0.25);
    Color expected = color(1.6, 0.7, 1.0);

    Color result = addColors(c1, c2);

    CU_ASSERT_DOUBLE_EQUAL(result.red, expected.red, EPSILON);
    CU_ASSERT_DOUBLE_EQUAL(result.green, expected.green, EPSILON);
    CU_ASSERT_DOUBLE_EQUAL(result.blue, expected.blue, EPSILON);
}

/*​
​ 	​Scenario​: Subtracting colors
​ 	  ​Given​ c1 ← color(0.9, 0.6, 0.75)
​ 	    ​And​ c2 ← color(0.7, 0.1, 0.25)
​ 	   ​Then​ c1 - c2 = color(0.2, 0.5, 0.5)

*/
void testSubtractingColors(void) {
    Color c1 = color(0.9, 0.6, 0.75);
    Color c2 = color(0.7, 0.1, 0.25);
    Color expected = color(0.2, 0.5, 0.5);

    Color result = subColors(c1, c2);

    CU_ASSERT_DOUBLE_EQUAL(result.red, expected.red, EPSILON);
    CU_ASSERT_DOUBLE_EQUAL(result.green, expected.green, EPSILON);
    CU_ASSERT_DOUBLE_EQUAL(result.blue, expected.blue, EPSILON);
}
/*
​
​ 	​Scenario​: Multiplying a color by a scalar
​ 	  ​Given​ c ← color(0.2, 0.3, 0.4)
​ 	  ​Then​ c * 2 = color(0.4, 0.6, 0.8)

*/
void testMultColors1(void) {
    Color c = color(0.2, 0.3, 0.4);
    Color expected = color(0.4, 0.6, 0.8);

    Color result = scalarMultColor(2, c);

    CU_ASSERT_DOUBLE_EQUAL(result.red, expected.red, EPSILON);
    CU_ASSERT_DOUBLE_EQUAL(result.green, expected.green, EPSILON);
    CU_ASSERT_DOUBLE_EQUAL(result.blue, expected.blue, EPSILON);
}

/*
Scenario​: Multiplying colors
​ 	  ​Given​ c1 ← color(1, 0.2, 0.4)
​ 	    ​And​ c2 ← color(0.9, 1, 0.1)
​ 	   ​Then​ c1 * c2 = color(0.9, 0.2, 0.04)

*/
void testMultColors2(void) {
    Color c1 = color(1, 0.2, 0.4);
    Color c2 = color(0.9, 1, 0.1);
    Color expected = color(0.9, 0.2, 0.04);

    Color result = multColors(c1, c2);

    CU_ASSERT_DOUBLE_EQUAL(result.red, expected.red, EPSILON);
    CU_ASSERT_DOUBLE_EQUAL(result.green, expected.green, EPSILON);
    CU_ASSERT_DOUBLE_EQUAL(result.blue, expected.blue, EPSILON);
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
    if (  NULL == CU_add_test(pSuite, "test of testTuplePoint", testTuplePoint)
            || NULL == CU_add_test(pSuite, "test of testTupleVector", testTupleVector)
            || NULL == CU_add_test(pSuite, "test of testPointCreate", testPointCreate)
            || NULL == CU_add_test(pSuite, "test of testVectorCreate", testVectorCreate)
            || NULL == CU_add_test(pSuite, "test of testAddTuples", testAddTuples)
            || NULL == CU_add_test(pSuite, "test of testSubtractPoints", testSubtractPoints)
            || NULL == CU_add_test(pSuite, "test of testSubtractVectorFromPoint", testSubtractVectorFromPoint)
            || NULL == CU_add_test(pSuite, "test of testSubtractTwoVectors", testSubtractTwoVectors)
            || NULL == CU_add_test(pSuite, "test of testSubtractVectorFromZeroVector", testSubtractVectorFromZeroVector)
            || NULL == CU_add_test(pSuite, "test of testNegateTuple", testNegateTuple)
            || NULL == CU_add_test(pSuite, "test of testMultiplyTupleByScalar", testMultiplyTupleByScalar)
            || NULL == CU_add_test(pSuite, "test of testMultiplyTupleByFraction", testMultiplyTupleByFraction)
            || NULL == CU_add_test(pSuite, "test of testDivideTupleByScalar", testDivideTupleByScalar)
            || NULL == CU_add_test(pSuite, "test of testMagnitudeOfVector1", testMagnitudeOfVector1)
            || NULL == CU_add_test(pSuite, "test of testMagnitudeOfVector2", testMagnitudeOfVector2)
            || NULL == CU_add_test(pSuite, "test of testMagnitudeOfVector3", testMagnitudeOfVector3)
            || NULL == CU_add_test(pSuite, "test of testMagnitudeOfVector4", testMagnitudeOfVector4)
            || NULL == CU_add_test(pSuite, "test of testMagnitudeOfVector5", testMagnitudeOfVector5)
            || NULL == CU_add_test(pSuite, "test of testNormalizeVector1", testNormalizeVector1)
            || NULL == CU_add_test(pSuite, "test of testNormalizeVector2", testNormalizeVector2)
            || NULL == CU_add_test(pSuite, "test of testMagnitudeOfNormalizedVector", testMagnitudeOfNormalizedVector)
            || NULL == CU_add_test(pSuite, "test of testDotProduct", testDotProduct)
//            || NULL == CU_add_test(pSuite, "test of testCrossProduct", testCrossProduct)

            /* color tests*/
            || NULL == CU_add_test(pSuite, "test of testColors", testColors)
            || NULL == CU_add_test(pSuite, "test of testAddingColors", testAddingColors)
            || NULL == CU_add_test(pSuite, "test of testSubtractingColors", testSubtractingColors)
            || NULL == CU_add_test(pSuite, "test of testMultColors1", testMultColors1)
            || NULL == CU_add_test(pSuite, "test of testMultColors2", testMultColors2)
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
