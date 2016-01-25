Deploy Add-on
=============

Deploy Add-on is a suite of modules that provide a working implementation of
Deploy.

Modules
-------

- Deploy Add-on: Base implemention and make file
- Deploy Add-on Content: Views and rules components to manage content
deployments
- Deploy Add-on Next: Deploy endpoint, default Next plan and ad hoc plan
functionality
- Deploy Add-on Services: Automatic creation of default services endpoint

Required (included in make):
- Deploy
- Entity Dependency
- Entity Menu Links
- Services
- UUID
- UUID Extras (Redirect UUID)

Deploy, Service and UUID related contrib modules are included in the make file
for convenience however there are several dependencies not included.

Other Required dependencies:
- Entity API
- Features
- Libraries
- Redirect
- Rules
- Strongarm
- Transliteration
- Views
- Views Bulk Operations

Optional:
- Bean
- File Entity
- Webform UUID

Overview
--------

This is a glue module that provides a commonly used content deployment
implementation for dev -> stage -> production style workflows. It includes
support for popular contrib modules such as Bean, Webform and Redirect.

The Deploy Add-on Next module creates a Push to Next deploy plan where all
content changes get added to by default. Alternatively, this can be set to use
an ad hoc plan. It is user session based and can automatically be set on login
by enabling that option on the user profile.

The Deploy Add-on Content module is a set of views and rules components that
allow you to add and remove content from the Push to Next deployment plan or
your ad hoc plan. Views for nodes, taxonomy terms, blocks, files and users are
included. If you enable the module and you are missing a module dependency such
as Bean, the corresponding view will be disabled and it will need to be enabled
manually.

The Deploy Add-on Services module creates a services endpoint to deploy content
to with all available entity types.

There are also extra features including a debug option to see verbose messages
added to Watchdog, skipping deployment of user 1, and skipping deployment of
configured roles.

Installation
------------

- If you are installing on a content editing site, enable the Deploy Add-on
Content module. This will automatically enable the Deploy Add-on base module
and Deploy Add-on Next.

- If you are installing on an endpoint such as production, you only need to
enable the Deploy Add-on Services module.

- Add the necessary configuration which you can copy and paste from the
configuration section below into your settings.php file. Alternatively, you can
update the configuration at admin/config/content/deploy_addon_next. You must
have the Encrypt module installed and configured to enter a password.

Configuration
-------------

Configuration is done by adding variables to the $conf array in your
settings.php file or by editing the admin configuration at
admin/config/content/deploy_addon_next. If you are not using the Encrypt
module, you must add the password to the settings.php file.

// Set this to add extra debug log entries to the watchdog.
$conf['deploy_addon_debug'] = 1;

// Full URL of endpoint.
$conf['deploy_addon_next_url'] = 'https://hostname/deploy';

// Username of deploy user on endpoint.
$conf['deploy_addon_next_user'] = 'username';

// Password of deploy user on endpoint.
$conf['deploy_addon_next_pass'] = 'password';

// Password of deploy user is encrypted using Encrypt module. This is enforced
// if you want to save the password in the database.
$conf['deploy_addon_next_pass_encrypt'] = 0;

// Prevent users belonging to these roles from being deployed.
$conf['deploy_addon_user_roles_block'] = array('some_user_role');

// Alter the custom services endpoint path which by default is "deploy".
// If you include and change this, edit the path on deploy_addon_next_url also.
$conf['deploy_addon_services_path'] = 'deploy';
