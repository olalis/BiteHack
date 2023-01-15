
import serial
import struct
from scipy.fft import fft, fftfreq
import numpy as np
import csv
from typing import List
import math

from telegram import Telegram

class ADCdata:
    def __init__(self) -> None:
        self.ADC_data_counter: int = 0 # max 15 cykli
        self.ADC_data_list: List[float] = []
        self.pulse: float = None
        

    def add_new_data(self, new_data: List[float]):
        if self.ADC_data_counter <= 15:
            self.ADC_data_list += new_data
            self.ADC_data_counter += 1

            if self.ADC_data_counter == 15:
                self.calculate_pulse
                self.ADC_data_counter = 0
                self.ADC_data_list = []
    

    def calculate_pulse(self) -> None:
        N = len(self.ADC_data_list)

        # sample spacing
        T = 1.0 / 100
        x = np.linspace(0.0, N*T, N, endpoint=False)

        yf = fft(self.ADC_data_list)
        xf = fftfreq(N, T)[:N//2]*6

        new_yf = yf[500:2000]
        self.pulse = xf[new_yf.argmax(axis=0) + 500]


class BlanketData:
    def __init__(self, telegram_bot = None) -> None:
        # temp_blanket_data_list: list[float, bool] = self.proces_data(data_frame)

        # self.thermometr1: float = blanket_data_list[0]
        # self.thermometr2: float = blanket_data_list[1]
        # self.thermometr3: float = blanket_data_list[2]
        # self.barometer: float = blanket_data_list[3]
        # self.air_humidity: float = blanket_data_list[4]
        # self.help_me_button: bool = blanket_data_list[5]
        # self.pulse: float = blanket_data_list[5]

        self.thermometr1: float = None
        self.thermometr2: float = None
        self.thermometr3: float = None
        self.barometer: float = None
        self.air_humidity: float = None
        self.help_me_button: bool = None
        self.pulse: ADCdata = ADCdata()

        self.telebot = telegram_bot

        self.signal_2_send = None

        #####
        self.hight_above_sea_level: float = None

    def get_serial_signal_to_send(self):
        return self.signal_2_send
        
    def update_data(self,blanket_data_list):
        self.thermometr1: float = blanket_data_list[0]
        self.thermometr2: float = blanket_data_list[1]
        self.thermometr3: float = blanket_data_list[2]
        self.barometer: float = blanket_data_list[3]
        self.air_humidity: float = blanket_data_list[4]
        self.help_me_button: bool = blanket_data_list[5]
        self.pulse.add_new_data(blanket_data_list[6])

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
        if self.pulse.pulse is not None:
            text += f"Puls:       {self.pulse}"
        text += f"HelpMe:     {self.help_me_button}\n"
        text += f"========================="
        text += f"Wysokość:   {self.hight_above_sea_level}\n"

        return text
    
    def process_temperature(self):
        self.signal_2_send = 1

    def process_presure(self, data):
        ACTUAL_TEMP = self.thermometr3 # 27.1
        ACTUAL_PRES = self.barometer #990.0

        p0 = 1013.25
        temp = ACTUAL_TEMP + 273.15
        p = ACTUAL_PRES
        self.hight_above_sea_level= -29.271769 * temp * math.log(p / p0)


    def emit_help_me_signal(self):
        if self.help_me_button:
            if self.telebot is not None:
                self.telebot.msg_all('POMOCY!!!!111OneOne')
            #włącz ledy i buzzer 


    def detect_degner(self):
        #######PARAMS########
        treshold_barometer: float = 0
        treshold_air_humadity: float = 0
        treshold_pulse: float = 0
        treshold_temperature: float = 0
        ####################
        
        if self.help_me_button:
            self.emit_help_me_signal()

        if self.barometer >= treshold_barometer:
            pass

        if self.air_humidity >= treshold_air_humadity:
            pass

        #if self.pulse is not None:
        #    if self.pulse.pulse >= treshold_pulse:
        #       pass


       # if self.pulse >= treshold_pulse:
       #     pass
    
    def transmit_debug_data(self):
        self.telebot.msg_all(f'''termometr 1: {self.thermometr1}\n
                                 termometr 2: {self.thermometr2}\n
                                 termometr 3: {self.thermometr3}\n
                                 barometr: {self.barometer}\n
                                 wilgotność: {self.air_humidity}\n''')
        




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
        
        temp_ADC_data_list = []
        for i in range(0,400,2):
            print(val[28:24:-1])
            temp_ADC_data_list.append(struct.unpack('>H', val[26+i:24+i:-1])[0]) #ADC
        
        temp_blanket_data_list.append(temp_ADC_data_list)

    return temp_blanket_data_list


bott = Telegram()
bott.msg_all('Service starting after system restart!\n For help type: /help')
data = BlanketData(bott)

blanket_data_counter = 0
blanket_data_counter_resetter = 10

debug_mode = True

while 1:
    with serial.Serial('/dev/ttyACM0', 115200, timeout = None) as ser:
        try: 
            x = ser.read(425)
            print(str(x))
            if x[0:4] == b'\xaa\xaa\xaa\xaa':
                data.update_data(processData(x))
                print(data)
                data.detect_degner()
                temp_write_sg = data.get_serial_signal_to_send()
                if temp_write_sg is not None:
                    ser.write(data.get_serial_signal_to_send())

                if debug_mode:
                    if blanket_data_counter == blanket_data_counter_resetter:
                        blanket_data_counter = 0
                        data.transmit_debug_data()


            else:
                print("Error!!!!!!")
                print(x[0:4])
                print("koniec errora")
        except serial.SerialException:
            pass
