import pymongo # meng-import library pymongo yang sudah kita install
import datetime
from flask import Flask
from flask import request
from flask import jsonify
app = Flask(__name__)

uri = f"mongodb+srv://ferromagnaperdana:cobacobaberhadiah510@gettingstartedsic5.2tdilmx.mongodb.net/?retryWrites=true&w=majority&appName=GettingStartedSIC5"

# Database & Collection Configuration
client = pymongo.MongoClient(uri)
db = client['air_quality_v1'] # ganti sesuai dengan nama database kalian
my_collections = db['device_1'] # ganti sesuai dengan nama collections kalian

# Send a ping to confirm a successful connection
try:
    client.admin.command('ping')
    print("Pinged your deployment. You successfully connected to MongoDB!")
except Exception as e:
    print(e)

# Gather data from ESP with HTTP within same local network
@app.route('/sensor/',methods=['POST'])
def receive_data():
    if request.method == 'POST':
        temperature = request.form['temperature']
        humidity = request.form['humidity']
        mq135 = request.form['mq135']
        pm25 = request.form['pm25']

        response = {
        'message': 'Data received successfully',
        'temperature': temperature,
        'humidity': humidity,
        'mq135': mq135,
        'pm25': pm25
        }

        document = {
        'temperature': temperature,
        'humidity': humidity,
        'mq135': mq135,
        'pm25': pm25,
        'timestamp': datetime.datetime.utcnow()
        }

        # Push data to MongoDB
        result = my_collections.insert_one(document)

    return jsonify(response), 200


# Get data from MongoDB
#for x in my_collections.find():
#    print(x)


if __name__ == '__main__':
    app.run(host='0.0.0.0')