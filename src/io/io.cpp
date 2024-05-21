#include "io.hpp"

void gestor_io(t_c_global_config &global_config)
{
	// open all listening sockets and bind to all ports (ipv4 is enough) OK
	// wait for conections and accept OK
	// create a std::string for each conection and filli it whit the input of the connection socket OK
	// \r\n OK
	// Select the configuration of the request destination.
	// calls struct s_engine_return {int fd, t_pid c_pid, bool unrecoverable} engine(std::string &msg,
	// t_c_individual_server_config &server) reads or splices the returned fd to the connection fd when all contents of
	// returned fd are writen to connection fd closes conection fd and rips c_pid if not 0 (non blocking) dont forget
	// SIGPIPE OK
	signal(SIGPIPE, SIG_IGN);
	Server server(global_config);
}
