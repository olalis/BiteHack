
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
        # self.pulse: float = blanket_data_list[5]
        self.help_me_button: bool = blanket_data_list[5]

        

    def proces_data(frame_data):
        pass

        #return [temp1, temp2, temp3, bar, hum, pulse, help]

    def __str__(self):
        text = "Dane pobrane z kocyka:\n"
        text += f"Termometr1: {self.thermometr1}\n"
        text += f"Termometr2: {self.thermometr2}\n"
        text += f"Termometr3: {self.thermometr3}\n"
        text += f"Barometr:   {self.barometer}\n"
        text += f"Wilgotność: {self.air_humidity}\n"
        # text += f"Puls:       {self.pulse}"
        text += f"HelpMe:     {self.help_me_button}\n"

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
        temp_blanket_data_list.append(struct.unpack('>f', val[7:3:-1])[0]) #Termomert1
        temp_blanket_data_list.append(struct.unpack('>f', val[11:7:-1])[0]) #Termomert2
        temp_blanket_data_list.append(struct.unpack('>f', val[15:11:-1])[0]) #Termomert3
        temp_blanket_data_list.append(struct.unpack('>f', val[19:15:-1])[0]) #Barometr
        temp_blanket_data_list.append(struct.unpack('>f', val[23:19:-1])[0]) #Wilgtnosc
        temp_blanket_data_list.append(struct.unpack('>B', val[24:23:-1])[0]) #helpMe
        # temp_blanket_data_list.append(struct.unpack('>f', val[25:])[0]) #ADC

    return temp_blanket_data_list

while 1:
    with serial.Serial('/dev/ttyACM0', 115200, timeout = None) as ser:
        try: 
            x = ser.read(25)
            # print(str(x))
            if x[0:4] == b'\xaa\xaa\xaa\xaa':
                data = BlanketData(processData(x))
                print(data)
            else:
                print("Error!!!!!!")

        except serial.SerialException:
            pass
