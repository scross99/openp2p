#ifndef P2P_ROOTNETWORK_AUTHENTICATEDSOCKET_HPP
#define P2P_ROOTNETWORK_AUTHENTICATEDSOCKET_HPP

#include <p2p/Socket.hpp>

#include <p2p/Event/Source.hpp>

#include <p2p/Root/Endpoint.hpp>
#include <p2p/Root/Message.hpp>
#include <p2p/Root/Packet.hpp>
#include <p2p/Root/PrivateIdentity.hpp>
#include <p2p/Root/PublicIdentity.hpp>

namespace p2p {

	namespace Root {
	
		class PrivateIdentity;
		
		class IdentityDelegate {
			public:
				virtual PrivateIdentity& getPrivateIdentity() = 0;
				
				virtual PublicIdentity& getPublicIdentity(const PublicKey& publicKey) = 0;
				
		};
		
		class AuthenticatedSocket: public Socket<std::pair<Endpoint, NodeId>, Message> {
			public:
				AuthenticatedSocket(IdentityDelegate& delegate, Socket<Endpoint, SignedPacket>& socket);
				
				bool isValid() const;
				
				Event::Source eventSource() const;
				
				bool receive(std::pair<Endpoint, NodeId>& endpoint, Message& message);
				
				bool send(const std::pair<Endpoint, NodeId>& endpoint, const Message& message);
				
			private:
				// Non-blocking.
				AuthenticatedSocket(const AuthenticatedSocket&) = delete;
				AuthenticatedSocket& operator=(AuthenticatedSocket) = delete;
				
				IdentityDelegate& delegate_;
				Socket<Endpoint, SignedPacket>& socket_;
				
		};
		
	}
	
}

#endif
