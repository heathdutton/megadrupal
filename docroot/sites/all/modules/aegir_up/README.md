DESCRIPTION
-----------

Aegir-up deploys a local instance of the Aegir Hosting System atop Vagrant and
Virtualbox, for development and testing purposes. It provides all the ease-of-
use of Aegir, wrapped in the convenience of Vagrant-based virtual machines.
Creating sites and platforms is a matter of a click or two, while rebuilding
the entire environment is a matter of minutes.

Aegir-up provides a collaborative, distributed development environment, that
encourages the use of Drupal development best-practices. Workspaces can easily
be cloned and shared via Git, or even bundled into blueprints that others can
use as templates for their own development environments.

One of Aegir-up's more powerful features is that a shared platform directory is
is mounted via NFS. This allows the files form entire platforms and any sites
installed on them to be available on the host machine. So you can edit any file
on your site or platform directly instead of having to SSH into the VM, or move
them via SFTP.


USAGE
-----

*N.B.* For installation instructions, please see INSTALL.txt

Usage documentation is in Drush's built-in help system. To see a list of
Aegir-up commands, you can run the following:

  drush --filter=vagrant

  drush vagrant-build --project-name=myaegirbox --blueprint=aegir --up

More detailed usage information is provided by running:

  drush <command> --help


OTHER DOCS & HELP
-----------------

More detailed documentation can be found in the docs/ folder, which are also
available as Drush topics:

  drush topic

Additional documentation, including user-contributed guides can be found on the
wiki at: http://community.openatria.com/c/aegir-up

Bug reports, feature and support requests should be submitted to the drupal.org
issue queue: http://drupal.org/project/issues/aegir-up

Also, the developers (See Credits section, below) can usually be found on IRC
in the #aegir and #praxis.coop channels on irc.freenode.net


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


CREDITS
-------

Developed by Christopher Gervais (aka ergonlogic) <http://ergonlogic.com/>
Maintained by C. Gervais and Guillaume Boudrias <http://praxis.coop/>
       and by Herman Van Rink <http://initfour.nl/>
