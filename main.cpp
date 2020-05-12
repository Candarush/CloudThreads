#include "FindCloud.hpp"
#include "MosqMQTTwrapper.hpp"
#include <iostream>
#include <string>

using namespace std;
using namespace MQTTW;

int main(){
    string image_path = "CloudSample.png";
    VideoCapture cap("CloudVideo.mp4");
    Mat compareMat = imread(image_path, IMREAD_COLOR);;
    CloudFinder cl(cap,5,5,4,compareMat);
    cl.ProcessCloudVideo();
    MosqMqttWrapper* MQTT = new MosqMqttWrapper("mqttclient", "localhost", 1883);
    mosqpp::lib_init();
    MQTT->send_message(cl.GetData().c_str());
    cin.get();
}
