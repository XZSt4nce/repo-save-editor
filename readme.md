R.E.P.O Save Editor (Qt/C++ Version)
=======

---
## Overview

R.E.P.O Save Editor is a graphical user interface (GUI) application built using Qt and C++. It's specifically designed to edit and manage save files (.es3 format) for the game R.E.P.O. This tool enables users to easily modify game data such as currency, player health, player upgrades, items, and other relevant game statistics through a user-friendly interface.

Inspired by: [N0edL's R.E.P.O Save Editor](https://github.com/N0edL/R.E.P.O-Save-Editor)

---
## Features

- Decrypt and Load .es3 Save Files: Easily open and decrypt .es3 save files for modification.

- User-Friendly Interface: Edit various game data such as:

- Player statistics (health, upgrades, and more)

- Currency and financial data

- Item quantities and properties

- General world statistics (level, total haul, charging stations, etc.)

- JSON-Based Editing: Internally converts decrypted .es3 data into JSON for clear, intuitive editing.

- Re-Encrypt and Save: Securely saves your modifications by re-encrypting the JSON data back into the .es3 format compatible with the game.

---
## How to Use

### Download and Installation

- Download: Obtain the latest release from the Releases page.

- Installation: Extract the downloaded files to your preferred location.

### Launching the Application

- Simply double-click on R.E.P.O_Save_Editor.exe (Windows) to launch the application.

### Editing Save Files

1. Open a Save File:

    - Click "File" → "Open" and navigate to your save file (.es3).

    - The tool decrypts and loads the save data for editing.

2. Edit Game Data:

    - Modify player health, currency, item amounts, player upgrades, and other game statistics using intuitive input fields.

3. Save Your Changes:

    - Click "File" → "Save" to re-encrypt and save your changes back to the original .es3 format.

---
## Contributing

Contributions are welcome! Feel free to fork the repository and submit pull requests for improvements, new features, or bug fixes.

---
## Developer Information

To build or contribute to this project, ensure you have the following:

- Qt 6.7 or newer (recommended)
- Crypto++ library
- C++20 compatible compiler

### Libraries installation

[![vcpkg crypto++](https://img.shields.io/badge/crypto++-8.9.0-green)](https://repology.org/metapackage/cryptopp)

With [vcpkg](https://github.com/Microsoft/vcpkg) on Windows
```
PS> vcpkg add port cryptopp
PS> vcpkg install
```


---
## Contributions

Enjoy using the R.E.P.O Save Editor! Feel free to open issues or suggestions on GitHub if you encounter any problems or have ideas for improvements.
