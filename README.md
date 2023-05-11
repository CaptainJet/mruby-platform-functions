## mruby-emscripten-functions
A small mruby gem to set an emscripten main loop.

```Ruby
conf.gem :github => 'captainjet/mruby-emscripten-functions', :branch => 'main'
```

## Usage

```Ruby
Platform::Emscripten.set_main_loop #=> Sets the main loop of the program to "Platform::Emscripten.main_loop", raises PlatformError if not on web
Platform::Emscripten.set_local_storage(key, value) #=> Sets a local storage key to the value, raises PlatformError if not on web
Platform::Emscripten.get_local_storage(key) #=> Gets the value of a local storage key, raises PlatformError if not on web
Platform::Emscripten.get_attribute_from_element(selector, attribute) #=> Get the attribute from a page element, raises PlatformError if not on web

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
    Platform.set_main_loop
else
    loop { Platform.main_loop }
end
```

## License

MIT
Emscripten Code from https://github.com/HellRok/Taylor