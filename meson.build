project(
  'mailnagtray',
  'c', 'cpp',
  version : '0.2',
  license: 'GPL',
  default_options: ['c_std=c11', 'cpp_std=c++17']
)

# QT Enablement
qt = import('qt6')
qt_deps = dependency('qt6', modules: ['Core', 'Gui', 'Widgets', 'DBus'])

# Set include directories
includes = include_directories('src')

# Main non processed source files
sources = files(
  'src/main.cpp',
  'src/tray.cpp',
  'src/emailswindow.cpp',
  'src/listwidget.cpp',
  'src/mailnagdbus.cpp',
  'src/util.cpp',
)

# QT processed files
qtprocessed = qt.preprocess(
  moc_headers: ['src/tray.h', 'src/emailswindow.h', 'src/listwidget.h'],
  include_directories: includes
)

sources += qtprocessed

exe = executable('mailnagtray',
  sources,
  include_directories: includes,
  dependencies: qt_deps,
  install: true,
  install_dir: 'bin'
)

# Installation files
install_data('data/mailnagtray-startup.desktop',
  rename : 'mailnagtray.desktop',
  install_dir : '/etc/xdg/autostart'
)
install_data('data/mailnagtray.desktop',
  install_dir : 'share/applications'
)

