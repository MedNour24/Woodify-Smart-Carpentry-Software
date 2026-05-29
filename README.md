# SmartCarpentry

SmartCarpentry is a Qt/C++ desktop application for managing carpentry operations end to end.
It combines business workflows (employees, materials, models, fabrication phases, pieces) with optimization and smart-assistance features such as nesting optimization, RFID attendance, and Arduino-based integrations.

## Highlights

- Desktop application built with Qt 6 and C++17
- Login and account creation flow with hashed password handling
- CRUD management for:
  - Employees
  - Wood/material inventory
  - Fabrication orders
  - Production phases
  - Models and pieces
- Piece generation and cutting workflow support
- AI-assisted nesting optimization (layout optimization to reduce waste)
- Cost estimation and performance dashboards
- PDF export in multiple modules
- RFID attendance/event logging support
- Arduino integration (board measurements, signals, fire-alert related events)

## Technology Stack

- Language: C++17
- UI Framework: Qt 6 (Widgets)
- Qt Modules: Core, Gui, Widgets, Sql, Charts, Multimedia, PrintSupport, Network, SerialPort
- Build Systems:
  - qmake (.pro)
  - CMake (alternative)
- Database Access: Qt SQL via ODBC (`QODBC`)

## Project Structure

Key files and folders:

- `Smartcarpentry.pro`: main qmake project file
- `CMakeLists.txt`: alternative CMake configuration
- `main.cpp`: app entry point
- `smartcarpentry.*`: main window and feature orchestration
- `login.*`: authentication and account creation UI
- `connsql.*`: singleton database connection manager
- `nestingengine.*`, `nestingwidget.*`, `panel.*`, `piecerect.*`: nesting optimizer module
- `rfidmanager.*`: RFID event handling
- `arduinohandler.*`, `arduinomanager.*`, `arduinoranim.*`: Arduino connectivity and events
- `tools/build_qmake.ps1`: helper build script used by VS Code tasks

## Prerequisites

- Windows 10/11 (primary target in current setup)
- Qt 6 with a desktop kit (MinGW or MSVC)
- qmake and corresponding make tool (`mingw32-make`, `jom`, or `nmake`)
- C++17-capable compiler
- ODBC driver configured for your target database

Optional:

- RFID reader device (serial)
- Arduino board(s)
- OpenCV (only if enabling OpenCV-specific code paths)

## Database Setup

The application initializes a database connection at startup through `ConnSQL`.

Current default connection values in code:

- DSN / Database name: `2A7`
- User: `carp`
- Password: `123`

You can:

- Create a matching ODBC DSN (`2A7`) and user credentials, or
- Update connection settings in `connsql.cpp` for your environment

SQL schema files available in the repository include:

- `2A7.sql`
- `arduino_scanner/piece_panel_schema.sql`

For RFID-specific schema/workflow details, see `RFID_ATTENDANCE_GUIDE.md`.

## Build and Run (Recommended: qmake)

### Option 1: VS Code Tasks

Use the included tasks:

- `qmake: configure`
- `qmake: build debug`
- `qmake: build release`
- `qmake: clean and reconfigure`

These tasks call `tools/build_qmake.ps1` and can also deploy Qt runtime DLLs using `windeployqt` when available.

### Option 2: PowerShell Script

From project root:

```powershell
powershell -ExecutionPolicy Bypass -File tools/build_qmake.ps1 -ConfigureOnly
powershell -ExecutionPolicy Bypass -File tools/build_qmake.ps1 -BuildMode debug
powershell -ExecutionPolicy Bypass -File tools/build_qmake.ps1 -BuildMode release
```

Run the built app:

```powershell
.\debug\Smartcarpentry.exe
# or
.\release\Smartcarpentry.exe
```

## Build with CMake (Alternative)

The project also includes a CMake configuration.

```powershell
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --config Debug
```

If your Qt installation is not auto-detected, provide `CMAKE_PREFIX_PATH` pointing to your Qt 6 installation.

## Configuration Notes

### RFID Port

Set environment variable before launch if needed:

```powershell
$env:SC_RFID_PORT = "COM3"
```

### OpenCV Support

OpenCV integration is optional and disabled by default.

- In qmake: add include/lib paths in `Smartcarpentry.pro`
- In CMake: build with `-DENABLE_OPENCV=ON` and provide expected OpenCV files

## Related Documentation

- `AI_NESTING_OPTIMIZER_README.md`
- `RFID_ATTENDANCE_GUIDE.md`
- `OPENCV_CMAKE_SETUP.md`

## Current Status

This repository appears to be an actively developed academic/prototype-style project with generated build artifacts (`debug/`, `release/`) committed.

For production hardening, consider:

- Moving DB credentials to environment variables or secure config
- Adding unit/integration tests and CI
- Cleaning generated artifacts from version control
- Documenting deployment profiles per target environment

## License

No license file is currently defined in the repository.

If you plan to share or publish this project publicly, add a `LICENSE` file (for example MIT, Apache-2.0, or GPL) and update this section.

## Contributors

