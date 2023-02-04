# autonomous_task_stereo_cam
 
 The directories are as follows:
 
 **car_side_esp32_sketch** contains the sketch that goes on the esp32 that is connected to the car.
 
 **esp32_webcam** is the sketch that goes on the webcam. 
 
**maskrcnn_python_notbook** is the python notebook that implements the maskrcnn model

**yolov8_python_notebook** is the python notebook that implements the yolov8 model.

_NOTE:_ the esp32 sketches use static IP. Only need to set the last number. So in the below, you would only need to change **181**.
You can get the gateway by running *ipconfig* at the console.

```
IPAddress local_IP(192, 168, 1, **181**);
// Set your Gateway IP address
IPAddress gateway(192, 168, 1, 1);

IPAddress subnet(255, 255, 0, 0);
```

Previous model is here: https://github.com/jonathanrandall/esp32_stereo_camera 


