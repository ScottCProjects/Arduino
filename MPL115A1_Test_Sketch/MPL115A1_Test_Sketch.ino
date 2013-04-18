//======================//
//		Includes		//
//======================//
#include <avr/io.h>
#include <stdio.h>
#define PRESH	0x00	// 80
#define	PRESL	0x02	// 82
#define	TEMPH	0x04	// 84
#define	TEMPL	0x06	// 86

#define A0MSB	0x08	// 88
#define A0LSB	0x0A	// 8A
#define B1MSB	0x0C	// 8C
#define B1LSB	0x0E	// 8E
#define	B2MSB	0x10	// 90
#define B2LSB	0x12	// 92
#define C12MSB	0x14	// 94
#define	C12LSB	0x16	// 96
#define	C11MSB	0x18	// 98
#define C11LSB	0x1A	// 9A
#define C22MSB	0x1C	// 9C
#define C22LSB	0x1E	// 9E

//======================//
// 		 Macros			//
//======================//
#define sbi(var, mask)   ((var) |= (uint8_t)(1 << mask))
#define cbi(var, mask)   ((var) &= (uint8_t)~(1 << mask))

//======================//
// 	 MPL115A1 Defines	//
//======================//
#define CS 8 //pin for chip select
#define SDN 9	// pin for interrupt

//======================//
//	General Functions	//
//======================//
static int uart_putchar(char c, FILE *stream);
uint8_t uart_getchar(void);

//======================//
//	MPL115A1 Functions	//
//======================//
void init_SPI(void);
void txdata(char data);
char rxdata(void);
char read(uint8_t address);
void write(uint8_t address, char data);
float calculatePressure(void);

//======================//
//	 Global Variables	//
//======================//
signed char sia0MSB, sia0LSB;
signed char sib1MSB, sib1LSB;
signed char sib2MSB, sib2LSB;
signed char sic12MSB, sic12LSB;
signed char sic11MSB, sic11LSB;
signed char sic22MSB, sic22LSB;
signed int sia0, sib1, sib2, sic12, sic11, sic22, siPcomp;
float decPcomp;
signed long lt1, lt2, lt3, si_c11x1, si_a11, si_c12x2;
signed long si_a1, si_c22x2, si_a2, si_a1x1, si_y1, si_a2x2;
unsigned int uiPadc, uiTadc;
unsigned char uiPH, uiPL, uiTH, uiTL;
#define printf Serial.print

void setup()
{
  Serial.begin(9600);
}
void loop()
{
	init_SPI();
	sbi(PORTB,CS);	// CS Idle High
	sbi(PORTB,SDN);	// SDN high turns sensor on
	
	printf("\n***************MPR115A1 Test****************\n\n");
	
	// main program loop
	while(1){
		printf("\nPress space to print pressure\n");
		while( !(UCSR0A & (1<<RXC0)) )
			;
		if (UDR0 == ' ')
                {
			printf("Pressure = ");
                        printf(calculatePressure());
                        printf("\n");
                }
	}
}

