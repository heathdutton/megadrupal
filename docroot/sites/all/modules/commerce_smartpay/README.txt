CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Installation


INTRODUCTION
------------

This module allows Drupal Commerce customers to pay using the 
Barclaycard Smartpay hosted payment solution
http://www.barclaycard.co.uk/business/smartpay/en

INSTALLATION
------------

 1. Download and copy the 'commerce_smartpay' folder into your modules directory
    Alternatively, use Drush command: drush dl commerce_smartpay

 2. Enable the module under /admin/modules within the group 'Commerce - Payment'
    Alternatively, use Drush command: drush en commerce_smartpay -y

 3. Enter your Smartpay Merchant details under /admin/commerce/config/smartpay
    You will need your Merchant account name, skin code and shared HMAC keys
	for your Test and Live environments.
