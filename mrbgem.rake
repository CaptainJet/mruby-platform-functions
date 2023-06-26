MRuby::Gem::Specification.new("mruby-platform-functions") do |spec|
  spec.license = "MIT"
  spec.author = "CaptainJet"

  spec.add_dependency("mruby-platform", github: "captainjet/mruby-platform", branch: "main")
  if cc.command == "emcc" || cc.command == "em++"
    spec.linker.flags << "-s EXPORTED_RUNTIME_METHODS=['UTF8ToString','stringToUTF8','lengthBytesUTF8'] -s EXPORTED_FUNCTIONS=['_malloc','_main']"
  end
end
