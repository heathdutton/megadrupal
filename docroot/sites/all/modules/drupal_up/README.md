Drupal-up
---------

Drupal-up is a Drush extension that facilitates building virtual machines for
local development of Drupal sites. Drupal-up implements a 
[Drush Vagrant](https://drupal.org/project/drush-vagrant) blueprint. This, in
turn, builds a virtual machine atop [Vagrant](http://www.vagrantup.com) and
[VirtualBox](http://www.virtualbox.com), and configures it using
[Puppet](http://www.puppetlabs.com).

Drupal-up provides blueprints for generating virtual machines to host Drupal 6,
7, and 8.x-dev sites.

Installation
------------

1. Install dependencies: [Drush](https://drupal.org/project/drush),
   [Vagrant](http://www.vagrantup.com), [VirtualBox](http://www.virtualbox.org)
   and, optionally, an NFS-server.
2. Install [Drush Vagrant](https://drupal.org/project/drush-vagrant) and [Drush
   Hosts](https://drupal.org/project/drush-hosts): `drush dl drush-vagrant
   drush-hosts`
3. Install Drupal-up: `drush dl drupal-up`

Usage
-----

Drupal-up provides stable Drupal 6 and 7 blueprints for the Drush Vagrant
extension. An 8.x-dev blueprint is also available.

To build a VM using one of the blueprints, issue the following commands:

1. `drush vagrant-build`
2. Enter a name for your project
3. Leave the git URL section empty
4. Select a Drupal 6, 7 or 8 blueprint
5. Allow `drush-hosts` to modify your `/etc/hosts` file
6. Proceed with initializing the project

This can be condensed into:

`drush -y vagrant-build --project-name=drupal7site --blueprint=drupal7 --hosts`

If you open up VirtualBox, you should see that your new machine is running
along with the URL for accessing your site, e.g. drupal7.drupal7site.local.
You can modify your /etc/hosts to provide a different URL for accessing the
site. You can then navigate into your new project directory (in this example: `cd ~/vagrant/projects/drupal7site` and shell into the box: `drush vagrant-shell`).

Dependencies
------------

- [Drush](https://drupal.org/project/drush)
- [Vagrant](http://www.vagrantup.com)
- [VirtualBox](http://www.virtualbox.org)
- [Facter](https://rubygems.org/gems/facter)
- [Drush Vagrant Integration](https://drupal.org/project/drush-vagrant)
- [Drush Hosts](https://drupal.org/project/drush-hosts)

Maintainers
-----------

- Christopher Gervaius ([ergonlogic](https://drupal.org/user/368613))
- Kosta Harlan ([kostajh](https://drupal.org/user/209141))
