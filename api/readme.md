# How to Use the Input Controls API
## User-Exported Functions
### Rundown
If you've used the SA2 Render Fix API, the process for Input Controls is exactly the same. If not, continue reading!

Input Controls is capable of calling a particular set of functions in your mod directly, these are called "user-exported functions". These functions are called `IC_EarlyInit` & `IC_Init`, & both are called at different times during Input Controls' boot process:

- `IC_EarlyInit`
  - Called just before IC begins to init its own features, this makes it useful for controling IC or other time sensitive actions.
- `IC_Init`
  - Called at the end of IC's boot process.

If you are unsure which of the two you should use, use `IC_Init`.

### How to Make Them
In order for Input Controls to find your exported functions, you need give them the correct name, parameters and order of parameters, mark them as exported, and finally ensure they are marked as `extern "C"`. I'll go through each of the steps then show you some examples:

1. Name: You already know their names (`IC_Init`/`IC_EarlyInit`).

2. Parameters: The params from left to right are:
    1. `const ICAPI_CORE*`, this is a pointer to IC's Core API
    2. `const char*`, this is a string path to _your_ mod's folder
    3. `const HelperFunctions&`, this is a reference/pointer to the modloader's HelperFunctions.

3. Marking as exported: By adding `__declspec(dllexport)` to your function definition, you tell Visual Studio to make that function exported. This will allow Input Controls to search and find your function. This is the same as a mod's regular `Init` function.

4. Marking as `extern "C"`: You can do this by either adding `extern "C"` to your function definition, or placing the function in an `extern "C"` block using `{` & `}` symbols. This is the same as a mod's regular `Init` function.

### Examples
With those points layed out, here's examples of both functions:

```cpp
extern "C" __declspec(dllexport) // Mark as extern "C" and as exported
void
IC_EarlyInit(const ICAPI_CORE* pApiCore, const char* path, const HelperFunctions& helperFunctions)
{
    // Ran just before Input Controls starts to init features
    // Your code here
}

extern "C" __declspec(dllexport) // Mark as extern "C" and as exported
void
IC_Init(const ICAPI_CORE* pApiCore, const char* path, const HelperFunctions& helperFunctions)
{
    // Ran after Input Controls has init'd features
    // Your code here
}
```

The examples marked each function individually as `extern "C"`, but placing them in a `extern "C"` block with `{` & `}` symbols would work the same.

## Getting the API
In order to call any API functions, you will first have to get the `ICAPI_CORE` structure pointer. This can be done in 1 of 2 ways;

1. Using one of the Input Controls user-exported functions (recommended)
2. Directly getting the exported `icapi_core` from Input Controls' DLL (NOT recommended)

Method '2' should be avoided as method '1' is significantly simpler and better supported. But, the option is there for mods designed for legacy Mod Loader versions or people who just like doing things manually.

The Core API is also constant, so you may save it to a global variable for later use.

## Using the API
Once you have the Core API pointer, you can access the other API modules through it. It's important you check the availability of the APIs before using them, as some are only available at certain times. Here's a rundown;

```
IC_EarlyInit : Config, SDL
IC_Init      : Config, Feature, User, Gamepad, Keyboard, Mouse, Window, SDL
After Init   : Feature, User, Gamepad, Keyboard, Mouse, Window, SDL
```

`After Init` refers to any time after Input Controls, and by extension all other mods, have finished booting entirely; in other words, when the user is in-game.

The API pointers are also set to `NULL` when they're not available, so a temporary run-time check can be used to ensure availability. Checks like this shouldn't be in production code, however, so should only be used as a test and deleted later.

Additionally, some APIs may only be available in later versions of Input Controls than the one the user has loaded. In a perfect world, everyone would be running the latest version of everything; but that isn't the case and it needs to be accounted for. To check for this, use the `version` member in `ICAPI_CORE` to check if the API you wish to use is available in the loaded version of Input Controls. For example:

In the header, it looks like:
```cpp
typedef struct // ICAPI_CORE example
{
    /****** Version >= 0 ************************************************************/
    uint32_t version;

    /**** Mod version ****/
    IC_VERSION ic_version;                      /* current Input Controls version   */
...
    /****** Version >= 1 ************************************************************/
    const ICAPI_EXAMPLE*     pApiExample;       /* Example API                      */
...
```
This tells us that the Example API requires version 1 or above. In your code, a check would look like:
```cpp
...
    if (ic_api->version >= 1)
    {
        const ICAPI_EXAMPLE* ic_example = ic_api->pApiExample;

        // Use the Example API here
    }
...
```

Once the API is confirmed available, you then need to check the function you wish to call is also available in the current loaded version of Input Controls. Just like the Core API, each individual API also has a `version` member variable at its top and large span comments defining which functions were introduced in which version. Luckily, the process is exactly the same as the Core API, eg:

```cpp
...
    const ICAPI_EXAMPLE* ic_example = ic_api->pApiExample;

    /** Assume the function needs version 1 or higher **/

    if (ic_example->version >= 1) // Check ICAPI_EXAMPLE version
    {
        ic_example->func();
    }
...
```

After this, you're free to use API as needed. Additional info and comments about each API and their functions can be found in the API header (`inputcont_api.h`).

## Complete Example
```cpp
extern "C" __declspec(dllexport)
void
IC_Init(const ICAPI_CORE* pApiCore, const char* path, const HelperFunctions& helperFunctions)
{
    /** Assume 'Example' API is version 1 of 'Core' **/

    if (pApiCore->version >= 1)
    {
        const ICAPI_EXAMPLE* ic_exam = pApiCore->pApiExample;

        /** Assume the function examples need versions 1 & 2 **/

        if (ic_exam->version >= 2)
        {
            ic_exam->func1(); // Needs version >= 1
            ic_exam->func2(); // Needs version >= 2
        }
    }
}
```