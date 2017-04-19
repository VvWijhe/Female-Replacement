#include <iostream>
#include <thread>

#include "Poco/AsyncChannel.h"
#include "Poco/ConsoleChannel.h"
#include "Poco/FileChannel.h"
#include "Poco/FormattingChannel.h"
#include "Poco/Logger.h"
#include "Poco/PatternFormatter.h"
#include "Poco/SplitterChannel.h"

#include "roomba_statemachine.h"

using namespace std;
using namespace systemcontrol;
using namespace Poco;

int main() {
//    thread t([]{
//        system("rfcomm connect /dev/rfcomm1 00:06:66:60:07:81");
//    });
//
//    this_thread::sleep_for(chrono::seconds(6));
//
//    systemcontrol::RoombaControl control("/dev/rfcomm1", systemcontrol::RoombaControl::b115200);
//
//    control.init();
//    control.setMotors(control.side, true);
//    control.setWheels(0);
//
//    t.join();

    try {
        //initialise logger
        AutoPtr<SplitterChannel> splitterChannel(new SplitterChannel());

        AutoPtr<Channel> consoleChannel(new ConsoleChannel());
        AutoPtr<Channel> fileChannel(new FileChannel("logfile.log"));
        splitterChannel->addChannel(consoleChannel);
        splitterChannel->addChannel(fileChannel);

        AutoPtr<Channel> formattingChannel(
                new FormattingChannel(new PatternFormatter("%d-%m-%Y %L%H:%M:%S %p: %t"), splitterChannel));
        Logger::create("logger", formattingChannel, Message::PRIO_TRACE);

        // create statemachine
        shared_ptr<RoombaStateContext> process = make_shared<RoombaStateContext>(make_shared<states::Initialise>());

        process->runAll();
    } catch (exception &e) {
        cerr << e.what() << endl;
    }

    return 0;
}