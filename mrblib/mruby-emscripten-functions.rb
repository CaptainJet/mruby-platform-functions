module Platform
  module Emscripten
    unless respond_to?(:set_main_loop)
      def self.set_main_loop
        raise PlatformError, "set_main_loop only available for web platform"
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
end
