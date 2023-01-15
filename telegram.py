import time
import telepot
from telepot.loop import MessageLoop

from auth import token, recievers


class Telegram:
    def __init__(self):
        self.bot = telepot.Bot(token)
        self.initialised = False
        self.bot_id = 5981943769

        self.debug_mode = False

        print(self.bot.getMe())

        self.targeted_reciever = recievers
        MessageLoop(self.bot, self.handle).run_as_thread()

    def msg_me(self, message):
        self.bot.sendMessage(self.bot_id, message)

    def handle(self, msg):
        actual_message = msg['text']
        if(actual_message == '/ping'):
            self.bot.sendMessage(msg['from']['id'], 'ping recieved!')
        elif(actual_message == '/init'):
            self.bot.sendMessage(msg['from']['id'], 'communication initialised!')
            self.initialised = True
        elif(actual_message == '/deinit'):
            self.bot.sendMessage(msg['from']['id'], 'communication deinitialised!')
            self.initialised = False
        elif(actual_message == '/debug_on'):
            self.bot.sendMessage(msg['from']['id'], 'communication deinitialised!')
            self.debug_mode = True
        elif(actual_message == '/debug_off'):
            self.bot.sendMessage(msg['from']['id'], 'communication deinitialised!')
            self.debug_mode = False
        else:
            self.bot.sendMessage(msg['from']['id'], 'Unknown command!')


    def msg_all(self, message):
        for ID in self.targeted_reciever:
            self.bot.sendMessage(ID, message)

    

# bott = Telegram()
# bott.msg_all('Service starting after system restart!\n For help type: /help')

# while True:
#     time.sleep(1)
#     if bott.initialised:
#         bott.msg_all("test_message")