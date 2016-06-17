import paho.mqtt.client as mqtt

mqttc = mgtt.Client("python_pub")
mqttc.connect("localhost", 1833)
mqttc.publish("test/topic", "hello MQTT")
mqttc.loop(2)
