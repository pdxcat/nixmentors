source ENV['GEM_SOURCE'] || 'https://rubygems.org'

# here are my changes!!!!

group :development, :test do
  gem 'rake',                    require: false
  gem 'puppetlabs_spec_helper',  require: false
  gem 'puppet-lint',             require: false
  gem 'rubocop',                 require: false
end

facterversion = ENV['FACTER_GEM_VERSION']
puppetversion = ENV['PUPPET_GEM_VERSION']

if facterversion == ENV['FACTER_GEM_VERSION']
  gem 'facter', facterversion, require: false
else
  gem 'facter', require: false
end

if puppetversion == ENV['PUPPET_GEM_VERSION']
  gem 'puppet', puppetversion, require: false
else
  gem 'puppet', require: false
end

# vim:ft=ruby
