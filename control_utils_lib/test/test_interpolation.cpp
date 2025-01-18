#include "CppUTest/TestHarness.h"

extern "C" {
#include "interpolation_table.h"
}

TEST_GROUP(interpolation_table_test){void setup(){} void teardown(){}};

TEST(interpolation_table_test, test_macro) {
    static float x_values[] = {0, 1, 2, 7};
    static float y_values[] = {0, 1, 4, 9};

    CONTROL_UTILS_DECLARE_INTERPOLATION_TABLE_WITH_SLOPE_BUFFER(table, x_values, y_values);

    CHECK_EQUAL(4, table.size);
    CHECK_EQUAL(3, table.slopeBufferSize);
    CHECK_EQUAL(false, table.initialized);

    CONTROL_UTILS_DECLARE_INTERPOLATION_TABLE(table2, x_values, y_values);
    CHECK_EQUAL(4, table2.size);
    CHECK_EQUAL(0, table2.slopeBufferSize);
    CHECK_EQUAL(false, table2.initialized);
}

TEST(interpolation_table_test, test_init_with_slope_buffer) {
    CHECK_EQUAL(false, control_utils_interpolation_table_init(NULL));

    static float x_values[] = {0, 1, 2, 3, 3.5};
    static float y_values[] = {0, 2, 5, 6, 8};
    CONTROL_UTILS_DECLARE_INTERPOLATION_TABLE_WITH_SLOPE_BUFFER(table, x_values, y_values);

    static float expected_slopes[] = {2, 3, 1, 4};

    CHECK_EQUAL(true, control_utils_interpolation_table_init(&table));
    CHECK_EQUAL(true, table.initialized);

    for (size_t i = 0; i < table.slopeBufferSize - 1; i++) {
        DOUBLES_EQUAL(expected_slopes[i], table.slopeBuffer[i], 0.001);
    }
}

TEST(interpolation_table_test, test_init_without_slope_buffer) {
    static float x_values[] = {0, 1, 2, 3, 3.5};
    static float y_values[] = {0, 2, 5, 6, 8};
    CONTROL_UTILS_DECLARE_INTERPOLATION_TABLE(table, x_values, y_values);

    CHECK_EQUAL(true, control_utils_interpolation_table_init(&table));
    CHECK_EQUAL(true, table.initialized);
}

void assert_interpolation(control_utils_interpolation_table_t *table, float x, float expected_y) {
    float y;
    CHECK_EQUAL(true, control_utils_interpolation_table_interpolate(table, x, &y));
    DOUBLES_EQUAL(expected_y, y, 0.001);
}

void test_interpolate_helper(control_utils_interpolation_table_t *table) {
    assert_interpolation(table, -4, -3);
    assert_interpolation(table, 1.0, 2.0);
    assert_interpolation(table, 2.5, 5.5);
    assert_interpolation(table, 4, 8);
}

TEST(interpolation_table_test, test_interpolate) {
    static float x_values[] = {-3, 0, 1, 2, 3, 3.5};
    static float y_values[] = {-3, 0, 2, 5, 6, 8};
    CONTROL_UTILS_DECLARE_INTERPOLATION_TABLE_WITH_SLOPE_BUFFER(table, x_values, y_values);

    CHECK_EQUAL(true, control_utils_interpolation_table_init(&table));

    test_interpolate_helper(&table);
}

TEST(interpolation_table_test, test_interpolate_no_static_buffer) {
    static float x_values[] = {-3, 0, 1, 2, 3, 3.5};
    static float y_values[] = {-3, 0, 2, 5, 6, 8};
    CONTROL_UTILS_DECLARE_INTERPOLATION_TABLE(table, x_values, y_values);

    CHECK_EQUAL(true, control_utils_interpolation_table_init(&table));

    test_interpolate_helper(&table);
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