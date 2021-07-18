#include "ql_type.h"
#include "ql_trace.h"
#include "ql_uart.h"
#include "ql_system.h"
#include "ql_stdlib.h"
#include "ql_error.h"
#include "ql_gpio.h"
#include "motor.h"

motor_ctrl_t motor_ctrl;

void init_motor_gpio(void)
{
	 Ql_GPIO_Init(PINNAME_SIM2_CLK, PINDIRECTION_OUT, 0, PINPULLSEL_PULLUP);
	 Ql_GPIO_Init(PINNAME_SIM2_DATA, PINDIRECTION_OUT, 0, PINPULLSEL_PULLUP);
	 Ql_GPIO_Init(PINNAME_SIM2_RST, PINDIRECTION_OUT, 0, PINPULLSEL_PULLUP);
}

void sleep_motor(void)
{
	MOTOR_NSLEEP_OFF;
	MOTOR_REVERSE_OFF;
	MOTOR_FORWARD_OFF;
}

void forward_motor(void)
{
	MOTOR_NSLEEP_ON;
	MOTOR_REVERSE_OFF;
	MOTOR_FORWARD_ON;
}

void reverse_motor(void)
{
	MOTOR_NSLEEP_ON;
	MOTOR_REVERSE_ON;
	MOTOR_FORWARD_OFF;
}


void init_infrared_sensor()
{
	Ql_GPIO_Init(PINNAME_PCM_CLK, PINDIRECTION_OUT, 0, PINPULLSEL_PULLUP);
	Ql_GPIO_Init(PINNAME_PCM_IN, PINDIRECTION_OUT, 0, PINPULLSEL_PULLUP);
	Ql_GPIO_Init(PINNAME_PCM_OUT, PINDIRECTION_IN, 0, PINPULLSEL_PULLUP);
	Ql_GPIO_Init(PINNAME_PCM_SYNC, PINDIRECTION_IN, 0, PINPULLSEL_PULLUP);

	GPIO_OUT_UNLOCKED_SENSOR_OFF;
	GPIO_OUT_LOCKED_SENSOR_OFF;
}

void motor_init_lockio()
{
    init_motor_gpio();
    sleep_motor();
    init_infrared_sensor();
}


/*!
 * 获取锁状态
 * \n
 *
 * @param cnt 读取锁状态的次数,每次会延时10ms
 * @return 成功返回锁逻辑状态(非0)，否则返回0
 *	全遮住是11，开锁0,1，关锁1,0，允许闭锁应该是，00或者11
 * \n
 * @see LOCK_STATUS_t
 */
u8 motor_get_lock_status_10ms(u8 cnt)
{
    u8 i = 0;

    // 驱动红外
    GPIO_OUT_UNLOCKED_SENSOR_ON;
	GPIO_OUT_LOCKED_SENSOR_ON;
    // 延时一点点时间
    // 读取传感器电平
    for (i = 0; i < cnt; i++)
    {
        GPIO_IN_UNLOCKED_SENSOR;
		GPIO_IN_LOCKED_SENSOR;
        Ql_Sleep(10);
    }
    // 返回锁的逻辑状态
    if (GPIO_IN_LOCKED_SENSOR==0&&GPIO_IN_UNLOCKED_SENSOR==1)
    {
        motor_ctrl.cStatus = LOCK_STATUS_LOCKED;
        return LOCK_STATUS_LOCKED;
    }
    else if (GPIO_IN_UNLOCKED_SENSOR==0&&GPIO_IN_LOCKED_SENSOR==1)
    {
        motor_ctrl.cStatus = LOCK_STATUS_UNLOCKED;
        return LOCK_STATUS_UNLOCKED;
    }
    else if ((GPIO_IN_UNLOCKED_SENSOR==0&&GPIO_IN_LOCKED_SENSOR==0)||(GPIO_IN_UNLOCKED_SENSOR==1&&GPIO_IN_LOCKED_SENSOR==1))
    {
        motor_ctrl.cStatus = LOCK_STATUS_LOCKABLE;
        return LOCK_STATUS_LOCKABLE;
    }

	  // 关闭红外
    GPIO_OUT_UNLOCKED_SENSOR_OFF;
	GPIO_OUT_LOCKED_SENSOR_OFF;
	
    return 0;
}

/*!
 * 立即开锁
 * \n
 *
 * @param cnt 电机转动时间，以10ms为单位
 * @return 成功返回0
 * \n
 * @see
 */
u8 motor_unlock_10ms(u8 motor_run_time)
{
    u8 i;
    u8 lock_status;
    motor_ctrl.cCmd = LOCK_CTRL_CMD_UNLOCK;

    // 检查锁状态 如果显示已经开锁，立即返回
    if (LOCK_STATUS_UNLOCKED == motor_get_lock_status_10ms(10))
        return 0;

    // 电机正传
    forward_motor();
    // 最多n次检查传感器是否是状态1(已解锁)
    for (i = 0; i < motor_run_time; i++)
    {
        if (LOCK_STATUS_UNLOCKED == motor_get_lock_status_10ms(10))
            break;
        Ql_Sleep(10);
    }
    // 电机停转
    sleep_motor();

    // 返回结果
    lock_status=motor_get_lock_status_10ms(10);
    if (LOCK_STATUS_UNLOCKED ==lock_status )
        return 0;
    else if(lock_status==LOCK_STATUS_LOCKED)
    return 1;
}


/*!
 * 使能关锁
 * \n
 *
 * @param cnt 电机转动时间，以10ms为单位
 * @return 成功返回0
 * \n
 * @see
 */
u8 motor_lockable_10ms(u8 motor_run_time)
{
    u8 i;

    motor_ctrl.cCmd = LOCK_CTRL_CMD_LOCKABLE;
    // 驱动红外
    // 检查锁状态 如果显示可锁/已锁，立即返回
    if (LOCK_STATUS_LOCKED == motor_get_lock_status_10ms(10))
        return 0;
    
    if (LOCK_STATUS_LOCKABLE == motor_get_lock_status_10ms(10))
        return 0;

    // 电机正传
    reverse_motor();
    // 最多n次检查传感器是否是状态2(可上锁)
    for (i = 0; i < motor_run_time; i++)
    {
        if (LOCK_STATUS_LOCKABLE == motor_get_lock_status_10ms(10))
            break;
        Ql_Sleep(10);
    }

    Ql_Sleep(400);
    // 电机停转
    sleep_motor();

    // 返回结果
    if (LOCK_STATUS_LOCKABLE == motor_get_lock_status_10ms(10))
        return 0;
    return 1;
}


/*!
 * 等待上锁
 * \n
 *
 * @param cnt 等待次数，每次 1s
 * @return 成功返回0，否则返回非0
 * \n
 * @see
 */
u8 motor_wait_locked_1s(u8 cnt)
{
    u8 i;
    if (LOCK_STATUS_LOCKED == motor_get_lock_status_10ms(10))
    {
       
        reverse_motor();
        Ql_Sleep(100);
        sleep_motor();

        return 0;
    }
    // 每隔1s检查一次是否已经锁上
    for (i = 0; i < cnt; i++)
    {
        if (LOCK_STATUS_LOCKED == motor_get_lock_status_10ms(10))
        {
            break;
        }
        Ql_Sleep(100);
    }

    // 返回结果
    if (LOCK_STATUS_LOCKED == motor_get_lock_status_10ms(10))
    {
        reverse_motor();
        Ql_Sleep(100);
        sleep_motor();
        return 0;
    }
    return 1;
}







