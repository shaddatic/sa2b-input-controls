# SA2 Input Controls
Is _the_ input mod for Sonic Adventure 2!

It completely replaces the sub-par input code created for the HD releases with all-new and modern input methods, all written entirely in C! And, on top of that, includes optional enhancements and bug fixes to the game itself; some issues of which have stood since as far back as the Dreamcast!

## Features
### Core Features
- Switches the game over to `SDL2`, allowing for basically any gamepad to work as-is!
- Configurable Gamepad settings through `SDL` controller mappings!
- All-new Gamepad input code!
  - Industry standard 'Scaled Radial Deadzone', called 'Circular' in-mod, which allows for extremely fine and responsive analog controls!
  - More traditional 'Axial Deadzone', called 'Square' in-mod, which allows for easier exact cardinal movement at the cost of finer control
  - Inner deadzone size settings, and hidden outer deadzone settings (by modifying `config.ini`)!
  - Vibration intensity settings
- Extended virtual Dreamcast controller button support:
  - `C`, which is mapped to `LB`/`L1`/`ZL` (XB/PS/GC)
  - `D`, which is mapped to `RB`/`R1`/`ZR` or `Z` (XB/PS/GC)
  - `Z`, which is mapped to `Back`/`Select` or `Share` (XB/PS)
- All-new Keyboard input code!
  - Keyboard controls are now set inside Input Controls' config menu
  - Keyboard settings are now based on a controller, and not weird colors (?)
  - Buttons/Analogs that before weren't configurable now are!
    - D-pad buttons
    - Right analog stick
    - `Back`/`Select` button
    - Shoulder buttons
    - Analog stick buttonms
- All-new Mouse input code!
  - Analog emulation, allowing the mouse to emulate an analog stick!
    - Mouse Movement: use the raw mouse movement as an analog value
    - Click & Drag: click & drag a virtual analog stick
    - On-screen visualizer, for visual feedback!
      - Arrow: an arrow points in the direction of analog input, and slowly fills with color to represent magnitude
      - Point: a point tethered to the center of the screen gives you exact analog positioning, and turns red when the analog stick is fully 'pushed'
- Enhanced input code!
  - Game can now optionally read the raw analog values directly from the controller, allowing for significantly finer control than the virtual Dreamcast controller can provide!
  - Camera is now equiped with analog controls, just like on GameCube! But can now additionally read the right analog stick
  - Right stick camera input can now be flipped to better match other games
- Debug settings
  - Gamepad print axis: for testing deadzone settings! `X` and `Y` are the analog coordinates, `M` is the magnitude of the analog input
  - Keyboard print key: for getting the key index of a pressed key in the case it's not labled in the config menu! (by modifiying `config.ini`)

### Input Controls API
- Config API
  - For reading the mod's user-config file directly
- Feature API
  - For checking the on-state of Input Controls features
- User API
  - For getting raw input from the user (Gamepad+Keyboard), and for getting their gamepad and keyboard index settings
- Gamepad API
  - For reading directly from the gamepad, checking validity, and sending force feedback messages
- Keyboard API
  - For getting key input from the keyboard with 'Down', 'Press', 'Release' functions
- Mouse API
  - For getting raw input from the mouse, including buttons, wheel input X and Y, mouse vector and mouse position on the game window surface
- Window API
  - For registering window message handlers and translating window surface coordinates to the game's internal 640x480 screen coordinates
- SDL API
  - For registering SDL_Event handlers, and getting DLL exports from the mounted library

### Changes From Vanilla
- Virtual Dreamcast Controller
  - Analog stick value ranges are now `-128` to `128`
  - Opposing Dpad directions cancel each other out
  - `PDD_DGT_TR`/`L` now turn on and turn off at different values, which matches Dreamcast
- Gamepad Controls
  - The right analog stick no longer sets the L/R trigger analog values too, the two are now wholly seperate
- Keyboard Controls
  - The left analog stick directions and d-pad buttons no longer map to the same keys at all times
  - The L/R trigger keys no longer just set the `PDD_DGT_TR`/`L` button flags, they now also correctly set the L/R analog values too

### Bug Fixes
- Fixed the hyper sensitive analog movement. This was caused by a division bug introduced in the GameCube version
  - This also fixed rail grinding being very hard to perform, a major complaint about the game
- Fixed the "8 directional analog movement". It never actually was, it was always fully analog, but poor deadzone code added in the HD releases mixed with the deadzone code left from the GameCube version led to very "sticky" directional movement
- Fixed cart controls being stiffer by about 12%. The HD releases broke the virtual Dreamcast controller's analog range (`-128~128` to `-225~224`) and this caused the carts to turn much more than intended. SoC (the HD dev)'s solution was to divide all cart input by `2`. But, since `225` isn't double `128`, it caused the carts to turn _less_ than intended. If you struggled with those cart missions, now you can say it wasn't you; it was the game!

### Bug Additions
- Optionally re-added the Dreamcast versions analog drift bug for those who want an accurate Dreamcast experience! Or those looking for nostalgia!

## SDL Controller Mappings
Although a more advanced feature, SDL supports custom controller mappings through an external file. There are many tools and tutorials out that can help you create an SDL Controller Mapping file, including Steam, so you can get the controller exactly how you like it! Once you've created the mapping file, place it in SA2 Input Controls' mod folder, next to `mod.ini`, as `gamecontrollerdb.txt`. Input Controls will automaticaly load it at game boot!

## Installation
Install SA2 Input Controls into your mod list, either manually or via the '1-Click Install' feature. SDL2 is not a seperate download and comes pre-packaged with the mod, no further setup is required. Once enabled, it is recommended you place Input Controls very near the top of your mod list - a safe bet is to place the big 3 (Render Fix, Input Controls, Volume Controls) as your first 3 mods - although this is not a requirement, but it is recommended. After that, configure, play, and enjoy! And remember to check for updates!

## Making New Issues
If you encounter an issue with the mod, and it's not already on the issue tracker, please feel free to make a new GitHub issue! But remember, it's one issue per GitHub issue. Unless the issues are related, please make a seperate entry for each issue.
