#include <iostream>
#include <sstream>
#include <OpenP2P.hpp>
#include <OpenP2P/OFTorrent.hpp>

using namespace OpenP2P;

class BlockGen: public OFTorrent::OStreamGenerator{
	public:
		BlockGen() : count_(0){ }

		OStream& getOStream(){
			std::ostringstream s;
			s << "out" << count_ << ".txt";
			count_++;
			fileStream.open(s.str());
			return fileStream;
		}

	private:
		std::size_t count_;
		FileOStream fileStream;

};

int main(int argc, char *argv[]){
	if(argc != 2){
		std::cout << "create [filename]" << std::endl;
		return 0;
	}
	FileIStream fileStream(argv[1]);
	if(!fileStream.isOpen()){
		std::cout << "Failed to open file" << std::endl;
	}

	NullIStream nullStream;

	OFTorrent::XORIStream xorStream(fileStream, nullStream);

	BlockGen blockGen;

	OFTorrent::BlockOStream blockStream(blockGen, OFTorrent::BLOCKSIZE_512KB);

	BinaryIStream binaryStream(xorStream);
	binaryStream >> blockStream;

	return 0;
}

