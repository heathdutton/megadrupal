PUPPET
------

Puppet is central to how Drush Vagrant and Aegir-up install and maintain
systems and software. As such, they come with custom Puppet modules with some
simple helper classes, as well as including the production Puppet module:

 * puppet-drush (in Drush Vagrant)
 * puppet-aegir (in Aegir-up)

These modules currently live in the lib/puppet-modules/ directories of their
respective projects.

The inclusion of these modules is done via Puppet-Librarian which is available
in recent Debian archives (http://packages.debian.org/jessie/librarian-puppet),
as a ruby gem (http://rubygems.org/gems/librarian-puppet), or as source
(https://github.com/rodjek/librarian-puppet).

To update a Puppet modules to the latest version, simply run:

    lib/puppet-modules$ librarian-puppet update
