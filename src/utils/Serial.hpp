#include <boost/asio.hpp>
#include <iostream>
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
		serial.set_option(boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::software));
	}

	int read(char* buffer, int buffer_size)
	{
		//serial.read_some(boost::asio::buffer(buffer, buffer_size));
		return boost::asio::read(serial, boost::asio::buffer(buffer, buffer_size));
	}

	int write(std::string buffer)
	{
		return serial.write_some(boost::asio::buffer(buffer.c_str(), buffer.size()));
	}

	int write(std::vector<uint8_t> buffer)
	{
		printf("buffer.size() = %d\n", buffer.size());
		return serial.write_some(boost::asio::buffer(buffer, buffer.size()));
	}

	int write(Serializable* buffer)
	{
		//serial.write_some(boost::asio::buffer(buffer->serialize()));
		
		return boost::asio::write(serial, boost::asio::buffer(buffer->serialize()));
	}
};