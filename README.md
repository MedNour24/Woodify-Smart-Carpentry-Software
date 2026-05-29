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

## Quick Start

1. Import `2A7.sql` into your database and make sure the ODBC connection is ready.
2. Build the project with the qmake PowerShell script or the VS Code tasks.
3. Set `AZURE_AI_API_KEY`, `GEMINI_API_KEY`, `FACEPP_API_KEY`, and `FACEPP_API_SECRET` if you want the AI and face features.
4. Launch `Smartcarpentry.exe` and sign in.
5. Use the left menu to open employees, materials, fabrication, models, and pieces.

## How To Use

1. Prepare the database first.
  - Make sure the ODBC DSN or connection settings match your environment.
  - Import the schema from `2A7.sql` and any module-specific SQL files you need.

2. Build the application.
  - Use the Qt kit and build system that match your compiler.
  - The recommended path is the qmake flow described above.

3. Set the required environment variables before launching if you use the online features.
  - `AZURE_AI_API_KEY` for the employee assistant and image generation.
  - `GEMINI_API_KEY` if you want the furniture assistant to use Gemini.
  - `FACEPP_API_KEY` and `FACEPP_API_SECRET` for face detection and comparison.
  - Optional: `SC_SKIP_FACE_ID=1` to bypass face verification during local testing.

4. Start the app and log in.
  - Launch `Smartcarpentry.exe` from the `debug` or `release` folder.
  - Enter a valid account in the login screen.

5. Use the main dashboard.
  - Navigate with the left sidebar to access employees, wood inventory, fabrication, models, and pieces.
  - Use the search bar at the top to filter records.

6. Work module by module.
  - Employees: add, edit, delete, filter, export, and manage training or RFID-related data.
  - Bois/materials: manage inventory and recommendations.
  - Fabrication: track jobs, phases, charts, and smart alerts.
  - Models and pieces: generate pieces, preview models, and run nesting optimization.

7. Use the smart features when needed.
  - RFID and Arduino integrations require the device and serial port to be connected.
  - AI-assisted modules require the relevant API keys to be present before launch.

8. Close the app normally from the interface when finished.

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

