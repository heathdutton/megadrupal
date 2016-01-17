AegirVPS Module
===============

This module manages Koumbit's AegirVPS services.

It installs and maintains Aegir and Hosting Queue Runner via the Aegir Puppet
module. It also provides a method for automating the building and importing of
Drupal platforms using makefiles from the kPlatforms project.


Dependencies
------------

The AegirVPS module depends on:
 * the Aegir and Drush Puppet modules
 * the Koumbit Apt and Common modules

These are all included as Git sub-modules withing the modules/ directory.

Instructions
------------

To use this module, follow these directions:

1. Your modules directory will need all the files included in this
   repository placed under a directory called "aegirvps".

2. Since Debian package installations depends on the Apt module, which in turn
   depends on some functions provided by the Common module, you'll need to add
   the following line in your manifests:

       import "common"

3. Then add the following line to your manifests:

       include aegirvps


Options, variables and parameters
---------------------------------

A number of options are available for the package-based installation of Aegir.
You can:

  * Specify the front-end URL:
      $aegir_hostmaster_url = 'aegir.koumbit.org'

  * Use a remote database server:
      $aegir_db_host = 'mysql2.koumbit.net'
      $aegir_db_user = 'root'
      $aegir_db_password = 'Pa55w0rd'

  * Set the email of the admin user in the Aegir front-end:
      $aegir_email = 'admin@koumbit.org'

  * Specify an alternate makefile to build the front-end with:
      $aegir_makefile = 'alternate-aegir.make'

A Message of the Day (motd) is generated with some basic usage instructions, and
links to documentation, and the client's private Redmine project. In order for
that last link to be properly generated, make sure to add the following varia-
ble to the manifest:

  $aegirvps_client_name = 'aegirdemo'

--------------------------------------------------------------------------------
Original author: Christopher Gervais (mailto:chris@koumbit.org)
Copyright:: Copyright (c) 2012 Réseau Koumbit Networks
License::   GPLv2 or later
