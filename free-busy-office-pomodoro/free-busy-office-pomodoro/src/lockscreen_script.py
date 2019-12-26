import pyautogui, time, subprocess
import serial

def main():

    # Uncomment the following line if you are having problems with pyautogui and Ubuntu 18.04
    # subprocess.call("xhost +", shell= True)

    # Remember that your serial port for the M5Stack could be different
    with serial.Serial('/dev/ttyUSB0', 115200, timeout=1) as ser:

        while(True):
            line = ser.readline()   # read a '\n' terminated line

            if (line == "lock\r\n"): #\r and \n are necessary
                print("Locking screen...")

                # You have to put here the Lock screen hotkey of your computer
                # Try to avoid the "COMMAND" key! In most of the graphical engines is an
                # special key that pyautogui is not able to properly handle
                pyautogui.hotkey('ctrlleft','alt', 'l')
            elif (line == "unlock\r\n"):
                print("Loggin screen...")
                pyautogui.hotkey('enter')

if __name__== "__main__":
  main()
