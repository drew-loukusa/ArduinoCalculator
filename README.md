# ArduinoCalculator
A simple Arduino based calculator.

![alt text](https://github.com/drew-loukusa/ArduinoCalculator/blob/master/DSC_0001.JPG)

This calculator supports following features:
- Addition        ('A' key)
- Subtraction     ('B' key)
- Multiplication  ('\*' key)
- Division        ('D' key)
- Clear screen    ('C' key)
- Results as floats
- Result Rollover:
      Once you've calculated a result, press another operator key to use that result as the input to another operation.
      Example: 
      
              LCD: 12+3 = 
                   10
              >> Press "*" 
              LCD: 10*
              >> Press "5"
              LCD: 10*5
              >> Press "="
              LCD: 10*5=
                   50

Future Additions:
- Different modes (Scientific?)
- Allow input of floats

I designed a basic FSM (finite state machine) to model the workings of the calculator and implemented it using a switch statement and an enum of custom state names. See diagram below:

![alt text](https://github.com/drew-loukusa/ArduinoCalculator/blob/master/CalculatorFSM.jpg)

I used the following tutorials to setup my wiring and as a basis for my code:

LCD Wiring and Code: https://www.arduino.cc/en/Tutorial/HelloWorld 

Keypad Wiring and Code: https://playground.arduino.cc/Main/KeypadTutorial

