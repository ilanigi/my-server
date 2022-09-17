#include <boost/asio.hpp>

class Client{
    public:
        Client();
        ~Client();
        void register_user();
    private:
        bool user_file_exist();
        boost::asio::ip::tcp::socket * socket;
        static boost::asio::ip::tcp::socket init_connection(std::string ,std::string );
        static std::vector<std::string> get_connection_credentials();
};
