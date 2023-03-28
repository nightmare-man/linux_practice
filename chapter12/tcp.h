#pragma once
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <exception>
#define tcpserver TcpServer
#define tcpclient TcpClient
namespace tcp{
    class TcpException:public std::exception{
        public:
        TcpException(const char* s)noexcept:msg{s}{};
        const char* what()const noexcept override {return msg;};
        private:
        const char* msg;
    };
    class TcpServer{
        public:
            TcpServer(const char* ,int );
            ~TcpServer(){close(socket_id);};
            int Accept();
            int Read(char*,int)const;
            int Write(const char*,int)const;
            void Handle(void(*callback)(int)){cb=callback;};
        private:
            std::vector<int> connected_id;
            int socket_id;
            const char* addr_str;
            int port;
            void (*cb)(int);
    };
    class TcpClient{
        public:
            TcpClient(const char*,int);
            ~TcpClient(){close(socket_id);};
            int Read(char*,int)const;
            int Write(const char*,int)const;
        private:
            int socket_id;
            const char* addr_str;
            int port;
    };
}