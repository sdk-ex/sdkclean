# Welcome to the `sdkclean` repository

## Description

This project contains a custom tool for cleaning Linux file systems. Although it is intended to be used on minimal builds, logic has been added to detect the specific GCC version (e.g., `gcc-12`) to ensure compatibility across different Ubuntu environments.

## Prerequisites

Before running, ensure you have the standard build tools installed:

```bash
sudo apt install build-essential
```

## Running the Project

### Compatibility Note

We maintain a mostly POSIX-compliant codebase. The primary exception is the argument parsing, which utilizes **GNU `getopt_long`**. This allows `sdkclean` to accept descriptive double-dash flags (like `--dry-run`) in addition to standard single-letter flags.

### Build & Install

To build the project locally:

```bash
./run
```

**Expected Output:**
```text
Compiling with gcc-12...
Build successful. Binary created at ./bin/sdkclean
```

To install (optional):

```bash
./run install
```

### Usage

Call the binary directly:

```bash
./bin/sdkclean [FLAGS] [TARGETS]
```

## Modes of Operation

`sdkclean` operates in three safety modes:

* **Standard Mode** (Default):
  Executes the requested cleaning commands immediately and reports files as they are removed.

* **Dry Run** (`-n`, `--dry-run`):
  Parses arguments and checks paths, but **does not delete anything**. It prints exactly what *would* have happened.
  * *Note: This mode does not require sudo permissions.*

* **Interactive** (`-i`, `--ask`):
  Stops to ask for explicit user confirmation (`y/N`) before executing any destructive action.

## Options

These flags can be paired with any mode above:

* `-c`, `--clean`:
  Performs standard cleanup for the APT package manager (clears the local repository of retrieved package files).

* `-j`, `--journal`:
  Performs systemd journal maintenance. Rotates logs and vacuums entries older than 2 days.

* `-t`, `--trash` [optional path]:
  Empties the Trash directory.
  * *Default:* Cleans the current user's trash (`~/.local/share/Trash`).
  * *Custom:* You can provide a specific path (e.g., `-t /tmp/junk`).

