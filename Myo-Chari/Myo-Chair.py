import myo as libmyo
import sys
import time
import serial

ser = serial.Serial('COM4', 9600, timeout=0)

libmyo.init('myo-sdk/bin')
feed = libmyo.device_listener.Feed()
hub = libmyo.Hub()
hub.run(1000, feed)


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
        if ser.bytesize == 8:
            print(ser.read())
            myo.vibrate("medium")   # Send Vibration
            time.sleep(1)           # Wait 1 second
        if i >= 15:         # To Limit Loop REMOVE
            hub.shutdown()  # To Limit Loop REMOVE
            ser.close()     # To Limit Loop REMOVE
            sys.exit()      # To Limit Loop REMOVE
        i += 1;             # To Limit Loop REMOVE

except KeyboardInterrupt:
    print("Quitting...")
finally:
    hub.shutdown()