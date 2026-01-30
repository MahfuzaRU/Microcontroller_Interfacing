// i2c protocol master

//list of register

#define RCC_BASE     (0x40023800)
//#define GPIOA_BASE   (0x40020000)
#define GPIOB_BASE   (0x40020400)
#define I2C1_BASE    (0x40005400)
//#define TIM2_BASE    (0x40000000)

#define RCC_AHB1ENR (*(volatile unsigned int *) (RCC_BASE + 0x30))
#define RCC_APB1ENR (*(volatile unsigned int *) (RCC_BASE + 0X40))

#define GPIOB_MODER  (*(volatile unsigned int *) (GPIOB_BASE + 0x00))
#define GPIOB_OTYPER (*(volatile unsigned int *) (GPIOB_BASE + 0x04))
#define GPIOB_PUPDR  (*(volatile unsigned int *) (GPIOB_BASE + 0x0C))
#define GPIOB_AFRH   (*(volatile unsigned int *) (GPIOB_BASE + 0x24))

#define I2C1_CR1     (*(volatile unsigned int *) (I2C1_BASE + 0x00))
#define I2C1_CR2	 (*(volatile unsigned int *) (I2C1_BASE + 0x04))
#define I2C1_CCR	 (*(volatile unsigned int *) (I2C1_BASE + 0x1C))
#define I2C1_TRISE   (*(volatile unsigned int *) (I2C1_BASE + 0x20))
#define I2C1_SR1	 (*(volatile unsigned int *) (I2C1_BASE + 0x14))
#define I2C1_SR2	 (*(volatile unsigned int *) (I2C1_BASE + 0x18))
#define I2C1_DR		 (*(volatile unsigned int *) (I2C1_BASE + 0x10))

//function defination to reduce warning
void I2C1_Init();

//send function
void I2C1_Send(char saddr, int n, char* str);

//main function
int main(void){
	I2C1_Init();
	while(1){
		I2C1_Send(0x12, 7, "MAHFUZA");
	}
}

void I2C1_Init(){
	//enable clock of gpiob through AHB1ENR
	RCC_AHB1ENR |= (1<<1);

	//set pb8 and pb9 to alternate function through GPIOB_MODER, default input mode
	GPIOB_MODER |= (1<<19);
	GPIOB_MODER &= ~(1<<18);
	GPIOB_MODER |= (1<<17);
	GPIOB_MODER &= ~(1<<16);

	//pb8 pb9 output type open -drain for high to low
	GPIOB_OTYPER |= (1<<8);
	GPIOB_OTYPER |= (1<<9);

	//SET PB8 PB9 OUTPUT PULL UP FOR ALWAYS HIGH
	GPIOB_PUPDR &= ~(1<<19);
	GPIOB_PUPDR |= (1<<18);
	GPIOB_PUPDR &= ~(1<<17);
	GPIOB_PUPDR |= (1<<16);

	//SET PB8 AND PB9 ALTERNATIVE FUNCTION
	GPIOB_AFRH &= ~(0xFF<<0);
	GPIOB_AFRH |= (1<<2);
	GPIOB_AFRH |= (1<<6);

	//Enable clock for i2c1
	RCC_APB1ENR |= (1<<21);

	//i2c peripheral reset mode
	I2C1_CR1 |= (1<<15);

	//come out of reset mode
	I2C1_CR1 &= ~(1<<15);

	//set i2c clock frequency default 16 MHz
	I2C1_CR2 |= (1<<4);

	//SET i2c standard mode , 100 KHz
	I2C1_CCR = 80;

	//set rise time of clock frequency
	I2C1_TRISE = 17;

	I2C1_CR1 |= (1<<0); //enable peripheral


}

void I2C1_Send(char saddr, int n, char* str){
	//wait when bus is busy
	while(I2C1_SR2 & (1<<1)){

	}

	//GEnerate start conditions and wait
	I2C1_CR1 |= (1<<8);    //SET START CONDITIONS

	//THEN CHECK AND WAIT IT SET OR NOT
	while(!(I2C1_SR1 & (1<<0))){

	}

	//send slave address + write (0) bit
	I2C1_DR = (saddr<<1);    //set 0 to bit 0 position master write and automatically left shift
	while(!(I2C1_SR1 & (1<<1))){}

	//clear addr flag to ensure address is sent
	(void)I2C1_SR2;    //2nd read sr2 int temp = i2c1_sr2 will generate warning

	for(int i=0; i<n; i++){
		//wait until transmitter is empty

		while(!(I2C1_SR1 & (1<<7))) {}

		//place the data to the data register one byte at time
		I2C1_DR = *str++;
	}

	//wait untill transfer finished
	while(!(I2C1_SR1 & (1<<2))) {}

	//generate stop
	I2C1_CR1 |= (1<<9);


}












