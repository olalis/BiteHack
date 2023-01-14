import serial
import struct

class BlanketData:
    def __init__(self, blanket_data_list) -> None:
        # temp_blanket_data_list: list[float, bool] = self.proces_data(data_frame)

        self.thermometr1: float = blanket_data_list[0]
        self.thermometr2: float = blanket_data_list[1]
        self.thermometr3: float = blanket_data_list[2]
        self.barometer: float = blanket_data_list[3]
        self.air_humidity: float = blanket_data_list[4]
        self.pulse: float = blanket_data_list[5]
        self.help_me_button: bool = blanket_data_list[6]

        

    def proces_data(frame_data):
        pass

        #return [temp1, temp2, temp3, bar, hum, pulse, help]

    def __str__(self):
        text = "Dane pobrane z kocyka:\n"
        text += f"Termometr1: {self.thermometr1}"
        text += f"Termometr2: {self.thermometr2}"
        text += f"Termometr3: {self.thermometr3}"
        text += f"Barometr:   {self.barometer}"
        text += f"Wilgotność: {self.air_humidity}"
        text += f"Puls:       {self.pulse}"
        text += f"HelpMe:     {self.help_me_button}"

        return text
    
    def process_temperature(self):
        pass


    def emit_help_me_signal(self):
        if self.help_me_button:
            pass
            #włącz ledy i buzzer 


    def detect_degner(self):
        #######PARAMS########
        treshold_barometer: float = 0
        treshold_air_humadity: float = 0
        treshold_pulse: float = 0
        treshold_temperature: float = 0
        ####################

        if self.barometer >= treshold_barometer:
            pass

        if self.air_humidity >= treshold_air_humadity:
            pass

        if self.pulse >= treshold_pulse:
            pass

class ADCdata:
    def __init__(self) -> None:
        pass

def processData(val: bytes) -> list:
    # val = b'\xaa\xaa\xaa\xaa\x41\xc2\x66\x66\x41'
    temp_blanket_data_list = []
    if val[0:4] == b'\xaa\xaa\xaa\xaa':
        temp_blanket_data_list.append(struct.unpack('>f', val[4:8])[0]) #Termomert1
        temp_blanket_data_list.append(struct.unpack('>f', val[8:12])[0]) #Termomert2
        temp_blanket_data_list.append(struct.unpack('>f', val[12:16])[0]) #Termomert3
        temp_blanket_data_list.append(struct.unpack('>f', val[16:20])[0]) #Barometr
        temp_blanket_data_list.append(struct.unpack('>f', val[20:24])[0]) #Wilgtnosc
        temp_blanket_data_list.append(struct.unpack('>b', val[24])[0]) #helpMe
        # temp_blanket_data_list.append(struct.unpack('>f', val[25:])[0]) #ADC

    return temp_blanket_data_list

while 1:
    with serial.Serial('/dev/ttyS0', 9600, timeout = None) as ser:
        try: 
            x = ser.read()
            # print(str(x))
            data = BlanketData(processData(x))
            print(data)

        except serial.SerialException:
            pass
