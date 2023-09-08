#include "test.h"

#define Ok(message,value) printf("Ok: %s Value: %f\n", message, value)
#define Fail(message,value) printf("Fail: %s Value: %f\n", message, value); exit(666)

#define assert(x, message, value) if(x){Ok(#message,value);}else{Fail(#message,value);}

float tolerance = 1e-3; // Tolerance value for float comparison

void test_rotate_x(){
    printf("Start test_rotate_x:\n"); // "test_rotate_x:\n

    unsigned int test_count = 0;
    
    struct Triangle triangle;
    float angle = 0.0f;

    printf("Test %d with 90d: \n", ++test_count);
    {
        angle = 90.0f;

        set_vector(&triangle.v1, 0.0f, 1.0f, 0.0f);
        set_vector(&triangle.v2, -1.0f, -1.0f, 0.0f);
        set_vector(&triangle.v3, 1.0f, -1.0f, 0.0f);

        rotate_point_around_x(angle, &triangle.v1.y, &triangle.v1.z);
        rotate_point_around_x(angle, &triangle.v2.y, &triangle.v2.z);
        rotate_point_around_x(angle, &triangle.v3.y, &triangle.v3.z);

        /*
        <Vector (0.0000, 0.0000, 1.0000)>
        <Vector (-1.0000, -0.0000, -1.0000)>
        <Vector (1.0000, -0.0000, -1.0000)>
        */

        assert(fabs(triangle.v1.x) < tolerance, "triangle.v1.x == 0.0f", triangle.v1.x);
        assert(fabs(triangle.v1.y) < tolerance, "triangle.v1.y == 0.0f", triangle.v1.y);
        assert(triangle.v1.z == 1.0f, "triangle.v1.z == 1.0f", triangle.v1.z);

        assert(triangle.v2.x == -1.0f, "triangle.v2.x == -1.0f", triangle.v2.x);
        assert(fabs(triangle.v2.y) < tolerance, "triangle.v2.y == -0.0f", triangle.v2.y);
        assert(triangle.v2.z == -1.0f, "triangle.v2.z == -1.0f", triangle.v2.z);

        assert(triangle.v3.x == 1.0f, "triangle.v3.x == 1.0f", triangle.v3.x);
        assert(fabs(triangle.v3.y) < tolerance, "triangle.v3.y == -0.0f", triangle.v3.y);
        assert(triangle.v3.z == -1.0f, "triangle.v3.z == -1.0f", triangle.v3.z);


    }

    printf("End test_rotate_x:\n"); // "test_rotate_x:\n
}
void test_rotate_y(){
    printf("Start test_rotate_y:\n"); // "test_rotate_y:\n

    unsigned int test_count = 0;
    
    struct Triangle triangle;
    float angle = 0.0f;

    printf("Test %d with 90d: \n", ++test_count);
    {
        angle = 90.0f;

        set_vector(&triangle.v1, 0.0f, 1.0f, 0.0f);
        set_vector(&triangle.v2, -1.0f, -1.0f, 0.0f);
        set_vector(&triangle.v3, 1.0f, -1.0f, 0.0f);

        rotate_point_around_y(angle, &triangle.v1.x, &triangle.v1.z);
        rotate_point_around_y(angle, &triangle.v2.x, &triangle.v2.z);
        rotate_point_around_y(angle, &triangle.v3.x, &triangle.v3.z);

        /*
        <Vector (0.0000, 1.0000, 0.0000)>
        <Vector (-0.0000, -1.0000, 1.0000)>
        <Vector (0.0000, -1.0000, -1.0000)>
        */

        assert(fabs(triangle.v1.x) < tolerance, "triangle.v1.x == 0.0f", triangle.v1.x);
        assert(triangle.v1.y == 1.0f, "triangle.v1.y == 1.0f", triangle.v1.y);
        assert(fabs(triangle.v1.z) < tolerance, "triangle.v1.z == 0.0f", triangle.v1.z);

        assert(fabs(triangle.v2.x) < tolerance, "triangle.v2.x == -0.0f", triangle.v2.x);
        assert(triangle.v2.y == -1.0f, "triangle.v2.y == -1.0f", triangle.v2.y);
        assert(triangle.v2.z == 1.0f, "triangle.v2.z == 1.0f", triangle.v2.z);

        assert(fabs(triangle.v3.x) < tolerance, "triangle.v3.x == 0.0f", triangle.v3.x);
        assert(triangle.v3.y == -1.0f, "triangle.v3.y == -1.0f", triangle.v3.y);
        assert(triangle.v3.z == -1.0f, "triangle.v3.z == -1.0f", triangle.v3.z);
    }

    printf("End test_rotate_y:\n"); // "test_rotate_y:\n
}
void test_rotate_z(){
    printf("Start test_rotate_z:\n"); // "test_rotate_z:\n

    unsigned int test_count = 0;
    
    struct Triangle triangle;
    float angle = 0.0f;

    printf("Test %d with 90d: \n", ++test_count);
    {
        angle = 90.0f;

        set_vector(&triangle.v1, 0.0f, 1.0f, 0.0f);
        set_vector(&triangle.v2, -1.0f, -1.0f, 0.0f);
        set_vector(&triangle.v3, 1.0f, -1.0f, 0.0f);

        rotate_point_around_z(angle, &triangle.v1.x, &triangle.v1.y);
        rotate_point_around_z(angle, &triangle.v2.x, &triangle.v2.y);
        rotate_point_around_z(angle, &triangle.v3.x, &triangle.v3.y);

        /*
            <Vector (-1.0000, 0.0000, 0.0000)>
            <Vector (1.0000, -1.0000, 0.0000)>
            <Vector (1.0000, 1.0000, 0.0000)>
        */
        
        assert(fabs(triangle.v1.x - (-1.0f)) < tolerance, "triangle.v1.x == -1.0f", triangle.v1.x);
        assert(fabs(triangle.v1.y) < tolerance, "triangle.v1.y == 0.0f", triangle.v1.y);
        assert(fabs(triangle.v1.z) < tolerance, "triangle.v1.z == 0.0f", triangle.v1.z);

        assert(triangle.v2.x == 1.0f, "triangle.v2.x == 1.0f", triangle.v2.x);
        assert(fabs(triangle.v2.y - (-1.0f)) < tolerance, "triangle.v2.y == -1.0f", triangle.v2.y);
        assert(fabs(triangle.v2.z) < tolerance, "triangle.v2.z == 0.0f", triangle.v2.z);

        assert(fabs(triangle.v3.x - 1.0f) < tolerance, "triangle.v3.x == 1.0f", triangle.v3.x);
        assert(triangle.v3.y == 1.0f, "triangle.v3.y == 1.0f", triangle.v3.y);
        assert(fabs(triangle.v3.z) < tolerance, "triangle.v3.z == 0.0f", triangle.v3.z);
    }

    printf("Test  %d with 43d: \n", ++test_count);
    {
        angle = 43.0f;
        
        set_vector(&triangle.v1, 0.0f, 1.0f, 0.0f);
        set_vector(&triangle.v2, -1.0f, -1.0f, 0.0f);
        set_vector(&triangle.v3, 1.0f, -1.0f, 0.0f);

        rotate_point_around_z(angle, &triangle.v1.x, &triangle.v1.y);
        rotate_point_around_z(angle, &triangle.v2.x, &triangle.v2.y);
        rotate_point_around_z(angle, &triangle.v3.x, &triangle.v3.y);

        /*
            <Vector (-0.6820, 0.7314, 0.0000)>
            <Vector (-0.0494, -1.4134, 0.0000)>
            <Vector (1.4134, -0.0494, 0.0000)>
        */

        assert(fabs(triangle.v1.x - (-0.6820f)) < tolerance, "triangle.v1.x == -0.6820f", triangle.v1.x);
        assert(fabs(triangle.v1.y - 0.7314f) < tolerance, "triangle.v1.y == 0.7314f", triangle.v1.y);
        assert(fabs(triangle.v1.z) < tolerance, "triangle.v1.z == 0.0f", triangle.v1.z);

        assert(fabs(triangle.v2.x - (-0.0494f)) < tolerance, "triangle.v2.x == -0.0494f", triangle.v2.x);
        assert(fabs(triangle.v2.y - (-1.4134f)) < tolerance, "triangle.v2.y == -1.4134f", triangle.v2.y);
        assert(fabs(triangle.v2.z) < tolerance, "triangle.v2.z == 0.0f", triangle.v2.z);

        assert(fabs(triangle.v3.x - 1.4134f) < tolerance, "triangle.v3.x == 1.4134f", triangle.v3.x);
        assert(fabs(triangle.v3.y - (-0.0494f)) < tolerance, "triangle.v3.y == -0.0494f", triangle.v3.y);
        assert(fabs(triangle.v3.z) < tolerance, "triangle.v3.z == 0.0f", triangle.v3.z);
    }

    printf("Test  %d with 0d: \n", ++test_count);
    {
        angle = 0.0f;
        
        set_vector(&triangle.v1, 0.0f, 1.0f, 0.0f);
        set_vector(&triangle.v2, -1.0f, -1.0f, 0.0f);
        set_vector(&triangle.v3, 1.0f, -1.0f, 0.0f);

        rotate_point_around_z(angle, &triangle.v1.x, &triangle.v1.y);
        rotate_point_around_z(angle, &triangle.v2.x, &triangle.v2.y);
        rotate_point_around_z(angle, &triangle.v3.x, &triangle.v3.y);

        /*
            <Vector (0.0000, 1.0000, 0.0000)>
            <Vector (-1.0000, -1.0000, 0.0000)>
            <Vector (1.0000, -1.0000, 0.0000)>
        */
       
        assert(fabs(triangle.v1.x) < tolerance, "triangle.v1.x == 0.0f", triangle.v1.x);
        assert(fabs(triangle.v1.y - 1.0f) < tolerance, "triangle.v1.y == 1.0f", triangle.v1.y);
        assert(fabs(triangle.v1.z) < tolerance, "triangle.v1.z == 0.0f", triangle.v1.z);

        assert(fabs(triangle.v2.x - (-1.0f)) < tolerance, "triangle.v2.x == -1.0f", triangle.v2.x);
        assert(fabs(triangle.v2.y - (-1.0f)) < tolerance, "triangle.v2.y == -1.0f", triangle.v2.y);
        assert(fabs(triangle.v2.z) < tolerance, "triangle.v2.z == 0.0f", triangle.v2.z);

        assert(triangle.v3.x - 1.0f < tolerance, "triangle.v3.x == 1.0f", triangle.v3.x);
        assert(fabs(triangle.v3.y - (-1.0f)) < tolerance, "triangle.v3.y == -1.0f", triangle.v3.y);
        assert(fabs(triangle.v3.z) < tolerance, "triangle.v3.z == 0.0f", triangle.v3.z);


    }

    printf("End test_rotate_z:\n"); // "test_rotate_z:\n 
}
