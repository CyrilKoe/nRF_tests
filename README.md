## Set-up your environment on Windows 10
### Install git
https://git-scm.com/download/win
### Install the SDK 1.6.0
Download and install the latest version of the __nRF connect__ with the link below : \
https://www.nordicsemi.com/Products/Development-tools/nRF-Connect-for-desktop/Download#infotabs \
Install with default selections in order to install required tools as `J-Link`.
Open `nRF connect` application and install `Bluetooth low energy` and `Toolchain manager` apps. \
Open the `toolchain manager` and you can now install the `nRF Connect SDK v1.6.0`. \

<p align="center">
    <img src="medias/tutorial_install_sdk.jpg" alt="Installation of the SDK" width="600" />
</p>

A window will open to propose you the firsts steps to build a project, click `close`. \
Once the installation of the SDK is complete click the `open ide` button. \
Configure the window depending on your board :

<p align="center">
    <img src="medias/tutorial_open_ide.jpg" alt="Opening the IDE" width="600" />
</p>

_( If the window does not open you can find it on SEGGER with `File` , `Open nRF Connect SDK Project` )_ \
__SEGGER Embeded Studio__ will open on the example project, now click `View` , `Toolbars` , `Show all toolbars` to open all toolbars. \

### Build and flash the application

Now try building and debuging your application on the card with the following buttons (don't forget to continue running after the first breakpoint).

<p align="center">
    <img src="medias/tutorial_compiler.jpg" alt="Compile and debug buttons" width="600" />
</p>

You can also only flash with `Target` , `Download zephyr/zephyr.elf` \
_( If the flashing does not work try `Target` , `Connect J-Link` )_ \
_( If connecting gives `Cannot connect: active project is not an executable` , select `zephyr` as your current project as below ) :_

<p align="center">
    <img src="medias/tutorial_select_project.jpg" alt="Select current project" width="600" />
</p>

In order to activate the RTT logging modify `prj.conf` (located in the `peripheral_lbs` folder) and add the following lines :

```
# choose RTT console
CONFIG_UART_CONSOLE=n
CONFIG_USE_SEGGER_RTT=y
CONFIG_RTT_CONSOLE=y

# General config
CONFIG_LOG=y
CONFIG_LOG_DEFAULT_LEVEL=4
```

Now build and debug and you will see the `printk` output.

