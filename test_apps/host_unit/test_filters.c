#include "filters.h"
#include "unity.h"

void setUp(void) {}
void tearDown(void) {}

static void test_ema_primes_then_smooths(void)
{
    ema_t f;
    ema_init(&f, 0.5f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 10.0f, ema_update(&f, 10.0f));
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 15.0f, ema_update(&f, 20.0f));
}

static void test_hysteresis_does_not_chatter(void)
{
    hysteresis_t h;
    hysteresis_init(&h, 25.0f, 28.0f);
    TEST_ASSERT_FALSE(hysteresis_update(&h, 26.0f)); /* entre les seuils : reste faux */
    TEST_ASSERT_TRUE(hysteresis_update(&h, 28.5f));  /* franchit le seuil haut */
    TEST_ASSERT_TRUE(hysteresis_update(&h, 27.0f));  /* entre les seuils : reste vrai */
    TEST_ASSERT_FALSE(hysteresis_update(&h, 24.9f)); /* franchit le seuil bas */
}

static void test_median3_rejects_outlier(void)
{
    TEST_ASSERT_EQUAL_INT32(5, median3(9, 5, 1));
    TEST_ASSERT_EQUAL_INT32(21, median3(20, 21, 4000)); /* la mesure aberrante 4000 est écartée */
    TEST_ASSERT_EQUAL_INT32(7, median3(7, 7, 7));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_ema_primes_then_smooths);
    RUN_TEST(test_hysteresis_does_not_chatter);
    RUN_TEST(test_median3_rejects_outlier);
    return UNITY_END();
}
