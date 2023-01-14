import serial

class BlanketData:
    def __init__(self, data_frame: str) -> None:
        temp_blanket_data_list: list[float, bool] = self.proces_data(data_frame)

        self.thermometr1: float = temp_blanket_data_list[0]
        self.thermometr2: float = temp_blanket_data_list[1]
        self.thermometr3: float = temp_blanket_data_list[2]
        self.barometer: float = temp_blanket_data_list[3]
        self.air_humidity: float = temp_blanket_data_list[4]
        self.pulse: float = temp_blanket_data_list[5]
        self.help_me_button: bool = temp_blanket_data_list[6]

        

    def proces_data(frame_data):
        pass

        #return [temp1, temp2, temp3, bar, hum, pulse, help]

    
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

        

while 1:
    with serial.Serial('/dev/ttyS0', 9600, timeout = None) as ser:
        x = ser.read()
        print(x)

