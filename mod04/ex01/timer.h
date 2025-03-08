#ifndef _TIMER_H_
#define _TIMER_H_

#define TIM0_NORMAL_MODE()	(TCCR0A = 0, TCCR0B = (1 << CS00))
#define TIM0_CTC_MODE()		(TCCR0A = (1 << WGM01), TCCR0B = (1 << CS00))
#define TIM0_FAST_PWM()		(TCCR0A = (1 << WGM00) | (1 << WGM01), TCCR0B = (1 << CS00))

#define TIM0_SET_PRESCALER_1()		(TCCR0B = (TCCR0B & ~0x07) | (1 << CS00))
#define TIM0_SET_PRESCALER_8()		(TCCR0B = (TCCR0B & ~0x07) | (1 << CS01))
#define TIM0_SET_PRESCALER_64()		(TCCR0B = (TCCR0B & ~0x07) | (1 << CS00) | (1 << CS01))
#define TIM0_SET_PRESCALER_256()	(TCCR0B = (TCCR0B & ~0x07) | (1 << CS02))
#define TIM0_SET_PRESCALER_1024()	(TCCR0B = (TCCR0B & ~0x07) | (1 << CS00) | (1 << CS02))

#define TIM0_ENABLE_INTERRUPT_COMPARE_MATCH() (TIMSK0 |= (1 << OCIE0A))
#define TIM0_ENABLE_INTERRUPT_OVERFLOW()      (TIMSK0 |= (1 << TOIE0))
#define TIM0_DISABLE_INTERRUPTS()             (TIMSK0 &= ~((1 << OCIE0A) | (1 << TOIE0)))

#define TIM0_SET_OCR0A(value) (OCR0A = value)
#define TIM0_SET_OCR0B(value) (OCR0B = value)



#define TIM1_NORMAL_MODE()			(TCCR1A = 0, TCCR1B = (1 << CS10))
#define TIM1_CTC_MODE()				(TCCR1A = 0, TCCR1B = (1 << WGM12) | (1 << CS10))
#define TIM1_FAST_PWM_MODE()		(TCCR1A = (1 << WGM10) | (1 << WGM11), TCCR1B = (1 << WGM12) | (1 << WGM13) | (1 << CS10))
#define TIM1_FAST_PWM_8BIT_MODE()	(TCCR1A = (1 << WGM10), TCCR1B = (1 << WGM12) | (1 << CS10))

#define TIM1_SET_PRESCALER_1()		(TCCR1B = (TCCR1B & ~0x07) | (1 << CS10))
#define TIM1_SET_PRESCALER_8()		(TCCR1B = (TCCR1B & ~0x07) | (1 << CS11))
#define TIM1_SET_PRESCALER_64()		(TCCR1B = (TCCR1B & ~0x07) | (1 << CS11) | (1 << CS10))
#define TIM1_SET_PRESCALER_256()	(TCCR1B = (TCCR1B & ~0x07) | (1 << CS12))
#define TIM1_SET_PRESCALER_1024()	(TCCR1B = (TCCR1B & ~0x07) | (1 << CS12) | (1 << CS10))

#define TIM1_ENABLE_INTERRUPT_COMPARE_MATCH()	(TIMSK1 |= (1 << OCIE1A))
#define TIM1_ENABLE_INTERRUPT_OVERFLOW()		(TIMSK1 |= (1 << TOIE1))
#define TIM1_DISABLE_INTERRUPTS()				(TIMSK1 &= ~((1 << OCIE1A) | (1 << TOIE1)))

#define TIM1_SET_OCR1A(value) (OCR1A = value)
#define TIM1_SET_OCR1B(value) (OCR1B = value)


#endif // _TIMER_H_