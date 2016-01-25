DESCRIPTION
-----------

Drush Vagrant Integration is a system that provide Drush wrappers around
Vagrant (http://vagrantup.com), allowing the use of Drush aliases, for example.
It also provide a system of templates (blueprints) to build Vagrant environ-
ments in VirtualBox (http://virtualbox.org). It is implemented as a Drush
extension.


STRUCTURE
---------

  blueprints/                 Drush Vagrant blueprints directory
    default/                  The 'default' blueprint
      ...                     See 'blueprints.md' for details
  docs/                       In-depth documentation
  includes/                   Code specific to each Drush Vagrant commands
    alias.vagrant.inc         Output code for a remote site alias
    blueprints.vagrant.inc    Functions to list blueprints & invoke hook
    build.vagrant.inc         Functions to build a project
    delete.vagrant.inc        Functions to delete a project
    list.vagrant.inc          Functions to list projects, VMs, and their status
    shell.vagrant.inc         Functions to connect to a VM via SSH
    user.vagrant.inc          Generate user-specific alias and settings
    vagrant.inc               Common helper functions
  lib/                        Library of miscellaneous required files
    gitignore                 Template .gitignore
    global.rb                 Global variables inherited by other config files
    puppet-modules            Common default Puppet modules
    Vagrantfile               Vagrant control file
  README.md                   Basic usage documentation
  templates/                  Templates used to generate various config files
    alias.tpl.php             Outputs a remote site alias
    blueprint.tpl.php         Records blueprint-specific data
    config.tpl.php            Generates variables used in Vagrantfile
    help.tpl.php              Formats help text
    project_alias.tpl.php     Generates aliases for Vagrant projects
    vm_alias.tpl.php          Generates aliases for VMs
  vagrant.blueprints.inc      Implements blueprint hook, and various default
                                functions to build blueprints
  vagrant.drush.inc           Main Drush commanfile
  vagrant.drush.load.inc      Ensures Vagrant is installed & re-writes group
                                aliases

