Context.IO integration API

Requirements
------------

* Libraries API
  https://www.drupal.org/project/libraries

* Composer
  https://getcomposer.org

* External library: Context.IO PHP API
  https://github.com/contextio/PHP-ContextIO

Installation instructions
-------------------------

1. Download and enable libraries module.

2. Download Context.IO PHP API from https://github.com/contextio/PHP-ContextIO
   and extract it to sites/all/libraries or sites/sitename.com/libraries. the
   name of the folder must be PHP-ContextIO (the version number from the end
   should be removed).

3. Navigate to the sites/all/libraries/PHP-ContextIO folder from terminal.

4. Run `composer install` command.

5. Enable contextio and contextio_admin modules on your site.

6. Navigate to admin/config/services/contextio and fill in your key and secret
   key.

7. Use.

Context.IO API keys
-------------------

To get your own API key you have to sign up for Context.IO at
https://context.io/#signup to get your API keys.


