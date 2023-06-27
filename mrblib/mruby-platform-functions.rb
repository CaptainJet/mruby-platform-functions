module Platform
  module Emscripten
    unless respond_to?(:set_main_loop)
      def self.set_main_loop(fps = 0)
        raise PlatformError, "set_main_loop only available for web platform"
      end
    end

    unless respond_to?(:get_canvas_width)
      def self.get_canvas_width
        raise PlatformError, "get_canvas_width only available for web platform"
      end
    end

    unless respond_to?(:get_canvas_height)
      def self.get_canvas_height
        raise PlatformError, "get_canvas_height only available for web platform"
      end
    end

    unless respond_to?(:set_local_storage)
      def self.set_local_storage(key, value)
        raise PlatformError, "set_local_storage only available for web platform"
      end
    end

    unless respond_to?(:get_local_storage)
      def self.get_local_storage(key)
        raise PlatformError, "get_local_storage only available for web platform"
      end
    end

    unless respond_to?(:get_attribute_from_element)
      def self.get_attribute_from_element(selector, attribute)
        raise PlatformError, "get_attribute_from_element only available for web platform"
      end
    end
  end

  module Windows
    unless respond_to?(:attach_console)
      def self.attach_console
        raise PlatformError, "attach_console only available for windows platform"
      end
    end
  end
end
