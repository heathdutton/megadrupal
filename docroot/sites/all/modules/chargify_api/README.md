Chargify API
============

Drupal module to interact with Chargify API (http://chargify.com).
Uses the Chargify API library (https://github.com/johannez/chargify).


Supported Resources
-------------------

- Products
- Customers
- Subscriptions
- Coupons
- Components
- Transactions



Installation
------------

1. Install the module as usual into your Drupal site.
2. Install the library through Composer. If you have Composer already installed,
it's simply "composer install". Otherwise install the Composer module (https://drupal.org/project/composer)
or follow the instructions on http://getcomposer.org
3. Enter the Domain and API key for your Chargify account on the configuration
page (/admin/config/services/chargify).
4. Make sure you set the proper permissions ("access chargify").


Usage
-----
The general concept is to first get a controller for the resource (product, customer, etc.) that
you want to interact with and then use the functions available.

For example to get a listing of all products in the system:
```php
<?php

$pc = chargify_api_controller('product');
$products = $pc->getAll();

?>
```

Sending data to Chargify is easy as well.
```php
<?php

$data = array(
  'customer' => array(
    'first_name' => 'Joe',
    'last_name' => 'Smith',
    'email' => 'joe4@example.com',
    'organization' => 'Example Corp.',
    'reference' => 'js',
  )
);

$cc = chargify_api_controller('customer');
$new_customer = $cc->create($data);
?>
```



