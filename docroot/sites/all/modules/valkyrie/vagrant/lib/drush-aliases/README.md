VALKYRIE-DRUSH-ALIASES
======================

This 'faux' plugin for Vagrant allows for Drush aliases to be generated in the
VM to be cleanly ignored when the VM is no longer available; having been
halted, destroyed, etc. Simply add it to your project (perhaps as a git
submodule), and include it in your Vagrantfile like so:

    ENV['VALKYRIE_PROJECT_PATH'] = File.dirname(File.expand_path(__FILE__))
    require_relative "drush-aliases.rb"


Background
----------

Drush aliases are great, but if they point to sites hosted on a Vagrant VM that
has been suspended (or otherwise unavailable), errors will result. Worse still,
if you are using the same aliases for sites hosted on more than one Vagrant VM,
your aliases will end up pointing at the wrong IP/hostname, leading to
behaviour that is difficult to diagnose.
