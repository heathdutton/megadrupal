SIMPLIFY COMMERCE
-----------------

DESCRIPTION
-----------
Integrate Simplify Commerce into your drupal applications to accept payments
easily. The Simplify Commerce Drupal module is built on top of the Commerce &
Libraries modules. Simplify Commerce [Drupal Commerce]
(https://drupal.org/project/commerce), module integrates with simplify
(https://www.simplify.com/commerce/) payment gateway into your Drupal Commerce
shop.

INSTALLATION
------------
- Install as usual, see http://drupal.org/node/70151 for further information.
- If not already installed, download the (dependency) module.
   Libraries module: http://drupal.org/project/libraries
- Download Simplify Commerce PHP SDK from
  https://www.simplify.com/commerce/docs/sdk/php and extract it to the folder
  'sites/all/libraries/simplify_commerce'.
- Note: If the folder libraries does not exist, Create the folder and then
  create the another folder 'simplify_commerce' within it.

CONFIGURATION
-------------
- Create an account at https://www.simplify.com/commerce/.
- Get your Public & Private Keys (Choose Sandbox keys for testing purposes)
   Ref: https://www.simplify.com/commerce/app#/account/apiKeys
- From the "Home » Administration » Store » Configuration" menu option, enable
  & edit the Simplify Commerce payment method and add your public & private
  API Keys.

NOTE
----
- Test with your 'Sandbox' keys first.
  Ref: https://www.simplify.com/commerce/docs/tutorial/index#testing
   
  When ready to process real payments, edit the values and input your 'Live'
  keys.

AUTHOR/MAINTAINER
-----------------
- Mayur Jadhav.
-
