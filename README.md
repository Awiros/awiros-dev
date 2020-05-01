<div align="center">
  <img src="https://user-images.githubusercontent.com/64395702/80786133-4d87e000-8ba0-11ea-9f09-c10713201b78.png">
</div>

<br/>[Awiros](www.awiros.com) is the ideal mechanism to leverage state-of-the-art Deep Learning Frameworks right from training to development to deployment at  scale. Awiros lets you leverage the science of Computer Vision and Artificial  Intelligence to analyze and interpret videos, converting them into actionable data for various use cases. Awiros seamlessly handles the development of apps to their consumption by the users, with unmatched flexibility and robustness.

- [Installation](#installation)
  - [Pre-Requisite](#pre-requisite)
  - [Setting Environment](#setting-environment)
  - [Configuration](#configuration)
- [Getting Started](#getting-started)
  - [Creating Application](#creating-application)
  - [Running Application](#running-application)
  - [Sample Application](#sample-application)

## Installation
### Pre-Requisite
- Any Linux based system (eg. Ubuntu, CentOS)
- For running GPU based applications, only Nvidia-GPU is compatible.
- [Docker](https://www.docker.com/)

### Setting Environment
- Download/Pull the docker image from [here]()
- Clone the repository in your home directory.
```
git clone https://github.com/Awiros/awiros-dev.git
```
- Create the docker container using the downloaded/pulled image.
  - Creating container with GPU support.
  ```
  docker run --runtime=nvidia --net=host -v /etc/machine-id:/etc/machine-id  -v $HOME/awiros-dev:/home/awiros-docker -p 3000-9000:3000-9000 --name <container-name> -it <image-name>
  ```
  - Creating container with only CPU support.
  ```
  docker run  --net=host -v /etc/machine-id:/etc/machine-id  -v $HOME/awiros-dev:/home/awiros-docker -p 3000-9000:3000-9000 --name <container-name> -it <image-name>

  ```
### Configuration
- Go into awiros-apps directory
```
cd awiros-dev/awiros-apps
```
- Give executable permissions
```
chmod a+x awiros
chmod +x configure
```
- Configure the environment
```
$ ./configure

node/test.js
Enter full name of your Awiros_js file : node/test.js

NAMES
<container-name>
Enter the name of your Docker container : <container-name>
```

## Getting Started
### Creating Application
- Run the below command.
```
$ ./awiros app create

==> creating app ..
Enter app name: awiros_<application-name>

Enter app ID: <any-random-number>

Enter app type (Default: basic): <press-Enter>
```
You have created a folder of your application in `awiros-dev/awiros-apps/apps` directory containing the cpp and header file. The app is registered with its app-id in `apps/app-registry.txt`.
- Building the application
```
$ ./awiros cmake apps
$ ./awiros make apps
$ ./awiros cmake
$ ./awiros make
```

### Running Application
- For running the application a video file or an rtsp url is required.
- Url/video file path need to be updated in `awiros-apps/node/new_updated_acs.json`
```
line 25: "url": "/home/awiros-docker/awiros-apps/<video-file-name>" OR "<rtsp-url>"
```
:information_source: Note: Please provide the path to video file with respect to the path inside docker container.
- Run the command to start the application.
```
./awiros run <application-name>
```
### Sample Application
- You are already provided with 2 applications.
  - Simple Camera app -- Application to stream video on your browser.
  - Crowd Cownt app -- Application to detect people on the given video and will stream on your browser.
  
:information_source: Note: To run the crowd_count application please download the trained model from [here](), and place it in `awiros-apps/model/common` folder.

- To run any of the 2 application, just run
```
./awiros run simple_camera/crowd_count
```
You will receive the port number on the terminal like this:
```
Web Streaming Initialized. PORT: <port-number>
```
Open your web browser and type the url:
```
localhost:<port-number>
```
## License
[The Unlicense](LICENSE)
