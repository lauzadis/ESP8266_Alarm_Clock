# ESP8266 Alarm Clock

## What is it?

An alarm clock that can be accessed via web interface. Set, create, and disable alarms from the internet. 

The NodeMCU ESP8266 module was chosen for its low power consumption, sipping an average 35mA current.

With Bootstrap, a responsive front-end framework, this interface is viewable from just about any device, ranging from typical desktops to smart fridges and smart watches.

## Why not just use your phone?
This is more of a personal decision for me. I spend too much time on my phone, and would prefer to leave it off overnight to avoid distractions in the morning.


## Materials
* 1 pcs. [NodeMCU ESP8266 ESP-12E](https://www.amazon.com/JacobsParts-ESP8266-Microcontroller-Development-MicroPython/dp/B0787LMTZD)
* 1 pcs. [Active Buzzer ARTC 4330](https://www.amazon.com/Cylewet-Electronic-Magnetic-Continuous-Arduino/dp/B01N7NHSY6/ref=sr_1_3?dchild=1&keywords=active+buzzer&qid=1593315364&s=electronics&sr=1-3)
* 1 pcs. [4 Pin Switch Button](https://www.amazon.com/6x6x4-5mm-Momentary-Tactile-Button-Switch/dp/B008DS1GY0)
* 1 pcs. 1K Ω Resistor
* 1 pcs. 220 Ω Resistor (can differ based on volume preference)
* 5 pcs. [Male-Male Jumper Cables](amazon.com/Solderless-Flexible-Breadboard-Jumper-Arduino/dp/B00ARTWJ44)
* 1 pcs. [Breadboard](https://www.amazon.com/Breadboards-Solderless-Breadboard-Distribution-Connecting/dp/B07DL13RZH/ref=sr_1_5?dchild=1&keywords=Breadboard+Arduino&qid=1593316288&sr=8-5)
* 1 pcs. [USB A-Male to Micro B Charger](https://www.amazon.com/AmazonBasics-Male-Micro-Cable-Black/dp/B072J1BSV6/ref=sr_1_3?dchild=1&keywords=USB+to+Micro+USB+Cable&qid=1593316313&sr=8-3)

Note: I recommend to purchase these items from a vendor such as AliExpress or Wish, as they have cheaper prices. The items will take longer to arrive from these places because they are shipped overseas.

I think I spent around \$10 on all of the parts, excluding the Breadboard and cables which I had from earlier projects. If this is your first project, expect to spend no more than \$30, and you will have leftover parts to re-use!

## Website Interface
![](https://i.imgur.com/2lVmfyS.png)
## Physical... Interface? The button is hard to press
![](https://i.imgur.com/7lss5XY.jpg)
## Wiring Schematic
Coming soon: need to find software.
## Future Plans
* Wire up a potentiometer for volume selection
* Add an RTC module to remove NTP clock dependency (if my WiFi goes down, I would still like to wake up!)
* Upgrade the active buzzer to a full speaker to play more than 8 bit tunes
* Learn CAD to design an enclosure and get it printed at a local library