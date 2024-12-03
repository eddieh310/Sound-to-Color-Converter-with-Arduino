import serial
import numpy as np
from scipy.fft import fft
import time
import random

# this file contains two methods, main and compute_dominant_frequency
# the main method is repsonsible for establishing a serial connection to the arduino
# since the arduino is connnected through usb, this works well
# the compute dominant frequency method computes dom freq based on the data we just streamed in 

# refrences:
# https://docs.scipy.org/doc/scipy/tutorial/fft.html
# https://pyserial.readthedocs.io/en/latest/pyserial.html
# https://numpy.org/



def compute_dominant_frequency(data, sampling_rate):
    # here i perform the fft operations
    fft_result = fft(data) # this method does 1-d discrete fft
    magnitude = np.abs(fft_result) # get magnitude
    # Only consider the positive frequencies
    half_length = len(magnitude) // 2 # divide in half bc 50/50 pos and neg freqs
    magnitude = magnitude[:half_length]
    # now we go from magnitude to frequency
    frequencies = np.fft.fftfreq(len(fft_result), d=1/sampling_rate)[:half_length] # this line generates array of frequencies that correspond to the bins in the FFT result that are positive
    
    # Find the dominant frequency
    dominant_index = np.argmax(magnitude)
    dominant_frequency = frequencies[dominant_index]
    return dominant_frequency


def main():
    serial_port = 'COM3' 
    baud_rate = 115200  # baud rate , this mathces with arduino baud that i set 
    sampling_rate = 4000  # 4 kHz 
    samples_to_collect = 256  # FFT size

    try:
        ser = serial.Serial(serial_port, baud_rate, timeout=1) # create serial connection object 
        time.sleep(2)  # wait for serial connection to initialize
        print("Connected to Arduino on", serial_port)
    except serial.SerialException as e:
        print("error on opening port")
        return

    buffer = []
    try:
        while True:
            if ser.in_waiting > 0:
                # read mic input from arduino 
                line = ser.readline().decode('utf-8', errors='ignore').strip()
                if line.isdigit():  # cheese way to make sure we received a mic value
                    sample = int(line)
                    buffer.append(sample)
                    if len(buffer) == samples_to_collect:
                        # make it into numpy array to work with scipy
                        data = np.array(buffer)
                        buffer = []  # Reset buffer
                        # compute dominant frequency
                        dominant_freq = compute_dominant_frequency(data, sampling_rate)
                        # send the frequency back to Arduino
                        ser.write(f"{dominant_freq}\n".encode())

                else:
                    print("Invalid data received: ",line)

    except KeyboardInterrupt:
        print("Exiting program.")
    finally:
        ser.close()


# these are 3 helper methods for testing the effectivness of serial connnection
def create_serial_connection(port, baud_rate=115200):
    try:
        ser = serial.Serial(port, baud_rate, timeout=1)
        time.sleep(2)  # Wait for serial connection to initialize
        print(f"Connected to Arduino on {port}")
        return ser
    except serial.SerialException as e:
        print(f"Error opening serial port {port}: {e}")
        return None

def send_frequency(ser, frequency):
    print(f"Sending frequency: {frequency} Hz")
    ser.write(f"{frequency}\n".encode())

def test_led_colors():
    # this method can be called in order to test some dummy values and ensure the serial
    # connection is working properly 
    serial_port = 'COM3'
    ser = create_serial_connection(serial_port)
    if ser is None:
        return
    test_frequencies = [60, 150, 250, 400, 500, 700]  # Test different frequencies
    for freq in test_frequencies:
        print("loading..... ")
        send_frequency(ser, freq)
        time.sleep(2)  
    ser.close()

if __name__ == "__main__":
  # test_dom_freq(250)
  main()
  #test_led_colors()
