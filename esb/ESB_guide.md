# ESP8266 guide

## Links
### Useful guide 
https://medium.com/@cgrant/using-the-esp8266-wifi-module-with-arduino-uno-publishing-to-thingspeak-99fc77122e82

### AT commands at
https://learn.acrobotic.com/datasheets/ESP8266_AT_Commands.pdf

## Used
cutecom or arduinoIde serial monitor
-- could not set easily picocom or minicom to use CR+LF (or other unidentified problem)


# Useful AT commands

## AT 
	Test if all Ok (Returns ok)

## AT+CWMODE?
	See current mode: 1. Station (Connect to AP)
								 2. AP
								 3. Station and AP

Using mode 1

## AT+GMR
	View versions

## AT+CWLAP
Find networks and list them

## AT+CIFSR
Show ip of station and AP

## AT+PING
Do a ping

## AT+CWJAP_CUR="abc","0123456789"
Connect to access point

## AT+CWJAP_DEF="abc","0123456789"
Connect to access point and save as default
Using this for local WiFi


# Working Get commands

	AT+CIPSTART="TCP","<IP>",8000
	AT+CIPSEND=16
	GET / HTTP/1.1
	IPD,4
	AT+CIPCLOSE
