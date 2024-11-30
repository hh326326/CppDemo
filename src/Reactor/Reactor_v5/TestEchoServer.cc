#include "EchoServer.h"

void test() {
    EchoServer server(4, 10, "127.0.0.1", 8888);
    server.start();
}

int main(int argc, char *argv[])
{
    test();
    return 0;
}

