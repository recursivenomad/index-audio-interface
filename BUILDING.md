# Build Environment



The build environment consists of:

- Raspberry Pi Pico SDK *(2.1.1)*
  - "picotool" *(2.1.1)*
  - "pico-extras" *(sdk-2.1.1)*
  - `PICO_SDK_PATH` and `PICO_EXTRAS_PATH` environment variables set
- Arm GNU Toolchain *(14.2.Rel1)*
- RISCV GCC Toolchain *(13.2.0)*
- GCC *(14.2.0)*
- CMake *(4.0.1)*
- Python *(3.13.3)*

A detailed guide with links for setting up build tools and a development environment on Windows can be found [in the Appendix](#appendix-a-detailed-environment-setup) of this file.

&nbsp;



# Build Instructions



From the project root, in a terminal enter:

```bash
mkdir build
cd build
cmake ..
make
```

The `build/` directory should contain your binaries.

### Drag-and-drop installation:

While unplugged, hold down the `BOOTSEL` button on your target board and plug it into your computer.  
Drag `build/usb_sound_card.uf2` into the `RPI-RP2` USB device.

The board should automatically reset and show up as a USB audio device to your computer!

&nbsp;



# Release Instructions



Todo...



&nbsp;

&nbsp;

---

&nbsp;

&nbsp;






# Appendix A: Detailed Environment Setup



> *Development environment heavily informed by Shawn Hymel's [Raspberry Pi Pico C/C++ Toolchain tutorial](https://shawnhymel.com/2096/how-to-set-up-raspberry-pi-pico-c-c-toolchain-on-windows-with-vs-code/) and [OpenOCD and Picotool for Raspberry Pi Pico tutorial](https://shawnhymel.com/2168/how-to-build-openocd-and-picotool-for-the-raspberry-pi-pico-on-windows/), as well as his [video tutorials](https://youtu.be/B5rQSoOmR5w) made for Digi-Key and [Raspberry Pi's Getting started with Raspberry Pi Pico guide.](https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf)*
> 
> *The main differences are that this guide implements 100% open-source tools (including the IDE and extensions, with the only exceptions being the development OS and the Raspberry Pi silicon), and it is more current (April 2025).*


## Before you begin

This is a **generic** guide for setting up your system for development of RP2040/RP2350 firmware.  
Some aspects may not be used/needed in order to build this specific project, but it ensures you have a compliant development environment for following the build instructions above.

