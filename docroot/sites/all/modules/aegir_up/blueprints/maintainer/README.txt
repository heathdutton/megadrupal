MAINTAINER BLUEPRINT
====================

The 'maintainer' blueprint is designed for maintainers of Drupal modules and
themes. Similar to the stock Aegir-up blueprint, it will build a local Aegir
server. It then also creates an SSH keypair that can be used to secure communi-
cations with git.drupal.org. Finally, it builds a platform based on a custom
makefile using Drush Make's '--working-copy' option. This leaves .git repos
untouched after download, making immediate maintenance of themes and modules
possible.


SETUP
-----

Unlike most other blueprints, 'maintainer' requires some manual intervention to
get it running properly. Start by building the project, along with adding hosts
entries:

    $ drush vagrant-build --blueprint=maintainer --hosts

You'll notice some errors in the first Puppet run, as the platform build will
initially fail. Not to worry, though, as this is to be expected. At this point
it is trying to 'git clone' the read-write urls of my (ergonlogic's) projects.

In the project root, you'll see a new 'keys' directory. It contains an SSH
keypair that was generated in the VM. Upload the public key (id_rsa.pub) to
your drupal.org user account. This will allow the 'aegir' user in the VM to
authenticate to git.drupal.org, and thus clone the read-write urls for your
projects.

Next we need to create a custom makefile that lists all your Drupal projects.
You can start by copying the default 'ergonlogic.make':

    $ cp makefiles/ergonlogic.make makefiles/<d.o-username>.make

Edit the new makefile to replace the projects with your own. If you don't make
a habit of including makefiles for the dependencies in your modules, you'll
also need to add read-only entries for any dependencies.

If your system username is different from your drupal.org username, you'll also
need to edit manifests/nodes.pp, and replace '$username':

    ssh_username => $username,

Finally, re-run Puppet provisioning on the VM:

    $ vagrant provision

It should now build a 'MyModules' platform in your Aegir-up install.


USAGE
-----

Now that you have a platform with all your projects installed, you can build a
site from the Aegir front-end: 'Content management' > 'Create content' > 'Site'
or /node/add/site.

To access the site, you'll need to add a hosts entry. This can easily be done
using Drush aliases:

    drush @<project-name>.maintainer hosts-add --fqdn=<site-url>

At this point, you can click on the 'Login to <site-url>' link in Aegir.

The source-code for your projects can be found in the VM at:

    /var/aegir/platforms/MyModules/sites/all/[modules|themes|libraries]


CREDITS
-------

Built and maintained by Christopher Gervais (http://ergonlogic.com)
With inspiration from Kevin Kaland (http://www.wizonesolutions.com)

