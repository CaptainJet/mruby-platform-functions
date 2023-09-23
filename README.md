## mruby-platform-functions

A small mruby gem to execute platform specific functions.

```Ruby
conf.gem :github => 'captainjet/mruby-platform-functions', :branch => 'main'
```

## Usage

```Ruby

# Emscripten
Platform::Emscripten.get_canvas_width #=> Gets the width of an webgl canvas. Assumes Module.canvas is available on the JS side.
Platform::Emscripten.get_canvas_height #=> Gets the height of an webgl canvas. Assumes Module.canvas is available on the JS side.
Platform::Emscripten.set_main_loop(fps = 0) #=> Sets the main loop of the program to "Platform::Emscripten.main_loop", raises PlatformError if not on web
Platform::Emscripten.set_local_storage(key, value) #=> Sets a local storage key to the value, raises PlatformError if not on web
Platform::Emscripten.get_local_storage(key) #=> Gets the value of a local storage key, raises PlatformError if not on web
Platform::Emscripten.save_from_memfs(memfsname, localfsname) #=> Save a file from the MEMFS to the local computer. Requires FileSaver.js to be loaded.
                                                             #=> <script type='text/javascript' src="https://cdn.jsdelivr.net/gh/eligrey/FileSaver.js/dist/FileSaver.min.js"> </script>

Platform::Emscripten.get_attribute_from_element(selector, attribute) #=> Get the attribute from a page element, raises PlatformError if not on web

# Windows
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

- MIT
- Code from https://github.com/HellRok/Taylor