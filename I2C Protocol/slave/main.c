// i2c protocol slave

//list of register

#define RCC_BASE     (0x40023800)
#define GPIOB_BASE   (0x40020400)
#define I2C1_BASE    (0x40005400)


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
#define I2C1_OAR1    (*(volatile unsigned int *) (I2C1_BASE + 0X08))

void I2C1_Init();

void I2C1_Read(int n, char* str);

//main function
int main(){
	I2C1_Init();
	char str[7];
	while(1){
		I2C1_Read(7, str);
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

	I2C1_OAR1 = (0x12<<1);
	I2C1_OAR1 |= (1<<14);

	I2C1_CR1 |= (1<<0); //enable peripheral


}

void I2C1_Read(int n, char* str){

	//enable acknowledge bit
	I2C1_CR1 |= (1<<10);

	while(!(I2C1_SR1 & (1<<1))){}

	(void)I2C1_SR2;

	for(int i=0; i<n; i++){
		while(!(I2C1_SR1 & (1<<6))){}
		str[i] = I2C1_DR;
	}

	I2C1_CR1 &= ~(1<<10);
}

/*
master  -   slave (wire connection)
D15     -    D15
D14     -    D14

stm a account login thakte hobe

after master & slave debug for send data master to slave
1. slave run -> master run -> master debug -> [send function in ,, step into invisible -> slave reset -> visible]

2. for receiving data  slave a data receive show
1. slave run -> master run -> slave debug -> [read function in ,, step into invisible -> master reset -> visible]



*/













