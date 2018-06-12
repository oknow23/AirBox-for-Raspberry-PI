import sys

from linebot import LineBotApi
from linebot.models import TextSendMessage
line_bot_api = LineBotApi('你的Channel access token (long-lived) ')

msg=sys.argv[1]

line_bot_api.push_message('Your user ID', 
     TextSendMessage(text=msg))