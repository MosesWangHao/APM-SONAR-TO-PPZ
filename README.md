## USE SRO4 SONAR IN PAPARAZZI PROJECT

SR04 sonar module + Arduino mini pro 

Also has a MB1043 driver for test, but that is a bit expensive to SR04. 
Add SRO4 lib to Arduino first before compile ApmSonar module.

All module is test in PPZ 5.8, you should config SONAR module first.


CONNETED
SR04 TRIG_PIN —> Arduino mini pro PIN3
SR04 ECHO_PIN —> Arduino mini pro PIN2
		  Arduino mini pro PINA4 —> PPZ AP I2CSDA
		  Arduino mini pro PINA4 —> PPZ AP I2CSCL



