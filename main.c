#include <avr/io.h>
#include <util/delay.h>


//-VARIABLES---------------------------------------------------------------------------------
char message[]   = { "IX A" };
char symbols[27] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I',
					 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 
					 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', ' ' };

#define totalChars   sizeof(message)-1 // #defined to be known at compile time ; to allow Variable Length Arrays
#define totalSymbols sizeof(symbols)   // explicit array size

int encodingValues[totalSymbols][3] = {
    {1, 1, 1}, {1, 1, 2}, {1, 1, 3},
    {1, 2, 1}, {1, 2, 2}, {1, 2, 3},
    {1, 3, 1}, {1, 3, 2}, {1, 3, 3},
    {2, 1, 1}, {2, 1, 2}, {2, 1, 3},
    {2, 2, 1}, {2, 2, 2}, {2, 2, 3},
    {2, 3, 1}, {2, 3, 2}, {2, 3, 3},
    {3, 1, 1}, {3, 1, 2}, {3, 1, 3},
    {3, 2, 1}, {3, 2, 2}, {3, 2, 3},
    {3, 3, 1}, {3, 3, 2}, {3, 3, 3}
};

int encodedAndEncryptedChars[totalChars][3];

int phi[3][3] = { 
	{1,  2, -1},
	{1,  1,  0},
	{2,  4, -1}
};

int psi[3][3] = {
	{ 1, -1,  3},
	{-1,  2, -1},
	{-2,  2, -5}
}

//-FUNCTIONS---------------------------------------------------------------------------------

void halt_program(void) {
	while(1) {
		PORTB |= (1 << PB0); //RED LED: HIGH
		PORTB |= (1 << PB1); //YELLOW LED: HIGH
		PORTB |= (1 << PB2); //GREEN LED: HIGH
		_delay_ms(100);
		PORTB &= ~(1 << PB0);
		PORTB &= ~(1 << PB1);
		PORTB &= ~(1 << PB2);
		_delay_ms(100);
	}
}

int check_message_compatability(char message[]) {
    int counter=0;
    for (int i=0; i <= totalChars-1; i++) { //check for incompatible chars
        for (int j=0; j <= totalSymbols-1; j++) {
            if (message[i] == symbols[j]) {
                counter += 1;
                break;
            }
        }
    }
    return (counter == totalChars) ? 1 : 0; //return 0 if incompatible chars found
}

//custom encrypt funciton
void multiply_matricies(int m, int n, int p, int matrixA[m][n], int matrixB[n][p]) {
	
}

void out_encrypted_char(int encoded[3]) { 
	for (int red = 0; red < encoded[0]; red++) { // RED LED HIGH ; LOW
	  PORTB |= (1 << PB0);  _delay_ms(500);
	  PORTB &= ~(1 << PB0); _delay_ms(500);
	}

	for (int yellow = 0; yellow < encoded[1]; yellow++) { // YELLOW LED HIGH ; LOW
	  PORTB |= (1 << PB1);  _delay_ms(500);
	  PORTB &= ~(1 << PB1); _delay_ms(500);
	}

	for (int green = 0; green < encoded[2]; green++) { // GREEN LED HIGH ; LOW
	  PORTB |= (1 << PB2);  _delay_ms(500);
	  PORTB &= ~(1 << PB2); _delay_ms(500);
	}
}


int main(void) {
    DDRB |= (1 << PB0);	// LED: port as output
    DDRB |= (1 << PB1);	// LED: port as output
    DDRB |= (1 << PB2);	// LED: port as output

    if(check_message_compatability(message)!=1) {
		halt_program();
	}

	for(int i = 0; i <= totalChars-1; i++) {
		encode_and_encrypt_char(message[i], encodedAndEncryptedChars[i]);
	}

	while(1) {
	    _delay_ms(3000);

		for (int i = 0; i <= totalChars-1; i++) { // for every encoded char in encodedChars: out to leds
			out_encrypted_char(encodedAndEncryptedChars[i]);  // access sub array located at [i] from a max of [totalChars=11]
			_delay_ms(1000);
		}

	    _delay_ms(10000); // loop
	}

	return 0;
}
