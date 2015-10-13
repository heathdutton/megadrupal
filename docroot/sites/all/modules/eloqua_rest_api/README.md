Eloqua REST API
================

This is a Drupal extension that integrates [Elomentary]()--a PHP library that
facilitates communication with Eloqua's REST API--with Drupal. It does nothing
on its own, other than make Elomentary available to Drupal and authenticate API
calls with configurable credentials.

Install this module if you're using another module that depends on it. You might
also want to use this module if you're a developer looking to more deeply
integrate Drupal and Eloqua.


## Installation & configuration
1. Install this module and its dependency, [Composer Manager](), via drush:
  `drush dl eloqua_rest_api composer_manager`
2. Enable Composer Manager: `drush en composer_manager`
3. Then enable this module: `drush en eloqua_rest_api`
4. Composer Manager may automatically download and enable requisite PHP
   libraries, but if not, run `drush composer-manager install` or
   `drush composer-manager update`.

Configuration is simple!
1. Ensure your user has the permission `administer eloqua rest api`.
2. Navigate to `admin/config/services/eloqua` to enter Eloqua credentials. It's
   recommended that you use admin credentials to your Eloqua Instance, otherwise
   your password may expire and break integrations.

More information on [installing and using Composer Manager]() is available on
GitHub.


## Developing with this module
1. In your custom / contributed module, add a dependency on this module in your
   .info file: `dependencies[] = eloqua_rest_api`
2. In your module code, you can return an Elomentary client by calling
   `eloqua_rest_api_client();`.
3. If you need to globally modify the Elomentary client, you can implement
   `hook_eloqua_rest_api_client_alter()`. See [eloqua_rest_api.api.php]() for
   more details.

For example:
```php
$client = eloqua_rest_api_client();
$contact = $client->api('contact')->show(123);
```

For more details on how to use Elomentary, see [Elomentary usage documentation]()
on GitHub.

Note also that this module references an early version of Elomentary that only
implements a subset of Eloqua's API. Work on [full integration is ongoing]()
and contributions are welcomed and encouraged!

[Elomentary]: https://github.com/tableau-mkt/elomentary
[Composer Manager]: https://www.drupal.org/project/composer_manager
[installing and using Composer Manager]: https://github.com/cpliakas/composer-manager-docs/blob/master/README.md#installation
[eloqua_rest_api.api.php]: eloqua_rest_api.api.php
[Elomentary usage documentation]: https://github.com/tableau-mkt/elomentary/blob/0.1/doc/index.md
[full integration is ongoing]: https://github.com/tableau-mkt/elomentary/issues
