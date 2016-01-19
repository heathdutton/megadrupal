VALKYRIE-CONFIG
===============

[![Build Status](https://travis-ci.org/GetValkyrie/valkyrie-config.svg?branch=master)](https://travis-ci.org/GetValkyrie/valkyrie-config)

This is a small utility to load configuration files from multiple locations and
merge the resulting settings. It is intended to allow configuration for Vagrant
to live in YAML files.

Simply add it to your project (perhaps as a git submodule), and include it in
your Vagrantfile like so:

    require_relative "path/to/valkyrie-config/config.rb"
    ref_path = File.dirname(File.expand_path(__FILE__))
    conf_paths = [
      'config0.yml',
      'config1.yml',
      'config2.yml',
      ]
    conf = load_config(conf_paths, ref_path)