- You will need 2 Raspberry Pi Picos and means of connecting them, as shown [here](https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf#debugprobe-wiring-section)  
*(It should also work with Pico W, Pico 2, and Pico 2 W as well, however this guide will assume both are original Picos)*  
If you don't need debugging capabilities, you can get by with just one.
- This guide was written for development on Windows 10, and may or may not apply to other versions of Windows.  
Transferring concepts to other operating systems should be reasonably doable for the curious.


## Overview

*In addition to the build environment outlined [above](#build-environment)...*

Debugging consists of:

- A second Pico running [`debugprobe`](https://github.com/raspberrypi/debugprobe/) firmware *(supports [Picoprobe Candybar (WIP)](https://gitlab.com/recursivenomad/picoprobe-candybar/) and [Raspberry Pi's Debugprobe](https://www.raspberrypi.com/products/debug-probe/))*
- OpenOCD *(custom Raspberry Pi build)*
- GDB *(via Arm/RISC-V GNU Toolchains)*
- picotool *(built w/ libusb)*

Development environment builds off of:

- VSCodium
  - CMake Tools extension
  - clangd language extension
    - clangd
  - Cortex-Debug extension
    - debug-tracker-vscode extension
    - MemoryView extension
    - Peripheral Viewer extension
    - RTOS Views extension

And this guide will utilize:

- Git for Windows
- A bash emulator (Git Bash, included in Git for Windows)
- A serial monitor (Serial Studio as an example)
- Zadig
- 7Zip


## Setup guide

> 🐉 **HERE BE DRAGONS: This guide is lengthy and involved**  
> You should certainly be able to get to where you need to be by following it, but the path may be treacherous along the way


### ***We'll begin with build tools***

**1.1. Create the following directory structure:**

`C:\tools\mingw32\`  
`C:\tools\armcc\`  
`C:\tools\riscv\`  
`C:\tools\sdk\pico\`

*You're welcome to use a different structure, but make sure you replace all paths appropriately throughout this guide if you do, and ensure there are **no spaces.***

&nbsp;



**1.2. Using [7-Zip](https://7-zip.org/), extract the contents of the 32-bit version of [MinGW-w64](https://github.com/niXman/mingw-builds-binaries/releases/tag/14.2.0-rt_v12-rev2)** to `C:\tools\mingw32\`

<!-- TODO: LLVM? <https://github.com/llvm/llvm-project/releases/> -->

*This project is built using using the archive:*  
*`i686-14.2.0-release-mcf-dwarf-ucrt-rt_v12-rev2.7z`*  
*SHA256 checksum: `D562C9AD39F9DFF07CC9742371349A42CBD99E2E52806975BF67AE7EE754EB6C`*

&nbsp;



**1.3. Create a symbolic link to redirect the command `make` to `mingw32-make.exe`**, as this will make calling the command easier during development.

To create this symlink, open a **command prompt** and enter:

  ```
  cd C:/tools/mingw32/bin
  mklink /H make.exe mingw32-make.exe
  ```

&nbsp;



**1.4. Install the [Arm GNU Toolchain](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads)** targeting AArch32 bare-metal to `C:\tools\armcc\`

Ensure the box is checked when installation is done so that the folder is **added to the system path**.  *(I leave all other boxes unchecked)*

*This project is built using the installer:*  
*`arm-gnu-toolchain-14.2.rel1-mingw-w64-i686-arm-none-eabi.exe`*  
*SHA256 checksum: `8A53F7E9688EAA6816843CD03261D7A1538E861F8FE91C1D4B72D2E5069CBCBD`*

<!-- TODO: clang? <https://github.com/arm/arm-toolchain/releases/> -->

&nbsp;



**1.5. Using 7-zip, extract Raspberry Pi's prebuilt [RISC-V GCC Toolchain](https://github.com/raspberrypi/pico-sdk-tools/releases/tag/v2.0.0-5)** to `C:\tools\riscv\`

*The above download link appears to follow the build instructions provided by Luke Wren in his [Hazard3 repo](https://github.com/Wren6991/Hazard3) (the RISC-V processors in the RP2350)*

***Note: I have not yet tested building for RISC-V***

<!-- TODO: clang? <https://buildbot.embecosm.com/job/riscv32-clang-win64/266/> -->

*This project is built using the archive:*  
*`riscv-toolchain-14-x64-win.zip` from release v2.0.0-5*  
*SHA256 checksum: `1002449CEFE425D357F4B87CBB2862078E830EA23D42184C946AB7A4684FCE8C`*

&nbsp;



**1.6. Install [CMake](https://github.com/Kitware/CMake/releases/tag/v4.0.1)**, ensuring that you **"Add CMake to the system PATH"** during installation.

For consistency, I prefer to install CMake to `C:\tools\cmake\`

*This project is built using the installer:*  
*`cmake-4.0.1-windows-x86_64.msi`*  
*SHA256 checksum: `B2CD97898BA97EE6D08EE2B7E99DEF5EB13C2C24EAE8D5F2C3CD983923C2DB49`*

&nbsp;



**1.7. Install [Python](https://www.python.org/downloads/release/python-3133/)**, ensuring boxes are checked so python.exe is **added to PATH**

For consistency, I prefer to install Python to `C:\tools\python\`

*This project is built using the installer:*  
*`python-3.13.3-amd64.exe`*  
*SHA256 checksum: `698F2DF46E1A3DD92F393458EEA77BD94EF5FF21F0D5BF5CF676F3D28A9B4B6C`*

&nbsp;



**1.8. Install [Git for Windows](https://github.com/git-for-windows/git/releases/tag/v2.49.0.windows.1)**

As I will be using Git Bash as my primary terminal in this guide, I strongly encourage installing it to `C:\tools\git\` so that there are **no spaces** in its file path.  Spaces in the absolute file path to `/usr/bin/sh/` can create very frustrating problems!

- Be sure to install *git-lfs* when prompted *(a default setting)*
- I like using VSCodium as my editor *(we will install it later)*
- I recommend adding Git to PATH when asked
- Everything else should be up to user preference

*This project is built using the installer:*  
*`Git-2.49.0-64-bit.exe`*  
*SHA256 checksum: `726056328967F242FE6E9AFBFE7823903A928AFF577DCF6F517F2FB6DA6CE83C`*

&nbsp;



**1.9. Using Git, download the Raspberry Pi Pico SDK and related repositories.**

In Git Bash enter:

  ```
  cd C:/tools/sdk/pico
  git clone https://github.com/raspberrypi/pico-sdk.git --branch master
  cd pico-sdk
  git submodule update --init
  git checkout 2.1.1 --recurse-submodules
  cd ..
  git clone https://github.com/raspberrypi/pico-extras.git --branch sdk-2.1.1
  git clone https://github.com/raspberrypi/pico-examples.git --branch sdk-2.1.1
  git clone https://github.com/raspberrypi/picotool.git --branch 2.1.1 ./picotool/source/picotool
  git clone https://github.com/raspberrypi/debugprobe.git --branch master
  cd debugprobe
  git checkout debugprobe-v2.2.2
  git config --local submodule.CMSIS_5.branch master
  GIT_LFS_SKIP_SMUDGE=1 git submodule update --init --depth 1
  ```

*If submodules have had significant changes since writing this guide, you may need to append `--force` the first time you checkout certain tags.*  
*Note that debugprobe's submodules will require your Git is installed with git-lfs.*

Debugprobe is pretty big and we don't need all its history (hence why we used `--depth 1`).  
If you want to further save on space at the cost of convenience, feel free to delete its `.git\` directory.

*The following are the SHA1 hashes of each repo's head as used in this project:*  
*pico-sdk: `bddd20f928ce76142793bef434d4f75f4af6e433`*  
*pico-extras: `f05d4f7371802440cadd36744789a26944d950ac`*  
*pico-examples: `84e8d489ca321a4be90ee49e36dc29e5c645da08`*  
*picotool: `de8ae5ac334e1126993f72a5c67949712fd1e1a4`*  
*debugprobe: `94124bc7cd81938d9524cd023e70d88ce74a53bc`*

&nbsp;



**1.10. Add `...\mingw32\bin`, `...\riscv\bin`, `...\picotool\bin`, `...\pico-sdk`, and `...\pico-extras` to your system path / as environment variables:**
  - Press the Windows key, search "env", and select "Edit the system environment variables".  
  - Select "Environment Variables..."  
  - In the "User variables" section, highlight the "Path" variable and select "Edit..."  
    *(You should already see the path you installed the Arm GNU Toolchain to in the list here)*  
  - Select "New" and add your full path for `C:\tools\mingw32\bin` to the list
  - Select "New" again and add your full path for `C:\tools\riscv\bin` to the list
  - Select "New" once more and add your full path for `C:\tools\sdk\pico\picotool\bin` to the list  
    *(The `picotool\bin\` directory will be created shortly)*
  - Click "OK" to exit  
  - In the "User variables" section again, select "New..."
    - Variable name: `PICO_SDK_PATH`
    - Variable value: `C:\tools\sdk\pico\pico-sdk`
  - Again, select "New..."
    - Variable name: `PICO_EXTRAS_PATH`
    - Variable value: `C:\tools\sdk\pico\pico-extras`
  - And once more, select "New..."
    - Variable name: `CMAKE_GENERATOR`
    - Variable value: `MinGW Makefiles`
  - Click "OK" to exit  
    *(Within the "Path" entry of "System variables", you should be able to verify `...\cmake\bin` and `...\git\cmd` are in the system path)*  
  - Click "OK" to exit any remaining environment variable windows

Close all previously open terminals to ensure that environment variables update.

*If you don't want to set MinGW Makefiles as your default CMake generator, then be sure to append `-G "MinGW Makefiles"` to any `cmake ..` commands in this guide.*

*As suggested by Shawn, opening a new command prompt at this point and entering commands like `gcc --version` , `make -v` , and `echo %PICO_SDK_PATH%` should now all return expected information.*  
*If checking from a bash emulator like Git Bash instead, you can confirm environment variables with `echo $PICO_SDK_PATH`*

&nbsp;



**1.11. Using 7-zip, open [libusb-1.0.28.tar.bz2](https://github.com/libusb/libusb/releases/tag/v1.0.28)** and extract the nested folder `libusb-1.0.28` to `C:\tools\sdk\pico\picotool\source\libusb\libusb-1.0.28\`


*This project is built using the archive:*  
*`libusb-1.0.28.tar.bz2`*  
*SHA256 checksum: `966BB0D231F94A474EAAE2E67DA5EC844D3527A1F386456394FF432580634B29`*

&nbsp;



**1.12. Build libusb**

libusb is required for picotool to build and read information for USB firmware files.

In a **bash** terminal *(such as Git Bash)* enter:

  ```
  cd C:/tools/sdk/pico/picotool/source/libusb/libusb-1.0.28/
  ./configure --prefix "C:/tools/sdk/pico/picotool/source/libusb/"
  make
  make install
  ```

*I received some compiler warnings from `os/windows_winusb.lo`, but none were fatal.*

> ***NOTE**: If your bash executable is installed at a file path with spaces (such as the common default of `C:\Program Files\`), then the `make` command may exit with an error!  Either reinstall your bash terminal to a path without spaces, or you can try manually editing the `configure` file as described [here](https://github.com/libusb/libusb/discussions/1629#discussioncomment-12769408).*

After successful installation, you can delete the `libusb-1.0.28/` folder, if you wish.

&nbsp;



**1.13. Build picotool:**

picotool is required for generating the UF2 files we will often upload to our Picos.  It will also help with providing debugging info later.


In a terminal enter:

  ```
  cd C:/tools/sdk/pico/picotool/source/picotool/
  mkdir build
  cd build
  cmake -E env LIBUSB_ROOT="C:/tools/sdk/pico/picotool/source/libusb/" cmake -DCMAKE_INSTALL_PREFIX="C:/tools/sdk/pico/picotool/" -DCMAKE_POLICY_VERSION_MINIMUM=3.5 ..
  make
  make install
  ```

*I received a few CMake warnings from picotool's and mbedtls' `CMakeLists.txt`, but none were fatal.*

<!-- TODO: If you build using a pico-sdk version prior to 2.1.2, you will need to add the CMake switch `-DCMAKE_POLICY_VERSION_MINIMUM=3.5` for CMake 4.x compatibility -->

You should now have `picotool.exe` installed in `C:\tools\sdk\pico\picotool\bin\`

To confirm picotool is correctly installed, in a terminal enter:

  ```
  picotool version
  ```

This should print out version information.

&nbsp;



**1.14. Verify the functionality of this development environment so far** by building and uploading a simple "blink" program.

In a terminal enter:

  ```
  cd C:/tools/sdk/pico/pico-examples/
  mkdir build
  cd build
  cmake ..
  cd blink
  make
  ```

With the Raspberry Pi Pico unplugged, hold down the BOOTSEL button and plug in the Pico.  
Copy the file `...\pico-examples\build\blink\blink.uf2` over to the Raspberry Pi Pico's root directory.

If you see a blinking green light on your Pico, then everything appears to have gone right up to this point! 🎉

&nbsp;



### ***Next we'll work on debug tools***

**2.1. Install a serial monitor.**

There are plenty of options out there, but I will be using [Serial Studio](https://serial-studio.github.io/) *(3.0.6)* if you'd like to follow along.

Initial settings I recommend for Serial Studio:

- Disable "Create CSV File"
- Frame Parsing: No Parsing
- Enable "Show Timestamp"
- Disable "Emulate VT-100"
- Enable "Send DTR Signal"

&nbsp;



**2.2. Verify serial output.**

With the Raspberry Pi Pico unplugged, make note of the COM ports available to choose from in the drop-down menu.

In a terminal enter:

  ```
  cd C:/tools/sdk/pico/pico-examples/build/hello_world/usb/
  make
  ```

*This assumes you ran CMake as described in 1.14.*

Hold down the BOOTSEL button and plug in the Pico.  
Copy the file `...\pico-examples\build\hello_world\usb\hello_usb.uf2` over to the Pico.  

Once the Pico restarts, check the drop-down again in Serial Studio to see what the new COM port index is, and select it.  
Select a baud rate of 115200.  
You should see the Pico repeatedly printing "`Hello world!`"

> *COM ports can become inaccessible due them to being used by other applications.  Be sure to close other applications that may try attaching to COM ports, such as 3D printing software.*

> *COM ports on Windows can just be annoying in general.  It should "just work", but if it doesn't it may be worth trying to play with manually changing the COM port used by the Pico via Device Manager, as well as manually increasing the baud rate there.* 🙃

&nbsp;



**2.3. Using 7-zip, extract [Raspberry Pi's custom build of OpenOCD](https://github.com/raspberrypi/pico-sdk-tools/releases/tag/v2.1.0-0):**

> *The link above takes you to Raspberry Pi's pre-built binaries so that the RP2350 is supported.  If you would prefer to use the official OpenOCD, note that 0.12.0 does not provide full debugging capabilities for the RP2040 or RP2350, so be sure to download the [latest automated build](https://github.com/openocd-org/openocd/releases/) so you at least have full RP2040 support.*  
  *In future [release versions](https://github.com/openocd-org/openocd/releases/latest) of OpenOCD, presumably 1.0.0 and up, you should be able to use the most recent official stable release instead.*  
  *You can track the progress of official RP2350 features [here](https://review.openocd.org/q/rp2350).*

Place the extracted files in `C:\tools\openocd\`.

<!-- TODO: Does this build support RISC-V debugging? -->

<!-- TODO: clang debug? <https://github.com/clangd/clangd/releases/> -->

*This project is built using the archive:*  
*`openocd-0.12.0+dev-x64-win.zip` from release v2.1.0-0*  
*SHA256 checksum: `B2A769913F692D92A14E9F59F3FB4CDB9BAD3C3610993D345AFE486B029F92B1`*

&nbsp;



**2.4. Add `C:\tools\openocd\bin` to your system path** - *See step 1.10 if you need instructions for editing the system path.*

To confirm OpenOCD installed correctly, in a terminal enter:

  ```
  openocd --version
  ```

&nbsp;



**2.5. Download [Zadig](https://github.com/pbatard/libwdi/releases/tag/v1.5.1)**.

This will be used to install drivers for the Pico's reset interface.

*This project is built using the installer:*  
*`zadig-2.9.exe`*  
*SHA256 checksum: `4ECAA95DF3DA3621486A043AEF8B3050B8BAFE7C901402871E816229EF82039B`*

&nbsp;



**2.6. Install drivers for the Pico:**  
  - With the Raspberry Pi Pico unplugged, hold down the BOOTSEL button and plug in the Pico.  
  - Launch Zadig.  
  - Ensure "`RP2 Boot (Interface 1)`" is selected as the device  
    *(If you don't see `RP2 Boot (Interface 1)`, you may need to select `Options > List All Devices`)*.  
  - Ensure the current driver is listed as "`(NONE)`".  
  - Ensure "`WinUSB (...)`" is selected as the driver to install.  
  - Click "`Install Driver`", and wait for it to successfully install.

To confirm the bootloader driver is working, in a terminal enter:

  ```
  picotool info -a
  ```

This should return detailed info about the connected Pico.  
*If this doesn't work, maybe try installing other driver types for `RP2 boot (Interface 1)`*

Once the driver is working, in a terminal enter:

  ```
  picotool reboot
  ```

Ensure the Pico is still running the "`hello_usb`" firmware by checking your serial monitor.  

Return to Zadig to install one more driver:
  - Ensure "`Reset (Interface 2)`" is selected as the device *(if you don't see `Reset (Interface 2)`, you may need to select `Options > List All Devices`)*.
  - Ensure the current driver is listed as "`(NONE)`".
  - Ensure "`WinUSB (...)`" is selected as the driver to install.
  - Click "`Install Driver`", and wait for it to successfully install.  
    *(You may need to specifically select from the drop-down "Install Driver" if you are replacing an erroneous driver install)*

To confirm the application driver is working, in a terminal enter:

  ```
  picotool reboot -f -u
  ```

This should reboot the Pico into bootloader mode  
*(Note that picotool will only work outside of the bootloader like this for firmware with USB interfaces)*.

Your Pico drivers should be successfully installed!  
I am pretty certain you should only have to do this once, and it will work for any Pico you plug in in the future.

&nbsp;



**2.7. Build and upload debugprobe** - In a terminal enter:

  ```
  cd C:/tools/sdk/pico/debugprobe/
  mkdir build
  cd build
  cmake -DDEBUG_ON_PICO=ON ..
  make
  ```

*I received some compiler warnings from the FreeRTOS submodule, but none were fatal.*

With the Raspberry Pi Pico you wish to use as your debugger unplugged, hold down the BOOTSEL button and plug in that Pico.  
Copy the file `...\debugprobe\build\debugprobe.uf2` over to the Pico.

The LED on the debugprobe should now be on.

&nbsp;



**2.8. Upload firmware to the target Pico via OpenOCD:**

Wire your 2 Picos together as show in [Raspberry Pi's getting started guide](https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf#debugprobe-wiring-section).

In a terminal enter:

  ```
  cd C:/tools/sdk/pico/pico-examples/build/blink/
  openocd -s C:/tools/openocd/scripts -f interface/cmsis-dap.cfg -f target/rp2040.cfg -c "program blink.elf verify reset exit"
  ```

*(The switch `-s C:/tools/openocd/scripts` should not be necessary when using an official release build of OpenOCD)*

After a slow upload process, the target Pico should now be blinking.

&nbsp;



**2.9. Launch the OpenOCD server:**

In a terminal enter:

  ```
  openocd -s C:/tools/openocd/scripts -f interface/cmsis-dap.cfg -f target/rp2040.cfg -c "adapter speed 5000"
  ```

**Leave this terminal open!**

&nbsp;



**2.10. Launch GDB:**

In a *separate* terminal enter:

  ```
  cd C:/tools/sdk/pico/pico-examples/build/blink/
  arm-none-eabi-gdb blink.elf
  target extended-remote localhost:3333
  monitor reset init
  break gpio_put
  continue
  ```

Every time you enter `continue`, the light should blink and wait for your next `continue`.

Once that works, congratulations!  You now have the debugging tools all set up! 🎉

&nbsp;



### ***And finally, we'll set up the integrated development environment (IDE)***

**3.1. Install [VSCodium](https://github.com/VSCodium/vscodium/releases/tag/1.99.22418)** with personal or default settings.

*This project is built using the installer:*  
*`VSCodium-x64-updates-disabled-1.99.22418.msi`*  
*SHA256 checksum: `93F39264BAAB5D6106B2C9DD14604DD8024AAC2E8CEFFBABFD13AEEF4BAB5FC9`*

&nbsp;



**3.2. Install the [CMake Tools VSCode extension](https://open-vsx.org/extension/ms-vscode/cmake-tools/1.20.53):**

Install the extension's .VSIX file linked above:  

- In the left pane of VSCodium, click on the "Extensions" icon (four assembling blocks).  
- Select the "..." at the top of the list/searchbar, and select "Install from VSIX..."  
- Navigate to the .VSIX you downloaded and select it for installation.  

*(You can alternatively do this by searching within VSCode's extensions marketplace)*

In the "Extensions" panel, locate the newly installed CMake Tools extension and click the gear icon.  Select "Settings" and update the following settings:

  - Cmake: Generator
    > `MinGW Makefiles`
  - Cmake > Options: Status Bar Visibility
    > visible *or* compact

Additionally, if you'd like to avoid being asked to configure your project every time you open it, assert the following setting (ie. don't leave it as an unmodified default):

  - Cmake: Configure On Open
    > unchecked

Restart VSCodium.

*This project is built using the files:*  
*`ms-vscode.cmake-tools-1.20.53.vsix`*  
*SHA256 checksum: `C8324E31A9A734699A65366437B5829222E02D3B5525A9F4B6FD0C3A0DA8340E`*

&nbsp;



**3.3. Confirm the CMake extensions are working by creating a test project:**

- Create a directory in your personal files called `...\pico-testing\` to contain test/tutorial projects.  
- Create a sub-directory called `...\pico-testing\hello-world\` .  
- In VSCodium go to `File > Open Folder...` and open `...\pico-testing\hello-world\` .  
*If you added VSCodium to your context menu during installation, you can also just navigate to the `...\pico-testing\` directory in File Explorer and right click on the `hello-world\` folder to select "`Open with VSCodium`"*
- When prompted if you trust this workspace, select "`Trust Parent`" to trust all future files within `...\pico-testing\` .  
- Close the "Workspace Trust" tab.
- In the "`Explorer`" pane (two overlapping sheets of paper), click on the "`New File...`" button (a sheet of paper with a "+") next to the project title `HELLO-WORLD`.  
  Call this new file "`main.c`".  We'll come back to this later.
- Create another "`New File...`" and call it "`CMakeLists.txt`"
- Manually input the following code into "`CMakeLists.txt`":



  ```cmake
  # CMakeLists.txt

  # This is the version Raspberry Pi uses, no explicit reasoning provided
  cmake_minimum_required(VERSION 3.13)

  # Tell CMake and the Pico SDK to not optimize anything for debugging
  set(PICO_DEOPTIMIZED_DEBUG ON)

  # Pre-initialize the Pico SDK
  include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

  # Set PROJECT_NAME
  project(hello-world)

  # Invoke the Pico SDK's CMakeLists.txt to import build settings and definitions
  pico_sdk_init()

  # Tell CMake what/where our executable source file is
  add_executable(${PROJECT_NAME} main.c)

  # Link the project to pico_stdlib as defined by the SDK
  target_link_libraries(${PROJECT_NAME} pico_stdlib)

  # Enable UART stdio output
  pico_enable_stdio_uart(${PROJECT_NAME} ON)

  # Create map/bin/hex/uf2 files, in addition to ELF
  pico_add_extra_outputs(${PROJECT_NAME})

  ```



As you enter this code, the CMake Tools extension should suggest common auto-completions for you.

- At the bottom of VSCodium, you should now see information like "`CMake: [Debug]: Ready`" and "`No Kit Selected`".  
- Click on "`No Kit Selected`" at the bottom of the screen and select "`[Scan for kits]`" at the top of the window.
- Once it's done scanning, click on "`No Kit Selected`" again, and this time you should see three options (one from RISC-V, one from MinGW, and one from ARM).  
  Select "`GCC 14.2.1 arm-none-eabi`".
- Ensure that "`CMake: [Debug]: Ready`" is what is selected next to that.

CMake's output should appear in VSCodium's integrated terminal, and it should conclude with the successful message:  

  ```
  [cmake] -- Configuring done (...)
  [cmake] -- Generating done (...)
  [cmake] -- Build files have been written to: .../pico-testing/hello-world/build
  ```

> *If certain terminals within VSCodium are not detecting environment variables, like Git Bash not returning `echo $PICO_SDK_PATH`, you may need to fully close VSCodium and restart it without selecting a specific workspace (it should automatically open the most recent workspace).*  
*This may relate to having multiple VSCodium "workspaces" (windows) open at once?  I believe [this issue](https://github.com/microsoft/vscode/issues/47816) is tracking the behaviour.*

&nbsp;



**3.4. Extract the [clangd language server](https://github.com/clangd/clangd/releases/tag/20.1.0)** somewhere helpful.

*(This guide will assume it is extracted to `C:\tools\clangd\` with the directory's version number removed)*

*This project is built using the archive:*  
*`clangd-windows-20.1.0.zip`*  
*SHA256 checksum: `B1DD6C43522D85030A4B222142709C8691778BD45C2AD896C529EB95F7EDFA8A`*

&nbsp;



**3.5. Add `C:\tools\clangd\bin` to your system path** - *See step 1.10 if you need instructions for editing the system path.*

&nbsp;



**3.6. Install the [clangd VSCode extension](https://open-vsx.org/extension/llvm-vs-code-extensions/vscode-clangd/0.1.26):**

Install the extension's above .VSIX file as described in 3.2.

Access the clangd extension settings as described in 3.2 and change the following setting:

  - Clangd: Arguments - Add Items:

    > `--query-driver=C:/tools/mingw32/bin/*`

    > `--query-driver=C:/tools/armcc/bin/*`

    > `--query-driver=C:/tools/riscv/bin/*`

    *These will grant the clangd language server permission to access your compiler directories.*

    > `--compile-commands-dir=${workspaceFolder}/build`

    *This sets the default location clangd will search for `compile_commands.json`*

    I also have these settings asserted, but they are not required:

    > `--clang-tidy=true` *(Great for bug-prone warnings and style enforcement, but can get noisy)*

    > `--completion-style=bundled`

    > `--header-insertion=never`

Restart VSCodium.

<!-- TODO: Update to clangd 21 when it releases (adds QueryDriver to CompileFlags) -->

*This project is built using the file:*  
*`llvm-vs-code-extensions.vscode-clangd-0.1.33.vsix`*  
*SHA256 checksum: `3CB5C8A0319D74900B1B703A5821691CB97C722DA12065DFB99859078DCDED05`*

&nbsp;



**3.7. Configure your global clangd settings:**

Press the `Windows` key, type `%LOCALAPPDATA%`, and press enter.  Navigate into (or possibly create) `...\AppData\Local\clangd\` .

Create the file `...\AppData\Local\clangd\config.yaml` and enter the following initial settings:

*(Note - you should only add system-specific settings to your global configuration file, as it will always take priority over local project configurations!)*



  ```yaml
  # config.yaml

  CompileFlags:
    #BuiltinHeaders: QueryDriver  # Option should become available in clangd 21
    Add: [ "-nobuiltininc",
      # Manually toggle between the compilers below when low-level diagnostics are needed
      "-isystemC:\\tools\\armcc\\bin\\..\\lib\\gcc\\arm-none-eabi\\14.2.1\\..\\..\\..\\..\\arm-none-eabi\\include\\c++\\14.2.1","-isystemC:\\tools\\armcc\\bin\\..\\lib\\gcc\\arm-none-eabi\\14.2.1\\..\\..\\..\\..\\arm-none-eabi\\include\\c++\\14.2.1\\arm-none-eabi","-isystemC:\\tools\\armcc\\bin\\..\\lib\\gcc\\arm-none-eabi\\14.2.1\\..\\..\\..\\..\\arm-none-eabi\\include\\c++\\14.2.1\\backward","-isystemC:\\tools\\armcc\\bin\\..\\lib\\gcc\\arm-none-eabi\\14.2.1\\include","-isystemC:\\tools\\armcc\\bin\\..\\lib\\gcc\\arm-none-eabi\\14.2.1\\include-fixed","-isystemC:\\tools\\armcc\\bin\\..\\lib\\gcc\\arm-none-eabi\\14.2.1\\..\\..\\..\\..\\arm-none-eabi\\include","-D__DBL_MIN_EXP__=(-1021)","-D__HQ_FBIT__=15","-D__UINT_LEAST16_MAX__=0xffff","-D__ARM_SIZEOF_WCHAR_T=4","-D__ATOMIC_ACQUIRE=2","-D__WCHAR_MAX__=0xffffffffU","-D__SFRACT_IBIT__=0","-D__FLT_MIN__=1.1754943508222875e-38F","-D__GCC_IEC_559_COMPLEX=0","-D__UFRACT_MAX__=0XFFFFP-16UR","-D__UINT_LEAST8_TYPE__=unsigned char","-D__DQ_FBIT__=63","-D__INTMAX_C(c)=c ## LL","-D__ULFRACT_FBIT__=32","-D__CHAR_BIT__=8","-D__USQ_IBIT__=0","-D__UINT8_MAX__=0xff","-D__ACCUM_FBIT__=15","-D__WINT_MAX__=0xffffffffU","-D__FLT32_MIN_EXP__=(-125)","-D__USFRACT_FBIT__=8","-D__ORDER_LITTLE_ENDIAN__=1234","-D__SIZE_MAX__=0xffffffffU","-D__ARM_ARCH_ISA_ARM=1","-D__LACCUM_IBIT__=32","-D__DBL_DENORM_MIN__=((double)4.9406564584124654e-324L)","-D__GCC_ATOMIC_CHAR_LOCK_FREE=1","-D__GCC_IEC_559=0","-D__FLT32X_DECIMAL_DIG__=17","-D__FLT_EVAL_METHOD__=0","-D__TQ_IBIT__=0","-D__LLACCUM_MAX__=0X7FFFFFFFFFFFFFFFP-31LLK","-D__FLT64_DECIMAL_DIG__=17","-D__GCC_ATOMIC_CHAR32_T_LOCK_FREE=1","-D__UINT_FAST64_MAX__=0xffffffffffffffffULL","-D__SIG_ATOMIC_TYPE__=int","-D__DBL_MIN_10_EXP__=(-307)","-D__FINITE_MATH_ONLY__=0","-D__ARMEL__=1","-D__FLT32X_MAX_EXP__=1024","-D__LFRACT_IBIT__=0","-D__GNUC_PATCHLEVEL__=1","-D__FLT32_HAS_DENORM__=1","-D__LFRACT_MAX__=0X7FFFFFFFP-31LR","-D__USA_FBIT__=16","-D__UINT_FAST8_MAX__=0xffffffffU","-D__FLT32_MAX_10_EXP__=38","-D__ARM_ARCH_4T__=1","-D__INT8_C(c)=c","-D__INT_LEAST8_WIDTH__=8","-D__UINT_LEAST64_MAX__=0xffffffffffffffffULL","-D__SA_FBIT__=15","-D__SHRT_MAX__=0x7fff","-D__LDBL_MAX__=1.7976931348623157e+308L","-D__FRACT_MAX__=0X7FFFP-15R","-D__LDBL_IS_IEC_60559__=1","-D__UFRACT_FBIT__=16","-D__UFRACT_MIN__=0.0UR","-D__UINT_LEAST8_MAX__=0xff","-D__GCC_ATOMIC_BOOL_LOCK_FREE=1","-D__UINTMAX_TYPE__=long long unsigned int","-D__LLFRACT_EPSILON__=0x1P-63LLR","-D__FLT_EVAL_METHOD_TS_18661_3__=0","-D__UINT32_MAX__=0xffffffffUL","-D__ULFRACT_MAX__=0XFFFFFFFFP-32ULR","-D__TA_IBIT__=64","-D__LDBL_MAX_EXP__=1024","-D__WINT_MIN__=0U","-D__FLT32X_IS_IEC_60559__=1","-D__INT_LEAST16_WIDTH__=16","-D__ULLFRACT_MIN__=0.0ULLR","-D__SCHAR_MAX__=0x7f","-D__WCHAR_MIN__=0U","-D__INT64_C(c)=c ## LL","-D__GCC_ATOMIC_POINTER_LOCK_FREE=1","-D__ATOMIC_SEQ_CST=5","-D__SIZEOF_INT__=4","-D__FLT32X_MANT_DIG__=53","-D__USACCUM_IBIT__=8","-D__STDC_HOSTED__=1","-D__LFRACT_MIN__=(-0.5LR-0.5LR)","-D__HA_IBIT__=8","-D__DBL_DIG__=15","-D__FLT_EPSILON__=1.1920928955078125e-7F","-D__APCS_32__=1","-D__SHRT_WIDTH__=16","-D__FLT32_IS_IEC_60559__=1","-D__USFRACT_IBIT__=0","-D__LDBL_MIN__=2.2250738585072014e-308L","-D__STDC_UTF_16__=1","-D__DBL_IS_IEC_60559__=1","-D__FRACT_MIN__=(-0.5R-0.5R)","-D__DA_IBIT__=32","-D__ARM_SIZEOF_MINIMAL_ENUM=1","-D__FLT32X_HAS_INFINITY__=1","-D__INT32_MAX__=0x7fffffffL","-D__UQQ_FBIT__=8","-D__INT_WIDTH__=32","-D__UACCUM_MAX__=0XFFFFFFFFP-16UK","-D__DECIMAL_DIG__=17","-D__LFRACT_EPSILON__=0x1P-31LR","-D__FLT64_EPSILON__=2.2204460492503131e-16F64","-D__ULFRACT_MIN__=0.0ULR","-D__LDBL_HAS_QUIET_NAN__=1","-D__ULACCUM_IBIT__=32","-D__FLT64_MANT_DIG__=53","-D__UACCUM_EPSILON__=0x1P-16UK","-D__GNUC__=14","-D__ULLACCUM_MAX__=0XFFFFFFFFFFFFFFFFP-32ULLK","-D__HQ_IBIT__=0","-D__FLT_HAS_DENORM__=1","-D__SIZEOF_LONG_DOUBLE__=8","-D__SA_IBIT__=16","-D__BIGGEST_ALIGNMENT__=8","-D__FLT64_MAX_10_EXP__=308","-D__GNUC_STDC_INLINE__=1","-D__DQ_IBIT__=0","-D__DBL_MAX__=((double)1.7976931348623157e+308L)","-D__ULFRACT_IBIT__=0","-D__INT_FAST32_MAX__=0x7fffffff","-D__DBL_HAS_INFINITY__=1","-D__HAVE_SPECULATION_SAFE_VALUE=1","-D__ACCUM_IBIT__=16","-D__THUMB_INTERWORK__=1","-D__INTPTR_WIDTH__=32","-D__UINT_LEAST32_MAX__=0xffffffffUL","-D__LACCUM_MAX__=0X7FFFFFFFFFFFFFFFP-31LK","-D__FLT32X_HAS_DENORM__=1","-D__INT_FAST16_TYPE__=int","-D__LDBL_HAS_DENORM__=1","-D__INT_LEAST32_MAX__=0x7fffffffL","-D__ARM_PCS=1","-D__ACCUM_MAX__=0X7FFFFFFFP-15K","-D__DBL_MAX_EXP__=1024","-D__USACCUM_EPSILON__=0x1P-8UHK","-D__WCHAR_WIDTH__=32","-D__FLT32_MAX__=3.4028234663852886e+38F32","-D__GCC_ATOMIC_LONG_LOCK_FREE=1","-D__SFRACT_MAX__=0X7FP-7HR","-D__FRACT_IBIT__=0","-D__PTRDIFF_MAX__=0x7fffffff","-D__UACCUM_MIN__=0.0UK","-D__UACCUM_IBIT__=16","-D__LLACCUM_MIN__=(-0X1P31LLK-0X1P31LLK)","-D__FLT32_HAS_QUIET_NAN__=1","-D__LONG_LONG_MAX__=0x7fffffffffffffffLL","-D__ULACCUM_MAX__=0XFFFFFFFFFFFFFFFFP-32ULK","-D__SIZEOF_WINT_T__=4","-D__LONG_LONG_WIDTH__=64","-D__FLT32_MAX_EXP__=128","-D__ULLACCUM_MIN__=0.0ULLK","-D__GXX_ABI_VERSION=1019","-D__UTA_FBIT__=64","-D__FLT_MIN_EXP__=(-125)","-D__UFRACT_IBIT__=0","-D__INT16_MAX__=0x7fff","-D__INT_FAST64_TYPE__=long long int","-D__FLT64_DENORM_MIN__=4.9406564584124654e-324F64","-D__DBL_MIN__=((double)2.2250738585072014e-308L)","-D__SIZEOF_POINTER__=4","-D__SIZE_TYPE__=unsigned int","-D__DBL_HAS_QUIET_NAN__=1","-D__FLT32X_EPSILON__=2.2204460492503131e-16F32x","-D__FLT64_MIN_EXP__=(-1021)","-D__LACCUM_MIN__=(-0X1P31LK-0X1P31LK)","-D__FRACT_FBIT__=15","-D__ULLACCUM_FBIT__=32","-D__GXX_TYPEINFO_EQUALITY_INLINE=0","-D__FLT64_MIN_10_EXP__=(-307)","-D__ULLFRACT_EPSILON__=0x1P-64ULLR","-D__USES_INITFINI__=1","-D__REGISTER_PREFIX__=","-D__UINT16_MAX__=0xffff","-D__DBL_HAS_DENORM__=1","-D__ACCUM_MIN__=(-0X1P15K-0X1P15K)","-D__LDBL_HAS_INFINITY__=1","-D__SQ_IBIT__=0","-D__FLT32_MIN__=1.1754943508222875e-38F32","-D__UINT8_TYPE__=unsigned char","-D__UHA_FBIT__=8","-D__FLT_DIG__=6","-D__NO_INLINE__=1","-D__SFRACT_MIN__=(-0.5HR-0.5HR)","-D__UTQ_FBIT__=128","-D__DEC_EVAL_METHOD__=2","-D__FLT_MANT_DIG__=24","-D__LDBL_DECIMAL_DIG__=17","-D__VERSION__=\"14.2.1 20241119\"","-D__UINT64_C(c)=c ## ULL","-D__ULLFRACT_FBIT__=64","-D__SOFTFP__=1","-D__FRACT_EPSILON__=0x1P-15R","-D__ULACCUM_MIN__=0.0ULK","-D__UDA_FBIT__=32","-D__LLACCUM_EPSILON__=0x1P-31LLK","-D__GCC_ATOMIC_INT_LOCK_FREE=1","-D__FLOAT_WORD_ORDER__=__ORDER_LITTLE_ENDIAN__","-D__USFRACT_MIN__=0.0UHR","-D__FLT32_MANT_DIG__=24","-D__UQQ_IBIT__=0","-D__USFRACT_MAX__=0XFFP-8UHR","-D__SCHAR_WIDTH__=8","-D__INT32_C(c)=c ## L","-D__ORDER_PDP_ENDIAN__=3412","-D__UHQ_FBIT__=16","-D__LLACCUM_FBIT__=31","-D__INT_FAST32_TYPE__=int","-D__UINT_LEAST16_TYPE__=short unsigned int","-D__UINT64_MAX__=0xffffffffffffffffULL","-D__FLT_IS_IEC_60559__=1","-D__UDQ_FBIT__=64","-D__GNUC_WIDE_EXECUTION_CHARSET_NAME=\"UTF-32LE\"","-D__INT8_TYPE__=signed char","-D__ELF__=1","-D__GCC_ASM_FLAG_OUTPUTS__=1","-D__SACCUM_EPSILON__=0x1P-7HK","-D__ULFRACT_EPSILON__=0x1P-32ULR","-D__LLFRACT_FBIT__=63","-D__FLT_RADIX__=2","-D__INT_LEAST16_TYPE__=short int","-D__LDBL_EPSILON__=2.2204460492503131e-16L","-D__UINTMAX_C(c)=c ## ULL","-D__SACCUM_MAX__=0X7FFFP-7HK","-D__FLT32X_MIN__=2.2250738585072014e-308F32x","-D__SIG_ATOMIC_MAX__=0x7fffffff","-D__UACCUM_FBIT__=16","-D__GCC_ATOMIC_WCHAR_T_LOCK_FREE=1","-D__USER_LABEL_PREFIX__=","-D__VFP_FP__=1","-D__SIZEOF_PTRDIFF_T__=4","-D__FLT32_HAS_INFINITY__=1","-D__SIZEOF_LONG__=4","-D__LACCUM_EPSILON__=0x1P-31LK","-D__LDBL_DIG__=15","-D__FLT64_IS_IEC_60559__=1","-D__FLT32X_MIN_EXP__=(-1021)","-D__INT_FAST16_MAX__=0x7fffffff","-D__GCC_CONSTRUCTIVE_SIZE=64","-D__FLT64_DIG__=15","-D__UINT_FAST32_MAX__=0xffffffffU","-D__UINT_LEAST64_TYPE__=long long unsigned int","-D__SFRACT_EPSILON__=0x1P-7HR","-D__FLT_HAS_QUIET_NAN__=1","-D__FLT_MAX_10_EXP__=38","-D__LONG_MAX__=0x7fffffffL","-D__SIZEOF_SIZE_T__=4","-D__FLT_HAS_INFINITY__=1","-D__GNUC_EXECUTION_CHARSET_NAME=\"UTF-8\"","-D__CHAR_UNSIGNED__=1","-D__UINT_FAST16_TYPE__=unsigned int","-D__ARM_32BIT_STATE=1","-D__INT_FAST32_WIDTH__=32","-D__CHAR16_TYPE__=short unsigned int","-D__PRAGMA_REDEFINE_EXTNAME=1","-D__SIZE_WIDTH__=32","-D__INT_LEAST16_MAX__=0x7fff","-D__INT64_MAX__=0x7fffffffffffffffLL","-D__SACCUM_FBIT__=7","-D__FLT32_DENORM_MIN__=1.4012984643248171e-45F32","-D__SIG_ATOMIC_WIDTH__=32","-D__INT_LEAST64_TYPE__=long long int","-D__INT16_TYPE__=short int","-D__INT_LEAST8_TYPE__=signed char","-D__STDC_VERSION__=201710L","-D__SQ_FBIT__=31","-D__ARM_ARCH_ISA_THUMB=1","-D__INT_FAST8_MAX__=0x7fffffff","-D__ARM_ARCH=4","-D__INTPTR_MAX__=0x7fffffff","-D__QQ_FBIT__=7","-D__UTA_IBIT__=64","-D__FLT64_HAS_QUIET_NAN__=1","-D__FLT32_MIN_10_EXP__=(-37)","-D__FLT32X_DIG__=15","-D__UINT16_C(c)=c","-D__PTRDIFF_WIDTH__=32","-D__LDBL_MANT_DIG__=53","-D__SFRACT_FBIT__=7","-D__SACCUM_MIN__=(-0X1P7HK-0X1P7HK)","-D__FLT64_HAS_INFINITY__=1","-D__SIG_ATOMIC_MIN__=(-__SIG_ATOMIC_MAX__ - 1)","-D__INTPTR_TYPE__=int","-D__UINT16_TYPE__=short unsigned int","-D__WCHAR_TYPE__=unsigned int","-D__SIZEOF_FLOAT__=4","-D__TQ_FBIT__=127","-D__USQ_FBIT__=32","-D__UINTPTR_MAX__=0xffffffffU","-D__INT_FAST64_WIDTH__=64","-D__ULLACCUM_IBIT__=32","-D__FLT32_DECIMAL_DIG__=9","-D__INT_FAST64_MAX__=0x7fffffffffffffffLL","-D__GCC_ATOMIC_TEST_AND_SET_TRUEVAL=1","-D__FLT_NORM_MAX__=3.4028234663852886e+38F","-D__UINT_FAST64_TYPE__=long long unsigned int","-D__INT_MAX__=0x7fffffff","-D__LACCUM_FBIT__=31","-D__USACCUM_MIN__=0.0UHK","-D__UHA_IBIT__=8","-D__INT64_TYPE__=long long int","-D__FLT_MAX_EXP__=128","-D__UTQ_IBIT__=0","-D__DBL_MANT_DIG__=53","-D__INT_LEAST64_MAX__=0x7fffffffffffffffLL","-D__GCC_ATOMIC_CHAR16_T_LOCK_FREE=1","-D__WINT_TYPE__=unsigned int","-D__UINT_LEAST32_TYPE__=long unsigned int","-D__SIZEOF_SHORT__=2","-D__ULLFRACT_IBIT__=0","-D__FLT32_NORM_MAX__=3.4028234663852886e+38F32","-D__LDBL_MIN_EXP__=(-1021)","-D__arm__=1","-D__FLT64_MAX__=1.7976931348623157e+308F64","-D__UDA_IBIT__=32","-D__WINT_WIDTH__=32","-D__INT_LEAST8_MAX__=0x7f","-D__INT_LEAST64_WIDTH__=64","-D__FLT32X_MAX_10_EXP__=308","-D__LFRACT_FBIT__=31","-D__LDBL_MAX_10_EXP__=308","-D__ATOMIC_RELAXED=0","-D__DBL_EPSILON__=((double)2.2204460492503131e-16L)","-D__INT_LEAST32_TYPE__=long int","-D__UINT8_C(c)=c","-D__FLT64_MAX_EXP__=1024","-D__SIZEOF_WCHAR_T__=4","-D__UINT64_TYPE__=long long unsigned int","-D__LLFRACT_MAX__=0X7FFFFFFFFFFFFFFFP-63LLR","-D__FLT64_NORM_MAX__=1.7976931348623157e+308F64","-D__INTMAX_MAX__=0x7fffffffffffffffLL","-D__INT_FAST8_TYPE__=int","-D__ULLACCUM_EPSILON__=0x1P-32ULLK","-D__USACCUM_MAX__=0XFFFFP-8UHK","-D__UHQ_IBIT__=0","-D__ARM_FEATURE_COPROC=1","-D__LLACCUM_IBIT__=32","-D__FLT64_HAS_DENORM__=1","-D__FLT32_EPSILON__=1.1920928955078125e-7F32","-D__DBL_DECIMAL_DIG__=17","-D__STDC_UTF_32__=1","-D__INT_FAST8_WIDTH__=32","-D__FLT32X_MAX__=1.7976931348623157e+308F32x","-D__TA_FBIT__=63","-D__DBL_NORM_MAX__=((double)1.7976931348623157e+308L)","-D__BYTE_ORDER__=__ORDER_LITTLE_ENDIAN__","-D__GCC_DESTRUCTIVE_SIZE=64","-D__UDQ_IBIT__=0","-D__INTMAX_WIDTH__=64","-D__ORDER_BIG_ENDIAN__=4321","-D__FLT32_DIG__=6","-D__ACCUM_EPSILON__=0x1P-15K","-D__UINT32_C(c)=c ## UL","-D__FLT_DENORM_MIN__=1.4012984643248171e-45F","-D__LLFRACT_IBIT__=0","-D__INT8_MAX__=0x7f","-D__LONG_WIDTH__=32","-D__UINT_FAST32_TYPE__=unsigned int","-D__FLT32X_NORM_MAX__=1.7976931348623157e+308F32x","-D__CHAR32_TYPE__=long unsigned int","-D__FLT_MAX__=3.4028234663852886e+38F","-D__USACCUM_FBIT__=8","-D__INT32_TYPE__=long int","-D__SIZEOF_DOUBLE__=8","-D__FLT_MIN_10_EXP__=(-37)","-D__UFRACT_EPSILON__=0x1P-16UR","-D__FLT64_MIN__=2.2250738585072014e-308F64","-D__INT_LEAST32_WIDTH__=32","-D__INTMAX_TYPE__=long long int","-D__FLT32X_HAS_QUIET_NAN__=1","-D__ATOMIC_CONSUME=1","-D__GNUC_MINOR__=2","-D__INT_FAST16_WIDTH__=32","-D__UINTMAX_MAX__=0xffffffffffffffffULL","-D__FLT32X_DENORM_MIN__=4.9406564584124654e-324F32x","-D__HA_FBIT__=7","-D__DBL_MAX_10_EXP__=308","-D__LDBL_DENORM_MIN__=4.9406564584124654e-324L","-D__INT16_C(c)=c","-D__ATOMIC_RELEASE=3","-D__STDC__=1","-D__PTRDIFF_TYPE__=int","-D__LLFRACT_MIN__=(-0.5LLR-0.5LLR)","-D__DA_FBIT__=31","-D__UINT32_TYPE__=long unsigned int","-D__FLT32X_MIN_10_EXP__=(-307)","-D__UINTPTR_TYPE__=unsigned int","-D__USA_IBIT__=16","-D__ARM_EABI__=1","-D__LDBL_MIN_10_EXP__=(-307)","-D__SIZEOF_LONG_LONG__=8","-D__ULACCUM_EPSILON__=0x1P-32ULK","-D__SACCUM_IBIT__=8","-D__GCC_ATOMIC_LLONG_LOCK_FREE=1","-D__FLT_DECIMAL_DIG__=9","-D__UINT_FAST16_MAX__=0xffffffffU","-D__LDBL_NORM_MAX__=1.7976931348623157e+308L","-D__GCC_ATOMIC_SHORT_LOCK_FREE=1","-D__ULLFRACT_MAX__=0XFFFFFFFFFFFFFFFFP-64ULLR","-D__UINT_FAST8_TYPE__=unsigned int","-D__USFRACT_EPSILON__=0x1P-8UHR","-D__ULACCUM_FBIT__=32","-D__QQ_IBIT__=0","-D__ATOMIC_ACQ_REL=4",
      #"-isystemC:\\tools\\riscv\\bin\\..\\lib\\gcc\\riscv32-unknown-elf\\13.2.0\\..\\..\\..\\..\\riscv32-unknown-elf\\include\\c++\\13.2.0","-isystemC:\\tools\\riscv\\bin\\..\\lib\\gcc\\riscv32-unknown-elf\\13.2.0\\..\\..\\..\\..\\riscv32-unknown-elf\\include\\c++\\13.2.0\\riscv32-unknown-elf","-isystemC:\\tools\\riscv\\bin\\..\\lib\\gcc\\riscv32-unknown-elf\\13.2.0\\..\\..\\..\\..\\riscv32-unknown-elf\\include\\c++\\13.2.0\\backward","-isystemC:\\tools\\riscv\\bin\\..\\lib\\gcc\\riscv32-unknown-elf\\13.2.0\\include","-isystemC:\\tools\\riscv\\bin\\..\\lib\\gcc\\riscv32-unknown-elf\\13.2.0\\include-fixed","-isystemC:\\tools\\riscv\\bin\\..\\lib\\gcc\\riscv32-unknown-elf\\13.2.0\\..\\..\\..\\..\\riscv32-unknown-elf\\include","-D__riscv=1","-D__DBL_MIN_EXP__=(-1021)","-D__riscv_atomic=1","-D__UINT_LEAST16_MAX__=0xffff","-D__FLT16_HAS_QUIET_NAN__=1","-D__ATOMIC_ACQUIRE=2","-D__FLT128_MAX_10_EXP__=4932","-D__FLT_MIN__=1.17549435082228750796873653722224568e-38F","-D__GCC_IEC_559_COMPLEX=0","-D__UINT_LEAST8_TYPE__=unsigned char","-D__INTMAX_C(c)=c ## LL","-D__CHAR_BIT__=8","-D__UINT8_MAX__=0xff","-D__WINT_MAX__=0xffffffffU","-D__FLT32_MIN_EXP__=(-125)","-D__ORDER_LITTLE_ENDIAN__=1234","-D__WCHAR_MAX__=0x7fffffff","-D__GCC_HAVE_SYNC_COMPARE_AND_SWAP_2=1","-D__GCC_HAVE_SYNC_COMPARE_AND_SWAP_4=1","-D__GCC_ATOMIC_CHAR_LOCK_FREE=2","-D__riscv_float_abi_soft=1","-D__GCC_IEC_559=0","-D__FLT32X_DECIMAL_DIG__=17","-D__FLT_EVAL_METHOD__=0","-D__FLT64_DECIMAL_DIG__=17","-D__GCC_ATOMIC_CHAR32_T_LOCK_FREE=2","-D__UINT_FAST64_MAX__=0xffffffffffffffffULL","-D__SIG_ATOMIC_TYPE__=int","-D__DBL_MIN_10_EXP__=(-307)","-D__FINITE_MATH_ONLY__=0","-D__FLT32X_MAX_EXP__=1024","-D__GCC_HAVE_SYNC_COMPARE_AND_SWAP_1=1","-D__GNUC_PATCHLEVEL__=0","-D__FLT32_HAS_DENORM__=1","-D__UINT_FAST8_MAX__=0xffffffffU","-D__FLT32_MAX_10_EXP__=38","-D__INT8_C(c)=c","-D__INT_LEAST8_WIDTH__=8","-D__UINT_LEAST64_MAX__=0xffffffffffffffffULL","-D__SHRT_MAX__=0x7fff","-D__LDBL_MAX__=1.18973149535723176508575932662800702e+4932L","-D__FLT64X_MAX_10_EXP__=4932","-D__LDBL_IS_IEC_60559__=1","-D__FLT64X_HAS_QUIET_NAN__=1","-D__UINT_LEAST8_MAX__=0xff","-D__GCC_ATOMIC_BOOL_LOCK_FREE=2","-D__FLT128_DENORM_MIN__=6.47517511943802511092443895822764655e-4966F128","-D__UINTMAX_TYPE__=long long unsigned int","-D__FLT_EVAL_METHOD_TS_18661_3__=0","-D__UINT32_MAX__=0xffffffffUL","-D__DBL_DENORM_MIN__=((double)4.94065645841246544176568792868221372e-324L)","-D__LDBL_MAX_EXP__=16384","-D__FLT128_MIN_EXP__=(-16381)","-D__WINT_MIN__=0U","-D__FLT128_MIN_10_EXP__=(-4931)","-D__FLT32X_IS_IEC_60559__=1","-D__INT_LEAST16_WIDTH__=16","-D__SCHAR_MAX__=0x7f","-D__FLT128_MANT_DIG__=113","-D__WCHAR_MIN__=(-__WCHAR_MAX__ - 1)","-D__INT64_C(c)=c ## LL","-D__GCC_ATOMIC_POINTER_LOCK_FREE=2","-D__ATOMIC_SEQ_CST=5","-D__riscv_cmodel_medlow=1","-D__SIZEOF_INT__=4","-D__FLT32X_MANT_DIG__=53","-D__FLT64X_EPSILON__=1.92592994438723585305597794258492732e-34F64x","-D__STDC_HOSTED__=1","-D__DBL_DIG__=15","-D__FLT_EPSILON__=1.19209289550781250000000000000000000e-7F","-D__SHRT_WIDTH__=16","-D__FLT32_IS_IEC_60559__=1","-D__LDBL_MIN__=3.36210314311209350626267781732175260e-4932L","-D__STDC_UTF_16__=1","-D__DBL_IS_IEC_60559__=1","-D__FLT64X_DENORM_MIN__=6.47517511943802511092443895822764655e-4966F64x","-D__FLT32X_HAS_INFINITY__=1","-D__FLT16_DIG__=3","-D__INT_WIDTH__=32","-D__DECIMAL_DIG__=36","-D__FLT64_EPSILON__=2.22044604925031308084726333618164062e-16F64","-D__FLT128_IS_IEC_60559__=1","-D__FLT64X_MIN_10_EXP__=(-4931)","-D__LDBL_HAS_QUIET_NAN__=1","-D__FLT16_MIN_EXP__=(-13)","-D__FLT64_MANT_DIG__=53","-D__FLT64X_MANT_DIG__=113","-D__GNUC__=13","-D__FLT_HAS_DENORM__=1","-D__SIZEOF_LONG_DOUBLE__=16","-D__BIGGEST_ALIGNMENT__=16","-D__FLT64_MAX_10_EXP__=308","-D__FLT16_MAX_10_EXP__=4","-D__DBL_MAX__=((double)1.79769313486231570814527423731704357e+308L)","-D__INT_FAST32_MAX__=0x7fffffff","-D__DBL_HAS_INFINITY__=1","-D__INT64_MAX__=0x7fffffffffffffffLL","-D__INTPTR_WIDTH__=32","-D__UINT_LEAST32_MAX__=0xffffffffUL","-D__FLT32X_HAS_DENORM__=1","-D__INT_FAST16_TYPE__=int","-D__LDBL_HAS_DENORM__=1","-D__FLT128_HAS_INFINITY__=1","-D__DBL_MAX_EXP__=1024","-D__WCHAR_WIDTH__=32","-D__FLT32_MAX__=3.40282346638528859811704183484516925e+38F32","-D__GCC_ATOMIC_LONG_LOCK_FREE=2","-D__FLT16_DECIMAL_DIG__=5","-D__PTRDIFF_MAX__=0x7fffffff","-D__FLT32_HAS_QUIET_NAN__=1","-D__LONG_LONG_MAX__=0x7fffffffffffffffLL","-D__SIZEOF_SIZE_T__=4","-D__FLT64X_MIN_EXP__=(-16381)","-D__SIZEOF_WINT_T__=4","-D__LONG_LONG_WIDTH__=64","-D__FLT32_MAX_EXP__=128","-D__GXX_ABI_VERSION=1018","-D__FLT_MIN_EXP__=(-125)","-D__INT16_MAX__=0x7fff","-D__INT_FAST64_TYPE__=long long int","-D__FLT128_NORM_MAX__=1.18973149535723176508575932662800702e+4932F128","-D__FLT64_DENORM_MIN__=4.94065645841246544176568792868221372e-324F64","-D__FLT16_DENORM_MIN__=5.96046447753906250000000000000000000e-8F16","-D__FLT64X_NORM_MAX__=1.18973149535723176508575932662800702e+4932F64x","-D__SIZEOF_POINTER__=4","-D__SIZE_TYPE__=unsigned int","-D__DBL_HAS_QUIET_NAN__=1","-D__FLT32X_EPSILON__=2.22044604925031308084726333618164062e-16F32x","-D__FLT64_MIN_EXP__=(-1021)","-D__FLT64_MIN_10_EXP__=(-307)","-D__riscv_mul=1","-D__FLT16_MIN_10_EXP__=(-4)","-D__FLT64X_DECIMAL_DIG__=36","-D__REGISTER_PREFIX__=","-D__UINT16_MAX__=0xffff","-D__DBL_HAS_DENORM__=1","-D__LDBL_HAS_INFINITY__=1","-D__FLT32_MIN__=1.17549435082228750796873653722224568e-38F32","-D__UINT8_TYPE__=unsigned char","-D__FLT_DIG__=6","-D__NO_INLINE__=1","-D__DEC_EVAL_METHOD__=2","-D__FLT_MANT_DIG__=24","-D__LDBL_DECIMAL_DIG__=36","-D__VERSION__=\"13.2.0\"","-D__UINT64_C(c)=c ## ULL","-D__INT_LEAST32_MAX__=0x7fffffffL","-D__GCC_ATOMIC_INT_LOCK_FREE=2","-D__FLT128_MAX_EXP__=16384","-D__FLT32_MANT_DIG__=24","-D__FLOAT_WORD_ORDER__=__ORDER_LITTLE_ENDIAN__","-D__FLT32X_MIN_EXP__=(-1021)","-D__FLT128_HAS_DENORM__=1","-D__FLT128_DIG__=33","-D__SCHAR_WIDTH__=8","-D__INT32_C(c)=c ## L","-D__ORDER_PDP_ENDIAN__=3412","-D__riscv_muldiv=1","-D__INT_FAST32_TYPE__=int","-D__UINT_LEAST16_TYPE__=short unsigned int","-D__UINT64_MAX__=0xffffffffffffffffULL","-D__FLT_IS_IEC_60559__=1","-D__riscv_xlen=32","-D__GNUC_WIDE_EXECUTION_CHARSET_NAME=\"UTF-32LE\"","-D__FLT64X_DIG__=33","-D__INT8_TYPE__=signed char","-D__ELF__=1","-D__FLT_RADIX__=2","-D__INT_LEAST16_TYPE__=short int","-D__LDBL_EPSILON__=1.92592994438723585305597794258492732e-34L","-D__UINTMAX_C(c)=c ## ULL","-D__FLT32X_MIN__=2.22507385850720138309023271733240406e-308F32x","-D__SIG_ATOMIC_MAX__=0x7fffffff","-D__GCC_ATOMIC_WCHAR_T_LOCK_FREE=2","-D__USER_LABEL_PREFIX__=","-D__SIZEOF_PTRDIFF_T__=4","-D__FLT32_HAS_INFINITY__=1","-D__SIZEOF_LONG__=4","-D__LDBL_DIG__=33","-D__FLT64_IS_IEC_60559__=1","-D__FLT16_IS_IEC_60559__=1","-D__FLT16_MAX_EXP__=16","-D__INT_FAST16_MAX__=0x7fffffff","-D__FLT64_DIG__=15","-D__UINT_FAST32_MAX__=0xffffffffU","-D__UINT_LEAST64_TYPE__=long long unsigned int","-D__FLT16_EPSILON__=9.76562500000000000000000000000000000e-4F16","-D__FLT_HAS_QUIET_NAN__=1","-D__FLT_MAX_10_EXP__=38","-D__LONG_MAX__=0x7fffffffL","-D__FLT64X_HAS_DENORM__=1","-D__FLT_HAS_INFINITY__=1","-D__GNUC_EXECUTION_CHARSET_NAME=\"UTF-8\"","-D__CHAR_UNSIGNED__=1","-D__UINT_FAST16_TYPE__=unsigned int","-D__INT_FAST32_WIDTH__=32","-D__CHAR16_TYPE__=short unsigned int","-D__PRAGMA_REDEFINE_EXTNAME=1","-D__SIZE_WIDTH__=32","-D__INT_LEAST16_MAX__=0x7fff","-D__FLT16_NORM_MAX__=6.55040000000000000000000000000000000e+4F16","-D__FLT32_DENORM_MIN__=1.40129846432481707092372958328991613e-45F32","-D__SIG_ATOMIC_WIDTH__=32","-D__INT_LEAST64_TYPE__=long long int","-D__INT16_TYPE__=short int","-D__INT_LEAST8_TYPE__=signed char","-D__FLT16_MAX__=6.55040000000000000000000000000000000e+4F16","-D__FLT128_MIN__=3.36210314311209350626267781732175260e-4932F128","-D__STDC_VERSION__=201710L","-D__INT_FAST8_MAX__=0x7fffffff","-D__FLT128_MAX__=1.18973149535723176508575932662800702e+4932F128","-D__INTPTR_MAX__=0x7fffffff","-D__FLT64_HAS_QUIET_NAN__=1","-D__FLT32_MIN_10_EXP__=(-37)","-D__FLT32X_DIG__=15","-D__UINT16_C(c)=c","-D__PTRDIFF_WIDTH__=32","-D__LDBL_MANT_DIG__=113","-D__riscv_m=2000000","-D__INT_FAST16_WIDTH__=32","-D__FLT64_HAS_INFINITY__=1","-D__FLT64X_MAX__=1.18973149535723176508575932662800702e+4932F64x","-D__FLT16_HAS_INFINITY__=1","-D__SIG_ATOMIC_MIN__=(-__SIG_ATOMIC_MAX__ - 1)","-D__FLT16_MANT_DIG__=11","-D__INTPTR_TYPE__=int","-D__UINT16_TYPE__=short unsigned int","-D__WCHAR_TYPE__=int","-D__SIZEOF_FLOAT__=4","-D__UINTPTR_MAX__=0xffffffffU","-D__INT_FAST64_WIDTH__=64","-D__FLT32_DECIMAL_DIG__=9","-D__INT_FAST64_MAX__=0x7fffffffffffffffLL","-D__GCC_ATOMIC_TEST_AND_SET_TRUEVAL=1","-D__FLT_NORM_MAX__=3.40282346638528859811704183484516925e+38F","-D__FLT64X_MAX_EXP__=16384","-D__UINT_FAST64_TYPE__=long long unsigned int","-D__riscv_a=2001000","-D__riscv_c=2000000","-D__riscv_i=2001000","-D__INT_MAX__=0x7fffffff","-D__INT64_TYPE__=long long int","-D__FLT_MAX_EXP__=128","-D__DBL_MANT_DIG__=53","-D__INT_LEAST64_MAX__=0x7fffffffffffffffLL","-D__GCC_ATOMIC_CHAR16_T_LOCK_FREE=2","-D__WINT_TYPE__=unsigned int","-D__UINT_LEAST32_TYPE__=long unsigned int","-D__SIZEOF_SHORT__=2","-D__FLT32_NORM_MAX__=3.40282346638528859811704183484516925e+38F32","-D__LDBL_MIN_EXP__=(-16381)","-D__riscv_compressed=1","-D__FLT64_MAX__=1.79769313486231570814527423731704357e+308F64","-D__WINT_WIDTH__=32","-D__INT_LEAST8_MAX__=0x7f","-D__INT_LEAST64_WIDTH__=64","-D__FLT32X_MAX_10_EXP__=308","-D__FLT16_MIN__=6.10351562500000000000000000000000000e-5F16","-D__FLT64X_IS_IEC_60559__=1","-D__LDBL_MAX_10_EXP__=4932","-D__ATOMIC_RELAXED=0","-D__DBL_EPSILON__=((double)2.22044604925031308084726333618164062e-16L)","-D__INT_LEAST32_TYPE__=long int","-D__UINT8_C(c)=c","-D__FLT64_MAX_EXP__=1024","-D__SIZEOF_WCHAR_T__=4","-D__UINT64_TYPE__=long long unsigned int","-D__FLT64_NORM_MAX__=1.79769313486231570814527423731704357e+308F64","-D__FLT128_HAS_QUIET_NAN__=1","-D__INTMAX_MAX__=0x7fffffffffffffffLL","-D__INT_FAST8_TYPE__=int","-D__FLT64X_MIN__=3.36210314311209350626267781732175260e-4932F64x","-D__GNUC_STDC_INLINE__=1","-D__FLT64_HAS_DENORM__=1","-D__FLT32_EPSILON__=1.19209289550781250000000000000000000e-7F32","-D__FLT16_HAS_DENORM__=1","-D__DBL_DECIMAL_DIG__=17","-D__STDC_UTF_32__=1","-D__INT_FAST8_WIDTH__=32","-D__FLT32X_MAX__=1.79769313486231570814527423731704357e+308F32x","-D__DBL_NORM_MAX__=((double)1.79769313486231570814527423731704357e+308L)","-D__FLT64X_HAS_INFINITY__=1","-D__BYTE_ORDER__=__ORDER_LITTLE_ENDIAN__","-D__INTMAX_WIDTH__=64","-D__ORDER_BIG_ENDIAN__=4321","-D__FLT32_DIG__=6","-D__UINT32_C(c)=c ## UL","-D__riscv_arch_test=1","-D__FLT_DENORM_MIN__=1.40129846432481707092372958328991613e-45F","-D__INT8_MAX__=0x7f","-D__LONG_WIDTH__=32","-D__DBL_MIN__=((double)2.22507385850720138309023271733240406e-308L)","-D__INT32_MAX__=0x7fffffffL","-D__UINT_FAST32_TYPE__=unsigned int","-D__FLT32X_NORM_MAX__=1.79769313486231570814527423731704357e+308F32x","-D__CHAR32_TYPE__=long unsigned int","-D__FLT_MAX__=3.40282346638528859811704183484516925e+38F","-D__INT32_TYPE__=long int","-D__SIZEOF_DOUBLE__=8","-D__FLT_MIN_10_EXP__=(-37)","-D__FLT64_MIN__=2.22507385850720138309023271733240406e-308F64","-D__INT_LEAST32_WIDTH__=32","-D__INTMAX_TYPE__=long long int","-D__FLT32X_HAS_QUIET_NAN__=1","-D__ATOMIC_CONSUME=1","-D__GNUC_MINOR__=2","-D__UINTMAX_MAX__=0xffffffffffffffffULL","-D__FLT32X_DENORM_MIN__=4.94065645841246544176568792868221372e-324F32x","-D__DBL_MAX_10_EXP__=308","-D__LDBL_DENORM_MIN__=6.47517511943802511092443895822764655e-4966L","-D__INT16_C(c)=c","-D__STDC__=1","-D__PTRDIFF_TYPE__=int","-D__riscv_div=1","-D__UINT32_TYPE__=long unsigned int","-D__FLT32X_MIN_10_EXP__=(-307)","-D__UINTPTR_TYPE__=unsigned int","-D__LDBL_MIN_10_EXP__=(-4931)","-D__FLT128_EPSILON__=1.92592994438723585305597794258492732e-34F128","-D__SIZEOF_LONG_LONG__=8","-D__FLT128_DECIMAL_DIG__=36","-D__GCC_ATOMIC_LLONG_LOCK_FREE=1","-D__FLT_DECIMAL_DIG__=9","-D__UINT_FAST16_MAX__=0xffffffffU","-D__LDBL_NORM_MAX__=1.18973149535723176508575932662800702e+4932L","-D__GCC_ATOMIC_SHORT_LOCK_FREE=2","-D__SIZE_MAX__=0xffffffffU","-D__UINT_FAST8_TYPE__=unsigned int","-D__ATOMIC_ACQ_REL=4","-D__ATOMIC_RELEASE=3",
      #"-isystemC:\\tools\\mingw32\\bin\\..\\lib\\gcc\\i686-w64-mingw32\\14.2.0\\include\\c++","-isystemC:\\tools\\mingw32\\bin\\..\\lib\\gcc\\i686-w64-mingw32\\14.2.0\\include\\c++\\i686-w64-mingw32","-isystemC:\\tools\\mingw32\\bin\\..\\lib\\gcc\\i686-w64-mingw32\\14.2.0\\include\\c++\\backward","-isystemC:\\tools\\mingw32\\bin\\..\\lib\\gcc\\i686-w64-mingw32\\14.2.0\\include","-isystemC:\\tools\\mingw32\\bin\\..\\lib\\gcc\\i686-w64-mingw32\\14.2.0\\include-fixed","-isystemC:\\tools\\mingw32\\bin\\..\\lib\\gcc\\i686-w64-mingw32\\14.2.0\\..\\..\\..\\..\\i686-w64-mingw32\\include","-D__DBL_MIN_EXP__=(-1021)","-D__pentiumpro__=1","-D__UINT_LEAST16_MAX__=0xffff","-D__FLT16_HAS_QUIET_NAN__=1","-D__ATOMIC_ACQUIRE=2","-D__WCHAR_MAX__=0xffff","-D__FLT128_MAX_10_EXP__=4932","-D__FLT_MIN__=1.17549435082228750796873653722224568e-38F","-D__GCC_IEC_559_COMPLEX=2","-D__UINT_LEAST8_TYPE__=unsigned char","-D__SIZEOF_FLOAT80__=12","-D__BFLT16_DENORM_MIN__=9.18354961579912115600575419704879436e-41BF16","-D__INTMAX_C(c)=c ## LL","-D__CHAR_BIT__=8","-D__MINGW32__=1","-D__UINT8_MAX__=0xff","-D__WINT_MAX__=0xffff","-D__FLT32_MIN_EXP__=(-125)","-D__BFLT16_NORM_MAX__=3.38953138925153547590470800371487867e+38BF16","-D__BFLT16_MIN_10_EXP__=(-37)","-D__ORDER_LITTLE_ENDIAN__=1234","-D__GCC_HAVE_SYNC_COMPARE_AND_SWAP_2=1","-D__GCC_HAVE_SYNC_COMPARE_AND_SWAP_4=1","-D__GCC_HAVE_SYNC_COMPARE_AND_SWAP_8=1","-D__GCC_ATOMIC_CHAR_LOCK_FREE=2","-D__GCC_IEC_559=2","-D__FLT32X_DECIMAL_DIG__=17","-D__FLT_EVAL_METHOD__=2","-D__FLT64_DECIMAL_DIG__=17","-D__GCC_ATOMIC_CHAR32_T_LOCK_FREE=2","-D__UINT_FAST64_MAX__=0xffffffffffffffffULL","-D__SIG_ATOMIC_TYPE__=int","-D__DBL_MIN_10_EXP__=(-307)","-D__FINITE_MATH_ONLY__=0","-D__FLT32X_MAX_EXP__=1024","-D__GCC_HAVE_SYNC_COMPARE_AND_SWAP_1=1","-D__GNUC_PATCHLEVEL__=0","-D__FLT32_HAS_DENORM__=1","-D__UINT_FAST8_MAX__=0xff","-D__FLT32_MAX_10_EXP__=38","-D_stdcall=__attribute__((__stdcall__))","-D__DEC64_MAX_EXP__=385","-D__INT8_C(c)=c","-D__INT_LEAST8_WIDTH__=8","-D__UINT_LEAST64_MAX__=0xffffffffffffffffULL","-D__SHRT_MAX__=0x7fff","-D__LDBL_MAX__=1.18973149535723176502126385303097021e+4932L","-D__FLT64X_MAX_10_EXP__=4932","-D__BFLT16_MAX_EXP__=128","-D__LDBL_IS_IEC_60559__=1","-D__FLT64X_HAS_QUIET_NAN__=1","-D__UINT_LEAST8_MAX__=0xff","-D__GCC_ATOMIC_BOOL_LOCK_FREE=2","-D__LAHF_SAHF__=1","-D__FLT128_DENORM_MIN__=6.47517511943802511092443895822764655e-4966F128","-D__UINTMAX_TYPE__=long long unsigned int","-D__BFLT16_DECIMAL_DIG__=4","-D__DEC32_EPSILON__=1E-6DF","-D__FLT_EVAL_METHOD_TS_18661_3__=2","-D__UINT32_MAX__=0xffffffffU","-D__DBL_DENORM_MIN__=((double)4.94065645841246544176568792868221372e-324L)","-D__LDBL_MAX_EXP__=16384","-D__FLT128_MIN_EXP__=(-16381)","-D__WINT_MIN__=0","-D__FLT128_MIN_10_EXP__=(-4931)","-D__FLT32X_IS_IEC_60559__=1","-D__INT_LEAST16_WIDTH__=16","-D__SCHAR_MAX__=0x7f","-D__FLT128_MANT_DIG__=113","-D__WCHAR_MIN__=0","-D__INT64_C(c)=c ## LL","-D__GCC_ATOMIC_POINTER_LOCK_FREE=2","-D__ATOMIC_SEQ_CST=5","-D__INT_LEAST64_MAX__=0x7fffffffffffffffLL","-D__FLT32X_MANT_DIG__=53","-D__FLT64X_EPSILON__=1.08420217248550443400745280086994171e-19F64x","-D__STDC_HOSTED__=1","-D__DEC64_MIN_EXP__=(-382)","-D__DBL_DIG__=15","-D__FLT_EPSILON__=1.19209289550781250000000000000000000e-7F","-D__SHRT_WIDTH__=16","-D__FLT32_IS_IEC_60559__=1","-D__LDBL_MIN__=3.36210314311209350626267781732175260e-4932L","-D__STDC_UTF_16__=1","-D__DBL_IS_IEC_60559__=1","-D__DEC32_MAX__=9.999999E96DF","-D__FLT64X_DENORM_MIN__=3.64519953188247460252840593361941982e-4951F64x","-D__FLT32X_HAS_INFINITY__=1","-D__INT_WIDTH__=32","-D__DECIMAL_DIG__=21","-D__FLT64_EPSILON__=2.22044604925031308084726333618164062e-16F64","-D__FLT64X_MIN_10_EXP__=(-4931)","-D__LDBL_HAS_QUIET_NAN__=1","-D__FLT16_MIN_EXP__=(-13)","-D__FLT64_MANT_DIG__=53","-D__FLT64X_MANT_DIG__=64","-D__BFLT16_DIG__=2","-D__GNUC__=14","-D_cdecl=__attribute__((__cdecl__))","-D__FLT_HAS_DENORM__=1","-D__SIZEOF_LONG_DOUBLE__=12","-D__BIGGEST_ALIGNMENT__=16","-D__SIZE_TYPE__=unsigned int","-D__FLT64_MAX_10_EXP__=308","-D__BFLT16_IS_IEC_60559__=0","-D__i686=1","-D__FLT16_MAX_10_EXP__=4","-D__DBL_MAX__=((double)1.79769313486231570814527423731704357e+308L)","-D_thiscall=__attribute__((__thiscall__))","-D__INT_FAST32_MAX__=0x7fffffff","-D__DBL_HAS_INFINITY__=1","-D__INT64_MAX__=0x7fffffffffffffffLL","-D__WINNT__=1","-D__HAVE_SPECULATION_SAFE_VALUE=1","-D__DEC32_MIN_EXP__=(-94)","-D__INTPTR_WIDTH__=32","-D__UINT_LEAST32_MAX__=0xffffffffU","-D__FLT32X_HAS_DENORM__=1","-D__INT_FAST16_TYPE__=short int","-D_fastcall=__attribute__((__fastcall__))","-D__LDBL_HAS_DENORM__=1","-D__SEG_GS=1","-D__BFLT16_EPSILON__=7.81250000000000000000000000000000000e-3BF16","-D__FLT128_HAS_INFINITY__=1","-D__DEC128_MAX__=9.999999999999999999999999999999999E6144DL","-D__DEC32_MIN__=1E-95DF","-D__DBL_MAX_EXP__=1024","-D__WCHAR_WIDTH__=16","-D__FLT32_MAX__=3.40282346638528859811704183484516925e+38F32","-D__DEC128_EPSILON__=1E-33DL","-D__GCC_ATOMIC_LONG_LOCK_FREE=2","-D__FLT16_DECIMAL_DIG__=5","-D__ATOMIC_HLE_RELEASE=131072","-D__WIN32__=1","-D__PTRDIFF_MAX__=0x7fffffff","-D__ATOMIC_HLE_ACQUIRE=65536","-D__FLT32_HAS_QUIET_NAN__=1","-D__LONG_LONG_MAX__=0x7fffffffffffffffLL","-D__SIZEOF_SIZE_T__=4","-D__BFLT16_HAS_INFINITY__=1","-D__FLT64X_MIN_EXP__=(-16381)","-D__SIZEOF_WINT_T__=2","-D__LONG_LONG_WIDTH__=64","-D__FLT32_MAX_EXP__=128","-D__GXX_ABI_VERSION=1019","-D__FLT_MIN_EXP__=(-125)","-D__GCC_HAVE_DWARF2_CFI_ASM=1","-D__i686__=1","-D__INT16_MAX__=0x7fff","-D__INT_FAST64_TYPE__=long long int","-D__BFLT16_MAX__=3.38953138925153547590470800371487867e+38BF16","-D__FLT128_NORM_MAX__=1.18973149535723176508575932662800702e+4932F128","-D__BFLT16_HAS_DENORM__=1","-D__FLT64_DENORM_MIN__=4.94065645841246544176568792868221372e-324F64","-D__FLT16_DENORM_MIN__=5.96046447753906250000000000000000000e-8F16","-D__FLT64X_NORM_MAX__=1.18973149535723176502126385303097021e+4932F64x","-D__SIZEOF_POINTER__=4","-D__DBL_HAS_QUIET_NAN__=1","-D__FLT32X_EPSILON__=2.22044604925031308084726333618164062e-16F32x","-D__DECIMAL_BID_FORMAT__=1","-D__FLT64_MIN_EXP__=(-1021)","-D__GXX_TYPEINFO_EQUALITY_INLINE=0","-D__FLT64_MIN_10_EXP__=(-307)","-D__FLT16_MIN_10_EXP__=(-4)","-D__FLT64X_DECIMAL_DIG__=21","-D__DEC128_MIN__=1E-6143DL","-D__REGISTER_PREFIX__=","-D__UINT16_MAX__=0xffff","-D__DBL_HAS_DENORM__=1","-D__cdecl=__attribute__((__cdecl__))","-D__LDBL_HAS_INFINITY__=1","-D__FLT32_MIN__=1.17549435082228750796873653722224568e-38F32","-D__UINT8_TYPE__=unsigned char","-D__FLT_DIG__=6","-D__NO_INLINE__=1","-D__DEC_EVAL_METHOD__=2","-D__i386=1","-D__FLT_MANT_DIG__=24","-D__LDBL_DECIMAL_DIG__=21","-D__VERSION__=\"14.2.0\"","-D__UINT64_C(c)=c ## ULL","-D_WIN32=1","-D__INT_LEAST32_MAX__=0x7fffffff","-D__GCC_ATOMIC_INT_LOCK_FREE=2","-D__FLT128_MAX_EXP__=16384","-D__FLT32_MANT_DIG__=24","-D_X86_=1","-D__FLOAT_WORD_ORDER__=__ORDER_LITTLE_ENDIAN__","-D__FLT32X_MIN_EXP__=(-1021)","-Di386=1","-D__BFLT16_MIN__=1.17549435082228750796873653722224568e-38BF16","-D__FLT128_HAS_DENORM__=1","-D__FLT128_DIG__=33","-D_INTEGRAL_MAX_BITS=64","-D__SCHAR_WIDTH__=8","-D__INT32_C(c)=c","-D__DEC64_EPSILON__=1E-15DD","-D__ORDER_PDP_ENDIAN__=3412","-D__DEC128_MIN_EXP__=(-6142)","-D__code_model_32__=1","-D__INT_FAST32_TYPE__=int","-D__UINT_LEAST16_TYPE__=short unsigned int","-D__DEC128_MAX_EXP__=6145","-D__i386__=1","-D__UINT64_MAX__=0xffffffffffffffffULL","-D__FLT_IS_IEC_60559__=1","-D__GNUC_WIDE_EXECUTION_CHARSET_NAME=\"UTF-16LE\"","-D__FLT64X_DIG__=18","-D__INT8_TYPE__=signed char","-D__GCC_ASM_FLAG_OUTPUTS__=1","-D__BFLT16_HAS_QUIET_NAN__=1","-D__FLT_RADIX__=2","-D__INT_LEAST16_TYPE__=short int","-D__LDBL_EPSILON__=1.08420217248550443400745280086994171e-19L","-D__UINTMAX_C(c)=c ## ULL","-D__FLT16_DIG__=3","-D__FLT32X_MIN__=2.22507385850720138309023271733240406e-308F32x","-D__SIG_ATOMIC_MAX__=0x7fffffff","-D__GCC_ATOMIC_WCHAR_T_LOCK_FREE=2","-D__USER_LABEL_PREFIX__=_","-D__SIZEOF_PTRDIFF_T__=4","-D__FLT32_HAS_INFINITY__=1","-D__SIZEOF_LONG__=4","-D__LDBL_DIG__=18","-D__FLT64_IS_IEC_60559__=1","-D__FLT16_IS_IEC_60559__=1","-D__FLT16_MAX_EXP__=16","-D__DEC32_SUBNORMAL_MIN__=0.000001E-95DF","-D__pentiumpro=1","-D__MSVCRT__=1","-D__INT_FAST16_MAX__=0x7fff","-D__GCC_CONSTRUCTIVE_SIZE=64","-D__FLT64_DIG__=15","-D__UINT_FAST32_MAX__=0xffffffffU","-D__UINT_LEAST64_TYPE__=long long unsigned int","-D__FLT16_EPSILON__=9.76562500000000000000000000000000000e-4F16","-D__ILP32__=1","-D__FLT_HAS_QUIET_NAN__=1","-D__FLT_MAX_10_EXP__=38","-D__LONG_MAX__=0x7fffffffL","-D__FLT64X_HAS_DENORM__=1","-D__DEC128_SUBNORMAL_MIN__=0.000000000000000000000000000000001E-6143DL","-D__FLT_HAS_INFINITY__=1","-D__GNUC_EXECUTION_CHARSET_NAME=\"UTF-8\"","-D_ILP32=1","-D__UINT_FAST16_TYPE__=short unsigned int","-D__DEC64_MAX__=9.999999999999999E384DD","-D__INT_FAST32_WIDTH__=32","-D__CHAR16_TYPE__=short unsigned int","-D__PRAGMA_REDEFINE_EXTNAME=1","-D__USING_MCFGTHREAD__=1","-D__SIZE_WIDTH__=32","-D__SEG_FS=1","-D__BITINT_MAXWIDTH__=65535","-D__INT_LEAST16_MAX__=0x7fff","-D__FLT16_NORM_MAX__=6.55040000000000000000000000000000000e+4F16","-D__DEC64_MANT_DIG__=16","-D__FLT32_DENORM_MIN__=1.40129846432481707092372958328991613e-45F32","-D__SIG_ATOMIC_WIDTH__=32","-D__INT_LEAST64_TYPE__=long long int","-D__INT16_TYPE__=short int","-D__INT_LEAST8_TYPE__=signed char","-D__FLT16_MAX__=6.55040000000000000000000000000000000e+4F16","-D__FLT128_MIN__=3.36210314311209350626267781732175260e-4932F128","-D__STDC_VERSION__=201710L","-D__SIZEOF_INT__=4","-D__DEC32_MAX_EXP__=97","-D__INT_FAST8_MAX__=0x7f","-D__FLT128_MAX__=1.18973149535723176508575932662800702e+4932F128","-D__INTPTR_MAX__=0x7fffffff","-D__FLT64_HAS_QUIET_NAN__=1","-D__stdcall=__attribute__((__stdcall__))","-D__FLT32_MIN_10_EXP__=(-37)","-D__FLT32X_DIG__=15","-D__GXX_MERGED_TYPEINFO_NAMES=0","-D__UINT16_C(c)=c","-D__PTRDIFF_WIDTH__=32","-D__LDBL_MANT_DIG__=64","-D__INT_FAST16_WIDTH__=16","-D__FLT64_HAS_INFINITY__=1","-D__FLT64X_MAX__=1.18973149535723176502126385303097021e+4932F64x","-D__FLT16_HAS_INFINITY__=1","-D__SIG_ATOMIC_MIN__=(-__SIG_ATOMIC_MAX__ - 1)","-D__FLT16_MANT_DIG__=11","-D__INTPTR_TYPE__=int","-D__UINT16_TYPE__=short unsigned int","-D__WCHAR_TYPE__=short unsigned int","-D__SIZEOF_FLOAT__=4","-D__UINTPTR_MAX__=0xffffffffU","-D__INT_FAST64_WIDTH__=64","-D__FLT32_DECIMAL_DIG__=9","-D__INT_FAST64_MAX__=0x7fffffffffffffffLL","-D__GCC_ATOMIC_TEST_AND_SET_TRUEVAL=1","-D__FLT_NORM_MAX__=3.40282346638528859811704183484516925e+38F","-D__FLT64X_MAX_EXP__=16384","-D__UINT_FAST64_TYPE__=long long unsigned int","-D__BFLT16_MIN_EXP__=(-125)","-D__INT_MAX__=0x7fffffff","-DWIN32=1","-D__INT64_TYPE__=long long int","-D__FLT_MAX_EXP__=128","-D__DBL_MANT_DIG__=53","-D__SIZEOF_FLOAT128__=16","-D__BFLT16_MANT_DIG__=8","-D__GCC_ATOMIC_CHAR16_T_LOCK_FREE=2","-D__DEC64_MIN__=1E-383DD","-D__WINT_TYPE__=short unsigned int","-D__UINT_LEAST32_TYPE__=unsigned int","-D__SIZEOF_SHORT__=2","-D__FLT32_NORM_MAX__=3.40282346638528859811704183484516925e+38F32","-D__LDBL_MIN_EXP__=(-16381)","-D__FLT64_MAX__=1.79769313486231570814527423731704357e+308F64","-D__WINT_WIDTH__=16","-D__INT_LEAST8_MAX__=0x7f","-D__INT_LEAST64_WIDTH__=64","-D__FLT32X_MAX_10_EXP__=308","-D__WIN32=1","-D__FLT16_MIN__=6.10351562500000000000000000000000000e-5F16","-D__FLT64X_IS_IEC_60559__=1","-D__LDBL_MAX_10_EXP__=4932","-D__ATOMIC_RELAXED=0","-D__DBL_EPSILON__=((double)2.22044604925031308084726333618164062e-16L)","-D__INT_LEAST32_TYPE__=int","-D__thiscall=__attribute__((__thiscall__))","-D__UINT8_C(c)=c","-D__FLT64_MAX_EXP__=1024","-D__SIZEOF_WCHAR_T__=2","-D__UINT64_TYPE__=long long unsigned int","-D__WINNT=1","-D__FLT64_NORM_MAX__=1.79769313486231570814527423731704357e+308F64","-D__FLT128_HAS_QUIET_NAN__=1","-D__INTMAX_MAX__=0x7fffffffffffffffLL","-D__INT_FAST8_TYPE__=signed char","-D__fastcall=__attribute__((__fastcall__))","-D__FLT64X_MIN__=3.36210314311209350626267781732175260e-4932F64x","-D__GNUC_STDC_INLINE__=1","-D__FLT64_HAS_DENORM__=1","-D__FLT32_EPSILON__=1.19209289550781250000000000000000000e-7F32","-D__FLT16_HAS_DENORM__=1","-D__DBL_DECIMAL_DIG__=17","-D__STDC_UTF_32__=1","-D__INT_FAST8_WIDTH__=8","-D__FLT32X_MAX__=1.79769313486231570814527423731704357e+308F32x","-D__DBL_NORM_MAX__=((double)1.79769313486231570814527423731704357e+308L)","-D__FLT64X_HAS_INFINITY__=1","-D__BYTE_ORDER__=__ORDER_LITTLE_ENDIAN__","-D__GCC_DESTRUCTIVE_SIZE=64","-D__INTMAX_WIDTH__=64","-D__ORDER_BIG_ENDIAN__=4321","-D__FLT32_DIG__=6","-D__UINT32_C(c)=c ## U","-DWINNT=1","-D__FLT_DENORM_MIN__=1.40129846432481707092372958328991613e-45F","-D__FLT128_IS_IEC_60559__=1","-D__INT8_MAX__=0x7f","-D__LONG_WIDTH__=32","-D__DBL_MIN__=((double)2.22507385850720138309023271733240406e-308L)","-D__INT32_MAX__=0x7fffffff","-D__UINT_FAST32_TYPE__=unsigned int","-D__BFLT16_MAX_10_EXP__=38","-D__FLT32X_NORM_MAX__=1.79769313486231570814527423731704357e+308F32x","-D__CHAR32_TYPE__=unsigned int","-D__FLT_MAX__=3.40282346638528859811704183484516925e+38F","-D__INT32_TYPE__=int","-D__SIZEOF_DOUBLE__=8","-D__FLT_MIN_10_EXP__=(-37)","-D__FLT64_MIN__=2.22507385850720138309023271733240406e-308F64","-D__INT_LEAST32_WIDTH__=32","-D__INTMAX_TYPE__=long long int","-D__FLT32X_HAS_QUIET_NAN__=1","-D__ATOMIC_CONSUME=1","-D__GNUC_MINOR__=2","-D__UINTMAX_MAX__=0xffffffffffffffffULL","-D__DEC32_MANT_DIG__=7","-D__FLT32X_DENORM_MIN__=4.94065645841246544176568792868221372e-324F32x","-D__DBL_MAX_10_EXP__=308","-D__LDBL_DENORM_MIN__=3.64519953188247460252840593361941982e-4951L","-D__INT16_C(c)=c","-D__ATOMIC_RELEASE=3","-D__STDC__=1","-D__PTRDIFF_TYPE__=int","-D__UINT32_TYPE__=unsigned int","-D__FLT32X_MIN_10_EXP__=(-307)","-D__UINTPTR_TYPE__=unsigned int","-D__DEC64_SUBNORMAL_MIN__=0.000000000000001E-383DD","-D__DEC128_MANT_DIG__=34","-D__LDBL_MIN_10_EXP__=(-4931)","-D__FLT128_EPSILON__=1.92592994438723585305597794258492732e-34F128","-D__SIZEOF_LONG_LONG__=8","-D__FLT128_DECIMAL_DIG__=36","-D__GCC_ATOMIC_LLONG_LOCK_FREE=2","-D__FLT_DECIMAL_DIG__=9","-D__UINT_FAST16_MAX__=0xffff","-D__LDBL_NORM_MAX__=1.18973149535723176502126385303097021e+4932L","-D__GCC_ATOMIC_SHORT_LOCK_FREE=2","-D__SIZE_MAX__=0xffffffffU","-D__UINT_FAST8_TYPE__=unsigned char","-D__ATOMIC_ACQ_REL=4","-D__declspec(x)=__attribute__((x))",
    ]

  ```


*The above massive single-lines of `-isystem` include paths and `-D__...` defines enable more accurate cross-compiled diagnostics, and come from the following commands:*

```
arm-none-eabi-gcc -v -c -xc++ /dev/nul
riscv32-unknown-elf-gcc -v -c -xc++ /dev/nul
gcc -v -c -xc++ /dev/nul
```
```
echo | arm-none-eabi-gcc -dM -E - | sed 's/"/\\"/g ; s/#define /"-D/ ; s/$/"/ ; s/ /=/' | tr '\n' ',' > "$LOCALAPPDATA/clangd/macros/arm-none-eabi-gcc_macros.txt"
```
```
echo | riscv32-unknown-elf-gcc -dM -E - | sed 's/"/\\"/g ; s/#define /"-D/ ; s/$/"/ ; s/ /=/' | tr '\n' ',' > "$LOCALAPPDATA/clangd/macros/riscv32-unknown-elf-gcc_macros.txt"
```
```
echo | gcc -dM -E - | sed 's/"/\\"/g ; s/#define /"-D/ ; s/$/"/ ; s/ /=/' | tr '\n' ',' > "$LOCALAPPDATA/clangd/macros/gcc_macros.txt"
```

*For the time being, if you require accurate low-level diagnostics, you will need to manually change which series of defines are enabled based on which toolchain you are building with.  Issues related to this are tracked [here](https://github.com/clangd/clangd/issues/533) and [kind of here](https://github.com/clangd/clangd/issues/2079).*


&nbsp;



**3.8. Confirm the clangd extension is working in our test project:**

Because we ran CMake (and in particular, that the CMake Tools extension had CMake generate `...\build\compile_commands.json`), we will be able to fully enjoy clangd's code completion, diagnostics, and other helpful features.

Manually input the following code into "`main.c`":



  ```c
  // main.c

  #include <stdio.h>
  #include "pico/stdlib.h"

  #define PIN_LED 25

  int main() {

    stdio_init_all();

    gpio_init(PIN_LED);
    gpio_set_dir(PIN_LED, GPIO_OUT);

    while (true) {

      printf("Hello, world!\n");
      gpio_put(PIN_LED, true);
      sleep_ms(250);
      gpio_put(PIN_LED, false);
      sleep_ms(750);

    }

  }
  ```



You should see plenty of autocompletion suggestions from clangd as you compose this code.

You will know that the clangd language server is *fully* working if you hover over `<stdio.h>` and see that it recognizes it as being from:  
`C:\tools\armcc\arm-none-eabi\include\stdio.h`  
If this header file is not recognized (ie. it is underlined red), then you may need to double-check your paths (with asterisks) are correct in the `--query-driver` arguements added above.

> *clangd will only work effectively after you've generated `compile_commands.json` via CMake configuration*

> *For some projects, the pico-sdk generates necessary headers at build-time.  For clangd to be able to accurately diagnose references to these files, you will need to fully build the project first.*

> *clangd will only diagnose files you currently have open.  Track [this issue](https://github.com/clangd/vscode-clangd/issues/158) for updates.*

> You can force a refresh of clangd's diagnostics by pressing `Ctrl` + `Shift` + `P`, typing `clangd: Restart` and pressing `Enter`.

You also may see that `#include "pico/stdlib.h"` is underlined yellow.  This is because clangd identified we don't need all of the files referenced within `pico/stdlib.h` and it would prefer we include each smaller component individually.  This is known as [include-what-you-use (IWYU)](https://github.com/include-what-you-use/include-what-you-use/blob/master/docs/WhyIWYU.md).  
If you'd like to disable this warning, you can add the following to a file named `.clangd` in your project's root:


  ```yaml
  Diagnostics:
    UnusedIncludes: None
  ```

*I would advise against adding this to your global `config.yaml` file, as you may have projects in the future which rely on this feature, and the global config overwrites any project-level configuration.  Read [this guide](https://clangd.llvm.org/guides/include-cleaner#configuration) from clangd for more context.  It also might be worth tracking [this issue](https://github.com/clangd/clangd/issues/1913) on their GitHub, as this behaviour may change in future versions of clangd.*

&nbsp;



**3.9. Configure the Pico SDK**

We have thorough diagnostics from clangd within our project directory, but if you `Ctrl` + `Click` on one of the SDK functions, such as `gpio_put()`, it will take you to the source file in the SDK and there will be errors reported.

To fix this, in a terminal enter:

  ```
  cd C:/tools/sdk/pico/pico-sdk/
  mkdir build
  cd build
  cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=TRUE ..
  ```

  ```
  cd C:/tools/sdk/pico/pico-extras/
  mkdir build
  cd build
  cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=TRUE ..
  ```

Now if you return to our `hello-world` project, you should be able to browse the SDK source code with full diagnostic accuracy!

*If your SDK diagnostics are not updating, try saving or re-opening the file as that will prompt clangd to re-check for the SDK's `compile_commands.json`.*

> *If you ever need to change the Pico SDK version for testing, be sure to call:*  
> `git checkout <VERSION> --recurse-submodules` in the SDK directory.  
> *You may need to append* `--force` *the first time you change to a commit where submodules were historically added/removed (I think).*  
> *It would be wise to then rebuild the compile commands, also passing `--fresh` when calling CMake above.*

&nbsp;



**3.10. Install the [Cortex-Debug VSCode extension](https://open-vsx.org/extension/marus25/cortex-debug/1.12.1):**

Install the extension's above .VSIX file, as described in 3.2.

This extension depends on the following additional extensions, so install them in the same way:

  - **[debug-tracker-vscode](https://open-vsx.org/extension/mcu-debug/debug-tracker-vscode/0.0.15)**

  - **[MemoryView](https://open-vsx.org/extension/mcu-debug/memory-view/0.0.25)**

  - **[RTOS Views](https://open-vsx.org/extension/mcu-debug/rtos-views/0.0.7)**

  - **[Peripheral Viewer](https://open-vsx.org/extension/mcu-debug/peripheral-viewer/1.4.6)**

*If you install Cortex-Debug through VSCodium's extensions marketplace, the dependencies should automatically install as well.*

Restart VSCodium.

*This project is built using the files:*  
*`marus25.cortex-debug-1.12.1.vsix`*  
*SHA256 checksum: `F315306B5D964C1E40D2802FDF8AA2EC86C62C0A73477D46964666A8381DA71F`*  
*`mcu-debug.debug-tracker-vscode-0.0.15.vsix`*  
*SHA256 checksum: `0BF99FFB60DC82D8D2E5F27A64DDE7B54FC292236902EDAAEFCCDBB1675BFD3E`*  
*`mcu-debug.memory-view-0.0.25.vsix`*  
*SHA256 checksum: `6E928BE25A3F3655596A63084FE537881273F814EA4C808C57F0EDCF530AEC89`*  
*`mcu-debug.rtos-views-0.0.7.vsix`*  
*SHA256 checksum: `9FDE20791C9C3F0E6C12B1C7136AB798806FA55F122F5A84C2218AC076686DB9`*  
*`mcu-debug.peripheral-viewer-1.4.6.vsix`*  
*SHA256 checksum: `273BBD73794F3ED78D99E7388A3A8CFD4A7FC022BE35FC794D8858FCBCFB1AAA`*  

&nbsp;



**3.11. Configure VSCodium debug settings:**

By default, VSCodium will operate based on your user settings.  
If VSCodium finds a `.vscode` directory in the root of your project, then the settings within that directory take priority.

If you'd like your project(s) to use a local `.vscode` folder, refer to [`...\pico-examples\ide\vscode\README.md`](https://github.com/raspberrypi/pico-examples/blob/sdk-1.5.1/ide/vscode/README.md) for more information.  
This guide will set up the global settings instead.

- In VSCodium, click the gear icon in the bottom left and select "Settings"

- Search for `@id:launch`

- Select "`Edit in settings.json`"

- Input the following launch configuration into `settings.json`:



  ```json

      "launch": {
          "version": "0.2.0",
          "configurations": [

              {
                  "name": "🍫 Pico: RP2040",  // Compatible with Picoprobe Candybar, Debugprobe, and Pico as debugger
                  "cwd": "${workspaceFolder}",
                  "executable": "${command:cmake.launchTargetPath}",
                  "request": "launch",
                  "type": "cortex-debug",
                  "servertype": "openocd",
                  "gdbPath" : "arm-none-eabi-gdb",
                  "device": "RP2040",
                  "svdFile": "${env:PICO_SDK_PATH}/src/rp2040/hardware_regs/rp2040.svd",
                  "searchDir": [ "C:/tools/openocd/scripts" ],  // Not necessary once using an official build of OpenOCD
                  "configFiles": [
                      "interface/cmsis-dap.cfg",
                      "target/rp2040.cfg"
                  ],

                  "openOCDLaunchCommands": [
                      // This may be able to go as high as 24000 for certain setups
                      "adapter speed 5000",
                  ],

                  "postLaunchCommands": [
                      //"monitor reset halt",                    // (cortex-debug already sends "monitor reset halt" after target-download)
                      "set remote hardware-breakpoint-limit 4",
                      "set remote hardware-watchpoint-limit 2",
                      "monitor reset_config srst_only",          // Invoke a full hardware reset if connected; see below
                      "monitor adapter assert srst",
                      "monitor adapter deassert srst",
                      "monitor halt",
                      "monitor reset_config none",
                  ],

                  "overrideRestartCommands": [
                      "monitor reset halt",
                  ],

                  "overrideResetCommands": [
                      "monitor reset halt",              // If no hard-reset line is connected, this still produces a soft-reset ("restart")
                      "monitor reset_config srst_only",  // Enable the hard-reset signal
                      "monitor adapter assert srst",     // Manually assert/deassert hard-reset signal to avoid sending "monitor reset (run)"
                      "monitor adapter deassert srst",   // TODO: Why doesn't "monitor reset halt" work with srst_only?
                      "monitor halt",
                      "monitor reset_config none",       // Disable the hard-reset signal when we aren't using it
                  ]
              },

              {
                  "name": "🍫 Pico: RP2350-ARM",
                  "cwd": "${workspaceFolder}",
                  "executable": "${command:cmake.launchTargetPath}",
                  "request": "launch",
                  "type": "cortex-debug",
                  "servertype": "openocd",
                  "gdbPath" : "arm-none-eabi-gdb",
                  "device": "RP2350",
                  "svdFile": "${env:PICO_SDK_PATH}/src/rp2350/hardware_regs/rp2350.svd",
                  "searchDir": [ "C:/tools/openocd/scripts" ],
                  "configFiles": [
                      "interface/cmsis-dap.cfg",
                      "target/rp2350.cfg"
                  ],

                  "openOCDLaunchCommands": [
                      "adapter speed 5000",
                  ],

                  "postLaunchCommands": [
                      "set remote hardware-breakpoint-limit 4",
                      "set remote hardware-watchpoint-limit 2",
                      "monitor reset_config srst_only",
                      "monitor adapter assert srst",
                      "monitor adapter deassert srst",
                      "monitor halt",
                      "monitor reset_config none"
                  ],

                  "overrideRestartCommands": [
                      "monitor reset halt"
                  ],

                  "overrideResetCommands": [
                      "monitor reset halt",
                      "monitor reset_config srst_only",
                      "monitor adapter assert srst",
                      "monitor adapter deassert srst",
                      "monitor halt",
                      "monitor reset_config none"
                  ]
              },

              {
                  "name": "🍫 Pico: RP2350-RISCV",
                  "cwd": "${workspaceFolder}",
                  "executable": "${command:cmake.launchTargetPath}",
                  "request": "launch",
                  "type": "cortex-debug",
                  "servertype": "openocd",
                  "gdbPath" : "riscv32-unknown-elf-gdb",
                  "device": "RP2350",
                  "svdFile": "${env:PICO_SDK_PATH}/src/rp2350/hardware_regs/rp2350.svd",
                  "searchDir": [ "C:/tools/openocd/scripts" ],
                  "configFiles": [
                      "interface/cmsis-dap.cfg",
                      "target/rp2350-riscv.cfg"
                  ],

                  "openOCDLaunchCommands": [
                      "adapter speed 5000",
                  ],

                  "postLaunchCommands": [
                      "set remote hardware-breakpoint-limit 4",
                      "set remote hardware-watchpoint-limit 2",
                      "monitor reset_config srst_only",
                      "monitor adapter assert srst",
                      "monitor adapter deassert srst",
                      "monitor halt",
                      "monitor reset_config none"
                  ],

                  "overrideRestartCommands": [
                      "monitor reset halt"
                  ],

                  "overrideResetCommands": [
                      "monitor reset halt",
                      "monitor reset_config srst_only",
                      "monitor adapter assert srst",
                      "monitor adapter deassert srst",
                      "monitor halt",
                      "monitor reset_config none"
                  ]
              },

          ]
      }

  ```

  > The [RP2040 datasheet](https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf#section_Processor-Debug) suggests an adapter speed up to 24000, depending on your setup.  For reference, the highest I was able to get mine was 20000.

  > The [RP2040 datasheet](https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf#_debug) also lists the 4 hardware breakpoints and 2 watchpoints.  As of writing, automatic breakpoints set by GDB do not appear to count towards its hardware breakpoint limit.  I've opened [an issue](https://sourceware.org/bugzilla/show_bug.cgi?id=31403) directly to GDB, as well as [an issue](https://github.com/Marus/cortex-debug/issues/978) in Cortex-Debug to request a workaround explicitly sending hardware breakpoints to GDB.  
  **In the meantime, beware that setting more than 4 breakpoints through the UI will cause problems.**  
  *(This is not an issue if your program is set to run from RAM)*

  > There may be a feature in future versions of VSCode to reduce the repitition of each `"launch"` entry; keep an eye on the issue [here](https://github.com/microsoft/vscode/issues/53557)

  > *Raspberry Pi [recommends](https://github.com/raspberrypi/pico-examples/tree/sdk-2.1.1/ide/vscode) using a launch.json file included in `pico-examples`, however I will be using the above launch configuration as it provides more robust functionality.  Additional, more recent launch configurations by Raspberry Pi [can be found](https://github.com/raspberrypi/pico-vscode/blob/95db93b8325b682c20f41461905a67d11fa778b4/scripts/pico_project.py#L1010) in their VSCode extension.*



You're good to save and close `settings.json`.

Raspberry Pi also recommends asserting certain CMake Tools settings on the project-level to avoid confusion against the debugger buttons (see [here](https://github.com/raspberrypi/pico-examples/blob/sdk-2.1.1/ide/vscode/settings.json)), but I personally prefer keeping all my CMake Tools UI at the bottom bar, and my debugger UI in the side panel.  
If you'd like to follow my preference, ensure these *(global)* settings are set:

- Debug > Show In Status Bar
  > never

- Cmake > Options: Status Bar Visibility
  > visible

- Cmake > Build Before Run
  > *checked*

Restart VSCodium.

> *As before, if certain terminals within VSCodium are not detecting environment variables, like Git Bash not returning* `arm-none-eabi-gdb -v`*, you may need to fully close VSCodium and restart it without selecting a specific workspace (it should automatically open the most recent workspace).*  
*This may relate to having multiple VSCodium "workspaces" (windows) open at once.*

And with that, we're all set to begin debugging!

&nbsp;



### ***Configure, build, and debug with a single click in your IDE!***

**4. Start debugging!**

Click on the "Run and Debug" icon (a play button with a bug next to it).

You should see at the top of the "Run and Debug" panel a green play button and a drop-down menu.  
Ensure this drop-down menu says "Pico: RP2040" *(assuming you're been using an RP2040-based Pico)*.

Press play.

The first time you run the debugger, you'll need to select what file you are targetting.  
Select `hello-world`.

When you first run the debugger, it may appear to hang with nothing changing in its output - do be patient (up to a minute).

And with that...

### ***Congratulations!*** 🎉

***Your entire toolchain, debugger, and IDE are all set up and ready for development!***

Set breakpoints with the red dots next to the line numbers (including conditional breakpoints) and step through code with the controls floating at the top of the window :)  
You can read more about using VSCode's debugging features [here](https://code.visualstudio.com/Docs/editor/debugging).

> ***Note:** there's [a bug](https://github.com/microsoft/vscode/issues/246841) in VSCode that causes the debugger to hang if you press the "Restart" button (the circular arrow **without** a red dot in the middle.  Until that issue is addressed, I recommend right-clicking on the button bar whilst debugging, and uncheck "Restart" as to avoid accidentally using it.*

The Cortex-Debug extension (and its dependencies) also feature the tabs "MEMORY" and "xRTOS" alongside the "TERMINAL" and "DEBUG CONSOLE" in the bottom pane.
I have not yet explored implementing FreeRTOS, so I hide the xRTOS view for simplicity.

And if you'd like to view raw memory, then feel free to enter the "MEMORY" tab, and while debugging a program on the Pico click the plus icon next to the drop-down.  When prompted, enter `0x20000000` as the memory address (this is the [starting address](https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf#_address_map) of the RP2040 SRAM)



&nbsp;

---

&nbsp;



### ***That's it!  You made it to the end!  Go make something cool!***

### ***Happy hacking!!***
