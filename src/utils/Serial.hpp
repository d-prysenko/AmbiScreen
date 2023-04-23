#include <boost/asio.hpp>

class Serial
{
private:
	boost::asio::io_service io;
	boost::asio::serial_port serial;

public:

	Serial(std::string name)
		: io(), serial(io, name)
	{
		serial.set_option(boost::asio::serial_port_base::baud_rate(115200));
		serial.set_option(boost::asio::serial_port_base::character_size(8));
		serial.set_option(boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none));
		serial.set_option(boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one));
	}

	int read(char* buffer, int buffer_size)
	{
		return serial.read_some(boost::asio::buffer(buffer, buffer_size));
	}

	int write(std::string buffer)
	{
		return serial.write_some(boost::asio::buffer(buffer.c_str(), buffer.size()));
	}
};