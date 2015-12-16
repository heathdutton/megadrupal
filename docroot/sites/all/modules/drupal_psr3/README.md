PSR3 Watchdog
=============

PSR3 compatible logging for Drupal.

Usage
-----

```PHP
$logger = new Drupal\PSR3\Logger\Watchdog();
$logger->error('Something bad happened', array(
  'code' => 123,
  'info' => $info,
));
```

Setup
-----

```
composer update
drush en drupal_psr3
```
