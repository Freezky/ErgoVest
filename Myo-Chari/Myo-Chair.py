import myo as libmyo
import sys
import time
import serial

ser = serial.Serial('COM4', 19200, timeout=0)

libmyo.init('myo-sdk/bin')
feed = libmyo.device_listener.Feed()
hub = libmyo.Hub()
hub.run(1000, feed)

nowClock = 0
afterClock = 0
counter = 0

try:
    i = 0;
    # Connect to Myo Device
    myo = feed.wait_for_single_device(timeout=10.0)  # seconds
    if not myo:
        print("No Myo connected after 10 seconds.")
        ser.close()
        sys.exit()

    # while connected and running
    while hub.running and myo.connected:
        afterClock = time.clock()
        if counter == 0:
            if ser.read().decode("utf-8") == "v":
                nowClock = time.clock()
                myo.vibrate("short")  # Send Vibration
                ser.flush()
                counter += 1
                print("Vibrate1")
                print(nowClock - afterClock)
        if ser.read().decode("utf-8") == "v" and counter >= 1 and abs(nowClock - afterClock) > 7:
            myo.vibrate("short")  # Send Vibration
            nowClock = time.clock()
            ser.flush()
            counter +=1
            print("Vibrate2")
        if i >= 100:        # To Limit Loop REMOVE
            hub.shutdown()  # To Limit Loop REMOVE
            ser.close()     # To Limit Loop REMOVE
            sys.exit()      # To Limit Loop REMOVE
        time.sleep(1)
        i += 1;             # To Limit Loop REMOVE




except KeyboardInterrupt:
    print("Quitting...")
finally:
    hub.shutdown()