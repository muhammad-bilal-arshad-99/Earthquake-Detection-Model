# Earthquake-Detection-Model

## How it Works
I built a small house from plaster that does two things: it senses table vibrations like a mini earthquake and screams a warning, and it automatically turns on lights when the room gets dark. An MPU‑6050 accelerometer sits on the base and continuously reads tiny accelerations. When I shake the base, the sensor sees sudden movements in any direction — not just one tilt. The Arduino Nano running my code filters out slow tilts (like someone accidentally bumping the table gently) and only reacts to sharp, shake‑like jolts. When that happens, five red LEDs inside the windows flash and a loud buzzer sounds for about half a second, staying on longer if the shaking continues. Meanwhile, a light‑dependent resistor (LDR) on the roof constantly checks how bright it is. If the room becomes dark, seven yellow LEDs light up automatically — no switch, no human needed.
The house itself is made of cardboard coated with a thin plaster‑of‑Paris mix, so it looks like a real concrete building but remains light enough to shake easily. 

## Components Used
1. Arduino Nano (Microcontroller)
2. MPU-6050 Accelerometer
3. LDR 
4. Active Buzzer
5. Red LEDs
6. Yellow LEDs
7. Resistors

## Skills and Concepts I Learned

Real‑time sensor filtering

Voltage dividers 

Timing and hold‑off logic

I²C communication


## Future Improvements (If I Revisit This Project)

Wireless sensor network

Data logging and visualization

Visual alert differentiation

## Watch the full project demonstration video here:

https://www.youtube.com/
