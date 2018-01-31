# Embedded CTF Development Flags
This repository contains an example project for MITRE's 2018
[Embedded System CTF](http://mitrecyberacademy.org/competitions/embedded/).
Participating teams can use this example to obtain "Development Flags" that 
demonstrate that they have reached certain milestones in setting up their 
development environment. 
While it may be possible to extract these flags WITHOUT setting up a 
development environment, the flags have been hidden with the intent of 
making reverse-engineering a more difficult process than simply obtaining
the flags as intended.

# Disclaimer
This code is insecure and does not meet MITRE standards for
quality. This code is being provided for educational purposes to serve as a
simple example for the 2018 MITRE eCTF competition. Use this code at your own
risk!

# Dependencies
After cloning this repository with git, make sure to run the following command
to pull in the crypto library that this project depends on:
```sh
git submodule update --init
```

# Server
The address of the server for the third flag will be communicated through
email and Slack.

# Potentially useful `socat` commands
For an interactive serial communication session with the PSoC:
```socat -d -d -lf /dev/stdout -x -v 2> dump1.txt  "READLINE,history=$HOME  /s.hist" /dev/ttyS3,b115200,raw,echo=0```

To bridge PSoC and TCP server communications:
```socat -d -d -lf /dev/stdout -x -v 2> dump2.txt /dev/ttyS3,b115200,raw,echo=0 TCP:127.0.0.1:1337```

Both commands log all traffic to a file (including direction of communication and timestamps).  Both commands may need to be modified to account for your system's specific serial COM port and the network address you are trying to communicate with.
