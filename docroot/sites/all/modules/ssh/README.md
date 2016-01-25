# [ssh module](http://drupal.org/project/ssh)

By [David Herminghaus](http://drupal.org/user/833794)

Sponsored by [HiRN GmbH](http://www.hirn.it)

## Contents

1. [ABOUT](#about)
2. [PREREQUISITES](#prerequisites)
3. [SETUP](#setup)
4. [DEPENDENCIES](#dependencies)
5. [USAGE](#usage)
6. [OTHER](#other)
7. [DISCLAIMER](#disclaimer)

## ABOUT

This rules extension provides a configurable, parametrized rules action
allowing to execute shell commands on pre-configured remote hosts.

The actual remote credentials are managed separately and require an additional
permission for improved security.


## DEPENDENCIES

* Drupal
** [Variable](https://www.drupal.org/project/variable)
* PHP
** >= 5.3
** exec() must not be listed in "disabled_functions" in php.ini
* Linux (or probably cygwin et. al., untested)
** openssh + libssh2 (or compatible)


## PREREQUISITES

1. Configure SSH properly for the account running the Drupal site.

   * Have SSH private and public keys properly generated
     for that account and stored in the proper locations.
     (Mind e.g. permissions mode 600 for private key files etc.)
   * Have the public key added to any remote host you want to set
     up in Drupal
   * Make sure there are no other issues (like remote host key/host keyfile
     mismatch issues etc), in other words: Test it by executing
     the SSH command in a console, as the user that also runs the Drupal site.

   ***SECURITY NOTE***

   In the likely case that you use apache, storing SSH credentials inside
   /var/www is not recommended as this is the default document root.
   It is recommended that you create the .ssh folder in any other folder.

   See [other](#other) for a typical apache example, dealing with the problem
   that apache has no useful home directory and ssh config defaults do not work.


## SETUP

1. Enable the module.
2. Set "administer ssh" permission, if required.
3. Configure basic parameters at admin/config/system/ssh
4. Configure remote presets at admin/config/system/ssh/hosts
5. Check admin/reports/status and resolve issues, if any.


## USAGE

If properly set up, you will find a new rules action "run ssh command" in the
"system" section. It takes one of the pre-defined hosts as a static argument,
and the actual remote command as a tokenizable command.


## OTHER

### Security considerations

Depending on the remote host, ssh can have massive security impacts. It cannot
be overemphasized just how careful


### Limitations

This module has been tested with (and designed for) the more common
libssh2/openssh config. Any other configuration should be handled with care.

#### Known hosts

Since the "known hosts" feature has been introduced on purpose, it is important
to understand this and to accept that setting up a new remote host requires
manual admin interaction, or at least action more than a click in Drupal.

To add a known host that has not been added, you may copy a code snippet
from every host settings entry at admin/config/system/ssh/hosts, and then
run it manually in a shell on behalf of the Drupal web server user.
This will add the host key entry and keep security at a level.


### Apache example

Apache is typically installed with "/var/www" as its "home" folder, which
makes ssh credential storage in that folder a security risk.

Here's a contemporary Debian/apache2 example:

  sudo mkdir -p /var/www-ssh
  sudo chown www-data:www-data /var/www-ssh
  sudo su - www-data -c "ssh-keygen -b 4096 -f /var/www-ssh/drupal_id_rsa"
  cat /var/www-ssh/drupal_id_rsa.pub

The latter gives you the public key to store on the remote host, and
"/var/www-ssh/drupal_id_rsa" is the keyfile path to configure in Drupal.


## DISCLAIMER

Although this should be a matter of course, I would like to make myself double
clear once again:

	USING THIS MODULE IS AT YOUR OWN RISK AND YOU SHOULD NOT USE IT
	UNLESS YOU ARE EXPERIENCED IN QUESTION OF SERVER AND NETWORK SECURITY
	AND PERFECTLY AWARE OF EVERY STEP YOU ARE DOING HERE.

	IF YOU ARE NOT, YOU SHOULD REALLY HIRE SOME WHO IS.

We will gladly assist, just ask. Also for custom extensions and any cross-platform
integration issues involving Drupal or any web service.
