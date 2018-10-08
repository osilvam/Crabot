#include "tubos.hpp"

tubos::tubos(){
	pipe(pipe_CS_to_MR);
	pipe(pipe_MR_to_CS);
	pipe(pipe_CAM_to_CS);
	pipe(pipe_CS_to_CAM);

	fd_MR_to_CS.fd = pipe_MR_to_CS[0];
	fd_MR_to_CS.events = POLLIN;

	fd_CS_to_MR.fd = pipe_CS_to_MR[0];
	fd_CS_to_MR.events = POLLIN;

	fd_CAM_to_CS.fd = pipe_CAM_to_CS[0];
	fd_CAM_to_CS.events = POLLIN;

	fd_CS_to_CAM.fd = pipe_CS_to_CAM[0];
	fd_CS_to_CAM.events = POLLIN;

}

