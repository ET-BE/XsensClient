#include <thread>
#include <chrono>

#include <XsensClient/XsensClient.h>
#include <XsensClient/datagrams.h>

/**
 * Spawn an Xsens client and empty its data queue a couple of times
 * at short intervals.
 *
 * @return
 */
int main() {

    XsensClient client;

    client.startThread();

    for (int i = 0; i < 5; i++) {

        auto data_list = client.consumeAllData();

        for (const auto& data : data_list) {

            // data->printHeader();

            if (data->messageType() == SPJointAngles) {
                auto joint_angles = std::dynamic_pointer_cast<JointAnglesDatagram>(data);
                const auto& structs = joint_angles->getStructs();

                std::cout << "Rotation of first joint: "
                          << structs[0].rotation[0] << ", "
                          << structs[0].rotation[0] << ", "
                          << structs[0].rotation[0] << std::endl;
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}
