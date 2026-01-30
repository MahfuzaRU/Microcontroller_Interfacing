//================ I2C SLAVE =================

// Register base
#define RCC_BASE     0x40023800
#define GPIOB_BASE   0x40020400
#define I2C1_BASE    0x40005400

// RCC
#define RCC_AHB1ENR (*(volatile unsigned int*)(RCC_BASE + 0x30))
#define RCC_APB1ENR (*(volatile unsigned int*)(RCC_BASE + 0x40))

// GPIOB
#define GPIOB_MODER  (*(volatile unsigned int*)(GPIOB_BASE + 0x00))
#define GPIOB_OTYPER (*(volatile unsigned int*)(GPIOB_BASE + 0x04))
#define GPIOB_PUPDR  (*(volatile unsigned int*)(GPIOB_BASE + 0x0C))
#define GPIOB_AFRH   (*(volatile unsigned int*)(GPIOB_BASE + 0x24))

// I2C
#define I2C1_CR1     (*(volatile unsigned int*)(I2C1_BASE + 0x00))
#define I2C1_CR2     (*(volatile unsigned int*)(I2C1_BASE + 0x04))
#define I2C1_SR1     (*(volatile unsigned int*)(I2C1_BASE + 0x14))
#define I2C1_SR2     (*(volatile unsigned int*)(I2C1_BASE + 0x18))
#define I2C1_DR      (*(volatile unsigned int*)(I2C1_BASE + 0x10))
#define I2C1_OAR1    (*(volatile unsigned int*)(I2C1_BASE + 0x08))

void I2C1_Init(void);
void I2C1_ReadSentence(char *buf);

int main(){
    char sentence[50];
    I2C1_Init();
    while(1){
        I2C1_ReadSentence(sentence);
    }
}

void I2C1_Init(){
    RCC_AHB1ENR |= (1<<1);
    RCC_APB1ENR |= (1<<21);

    GPIOB_MODER |= (1<<17)|(1<<19);
    GPIOB_MODER &= ~((1<<16)|(1<<18));

    GPIOB_OTYPER |= (1<<8)|(1<<9);

    GPIOB_PUPDR |= (1<<16)|(1<<18);
    GPIOB_PUPDR &= ~((1<<17)|(1<<19));

    GPIOB_AFRH |= (1<<2)|(1<<6);

    I2C1_CR1 |= (1<<15);
    I2C1_CR1 &= ~(1<<15);

    I2C1_CR2 = 16;
    I2C1_OAR1 = (0x12<<1) | (1<<14);

    I2C1_CR1 |= (1<<0);
}

void I2C1_ReadSentence(char *buf){
    char c;
    int i = 0;

    I2C1_CR1 |= (1<<10);

    while(!(I2C1_SR1 & (1<<1)));
    (void)I2C1_SR2;

    while(1){
        while(!(I2C1_SR1 & (1<<6)));
        c = I2C1_DR;

        if(c == '\n') break;
        buf[i++] = c;
    }

    buf[i] = '\0';
    I2C1_CR1 &= ~(1<<10);
}
