DEPENDENCIES
------------

Aegir-up depends on:

 * Drush
 * drush-vagrant
 * drush-hosts
 * Vagrant
 * and NFS server

Vagrant 1.2+ is required, and installation instructions can be found at:

 * http://docs.vagrantup.com/v2/installation/index.html.

Vagrant, in turn, depends on VirtualBox, and possibly Ruby and RubyGems). A
recent version of VirtualBox (4.0+) is required by Vagrant, and can be
downloaded from:

 * https://www.virtualbox.org/wiki/Downloads.

Drush 5.x or later is required. Drush can be installed in a number of ways, as
detailed here:

 * http://drupalcode.org/project/drush.git/blob/HEAD:/README.txt#l30.

Drush-vagrant (http://drupal.org/project/drush-vagrant) and Drush Hosts
(http://drupal.org/project/drush-hosts) are also required. They can be
installed by running:

 * drush dl drush-vagrant drush-hosts

NFS (Network File System) is pre-installed on some Unix-like OSes. Aegir-up
requires an NFS-server, in order to share entire directory trees transparently
between the host machine and the VM. If it isn't already, a recent version
should be easy to install using your OS's preferred packaging method. For
Debian-like systems, this means 'apt-get install nfs-kernel-server'.

Compatible releases of Vagrant, VirtualBox and Drush are available in Debian's
Testing branch, and so can be installed (along with all dependencies) with a
simple 'apt-get install vagrant', assuming the appropriate repos have been
added to /etc/apt/source.list.d/ This last is the developers' preferred and
recommended installation method.

Finally, a custom Vagrant base box is required, and will automatically be
downloaded upon first use of Aegir-up. This base box is built with a tool
called veewee (https://github.com/jedi4ever/veewee), and all definitions needed
to build this base box are included with the Aegir-up package. Note that
building a base box is not required for the use of Aegir-up.


INSTALLATION
------------

Installation should be as simple as:

  drush dl aegir-up

This should create a folder at ~/.drush/aegir-up, but might also be installed
to /path/to/drush/commands/aegir-up, depending on how Drush was installed.
The drush-vagrant and drush-hosts modules can be installed in the same fashion.

Now that Vagrant is available in Debian (Testing), .deb packaging is planned.


CAVEAT
------

**N.B.** Aegir-up is *NOT* intended for production hosting.

While we hope you find Aegir-up useful for development and testing, out-of-the-
box it is not equiped for production use (i.e., minimal or no security,
monitoring, backup/restore facilities, &c.)

For fully managed, production-grade Aegir servers and services, check out
Koumbit's AegirVPS services:
  <http://www.koumbit.org/en/services/AegirVPS>

Or other Aegir Service Providers:
  <http://community.aegirproject.org/service-providers>