float calculatePressure()
{
	write(0x24, 0x00);	// Start Both Conversions
	// write(0x20, 0x00);	// Start Pressure Conversion
	// write(0x22, 0x00);	// Start temperature conversion
	delay(10);	// Typical wait time is 3ms
	
	// Read pressure
	uiPH = read(PRESH);
	uiPL = read(PRESL);
	uiTH = read(TEMPH);
	uiTL = read(TEMPL);
	
	uiPadc = (unsigned int) uiPH << 8;
	uiPadc += (unsigned int) uiPL & 0x00FF;
	uiTadc = (unsigned int) uiTH << 8;
	uiTadc += (unsigned int) uiTL & 0x00FF;
	
	// Placing Coefficients into 16-bit Variables
	// a0
	sia0MSB = read(A0MSB);
	sia0LSB = read(A0LSB);
	sia0 = (signed int) sia0MSB << 8;
	sia0 += (signed int) sia0LSB & 0x00FF;
	
	// b1
	sib1MSB = read(B1MSB);
	sib1LSB = read(B1LSB);
	sib1 = (signed int) sib1MSB << 8;
	sib1 += (signed int) sib1LSB & 0x00FF;
	
	// b2
	sib2MSB = read(B2MSB);
	sib2LSB = read(B2LSB);
	sib2 = (signed int) sib2MSB << 8;
	sib2 += (signed int) sib2LSB & 0x00FF;
	
	// c12
	sic12MSB = read(C12MSB);
	sic12LSB = read(C12LSB);
	sic12 = (signed int) sic12MSB << 8;
	sic12 += (signed int) sic12LSB & 0x00FF;
	
	// c11
	sic11MSB = read(C11MSB);
	sic11LSB = read(C11LSB);
	sic11 = (signed int) sic11MSB << 8;
	sic11 += (signed int) sic11LSB & 0x00FF;
	
	// c22
	sic22MSB = read(C22MSB);
	sic22LSB = read(C22LSB);
	sic22 = (signed int) sic22MSB << 8;
	sic22 += (signed int) sic22LSB & 0x00FF;
	
	// Coefficient 9 equation compensation
	uiPadc = uiPadc >> 6;
	uiTadc = uiTadc >> 6;
	
	// Step 1 c11x1 = c11 * Padc
	lt1 = (signed long) sic11;
	lt2 = (signed long) uiPadc;
	lt3 = lt1*lt2;
	si_c11x1 = (signed long) lt3;
	
	// Step 2 a11 = b1 + c11x1
	lt1 = ((signed long)sib1)<<14;
	lt2 = (signed long) si_c11x1;
	lt3 = lt1 + lt2;
	si_a11 = (signed long)(lt3>>14);
	
	// Step 3 c12x2 = c12 * Tadc
	lt1 = (signed long) sic12;
	lt2 = (signed long) uiTadc;
	lt3 = lt1*lt2;
	si_c12x2 = (signed long)lt3;
	
	// Step 4 a1 = a11 + c12x2
	lt1 = ((signed long)si_a11<<11);
	lt2 = (signed long)si_c12x2;
	lt3 = lt1 + lt2;
	si_a1 = (signed long) lt3>>11;
	
	// Step 5 c22x2 = c22*Tadc
	lt1 = (signed long)sic22;
	lt2 = (signed long)uiTadc;
	lt3 = lt1 * lt2;
	si_c22x2 = (signed long)(lt3);
	
	// Step 6 a2 = b2 + c22x2
	lt1 = ((signed long)sib2<<15);
	lt2 = ((signed long)si_c22x2>1);
	lt3 = lt1+lt2;
	si_a2 = ((signed long)lt3>>16);
	
	// Step 7 a1x1 = a1 * Padc
	lt1 = (signed long)si_a1;
	lt2 = (signed long)uiPadc;
	lt3 = lt1*lt2;
	si_a1x1 = (signed long)(lt3);
	
	// Step 8 y1 = a0 + a1x1
	lt1 = ((signed long)sia0<<10);
	lt2 = (signed long)si_a1x1;
	lt3 = lt1+lt2;
	si_y1 = ((signed long)lt3>>10);
	
	// Step 9 a2x2 = a2 * Tadc
	lt1 = (signed long)si_a2;
	lt2 = (signed long)uiTadc;
	lt3 = lt1*lt2;
	si_a2x2 = (signed long)(lt3);
	
	// Step 10 pComp = y1 + a2x2
	lt1 = ((signed long)si_y1<<10);
	lt2 = (signed long)si_a2x2;
	lt3 = lt1+lt2;
	
	// Fixed point result with rounding
	//siPcomp = ((signed int)lt3>>13);
	siPcomp = lt3/8192;
	
	// decPcomp is defined as a floating point number
	// Conversion to decimal value from 1023 ADC count value
	// ADC counts are 0 to 1023, pressure is 50 to 115kPa respectively
	decPcomp = ((65.0/1023.0)*siPcomp)+50;
	
	return decPcomp;
}

void write(uint8_t address, char data)
{
	//write any data byte to any single address
	//adds a 0 to the MSB of the address byte (WRITE mode)

	address &= 0x7F;	

	//printf("\nWriting 0x%x to 0x%x\n", data, address);

	cbi(PORTB,CS);
	delay(1);
	txdata(address);
	delay(1);
	txdata(data);
	delay(1);
	sbi(PORTB,CS);
}

char read(uint8_t address)
{
	//returns the contents of any 1 byte register from any address
	//sets the MSB for every address byte (READ mode)

	char byte;

	address |= 0x80;

	cbi(PORTB,CS);
	txdata(address);
	byte = rxdata();
	sbi(PORTB,CS);

	return byte;
}

char rxdata(void)
{
	SPDR = 0x55;
	
	while((SPSR&0x80) == 0x00)
		;

	return SPDR;
}

void txdata(char data)
{
	SPDR = data;
	while((SPSR&0x80) == 0x00)
		;
}

void init_SPI(void)
{
	sbi(SPCR,MSTR); //make SPI master
	cbi(SPCR,CPOL); cbi(SPCR,CPHA); //SCL idle low, sample data on rising edge
	cbi(SPCR,SPR1); cbi(SPCR,SPR0); sbi(SPSR,SPI2X); //Fosc/4 is SPI frequency
	sbi(SPCR,SPE); //enable SPI
}

static int uart_putchar(char c, FILE *stream)
{
    if (c == '\n') uart_putchar('\r', stream);

    loop_until_bit_is_set(UCSR0A, UDRE0);
    UDR0 = c;

    return 0;
}

uint8_t uart_getchar(void)
{
    while( !(UCSR0A & (1<<RXC0)) )
		;
    return(UDR0);
}

