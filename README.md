## mruby-platform-functions

A small mruby gem to execute platform specific functions.

```Ruby
conf.gem :github => 'captainjet/mruby-emscripten-functions', :branch => 'main'
```

## Usage

```Ruby

# Emscripten
Platform::Emscripten.set_main_loop #=> Sets the main loop of the program to "Platform::Emscripten.main_loop", raises PlatformError if not on web
Platform::Emscripten.set_local_storage(key, value) #=> Sets a local storage key to the value, raises PlatformError if not on web
Platform::Emscripten.get_local_storage(key) #=> Gets the value of a local storage key, raises PlatformError if not on web
Platform::Emscripten.get_attribute_from_element(selector, attribute) #=> Get the attribute from a page element, raises PlatformError if not on web

#Windows
Platform::Windows.attach_console # Allows output to a console if run from one, when compiled with -mwindows

# You must define your own main_loop within Platform.

# Example
module Platform
    module Emscripten

        def self.main_loop
            update_input
            update_screen
        end
    end
end

if Platform.web?
    Platform::Emscripten.set_main_loop
else
    loop { Platform::Emscripten.main_loop }
end
```

## License

MIT
Emscripten Code from https://github.com/HellRok/Taylor
