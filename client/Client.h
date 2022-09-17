#include <boost/asio.hpp>

class Client{
    public:
        Client();
    private:
        static boost::asio::ip::tcp::socket init_connection(std::string ,std::string );
        static std::vector<std::string> get_connection_cradentilas();
};
