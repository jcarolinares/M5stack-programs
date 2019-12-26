# Office M5Stack assistant

The office M5Stack assistant is project made in Arduino for the M5Stack fire.

It's a tool to have at your desk office, as an indicator that you desire to not
to be interrupted or they can ask you things.

I used for a whole year and for me has been so useful.

Some of the features:

* Free -> People can interrupt your work -> Put the M5Stack vertical.
* Busy -> You need to be focused -> Please don't interrupt my work. -> Turn the M5 Stack 180º.
* Lunch -> You are not at your desk, it's lunch time. Please come back later. If you connect your M5stack to your PC and setup and launch the *lockscreen_script.py* you can automatically block your PC screen at the same time. -> Press button A.

* Pomodoro timer -> If you use [pomodoro timers](https://en.wikipedia.org/wiki/Pomodoro_Technique) to work hard, this option creates a pomodoro timer of 20 minutes in your M5 Stack. -> Press button B.

* Lock one hour in my Office Calendar -> Great for managers. If you need to quickly block one hour in your calendar to avoid meetings and have the work done, you can connect your M5Stack to your wifi and use IFTTT and your Google or Microsoft Calendar to quickly create and event of one hour. -> Press button C.

***
## Setup

This is an Arduino project made on [plattformio](https://platformio.org). Just install plattformio on your favourite IDE and compile the Arduino code of the project. Everything will be automatically and properly setup.

### Wifi and IFTTT integration

To properly setup the M5Stack with your wifi put the following **private** information if your network in the following *#define* inside *main.cpp*:

* #define WIFI_SSID " YOUR SSID"
* #define WIFI_PASSWORD "YOUR PASSWORD"

To properly connect your M5stack to IFTTT, you have yo put your Webhook URL for your Action in the *#define*:

* #define IFTTT_URL "WEBHOOK URL"

***

<a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/"><img alt="Creative Commons License " style="border-width:0" src="https://i.creativecommons.org/l/by-sa/4.0/88x31.png" /></a><br /><span xmlns:dct="http://purl.org/dc/terms/" property="dct:title">Free Busy Pomodoro M5 Stack</span> by <span xmlns:cc="http://creativecommons.org/ns#" property="cc:attributionName">Julián Caro Linares</span> licensed by <a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/">Creative Commons Attribution-ShareAlike 4.0 International License</a>.<br /><br />
