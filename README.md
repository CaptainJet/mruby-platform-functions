## mruby-emscripten-functions
A small mruby gem to set an emscripten main loop.

```Ruby
conf.gem :github => 'captainjet/mruby-emscripten-functions', :branch => 'main'
```

## Usage

```Ruby
Platform.set_main_loop #=> Sets the main loop of the program to "Platform.main_loop", raises PlatformError if not on web

# You must define your own main_loop within Platform.

# Example
module Platform

    def self.main_loop
        update_input
        update_screen
    end
end

Platform.set_main_loop
```

## License

MIT