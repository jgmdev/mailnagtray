project(
  'mailnagtray', 'c', 'cpp',
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
  'src/emailsdialog.cpp',
  'src/mailnagdbus.cpp'
)

# QT processed files
qtprocessed = qt.preprocess(
  moc_headers: ['src/tray.h', 'src/emailsdialog.h'],
  # ui_files: 'emails.ui',
  include_directories: includes
)

sources += qtprocessed

exe = executable('mailnagtray',
  sources,
  include_directories: includes,
  dependencies: qt_deps,
  install: true
)

