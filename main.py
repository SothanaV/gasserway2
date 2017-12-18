from flask import Flask, render_template, request, jsonify     			
from flask_socketio import SocketIO, send, emit									

app = Flask(__name__)
app.config['SECRET_KEY'] = 'secret!'
socketio = SocketIO(app)
global command1
global command2
global command3
command1 = "S"
command2 = "S"
command3 = "S"
@app.route("/data1/<gas1>/")
def getdata1(gas1):
	global command1
	socketio.emit('s2c_1',gas1)
	return command1
@app.route("/data2/<gas2>/")
def getdata2(gas2):
	global command2
	socketio.emit('s2c_2',gas2)
	return command2
@app.route("/data3/<gas3>/")
def getdata3(gas3):
	global command3
	socketio.emit('s2c_3',gas3)
	return command3

@socketio.on('c2s_com1')
def getcom1(com1):
	global command1
	command1 = com1
	print(com1)
	return command1
@socketio.on('c2s_com2')
def getcom2(com2):
	global command2
	command2 = com2
	print(com2)
	return command2
@socketio.on('c2s_com3')
def getcom3(com3):
	global command3
	command3 = com3
	print(com3)
	return command3


	
@app.route("/home")
def home():
	return render_template('home.html')
@app.route("/control")
def contr():
	return render_template('control.html')
@app.route("/control2")
def contr2():
	return render_template('control2.html')
@app.route("/control3")
def contr3():
	return render_template('control3.html')