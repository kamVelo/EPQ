from flask import Flask, render_template,redirect,request
from keys import checkPassword
from datetime import datetime as dt
app = Flask(__name__)


@app.route("/inputTempData/",methods=["POST"])
def inputTempData():
    password = request.form["password"]
    if(checkPassword(password)):
        date = str(dt.now())
        temp = str(request.form["temp"])

        f = open("tempData.csv")
        f.write(f"{date},{temp}\n")
        f.close()

app.run()