AJAX BASKET LINK
-------------------------------------------------------------------------------

http://drupal.org/sandbox/jamesquinton/1856586
git clone -b 7.x-1.x jamesquinton@git.drupal.org:project/commerce_ajax_basket_link.git

OVERVIEW
-------------------------------------------------------------------------------
Ajax Basket Link is a simple add-on module for Drupal Commerce.

It creates a block containing a Drupal Commerce basket/cart
link that updates on the fly as items are added using ajax.

This came about by a need to have a 'View cart (£n x items)' style link in the
header of an ecommerce site I was developing.

FEATURES
-------------------------------------------------------------------------------
Creates block with order total and item count that links to cart
Ajax updates as new products are added
Requirements
Drupal Commerce

USAGE
-------------------------------------------------------------------------------
To use, enable the module and then drag the Ajax Cart Block into the desired
region. This can then be styled as needed using CSS etc.

Module settings can be configured at:
admin/commerce/config/advanced-settings/commerce_ajax_basket_link

TESTING
-------------------------------------------------------------------------------
Code has been tested using Drupal's Coder module, making use of the following
parameters: 
$ drush coder commerce_ajax_basket_link --druplart --minor --security

