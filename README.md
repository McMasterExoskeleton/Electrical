# Electrical
McMaster Exoskeleton Electrical Repo

Follow the link below for detailed instructions on how to connect your raspberrypi to your laptop or to a monitor set up with wired keyboard and mouse peripherals (anything with a usb connection).

[https://docs.google.com/document/d/1ZM4OjxhLERR3PYTnz-XncFo0uSq0vDOrpFr3oSLz7uA/edit?tab=t.0#heading=h.ij9ww1wes34w
](url)

Once your raspberrypi is connected to your laptop, you need to wire the sensor. The driver file currently in this repo is configured to just read values from one sensor (as of January 29 2025). This will be updated, but for now just wire one sensor to the pi using the directions in the document below.

[https://docs.google.com/document/d/11rxs6Ok0OJzUIqv6gfYxtGaZftekOBfRPw3bDT9AFXY/edit?tab=t.0
](url)

Once the sensor is wired to the pi, you can double check to make sure it is correctly configured by running the command sudo i2cdetect -y 1 in the command window of the pi. It should show the address as x29. Now you're good to start downloading the necessary files and running the driver file. The easiest way to access what you need is by logging on to Github using the browser on the pi and downloading the bno05 header file, c file (containing the api) and the driver.cpp file. You can open these in the Geany editor which should already be on the pi if you want to make changes to the files. Making sure they're all in the same folder, you can now execute the following command in the terminal:

g++ -o drivertest driver.cpp bno055.c -lwiringPi

This should compile and execute the driver file which will print all 8 types of data with a delay between each set of measurements.

Happy sensing lol




