
#include "unity.h"
#include "co_circular_buffer.h"

void setUp(void)
{
    // Set up before each test
}

void tearDown(void)
{
    // Clean up after each test
}

#ifndef CO_CIRCULAR_BUFFER_MANAGE_MEMORY
void test_co_circular_buffer_init(void)
{
    uint8_t buffer[10];
    co_circular_buffer_init(buffer, 10);
    TEST_ASSERT_EQUAL(10, co_circular_buffer_get_empty());
    TEST_ASSERT_EQUAL(0, co_circular_buffer_get_size());
    co_circular_buffer_terminate();
}
#else
void test_co_circular_buffer_init(void)
{
    uint8_t buffer[10];
    co_circular_buffer_init(buffer, 10);
    TEST_ASSERT_EQUAL(10, co_circular_buffer_get_empty());
    TEST_ASSERT_EQUAL(0, co_circular_buffer_get_size());
    co_circular_buffer_terminate();
}
#endif

void test_co_circular_buffer_push_and_pop(void)
{
    uint8_t buffer[3];
    co_circular_buffer_init(buffer, 3);
    
    TEST_ASSERT_EQUAL(true, co_circular_buffer_is_empty());
    TEST_ASSERT_EQUAL(false, co_circular_buffer_is_full());

    co_circular_buffer_push(1);
    TEST_ASSERT_EQUAL(2, co_circular_buffer_get_empty());
    TEST_ASSERT_EQUAL(1, co_circular_buffer_get_size());
    TEST_ASSERT_EQUAL(false, co_circular_buffer_is_empty());
    TEST_ASSERT_EQUAL(false, co_circular_buffer_is_full());

    co_circular_buffer_push(2);
    TEST_ASSERT_EQUAL(1, co_circular_buffer_get_empty());
    TEST_ASSERT_EQUAL(2, co_circular_buffer_get_size());
    TEST_ASSERT_EQUAL(false, co_circular_buffer_is_empty());
    TEST_ASSERT_EQUAL(false, co_circular_buffer_is_full());


    co_circular_buffer_push(3);
    TEST_ASSERT_EQUAL(0, co_circular_buffer_get_empty());
    TEST_ASSERT_EQUAL(3, co_circular_buffer_get_size());
    TEST_ASSERT_EQUAL(false, co_circular_buffer_is_empty());
    TEST_ASSERT_EQUAL(true, co_circular_buffer_is_full());
    
    co_circular_buffer_push(4);
    TEST_ASSERT_EQUAL(0, co_circular_buffer_get_empty());
    TEST_ASSERT_EQUAL(3, co_circular_buffer_get_size());
    TEST_ASSERT_EQUAL(false, co_circular_buffer_is_empty());
    TEST_ASSERT_EQUAL(true, co_circular_buffer_is_full());

    uint8_t byte;
    uint8_t popped = 0;

    popped = co_circular_buffer_pop(&byte);
    TEST_ASSERT_EQUAL(4, byte);
    TEST_ASSERT_EQUAL(1, popped);
    TEST_ASSERT_EQUAL(1, co_circular_buffer_get_empty());
    TEST_ASSERT_EQUAL(2, co_circular_buffer_get_size());
    TEST_ASSERT_EQUAL(false, co_circular_buffer_is_empty());
    TEST_ASSERT_EQUAL(false, co_circular_buffer_is_full());
    
    popped = co_circular_buffer_pop(&byte);
    TEST_ASSERT_EQUAL(3, byte);
    TEST_ASSERT_EQUAL(1, popped);
    TEST_ASSERT_EQUAL(2, co_circular_buffer_get_empty());
    TEST_ASSERT_EQUAL(1, co_circular_buffer_get_size());
    TEST_ASSERT_EQUAL(false, co_circular_buffer_is_empty());
    TEST_ASSERT_EQUAL(false, co_circular_buffer_is_full());
    
    popped = co_circular_buffer_pop(&byte);
    TEST_ASSERT_EQUAL(2, byte);
    TEST_ASSERT_EQUAL(1, popped);
    TEST_ASSERT_EQUAL(3, co_circular_buffer_get_empty());
    TEST_ASSERT_EQUAL(0, co_circular_buffer_get_size());
    TEST_ASSERT_EQUAL(true, co_circular_buffer_is_empty());
    TEST_ASSERT_EQUAL(false, co_circular_buffer_is_full());

    popped = co_circular_buffer_pop(&byte);
    TEST_ASSERT_EQUAL(2, byte); // stays the same, no write on byte
    TEST_ASSERT_EQUAL(0, popped);
    TEST_ASSERT_EQUAL(3, co_circular_buffer_get_empty());
    TEST_ASSERT_EQUAL(0, co_circular_buffer_get_size());
    TEST_ASSERT_EQUAL(true, co_circular_buffer_is_empty());
    TEST_ASSERT_EQUAL(false, co_circular_buffer_is_full());

    co_circular_buffer_terminate();
}

void test_co_circular_buffer_push_arr_and_pop_arr(void)
{
    uint8_t buffer[10];
    co_circular_buffer_init(buffer, 10);

    uint8_t data_to_push[5] = {1, 2, 3, 4, 5};
    co_circular_buffer_push_arr(data_to_push, 5);
    TEST_ASSERT_EQUAL(5, co_circular_buffer_get_empty());
    TEST_ASSERT_EQUAL(5, co_circular_buffer_get_size());

    uint8_t data_popped[5];
    co_circular_buffer_pop_arr(data_popped, 5);
    TEST_ASSERT_EQUAL(10, co_circular_buffer_get_empty());
    TEST_ASSERT_EQUAL(0, co_circular_buffer_get_size());
    TEST_ASSERT_EQUAL_UINT8_ARRAY(data_to_push, data_popped, 5);

    co_circular_buffer_terminate();
}

void test_co_circular_buffer_wrap_around(void)
{
    uint8_t buffer[5];
    co_circular_buffer_init(buffer, 5);

    uint8_t data_to_push[5] = {1, 2, 3, 4, 5};
    co_circular_buffer_push_arr(data_to_push, 5);
    TEST_ASSERT_EQUAL(0, co_circular_buffer_get_empty());
    TEST_ASSERT_EQUAL(5, co_circular_buffer_get_size());

    uint8_t data_popped[3];
    co_circular_buffer_pop_arr(data_popped, 3);
    TEST_ASSERT_EQUAL(3, data_popped[0]);
    TEST_ASSERT_EQUAL(4, data_popped[1]);
    TEST_ASSERT_EQUAL(5, data_popped[2]);

    uint8_t more_data_to_push[3] = {6, 7, 8};
    co_circular_buffer_push_arr(more_data_to_push, 3);
    TEST_ASSERT_EQUAL(0, co_circular_buffer_get_empty());
    TEST_ASSERT_EQUAL(5, co_circular_buffer_get_size());

    uint8_t final_data_popped[5];
    co_circular_buffer_pop_arr(final_data_popped, 5);
    uint8_t expected_data[5] = {1, 2, 6, 7, 8};
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected_data, final_data_popped, 5);

    co_circular_buffer_terminate();
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_co_circular_buffer_init);
    RUN_TEST(test_co_circular_buffer_push_and_pop);
    RUN_TEST(test_co_circular_buffer_push_arr_and_pop_arr);
    RUN_TEST(test_co_circular_buffer_wrap_around);

    return UNITY_END();
}