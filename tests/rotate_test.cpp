#include "rotate.h"

#include <gtest/gtest.h>

namespace {

constexpr float kTolerance = 1e-3F;

struct Triangle Sample() {
    struct Triangle t{};
    set_vector(&t.v1, 0.0F, 1.0F, 0.0F);
    set_vector(&t.v2, -1.0F, -1.0F, 0.0F);
    set_vector(&t.v3, 1.0F, -1.0F, 0.0F);
    return t;
}

void ExpectVector(const struct Vector &v, float x, float y, float z) {
    EXPECT_NEAR(v.x, x, kTolerance);
    EXPECT_NEAR(v.y, y, kTolerance);
    EXPECT_NEAR(v.z, z, kTolerance);
}

}  // namespace

TEST(SetVector, StoresTheThreeCoordinates) {
    struct Vector v{};
    set_vector(&v, 1.5F, -2.0F, 3.25F);
    EXPECT_EQ(v.x, 1.5F);
    EXPECT_EQ(v.y, -2.0F);
    EXPECT_EQ(v.z, 3.25F);
}

TEST(RotatePoint, AQuarterTurnAroundXTakesYToZ) {
    struct Triangle t = Sample();
    rotate_point_around_x(90.0F, &t.v1.y, &t.v1.z);
    rotate_point_around_x(90.0F, &t.v2.y, &t.v2.z);
    rotate_point_around_x(90.0F, &t.v3.y, &t.v3.z);
    ExpectVector(t.v1, 0.0F, 0.0F, 1.0F);
    ExpectVector(t.v2, -1.0F, 0.0F, -1.0F);
    ExpectVector(t.v3, 1.0F, 0.0F, -1.0F);
}

TEST(RotatePoint, AQuarterTurnAroundYTakesXToMinusZ) {
    struct Triangle t = Sample();
    rotate_point_around_y(90.0F, &t.v1.x, &t.v1.z);
    rotate_point_around_y(90.0F, &t.v2.x, &t.v2.z);
    rotate_point_around_y(90.0F, &t.v3.x, &t.v3.z);
    ExpectVector(t.v1, 0.0F, 1.0F, 0.0F);
    ExpectVector(t.v2, 0.0F, -1.0F, 1.0F);
    ExpectVector(t.v3, 0.0F, -1.0F, -1.0F);
}

TEST(RotatePoint, AQuarterTurnAroundZTakesYToMinusX) {
    struct Triangle t = Sample();
    rotate_point_around_z(90.0F, &t.v1.x, &t.v1.y);
    rotate_point_around_z(90.0F, &t.v2.x, &t.v2.y);
    rotate_point_around_z(90.0F, &t.v3.x, &t.v3.y);
    ExpectVector(t.v1, -1.0F, 0.0F, 0.0F);
    ExpectVector(t.v2, 1.0F, -1.0F, 0.0F);
    ExpectVector(t.v3, 1.0F, 1.0F, 0.0F);
}

TEST(RotatePoint, AFullTurnLeavesThePointWhereItWas) {
    float x = 0.3F;
    float y = -0.7F;
    rotate_point_around_z(360.0F, &x, &y);
    EXPECT_NEAR(x, 0.3F, kTolerance);
    EXPECT_NEAR(y, -0.7F, kTolerance);
}

TEST(RotatePoint, ATurnAndItsReverseCancel) {
    float y = 0.25F;
    float z = 0.75F;
    rotate_point_around_x(43.0F, &y, &z);
    rotate_point_around_x(-43.0F, &y, &z);
    EXPECT_NEAR(y, 0.25F, kTolerance);
    EXPECT_NEAR(z, 0.75F, kTolerance);
}

TEST(RotateVector, TurnsLikeItsPointAndKeepsTheThirdAxis) {
    struct Vector v{};
    set_vector(&v, 1.0F, 0.0F, 5.0F);
    rotate_vector_around_z(90.0F, &v);
    ExpectVector(v, 0.0F, 1.0F, 5.0F);
    rotate_vector_around_x(90.0F, &v);
    ExpectVector(v, 0.0F, -5.0F, 1.0F);
    rotate_vector_around_y(90.0F, &v);
    ExpectVector(v, 1.0F, -5.0F, 0.0F);
}

