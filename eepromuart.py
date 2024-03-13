import serial
import time

# Configure the serial port
ser = serial.Serial('COM5', 2400)  # Replace with your serial port and baud rate
time. sleep(2)
passage = "Finance Minister Arun Jaitley Tuesday hit out at former RBI governor Raghuram Rajan for predicting that the next banking crisis would be triggered by MSME lending, saying postmortem is easier than taking action when it was required. Rajan, who had as the chief economist at IMF warned of impending financial crisis of 2008, in a note to a parliamentary committee warned against ambitious credit targets and loan waivers, saying that they could be the sources of next banking crisis. Government should focus on sources of the next crisis, not just the last one. In particular, government should refrain from setting ambitious credit targets or waiving loans. Credit targets are sometimes achieved by abandoning appropriate due diligence, creating the environment for future NPAs, Rajan said in the note. Both MUDRA loans as well as the Kisan Credit Card, while popular, have to be examined more closely for potential credit risk. Rajan, who was RBI governor for three years till September 2016, is currently.#"
ser.write(passage.encode('utf-8'))
print("Sent the passage through serial port")
time. sleep(2)

received_data = ser.readline().decode('utf-8').strip()
print(f"Baudrate for recieving at MCU: {received_data} bits\s")

received_data = ser.readline().decode('utf-8').strip()
print(f"Recieved data sent from MCU: {received_data}")

received_data = ser.readline().decode('utf-8').strip()
print(f"Baudrate for transmiting at MCU: {received_data} bits\s")

ser.close()
