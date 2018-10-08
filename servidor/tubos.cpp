#include "tubos.hpp"

tubos::tubos(){
	pipe(tubo_robot_arduino);
	pipe(tubo_arduino_robot);
	pipe(tubo_internet_arduino);
	pipe(tubo_internet_robot);
	pipe(tubo_robot_internet);

	fd_internet_arduino.fd = tubo_internet_arduino[0];
	fd_internet_arduino.events = POLLIN;

	fd_arduino_robot.fd = tubo_arduino_robot[0];
	fd_arduino_robot.events = POLLIN;

	fd_internet_arduino.fd = tubo_internet_arduino[0];
	fd_internet_arduino.events = POLLIN;

	fd_internet_robot.fd = tubo_internet_robot[0];
	fd_internet_robot.events = POLLIN;

	fd_robot_internet.fd = tubo_robot_internet[0];
	fd_robot_internet.events = POLLIN;

}

