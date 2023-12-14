To build docker containers:
sudo docker build . -f DockerfileS1 -t server1_container
sudo docker build . -f DockerfileS2 -t server2_container
sudo docker build . -f DockerfileC -t client_container 

To run docker containers:
sudo docker run -it --net=host server1_container
sudo docker run -it --net=host server2_container
sudo docker run -it --net=host client_container

To stop all containers
sudo docker stop $(sudo docker ps -a -q)
