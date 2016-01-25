#Which Environment?

The Which Environment? module allows site administrators to configure
environments which are relevant to their site. Using these environments'
hostnames and types, the module will then provide a JavaScript overlay
for the site, which is controllable through permissions.

The aim of this module is to reduce human error when editing configuration
or content which is intended for development or staging environments.

##Installation steps:

The module requires little configuration to get your site up and running with
Which Environment? However, it is important that you follow the preferred steps
below.

###Settings.php:

**hint: For those of you who aren't familiar with Drupal, the settings.php
file is usually located under /sites/default/settings.php**

    1. Enter the following code into a new line in your settings.php file. You
    should replace 'Development' with the name of your environment, e.g Staging or Production:

    $conf['which_environment'] = array(
      array(
        'type' => 'Development',
      ),
    );

    2. Save your changes.
    3. Navigate to admin/config/development/performance and clear all caches.

###Permissions:

The module only has one permission, 'view which environment', which should be
granted to users who you wish to be able to see the environment overlay.

    1. Navigate to admin/people/permissions and ctrl+f/cmd+f and search for
    'view which environment'.

    2. Grant the permission only to users whom are involved in site development,
    granting this permission to 'anonymous users' and 'authenticated users' may
    result in the overlay being seen by general visitors to your site.

    3. Navigate to any page of the site, you should now be able to see an
    overlay notifying you on which environment you are viewing. If you cannot
    see the overlay, you might need to clear your caches as we did earlier.
