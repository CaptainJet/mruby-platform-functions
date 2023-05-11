module Platform
  unless respond_to?(:set_main_loop)
    def self.set_main_loop
      raise PlatformError, "set_main_loop only available for web platform"
    end
  end
end
