#include "android_files_backup/ui/cli_application.h"

namespace android_files_backup {

void CliApplication::showDevices() {
    clearScreen();

    output_ << "**Wybór urządzenia**\n";

    controller_.refreshDevices();

    const auto &devices = controller_.devices();

    if (devices.isEmpty()) {
        output_ << "Nie znaleziono urządzeń \n";
        output_.flush();

        return;
    }

    output_ << "Znalezione urządzenia: \n";

    for (auto i = 0; i < devices.size(); i++) {
        auto &device = devices[i];
        output_ << "[" << i << "] " << device.serial << device.model
                << " state: " << deviceStateToString(device.state) << "\n";
    }

    output_.flush();
}

void CliApplication::choiceDevice() {
    showDevices();

    int devicesNum = controller_.devices_.size();

    if (devicesNum == 0) {
        return;
    }

    const int choice = readInteger("Wybierz urządzenie: ", 0, devicesNum - 1);

    const AdbDevice &device = controller_.devices_[choice];

    controller_.selectDevice(device.serial);

    output_ << "Wybrano " << device.serial << " " << device.model
            << "\n---------- \n";
    output_.flush();

    waitForEnter();
}

} // namespace android_files_backup