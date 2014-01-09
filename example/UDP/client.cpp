#include <OpenP2P.hpp>
#include <OpenP2P/UDP.hpp>
#include <iostream>

using namespace OpenP2P;

int main(){
	std::cout << "Start client" << std::endl;

	UDP::Socket socket;

	{
		Buffer buffer;
		BufferBuilder builder(buffer);
		BinaryOStream binaryStream(builder);
		Binary::WriteUint16(binaryStream, 0);

		socket.send(IP::Endpoint(IP::V4Address::Localhost(), 45557), buffer);
	}
	
	std::cout << "Sent: 0" << std::endl;
	
	uint16_t v = 1;
	
	while (true) {
		UDP::Endpoint endpoint;
		Buffer data;
		
		if (!socket.receive(endpoint, data)) {
			std::cout << "Server failed to respond in time" << std::endl;
			break;
		}
		
		uint16_t i;
		
		{
			BufferIterator iterator(data);
			BinaryIStream binaryStream(iterator);
			Binary::ReadUint16(binaryStream, i);
		}
		
		std::cout << "Received: " << i << " from " << endpoint << std::endl;
		
		if (i != v) {
			std::cout << "Incorrect data received: " << i << std::endl;
			break;
		}
		
		if (i < 10000) {
			v += 2;
			std::cout << "Sent: " << (i + 1) << std::endl;
			Buffer buffer;
			BufferBuilder builder(buffer);
			BinaryOStream binaryStream(builder);
			binaryStream << uint16_t(i + 1);
			socket.send(endpoint, buffer);
		}
		
		if (i >= 9999) {
			std::cout << "Completed Successfully" << std::endl;
			break;
		}
	}

	return 0;
}
