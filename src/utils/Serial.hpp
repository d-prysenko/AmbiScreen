#include "serial/serialib.h"
#include <iostream>

#define SERIAL_PORT "/dev/ttyACM1"

class Serial
{
private:
	serialib serial;

public:

	Serial(std::string name)
	{
		char errorOpening = serial.openDevice(name.c_str(), 115200);


    	// If connection fails, return the error code otherwise, display a success message
    	if (errorOpening!=1) {
			printf("Can't connect to %s, err: %d\n",name.c_str(), errorOpening);
			return;
		}
    	printf ("Successful connection to %s\n",name.c_str());
		// serial.set_option(boost::asio::serial_port_base::baud_rate(115200));
		// serial.set_option(boost::asio::serial_port_base::character_size(8));
		// serial.set_option(boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none));
		// serial.set_option(boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one));
		// serial.set_option(boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::software));
	}

	int read(char* buffer, int buffer_size)
	{
		//serial.read_some(boost::asio::buffer(buffer, buffer_size));
		// return boost::asio::read(serial, boost::asio::buffer(buffer, buffer_size));
	}


	int write(Serializable* buffer)
	{
		//serial.write_some(boost::asio::buffer(buffer->serialize()));
		std::vector<uint8_t> buf = buffer->serialize();
		

		return serial.writeBytes(buf.data(), buf.size());
		// return serial.writeBytes(c);
		
		// return boost::asio::write(serial, boost::asio::buffer(buffer->serialize()));
	}
};