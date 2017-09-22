/* usb1.c
 *
 * (c) Tom Trebisky  9-18-2017
 *
 * Derived from my inter.c
 *
 * First cut at code to talk to USB
 *
 * This uses a serial console, which I connect as follows:
 *  I use a CP2102 usb to serial gadget, which has 5 pins and
 *  works with 3.3 volt levels.  I make 3 connections:
 *  -- ground
 *  -- Tx on the CP2102 goes to A10 on the STM32
 *  -- Rx on the CP2102 goes to A9 on the STM32
 *
 * Notable things added to this code:
 *  -- add a basic printf taken from my Kyu project
 *
 * inter.c was kind of a test bed for a number of things:
 *
 *  -- getting the clock to run at 72 Mhz
 *  -- getting any kind of interrupt to work
 *  -- getting uart output
 *  -- getting the timer to do something
 *  -- getting output waveforms on some A port pin
 *  -- getting interrupts from sysclk
 *
 * Note that uart1 is on pins A9 and A10
 *  (it could alternately be on pins B6 and B7)
 *
 *  Uart 2 could be on pins A2 and A3
 *  Uart 3 could be on pins B10 and B11
 */

void rcc_init ( void );
void led_init ( int );

void led_on ( void );
void led_off ( void );

/* By itself, with an 8 Mhz clock this gives a blink rate of about 2.7 Hz
 *  so the delay is about 185 ms
 * With a 72 Mhz clock this yields a 27.75 ms delay
 */
void
delay ( void )
{
	volatile int count = 1000 * 200;

	while ( count-- )
	    ;
}

/* We scale the above to try to get a 500 ms delay */
void
big_delay ( void )
{
	volatile int count = 1000 * 540;

	while ( count-- )
	    ;
}

#define PC13	13
#define NBLINKS		2

/* Turn the LED on for a pulse */
static void
led_show ( void )
{
	led_on ();
	delay ();
	led_off ();
}

static void
led_demo ( void )
{
	int i;


	for ( ;; ) {
	    for ( i=0; i<NBLINKS; i++ ) {
		led_on ();
		delay ();
		led_off ();
		delay ();
	    }

	    big_delay ();
	}
}

void
startup ( void )
{
	int t;

	rcc_init ();

	serial_init ();
	printf ( " -- Booting !\n" );

	usb_init ();

#define A_BIT	7
	// gpio_a_init ( A_BIT );

	led_init ( PC13 );
	led_off ();

	/* This gives us a 1 us interrupt rate !
	 * So we toggle the output port at 500 kHz
	 */
	systick_init_int ( 72 );

	serial_puts ( "Hello World\n" );
	/*
	printf ( "Hello sailor ...\n" );
	t = 0xdeadbeef;
	printf ( "Data: %08x\n", t );
	printf ( "Data: %08x\n", 0x1234abcd );
	*/

/* With this timer running, I see a waveform on A1, A2, A3 that is
 * 2 us high, 2 us low (250 kHz).
 * The timer I set up is indeed running at 250 kHz and is tim2
 * These pins are:
 *  A1 = PWM2/2
 *  A2 = PWM2/3
 *  A3 = PWM2/4
 */
	// timer_init ();

	// led_demo ();

	for ( ;; ) {
	}
}

/* THE END */
