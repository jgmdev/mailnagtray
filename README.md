# Mailnag Tray

A tray icon applet for mail monitoring using mailnag and written on QT to
support any desktop environment that supports a standard system tray.

* OS: Linux
* License: GPLv2

## Requirements

* [Mailnag](https://github.com/pulb/mailnag) - mail notifications
* [QT6](https://www.qt.io/) - graphical interface and dbus ipc
* [Meson](https://mesonbuild.com/) - build the application

## Compilation

To build the application execute the following commands:

```sh
meson setup build --prefix /usr
meson compile -C build
```

## Installation

After a successfull build you can install the utility globally by executing:

```sh
sudo meson install -C build
```

## Screenshots

![RGB2CMYK main](https://raw.githubusercontent.com/jgmdev/mailnagtray/master/screenshots/tray.png)
