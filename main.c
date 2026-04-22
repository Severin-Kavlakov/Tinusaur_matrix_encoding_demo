#include <avr/io.h>
#include <util/delay.h>

void halt_program() {
	PORTB |= (1 << PB0); //RED LED: HIGH
	PORTB |= (1 << PB1); //YELLOW LED: HIGH
	PORTB |= (1 << PB2); //GREEN LED: HIGH
	while(1);
}

//-VARIABLES---------------------------------------------------------------------------------
char message[]   = {"HELLO WORLD"};
char symbols[]   = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
					'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
					'U', 'V', 'W', 'X', 'Y', 'Z', ' '				 };

#define totalChars 11 //changeable BUT must be accurate for this program to run
int realTotalChars = sizeof(message)-1; // 11, minus 1 because of '/0' aka NULL TERMINATOR char, automatically added at the end

#define totalSymbols 27 //constant

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

int encriptionKeyMatrix[3][3] = { 
	{1, 1, 7},
	{0, 1, 2},
	{0, 0, 1}
};

//-FUNCTIONS---------------------------------------------------------------------------------

void check_total_chars_count() {
	if (totalChars != realTotalChars) {
		halt_program();
	}
}

void check_message_compatability(char message[]) {
    int counter=0;
    
    for (int i=0; i <= totalChars-1; i++) { //check for incompatible chars
        for (int j=0; j <= totalSymbols-1; j++) {
            if (message[i] == symbols[j]) {
                counter += 1;
                break;
            }
        }
    }
    if (counter != totalChars) {  //STOP if incompatible chars found
		halt_program();
    }
}

void encode_char(char myChar, int encodedChar[3]) {
	for (int i = 0; i <= totalSymbols-1; i++) {
		if (myChar == symbols[i]) {
			encodedChar[0] = encodingValues[i][0];
			encodedChar[1] = encodingValues[i][1];
			encodedChar[2] = encodingValues[i][2];
		}
	}
}

void encrypt_char(char myChar) {
	
}




void out_encrypted_char(int encoded[3]) { 
	for (int red = 0; red < encoded[0]; red++) { // RED LED HIGH ; LOW
	  PORTB |= (1 << PB0);  _delay_ms(250);
	  PORTB &= ~(1 << PB0); _delay_ms(250);
	}
	
	for (int yellow = 0; yellow < encoded[1]; yellow++) { // YELLOW LED HIGH ; LOW
	  PORTB |= (1 << PB1);  _delay_ms(250);
	  PORTB &= ~(1 << PB1); _delay_ms(250);
	}
	
	for (int green = 0; green < encoded[2]; green++) { // GREEN LED HIGH ; LOW
	  PORTB |= (1 << PB2);  _delay_ms(250);
	  PORTB &= ~(1 << PB2); _delay_ms(250);
	}
}



int main(void) {
    DDRB |= (1 << PB0);	// LED: Set port as output
    DDRB |= (1 << PB1);	// LED: Set port as output
    DDRB |= (1 << PB2);	// LED: Set port as output

	check_total_chars_count();
    check_message_compatability(message);

	for(int i = 0; i <= totalChars-1; i++) {
		encode_char(message[i], encodedAndEncryptedChars[i]);
	}




	while(1) {
		for (int i = 0; i <= totalChars-1; i++) { // for every encoded char in encodedChars: out to leds
			out_encrypted_char(encodedAndEncryptedChars[i]);  // access sub array located at [i] from a max of [totalChars=11]
		}
	    _delay_ms(10000); // loop
	}
	
	return 0;
}
