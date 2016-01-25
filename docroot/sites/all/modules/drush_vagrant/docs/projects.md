DESCRIPTION
-----------

Each project in Vagrant represents one or more VMs that share a common subnet,
Vagrantfile, Puppet modules folder, &c.


STRUCTURE
---------

{{{
  <my_project>/
    manifests/         Puppet manifests
      nodes.pp         Manifest defining individual VMs (nodes)
      site.pp          Principle control manifest; includes/runs others
    modules/           Project-specific Puppet modules
      ...              As required; can include custom modules
    settings.rb        Parameters for VMs
    Vagrantfile        symlinked to <drush-vagrant-root>/lib/Vagrantfile
    .config/           Folder containing generated configuration
      files/           User dotfiles & public ssh key that get added to VMs
      config.rb        Configuration generated during project initialization
}}}

VAGRANTFILE
-----------

In Vagrant, a Ruby-based control-file is included in each project. These define
VM attributes such as hostnames, subnets, allocated memory, &c. The Vagrantfile
symlinked from projects built by Drush Vagrant's default build process has had
all the relevant settings extracted into separate files. These simplify project
control by presenting settings in a format similar to PHP ini files, or Drupal
.info files. See below for details of these included files.


SETTINGS.RB
-----------

While designed to be similar to Drupal .info files, settings.rb is written in
Ruby. Instead of parsing a text config file, settings.rb (and config.rb, below)
actually implement one or more Ruby classes. The Vm class (found in 
lib/global.rb) contains default settings:

{{{
class Vm
  ...
  Count     = 1                          # number of VMs to create
  Basebox   = "lucid32"                  # default basebox
  Box_url   = "http://files.vagrantup.com/lucid32.box"
  Memory    = 512                        # default VM memory
  Domain    = "local"                    # default domain
  Manifests = "manifests"                # puppet manifests folder name
  Modules   = {}                         # hash of puppet module folder names
  Site      = "site"                     # name of manifest to apply
  Gui       = false                      # start VM with GUI?
  Verbose   = false                      # make output verbose?
  Debug     = false                      # output debug info?
  Options   = ""                         # string of options to pass to Puppet
  Facts     = {}                         # hash of Factor facts
end
}}}

Each class that inherits from the Vm class will be built by Vagrant as a VM.
Class names should be capitalized. Each must define a unique machine-name and a
human-readable name to identify the VM:

{{{
class Example < Vm             # VM-specific overrides of default settings
  Shortname  = "example"       # Vagrant name
  Longname   = "Example"       # VirtualBox name
end
}}}

All default settings can be overridden in inheriting classes:

{{{
class Hm < Vm
  Count      = 2
  Memory     = 1024
  Basebox    = "debian-LAMP-2012-03-29"
  Box_url    = "http://ergonlogic.com/files/boxes/debian-LAMP-current.box"
  Shortname  = "aegir-up"
  Longname   = "Aegir-Up"
  Facts      = { "aegir_user"        => "aegir",
                 "aegir_root"        => "/var/aegir",
                 "aegir_user_exists" => "true",
                 "aegir_version"     => "6.x-1.9",
               }
end
}}}

CONFIG.RB
---------

In addition to the user configurable settings in settings.rb, a number of
automatically generated settings are contained in a project's .config/config.rb
file. Note that these are not intended to be altered by users.


