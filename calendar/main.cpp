#include "cli.hh"
#include <iostream>

using namespace std;

int main() {
    Cli *cli = new Cli(make_shared<Calendar>());

    while (!cli->exec_prompt()) {
        cout << endl;
    }

    delete cli;

    return 0;
}
