# Android Files Backup System

> **Work in progress — this is a developer release.**

## Minimal setup

First, enable USB debugging on your Android device.

Instructions:

```text
https://www.embarcadero.com/starthere/xe5/mobdevsetup/android/en/enabling_usb_debugging_on_an_android_device.html
```

Connect your phone to the computer using a USB cable.

When the **Allow USB debugging** prompt appears on your phone, accept it.

Then clone the repository, install the required dependencies, build the project, and run the application.

> **Important:** This version of the application does not ask the user for any configuration.

> **Important:** Before building the project, change the arguments passed to `createFilesPull_debugFunction()` in `main.cpp` so that the application copies the expected files.

The application does not modify files on your phone. It does not move, rename, or delete them.

Before running the application, make sure that:

- your phone is unlocked,
- USB debugging is enabled,
- the computer has permission to access the phone through USB debugging.

## Build and run

```bash
git clone https://github.com/MikolajKosela/android_files_backup
cd android_files_backup

sudo apt update
sudo apt install qt6-base-dev cmake ninja-build g++ adb

cmake -S . -B build -G Ninja
cmake --build build

./build/android_files_backup
```