TEST(RotateTriangle, TurnsEveryVertex) {
    struct Triangle t = Sample();
    rotate_triabgle_around_z(180.0F, &t);
    ExpectVector(t.v1, 0.0F, -1.0F, 0.0F);
    ExpectVector(t.v2, 1.0F, 1.0F, 0.0F);
    ExpectVector(t.v3, -1.0F, 1.0F, 0.0F);
    rotate_triabgle_around_x(180.0F, &t);
    ExpectVector(t.v1, 0.0F, 1.0F, 0.0F);
    rotate_triabgle_around_y(180.0F, &t);
    ExpectVector(t.v1, 0.0F, 1.0F, 0.0F);
    ExpectVector(t.v2, -1.0F, -1.0F, 0.0F);
}

TEST(CalculateTriangle, PlacesAnEquilateralTriangleAroundTheOrigin) {
    const struct Triangle t = calculate_triangle_points_by_side(3);
    const float radius = 3.0F * 0.57735F;
    ExpectVector(t.v1, radius, 0.0F, 0.0F);
    ExpectVector(t.v2, -radius * 0.5F, radius * 0.866F, 0.0F);
    ExpectVector(t.v3, -radius * 0.5F, -radius * 0.866F, 0.0F);
}

TEST(CalculateTriangle, ASideOfZeroIsAPoint) {
    const struct Triangle t = calculate_triangle_points_by_side(0);
    ExpectVector(t.v1, 0.0F, 0.0F, 0.0F);
    ExpectVector(t.v2, 0.0F, 0.0F, 0.0F);
    ExpectVector(t.v3, 0.0F, 0.0F, 0.0F);
}

TEST(RotateByDirection, AZeroTiltLeavesThePointAlone) {
    float x = 1.0F;
    float y = 2.0F;
    float z = 3.0F;
    rotate_by_diraction(37.0F, 0.0F, &x, &y, &z);
    EXPECT_NEAR(x, 1.0F, kTolerance);
    EXPECT_NEAR(y, 2.0F, kTolerance);
    EXPECT_NEAR(z, 3.0F, kTolerance);
}

TEST(RotateByDirection, TiltsAboutTheAxisAtTheGivenHeading) {
    // At heading 0 the tilt is a turn about the Y axis, so a point on X goes down to -Z.
    float x = 1.0F;
    float y = 0.0F;
    float z = 0.0F;
    rotate_by_diraction(0.0F, 90.0F, &x, &y, &z);
    EXPECT_NEAR(x, 0.0F, kTolerance);
    EXPECT_NEAR(y, 0.0F, kTolerance);
    EXPECT_NEAR(z, -1.0F, kTolerance);
}

TEST(RotateTriangleByDirection, KeepsTheSidesOfTheTriangle) {
    struct Triangle t = calculate_triangle_points_by_side(3);
    rotate_triangle_by_direction(90.0F, 90.0F, &t);
    const float dx = t.v1.x - t.v2.x;
    const float dy = t.v1.y - t.v2.y;
    const float dz = t.v1.z - t.v2.z;
    EXPECT_NEAR(dx * dx + dy * dy + dz * dz, 9.0F, 1e-2F);
}

TEST(GetHeights, AnswersTheZOfEachVertex) {
    struct Triangle t = Sample();
    t.v1.z = 1.0F;
    t.v2.z = -2.0F;
    t.v3.z = 0.5F;
    float h1 = 0.0F;
    float h2 = 0.0F;
    float h3 = 0.0F;
    get_heights(&t, &h1, &h2, &h3);
    EXPECT_EQ(h1, 1.0F);
    EXPECT_EQ(h2, -2.0F);
    EXPECT_EQ(h3, 0.5F);
}
