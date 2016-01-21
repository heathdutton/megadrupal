MoneySuite Stripe Payment Gateway
 * Original author: Leighton Whiting - Released under GENERAL PUBLIC LICENSE
 * Current maintenance by multiple MoneySuite users.
 * Re: new initiative: https://www.drupal.org/node/2315653 

MS Stripe integrates Stripe with the MS Core payment and checkout
system. This module will fully integrate Stripe to MS Core in such a way
that clients can make payments straight in the shop in PCI-compliant
way without leaving the actual shop page. Stripe is a simple way to accept
payments online. With Stripe you can accept Visa, MasterCard, American Express,
Discover, JCB, and Diners Club cards directly on your store.


INSTALLING MS STRIPE MODULE
-------------------------------------

1. Download Libraries module from http://drupal.org/project/libraries

2. Enable MS Stripe and Libraries modules as usual: /admin/modules

3. Download Stripe library from https://github.com/stripe/stripe-php and
   extract it to sites/all/libraries/stripe-php (right now working with
   stripe version 1.18.0 instead of latest). TODO: update stripe version

CONFIGURING PAYMENT METHOD
--------------------------

1. Create an account at https://stripe.com/

2. Insert your API keys at the Stripe configuration page
   admin/moneysuite/gateways/ms_stripe

3. Remember to test the functionality with the test keys before going live!


CREDITS
-------

Inspired by the Commerce Stripe module.
