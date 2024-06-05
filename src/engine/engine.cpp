#include "engine.hpp"

static FILE *write_response(std::string head, std::string file)
{
	FILE         *tmp = std::tmpfile();
	std::ifstream body(file);
	std::string   buffer;

	std::fputs(head.c_str(), tmp);
	while (std::getline(body, head))
	{
		std::fputs((head + "\n").c_str(), tmp);
	}
	std::rewind(tmp);
	return (tmp);
}

ReturnType handle_invalid_request(void)
{

	t_c_default_error_pages default_errors;
	std::string             head = "HTTP/1.1 400 Bad Request\r\n\r\n";
	return (ReturnType(write_response(head, default_errors.get_bad_request()), 0));
}

ReturnType handle_error(int error_code, t_c_individual_server_config const &config)
{
	FILE                          *file;
	t_c_default_error_pages const *error_pages = config.get_default_error_pages();
	std::stringstream              head;
	head << "HTTP/1.1 " << error_code << " ";
	switch (error_code)
	{
		case 400:
		{
			head << "Bad Request\r\n\r\n";
			file = write_response(head.str(), error_pages->get_bad_request());
			break;
		}
		case 403:
		{
			head << "Forbiden\r\n\r\n";
			file = write_response(head.str(), error_pages->get_forbidden());
			break;
		}
		case 404:
		{
			head << "Not found\r\n\r\n";
			file = write_response(head.str(), error_pages->get_not_found());
			break;
		}
		case 408:
		{
			head << "Request timeout\r\n\r\n";
			file = write_response(head.str(), error_pages->get_request_timeout());
			break;
		}
		case 411:
		{
			head << "Length required\r\n\r\n";
			file = write_response(head.str(), error_pages->get_length_requiered());
			break;
		}
		case 413:
		{
			head << "Content too large\r\n\r\n";
			file = write_response(head.str(), error_pages->get_content_too_large());
			break;
		}
		case 414:
		{
			head << "Uri too long\r\n\r\n";
			file = write_response(head.str(), error_pages->get_uri_too_long());
			break;
		}
		case 500:
		{
			head << "Internal server error\r\n\r\n";
			file = write_response(head.str(), error_pages->get_internal_server_error());
			break;
		}
		case 501:
		{
			head << "Not implemented\r\n\r\n";
			file = write_response(head.str(), error_pages->get_not_implemeted());
			break;
		}
		case 505:
		{
			head << "HTTP version not supported\r\n\r\n";
			file = write_response(head.str(), error_pages->get_http_version_not_supported());
			break;
		}
	}
	return (ReturnType(file, 0));
}

ReturnType::ReturnType(FILE *file, pid_t child)
{
	this->file = file;
	this->child = child;
}

ReturnType::~ReturnType()
{
}

FILE *ReturnType::getFile(void)
{
	return (file);
}

pid_t ReturnType::getChild(void)
{
	return child;
}

bool ReturnType::is_cgi(void)
{
	return (child > 0);
}