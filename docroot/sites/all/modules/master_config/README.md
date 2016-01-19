# Master Config
This module allows you to configure the master module on the fly, without the need to edit any configuration files on your site. Adding master module configuration though Master Config sets the variables directly in your database.

## Installation
1. Unzip files and place in [path/to/your/site]/sites/all/modules
2. Navigate to admin/config/modules and enable.

## Usage
1. Navigate to admin/config/system/master_config and set your master scopes - or leave in the default if you're happy with them.
2. Click the module definitions tab and fill out the required fields for each scope.

You're all set - the master module variables are now set in the database. If you use Drush, you'll be able to use the Master module's drush commands as usual.