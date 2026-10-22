# Megasumo_V3
3 KG Megasumo robot built from scratch, fits in a 20x20cm bounding box, and features a 3D-printed chassis designed from scratch.

Hardware:
-ESP32: Microcontroller & Brain of the Sumobot
-VL53L1X: Front sensors of the robot. They are TOF-based laser sensors that detect distance and return analog readings.
-SHARP IR: Side sensors of the robot, IR-based sensors that have a conic working envelope, also return distance as analog readings.
-QRE1113: IR-based reflectance sensors which are used on the bottom front corners of the robot to detect if the robot is about to exit the ring. Returns digital values in response to either black or white.
-JGB37-545: Mid-speed 12V DC motors that run at 150rpm and produce 7.5kg/cm of torque per motor
-PCBFun 60A Motor Driver: Relatively new, and documentation may still be lacking. It is a dual-channel motor driver with a rated peak current of 60A and comes with a 5v and GND pin to power the microcontroller

About The Robot:
The sumobot was designed to compete in an open 3kg category, with a maximum size of 20 cm x 20 cm. Its main functions on the ring include enemy detection using its 3 laser sensors to track and push out enemies from the ring. It also uses its bottom sensors effectively to reverse when it detects white near the edges of the ring.

During the competition, our team was able to beat 2 other robots before being eliminated from the competition by a different sumobot. The experience of building a robot from scratch and using it to compete was very fun, and we learnt many new tips in regard to making the sumobot. Our competition was full of high-speed sumobot builds, focusing more on enemy detection and pushing rather than ring detection, which also made us rethink how we would build the next design for a different competition.
