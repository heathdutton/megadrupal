# -*- mode: ruby -*-
# vi: set ft=ruby :

require_relative "../config.rb"
ref_path = File.dirname(File.expand_path(__FILE__))
conf_paths = [
  'config0.yml',
  'config1.yml',
  'config2.yml',
  ]
conf = load_config(conf_paths, ref_path)

result = YAML.load_file(ref_path+'/result.yml')

if conf == result
  puts 'Configs merging correctly.'
else
  puts 'ERROR: Merged configs do not match expected result.'
  puts ''
  puts '  Expected result was:'
  puts YAML.dump(result)
  puts ''
  puts '  Returned result was:'
  puts YAML.dump(conf)
  exit 1
end
