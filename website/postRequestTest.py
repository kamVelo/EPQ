import requests as rq

d = {"passwsord" : "theySpeakEnglishInWhat?", "temp": -10}
rq.post("http://www.growhab.net/inputTempData", data=d)