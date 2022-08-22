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
    password = request.form["password"]
    if(checkPassword(password)):
        date = str(dt.now())
        temp = str(request.form["temp"])

        f = open("tempData.csv", "a")
        f.write(f"{date},{temp}\n")
        f.close()
    return "poop"
@app.route("/dataOutage", methods=["POST"])
def dataOutage():
    password = request.form["password"]
    if(checkPassword(password)):
        file = open("outages.csv","a")
        lastRecordTime = getLastTempRecord()[0]
        currTime = dt.now()
        numFails = request.form["failed sends"]
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
app.run()