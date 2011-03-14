import serial
import time
import matplotlib.pyplot as p
import numpy.numarray as na
import os
def chart(array):
    os.system("clear")
    top = 3000 # max(array)
    out = ""
    for x in range(100):
        out +=" "
    out += str(top)
    print out
    
    out = ""
    for x in range(100):
        out +="-"
    print out
    
    for sensor in array:
        top = top/1.0
        value = min(100, sensor/top * 100)
        out = ""
        for x in range(int(value)):
            out += "#"
        print out

try:
    s = serial.Serial("/dev/tty.usbserial-A700dWpY")
 
    print "Opening" + s.portstr
    s.readline()
    
    #s = [0,0,0,0,0]
    #n = 0
    
    while 1:
        line =  s.readline()
        readings = line.split(",")
        readings = [float(x) for x in readings[1:]]
        
        #for i in range(5):
            
        
        #labels = ["1","2","3","4","5"]
        chart(readings)
except serial.SerialException:
    print "Did you connect the arduino?"


    
    
