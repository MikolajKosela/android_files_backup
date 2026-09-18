#include "android_files_backup/ui/cli_application.h"

namespace android_files_backup {

void CliApplication::choiceDevice() {
  clearScreen();

  showCaption("**Wybór urządzenia**\n");

  controller_.refreshDevices();

  const auto &devices = controller_.devices();

  if (devices.isEmpty()) {
    showCaption("Nie znaleziono urządzeń \n");

    return;
  }

  QStringList options;

  for (auto i = 0; i < devices.size(); i++) {
    auto &device = devices[i];
    options << device.serial + device.model +
                   " state: " + deviceStateToString(device.state);
  }
  int const choice = showMenu("Znalezione urządzenia:", options, 0);

  controller_.selectDevice(devices[choice].serial);

  showCaption("Wybrano " + devices[choice].serial + " " +
              devices[choice].model + "\n---------- \n");
  waitForEnter();
}

}  // namespace android_files_backup