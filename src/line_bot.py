import sys

from linebot import LineBotApi
from linebot.models import TextSendMessage
line_bot_api = LineBotApi('MN5xL58/dPw3ovF71cm8acMCOFMtlwtVhQmqM+yjjjqICMhAnEno72xXVHc7APNG8YeQl6gvyqkI5FsjZehBVbQ6gdh95QtHsX+5z39Mu96eOjPK1MyR6KnoejuoTUpUxS0EPFRCV6HFYjykpFnXkQdB04t89/1O/w1cDnyilFU=')

msg=sys.argv[1]

line_bot_api.push_message('Uff90fb80d93041f1083b2634237c41b1', 
     TextSendMessage(text=msg))