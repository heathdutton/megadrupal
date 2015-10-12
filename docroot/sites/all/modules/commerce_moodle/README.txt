Commerce Moodle Integration is a module that integrates Drupal Commerce
with Moodle.

This module automatically enrolls Drupal users to Moodle courses when the 
specified products are bought through the shopping cart. To relate the product
in Drupal Commerce to the course in Moodle, the SKU and the Course ID are used.

Tested on Moodle 2.3 and 2.4.

Dependencies
------------
 * Drupal Commerce
 * Moodle Connector

Instructions
------------
 * Install and configure Moodle Connector
 * Go to Store -> Configuration -> Commerce Moodle Integration settings 
(/admin/commerce/config/moodle) and set up which product types are going to be 
treated as Moodle courses
 * Start creating products in Drupal Commerce. Use for the SKU the same value 
you are using for the course ID in Moodle

Considerations
--------------
 * Users in both Moodle and Drupal are expected to share the same user names.
You should use LDAP or other methods to sync them.

Author
------
Pere Orga Esteve <pere@orga.cat>
