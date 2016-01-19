
Project PAAS
================================================================================

1. Introduction
--------------------------------------------------------------------------------
Project Paas is an automated testing tool for Drupal performance. This tool will
test your Drupal website from both the outside (frontend performance) and the
inside (for measuring backend performance). This module is a connector between
Project Paas and your website and is required to run the tests.
You can find more information about Project PaaS at
http://www.projectpaas.com.

In order to do its measurements, Project PaaS needs to hook into your site in 
some low level areas, most notable caching and the database. Replacing the cache
method with a wrapper method is handled automatically when activating the module
(the original cache method will continued to be used through the PaaS wrapper, 
so make sure you have configured caching as you will your production site). For 
the database, this module comes with a replacement database layer, currently 
only for MySQL.

2. Installation instructions
--------------------------------------------------------------------------------
Install and enable the module as you are used to, e.g. by downloading it from 
Drupal.org or using Drush. Make sure you configured your caching before 
enabling the module (or disable the module while configuring a different 
caching method).

Copy the directory paasmysql to includes/database.

Change yout settings.php to use the paasmysql driver instead of the standard 
mysql driver, e.g.:

    $databases = array (
      'default' => array (
        'default' => array (
          'database' => 'mydatabase',
          'username' => 'mydatabaseuser',
          'password' => 'mydatabasepassword',
          'host' => 'localhost',
          'port' => '',
          'driver' => 'paasmysql',
          'prefix' => '',
        ),
      ),
    );
    
Note that at this time only MySQL databases are supported by Project PaaS. 
Please let us know if you are interested in using PostgreSQL.

To measure cache actions, add the following lines to your settings.php file:

  include_once('./includes/cache.inc');
  include_once('./sites/all/modules/memcache/memcache.inc');
  include_once('./sites/all/modules/paas/cache.inc');
  $conf['cache_class_before_paas'] = 'MemCacheDrupal';
  $conf['cache_default_class'] = 'PaasCache';

When using the memcache module, provide the memcache class in the
'cache_class_before_paas' variable and include all three cache classes:

  include_once('./includes/cache.inc');
  include_once('./sites/all/modules/memcache/memcache.inc');
  include_once('./sites/all/modules/paas/cache.inc');
  $conf['cache_class_before_paas'] = 'MemCacheDrupal';
  $conf['cache_default_class'] = 'PaasCache';

Enable the module at admin/modules or via "drush -y en paas".
Start configuring your first test at http://www.projectpaas.com to obtain a 
connection key for your site and fill it in at admin/config/development/paas.
