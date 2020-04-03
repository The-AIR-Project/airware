# Electrical Schematic

`electrical.pdf` contains a simple diagram showing necessary connections for the
electrical aspect of our design.
- The only power requirements is that it's able to supply the Arduino with
7-12V, the servo with 6.8V, and supply up to 3A (anything above 2.5A <i>should</i>
be okay, but we went with 3A to be safe)
    - If you use a different MCU or motor, just make sure you meet its specs of
    course
- The 220 ohm resistors on the LEDs can be any value that works for you
    - the red LED we used should have gotten a larger resistor, which is why
    in the code it is being powered by PWM (it was too bright!)
