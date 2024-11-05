import board
import busio
import adafruit_bmp3xx
import time
i2c = busio.I2C(board.SCL, board.SDA)

bmp = adafruit_bmp3xx.BMP3XX_I2C(i2c)

bmp.sea_level_pressure = 1018.9

def read_sensor_data():
	temperature = bmp.temperature
	pressure = bmp.pressure
	altitude = bmp.altitude
	
	print(f"Temperatura {temperature:.2f} C")
	print(f"Cisnienie: {pressure:.2f} hPa")
	print(f"Wysokośc npm: {altitude:.2f} m")
	time.sleep(1)
	
while True:
	read_sensor_data()
