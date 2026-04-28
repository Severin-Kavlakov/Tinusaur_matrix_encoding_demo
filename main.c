#include <avr/io.h>
#include <util/delay.h>


//-VARIABLES---------------------------------------------------------------------------------
char message[]   = { "WIP" };
char symbols[] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I',
					 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 
					 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', ' ' };

#define totalChars   sizeof(message)-1 // #defined to be known at compile time ; to allow Variable Length Arrays
#define totalSymbols sizeof(symbols)-1

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

int encoded	 [totalChars][3] = {0}; 
int encrypted[totalChars][3] = {0};

int keyMatrix[3][3] = { 
	{1,  2, -1},
	{1,  1,  0},
	{2,  4, -1}
};

int raisedToPower[3][3] = {0};

//-FUNCTIONS---------------------------------------------------------------------------------

void halt_program(void) {
	for(;;) {
		PORTB |= (1 << PB0);
		PORTB |= (1 << PB1);
		PORTB |= (1 << PB2);
		_delay_ms(50);
		PORTB &= ~(1 << PB0);
		PORTB &= ~(1 << PB1);
		PORTB &= ~(1 << PB2);
		_delay_ms(50);
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

void encode_char(char myChar, int encoded[3]) { //halt when found myChar in symbols [i]
	for (int i=0; i <= totalSymbols-1; i++) {
		if (myChar == symbols[i]) {
			encoded[0] = encodingValues[i][0];
			encoded[1] = encodingValues[i][1];
			encoded[2] = encodingValues[i][2];
			break; 								
		}
	}
}

void raise_to_power(int matrix[3][3], int power, int raisedToPower[3][3]) {
	
}

void encrypt_char(int encoded[3], int matrix[3][3], int encrypted[3]) {
    int product[3] = {0}; 
    for (int row = 0; row <= 2; row++) {
        for (int col = 0; col <= 2; col++) {
            product[row] += matrix[row][col] * encoded[col]; // product of multiplication
        }
    }
	
	int mod = 0;
    for (int i=0; i <= 2; i++) {
        mod = product[i] % 3; 		   // leftover from product/3 
        if (mod < 0) { 	  		   
			mod += 3;			   // if leftover<0    leftover + 3
		}
        encrypted[i] = mod + 1;    // add 1			   0..2 -> 1..3
    }
}

void out_char(int encrypted[3]) { 
	for (int red = 0; red < encrypted[0]; red++) { // RED LED HIGH ; LOW
	  PORTB |= (1 << PB0);  _delay_ms(250);
	  PORTB &= ~(1 << PB0); _delay_ms(250);
	}
	
	_delay_ms(750);

	for (int yellow = 0; yellow < encrypted[1]; yellow++) { // YELLOW LED HIGH ; LOW
	  PORTB |= (1 << PB1);  _delay_ms(250);
	  PORTB &= ~(1 << PB1); _delay_ms(250);
	}
	
	_delay_ms(750);

	for (int green = 0; green < encrypted[2]; green++) { // GREEN LED HIGH ; LOW
	  PORTB |= (1 << PB2);  _delay_ms(250);
	  PORTB &= ~(1 << PB2); _delay_ms(250);
	}
	
	_delay_ms(750);
}



int main(void) {
    DDRB |= (1 << PB0);	// RED    LED: port as output
    DDRB |= (1 << PB1);	// YELLOW LED: port as output
    DDRB |= (1 << PB2);	// GREEN  LED: port as output


    if(check_message_compatability(message) != 1) {
		halt_program();
	}
	
	for(int i=0; i <= totalChars-1; i++) {
		encode_char (/* char* */message[i], 		   encoded  [i]);	
	}
	
	//FOR LOOP ; use OUTPUT of raiseToPower() ; loop thru chars in encoded
	//encrypt_char(/* int*  */encoded[i], keyMatrix, encrypted[i]);
	for (int i=0; i <= totalChars-1; i++) { // encrypt (multiply) each letter with a matrix, raised to the power of its position in message[]
		raise_to_power(keyMatrix, i+1, raisedToPower);
		
	}
	
	while(1) { 
	    _delay_ms(3000);
		for (int i=0; i <= totalChars-1; i++) { // for every encoded char in encodedChars: out to leds
			out_char(encrypted[i]);
			_delay_ms(1000);
		}
	    _delay_ms(10000);
	}

	return 0;
}
