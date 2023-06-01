MRuby::Gem::Specification.new('mruby-platform-functions') do |spec|
  spec.license = 'MIT'
  spec.author  = 'CaptainJet'

  spec.add_dependency('mruby-platform', :github => 'captainjet/mruby-platform', :branch => "main")
end
