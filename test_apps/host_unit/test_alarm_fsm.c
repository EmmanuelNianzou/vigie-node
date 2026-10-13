#include "alarm_fsm.h"
#include "unity.h"

static alarm_fsm_t fsm;
static const alarm_cfg_t cfg = {.t_confirm_ms = 5000, .t_mute_ms = 60000};
static const alarm_input_t none = {0};

void setUp(void) { alarm_fsm_init(&fsm, &cfg); }
void tearDown(void) {}

static alarm_output_t step(alarm_input_t in, uint32_t t)
{
    return alarm_fsm_step(&fsm, &in, t);
}

static void arm(uint32_t t)
{
    alarm_input_t in = {.btn_short = true};
    step(in, t);
}

static void test_arm_with_button_then_disarm(void)
{
    alarm_input_t btn = {.btn_short = true};
    TEST_ASSERT_EQUAL(ALARM_ARMED, step(btn, 100).state);
    TEST_ASSERT_EQUAL(ALARM_DISARMED, step(btn, 200).state);
}

static void test_detection_goes_to_pre_alert_then_back_without_confirmation(void)
{
    alarm_input_t pir = {.pir = true};
    arm(0);
    TEST_ASSERT_EQUAL(ALARM_PRE_ALERT, step(pir, 1000).state);
    TEST_ASSERT_EQUAL(ALARM_PRE_ALERT, step(none, 2000).state);
    TEST_ASSERT_EQUAL(ALARM_ARMED, step(none, 6000).state); /* t_confirm écoulé sans nouvelle détection */
}

static void test_second_detection_after_release_raises_alert(void)
{
    alarm_input_t pir = {.pir = true};
    arm(0);
    step(pir, 1000);                                            /* ARMED -> PRE_ALERT */
    TEST_ASSERT_EQUAL(ALARM_PRE_ALERT, step(pir, 1500).state);  /* détection soutenue : pas de confirmation */
    step(none, 2000);                                           /* la détection retombe */
    alarm_output_t out = step(pir, 2500);                       /* seconde détection */
    TEST_ASSERT_EQUAL(ALARM_ALERT, out.state);
    TEST_ASSERT_TRUE(out.buzzer_on);
    TEST_ASSERT_TRUE(out.publish_event);
}

static void test_alert_mute_then_rearm_after_mute_time(void)
{
    alarm_input_t pir = {.pir = true};
    alarm_input_t btn = {.btn_short = true};
    arm(0);
    step(pir, 1000);
    step(none, 1500);
    step(pir, 2000);
    TEST_ASSERT_EQUAL(ALARM_MUTED, step(btn, 3000).state);
    TEST_ASSERT_EQUAL(ALARM_MUTED, step(none, 30000).state);
    TEST_ASSERT_EQUAL(ALARM_ARMED, step(none, 3000 + 60000).state);
}

static void test_disarm_command_from_alert(void)
{
    alarm_input_t pir = {.pir = true};
    alarm_input_t off = {.cmd_disarm = true};
    arm(0);
    step(pir, 1000);
    step(none, 1500);
    step(pir, 2000);
    TEST_ASSERT_EQUAL(ALARM_DISARMED, step(off, 2500).state);
}

static void test_sensor_fault_from_any_state_and_recovery(void)
{
    alarm_input_t fault = {.sensor_fault = true};
    alarm_input_t fault_and_reset = {.sensor_fault = true, .btn_long = true};
    alarm_input_t reset = {.btn_long = true};
    arm(0);
    TEST_ASSERT_EQUAL(ALARM_FAULT, step(fault, 100).state);
    TEST_ASSERT_EQUAL(ALARM_FAULT, step(fault_and_reset, 200).state); /* le défaut persiste : pas de reprise */
    TEST_ASSERT_EQUAL(ALARM_DISARMED, step(reset, 300).state);        /* défaut disparu + reset volontaire */
}

static void test_commands_are_ignored_in_wrong_state(void)
{
    alarm_input_t off = {.cmd_disarm = true};
    TEST_ASSERT_EQUAL(ALARM_DISARMED, step(off, 100).state);
    TEST_ASSERT_FALSE(step(off, 200).publish_event);
}

static void test_clock_wraparound_does_not_break_timeouts(void)
{
    alarm_input_t pir = {.pir = true};
    const uint32_t near_wrap = 0xFFFFFFFFu - 1000u;
    arm(near_wrap - 10);
    step(pir, near_wrap);
    /* 5 secondes plus tard : now a débordé, l'écart non signé reste correct */
    TEST_ASSERT_EQUAL(ALARM_ARMED, step(none, near_wrap + 5000u).state);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_arm_with_button_then_disarm);
    RUN_TEST(test_detection_goes_to_pre_alert_then_back_without_confirmation);
    RUN_TEST(test_second_detection_after_release_raises_alert);
    RUN_TEST(test_alert_mute_then_rearm_after_mute_time);
    RUN_TEST(test_disarm_command_from_alert);
    RUN_TEST(test_sensor_fault_from_any_state_and_recovery);
    RUN_TEST(test_commands_are_ignored_in_wrong_state);
    RUN_TEST(test_clock_wraparound_does_not_break_timeouts);
    return UNITY_END();
}
