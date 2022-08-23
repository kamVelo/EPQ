from flask import Flask, render_template,redirect,request
from keys import checkPassword
from datetime import datetime as dt
import os
app = Flask(__name__)

@app.route("/")
@app.route("/index/")
def home():
    return "This does nothing"

@app.route("/inputTempData",methods=["POST"])
def inputTempData():
    data = list(request.form.items())
    password = data[0][1]
    if(checkPassword(password)):
        date = str(dt.now())
        temp = str(data[1][1])

        f = open("tempData.csv", "a")
        f.write(f"{date},{temp}\n")
        f.close()
    return str((password, checkPassword(password)))

@app.route("/inputECData",methods=["POST"])
def inputECData():
    data = list(request.form.items())
    password = data[0][1]
    if(checkPassword(password)):
        date = str(dt.now())
        EC = str(data[1][1])

        f = open("ECData.csv", "a")
        f.write(f"{date},{EC}\n")
        f.close()
    return str((password, checkPassword(password)))


@app.route("/dataOutage", methods=["POST"])
def dataOutage():
    data = list(request.form.items())
    password = data[0][1]
    if(checkPassword(password)):
        file = open("outages.csv","a")
        lastRecordTime = getLastTempRecord()[0]
        currTime = dt.now()
        numFails = data[1][1]
        file.write(f"{lastRecordTime},{currTime},{numFails}\n")
        file.close()
    return "poop"

def getLastTempRecord():
    """
    This returns the last temperature record as a list
    :return: [datetime,temperature]
    """
    line = ""
    with open("tempData.csv", "rb") as file:
        file.seek(-2, os.SEEK_END)
        while file.read(1) != "/n":
            file.seek(-2,os.SEEK_CUR)
        line = file.readline().decode()
    return line.split(",")

if __name__ == "__main__":
    app.run()