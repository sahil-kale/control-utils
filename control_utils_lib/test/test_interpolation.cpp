#include "CppUTest/TestHarness.h"

extern "C" {
#include "interpolation_table.h"
}

TEST_GROUP(interpolation_table_test){void setup(){} void teardown(){}};

TEST(interpolation_table_test, test_macro) {
    static float x_values[] = {0, 1, 2, 7};
    static float y_values[] = {0, 1, 4, 9};

    CONTROL_UTILS_DECLARE_INTERPOLATION_TABLE(table, x_values, y_values);

    CHECK_EQUAL(4, table.size);
    CHECK_EQUAL(3, table.slopeBufferSize);
    CHECK_EQUAL(false, table.initialized);
}

TEST(interpolation_table_test, test_init) {
    CHECK_EQUAL(false, control_utils_interpolation_table_init(NULL));

    static float x_values[] = {0, 1, 2, 3, 3.5};
    static float y_values[] = {0, 2, 5, 6, 8};
    CONTROL_UTILS_DECLARE_INTERPOLATION_TABLE(table, x_values, y_values);

    static float expected_slopes[] = {2, 3, 1, 4};

    CHECK_EQUAL(true, control_utils_interpolation_table_init(&table));
    CHECK_EQUAL(true, table.initialized);

    for (size_t i = 0; i < table.slopeBufferSize - 1; i++) {
        DOUBLES_EQUAL(expected_slopes[i], table.slopeBuffer[i], 0.001);
    }
}

TEST(interpolation_table_test, test_interpolate) {
    static float x_values[] = {-3, 0, 1, 2, 3, 3.5};
    static float y_values[] = {-3, 0, 2, 5, 6, 8};
    CONTROL_UTILS_DECLARE_INTERPOLATION_TABLE(table, x_values, y_values);

    CHECK_EQUAL(true, control_utils_interpolation_table_init(&table));

    float y;

    // Test a value outside of the x range - should return the first value
    CHECK_EQUAL(true, control_utils_interpolation_table_interpolate(&table, -4, &y));
    DOUBLES_EQUAL(-3, y, 0.001);

    // Test a value inside the x range
    CHECK_EQUAL(true, control_utils_interpolation_table_interpolate(&table, 1.0, &y));
    DOUBLES_EQUAL(2.0, y, 0.001);

    CHECK_EQUAL(true, control_utils_interpolation_table_interpolate(&table, 2.5, &y));
    DOUBLES_EQUAL(5.5, y, 0.001);

    // Test a value outside of the x range - should return the last value
    CHECK_EQUAL(true, control_utils_interpolation_table_interpolate(&table, 4, &y));
    DOUBLES_EQUAL(8, y, 0.001);
}

TEST(interpolation_table_test, test_interpolate_invalid) {
    CHECK_EQUAL(false, control_utils_interpolation_table_interpolate(NULL, 0, NULL));
    control_utils_interpolation_table_t table;
    CHECK_EQUAL(false, control_utils_interpolation_table_interpolate(&table, 0, NULL));
    float y;
    CHECK_EQUAL(false, control_utils_interpolation_table_interpolate(NULL, 0, &y));

    table.initialized = false;
    CHECK_EQUAL(false, control_utils_interpolation_table_interpolate(&table, 0, &y));
}