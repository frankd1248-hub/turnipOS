# Change Log

### Log 2: Dec. 19, 2025, Version: 0.6

Fixes:

- A very nasty segmentation fault occurring because I referenced a std::move'd unique_ptr of a Package.

Features:

- Almost working editor package, maybe a plane ride will give me time to fix it.
- Command: `editor <file>` --> Creates &lt;file&gt; if nonexistent

Planned:

- `pacman` command, still. I mainly want to get a working package before I work on a package manager.

### Log 1: Dec. 18, 2025, Version: 0.5

Features:

- Abstract base classes: AuthService, FileService, LogService, WebService, Package, App, Command.
- Implemented and somewhat tested commands:
    - `exit` (usable from login)
    - `login <username> <password>` (usable from login), Unlimited attempts.
    - `pwd` --> Prints current working directory, defaults to `~/turnipOS` on Linux filesystem.
    - `chdir <path>` --> Changes current working directory to &lt;path&gt;.
    - `mkdir <path>` --> Creates a path at &lt;path&gt;.
    - `list` --> Lists all subdirectories and files at the current working directory.
    - `touch <path>` --> Creates a file at &lt;path&gt;
    - `save <path>` --> Saves current system state to &lt;path&gt;
    - `load <path>` --> Loads current system state from &lt;path&gt;
    - `savepkg [package]` --> Saves a package. If [package] is empty, saves all packages.
    - `loadpkg [package]` --> Loads a package. if [package] is empty, loads all packages.
    - `webget "<url>" <path>` --> Downloads the file at &lt;url&gt; to &lt;path&gt;

Planned:

- Installing / uninstalling packages with a `pacman` command
- Completing the Editor package as a pilot for others. (Halfway through currently)