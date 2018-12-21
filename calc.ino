
#include <LiquidCrystal.h>
#include <Keypad.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const byte ROWS = 4; 
const byte COLS = 4; 
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {33, 35, 37, 39}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {41, 43, 45, 47}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  Serial.begin(9600);
}

// Global Vars for the program:
float result = 0;
String num_a = "", num_b = "";
char op = ' ';
bool bad_result = false;
char ops[] = {'A','B','D','*','#'};
char nums[] = {'0','1','2','3','4','5','6','7','8','9'};

// Forward Declerations:
float calculate_result();
void clear_reset();
bool valid_key(char key);
bool is_num(char key);
bool is_op(char key);
char char_to_op(char key);
char get_valid_key();

// States for our FSM:
enum State {
	INITIAL_STATE,
	A_SET_STATE,
	OP_SET_STATE,
	B_SET_STATE,
	RESULT_SET_STATE,
};

// Setup initial state and start the loop:
State state = INITIAL_STATE;
void loop() {	  
	char key;
	switch(state){ 	    
		case INITIAL_STATE:
			Serial.println("INITIAL");			
			key = get_valid_key(); 
			if (is_num(key)) { //INPUT_NUM          			
				lcd.print(key); 
				num_a += key;
				state = A_SET_STATE;             
			}       
			break;

		case A_SET_STATE:
			Serial.println("A_SET");			
			key = get_valid_key();
			if (key != '#'){          
				if(is_op(key)){  //INPUT_OP
					Serial.println("\tINPUT_OP");
					op = char_to_op(key);
					lcd.print(op);          
					state = OP_SET_STATE;            
				}
				if(is_num(key)){ //INPUT_NUM
					Serial.println("\tINPUT_NUM");
					lcd.print(key); 
					num_a += key;
					state = A_SET_STATE;                      
				}
				if( key == 'C' ){ //INPUT_CLEAR
					Serial.println("\tINPUT_CLEAR"); 
					clear_reset();
					state = INITIAL_STATE;            
				}
			}
			break;

		case OP_SET_STATE:
			Serial.println("OP_SET");    
				
			key = get_valid_key();		
			if (is_num(key) || key == 'C'){          
				if(is_num(key)){  //INPUT_NUM
					Serial.println("\tINPUT_NUM");
					lcd.print(key); 
					num_b += key;
					state = B_SET_STATE;            
				}
				if( key == 'C' ){ //INPUT_CLEAR
					Serial.println("\tINPUT_CLEAR");
					clear_reset();
					state = INITIAL_STATE;            
				} 
			}
			break;

		case B_SET_STATE:
			Serial.println("B_SET");		
			key = get_valid_key();	
			if(is_num(key) || key == '#' || key == 'C'){          
				if(is_num(key)){ //INPUT_NUM
					Serial.println("\tINPUT_NUM");
					lcd.print(key); 
					num_b += key;
					state = B_SET_STATE;
				}        
				if( key == '#'){ //INPUT_POUND
					Serial.println("\tINPUT_POUND");
					result = calculate_result();
					if(!bad_result){
						lcd.print(" = ");
						lcd.setCursor(0,1);
						lcd.print(result);
						state = RESULT_SET_STATE;
					} 
					else{ 
						bad_result = false;
						while(true){
							char key = keypad.getKey();
							if(key == 'C') break;
						}
						clear_reset();
						state = INITIAL_STATE;  
					}                   
				}
				if( key == 'C'){ //INPUT_CLEAR
					Serial.println("\tINPUT_CLEAR");
					clear_reset();
					state = INITIAL_STATE;            
				}
			}
			break;

		case RESULT_SET_STATE:
			Serial.println("RESULT_SET");			
			key = get_valid_key();		
			if(!is_num(key) && key != '#'){          
				if(is_op(key)){ //SET_A_TO_RESULT
					Serial.println("\tSET_A_TO_RESULT");
					num_a = String(result); 
					num_b = "";
					op = char_to_op(key);
					lcd.clear();
					lcd.print(num_a);
					lcd.print(op);
					state = OP_SET_STATE;
				}
				if(key == 'C')  { //INPUT_CLEAR
					Serial.println("\tINPUT_CLEAR");
					clear_reset();
					state = INITIAL_STATE;          
				}	
			}
			break;
		
	}
}

// Returns a valid key from the keypad. 
// Program waits inside this function until a valid key is typed
// This method is essetially a 'get_next_event()' method
char get_valid_key(){
  char key;
  while(true){
    key = keypad.getKey();
    if(valid_key(key)) return key;
  }    
}

// Returns the appropriate symbol for a given letter. 
char char_to_op(char key){
  if     (key == 'A')  return '+';
  else if(key == 'B')  return '-';
  else if(key == '*')  return '*'; 
  else if(key == 'D')  return '/';
}

// Checks if a given key is valid.
// Exists because the keypad will continually generate blank keys
bool valid_key(char key){
  for(int i = 0; i < 4; ++i){
    for(int j = 0; j < 4; ++j){
      if(key == keys[i][j])         
        return true;      
    }
  }
  return false;
}

bool is_num(char key){
  for(int i = 0; i < 10; ++i){
    if(key == nums[i]) return true;        
  }
  return false;
}

bool is_op(char key){
  for(int i = 0; i < 5; ++i){ 
    if(key == ops[i]) return true;
  }  
  return false;
}
float calculate_result(){
	if     (op == '+'){result = num_a.toFloat()+num_b.toFloat();} 
	else if(op == '-'){result = num_a.toFloat()-num_b.toFloat();}
	else if(op == '*'){result = num_a.toFloat()*num_b.toFloat();}
	else if(op == '/'){
		if(num_a != "" && num_b.toInt() == 0){
			lcd.clear();
			lcd.print("No div by 0!");			
			bad_result = true;
		}
		else{result = num_a.toFloat()/num_b.toFloat();}
	}
	return result;
}

void clear_reset(){ 
	lcd.clear(); lcd.setCursor(0,0); num_a =""; num_b =""; 
	op = ""; result = 0;
}
