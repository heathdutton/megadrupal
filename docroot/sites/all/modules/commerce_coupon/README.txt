Commerce Coupon
===============

Description
-----------

Commerce Coupon module provides coupon functionality for Drupal Commerce
(http://drupal.org/project/commerce).

Coupons allow users to redeem discounts for Drupal Commerce products/orders
during checkout.
e.g. '10% off' or '$10 off'

Commerce Coupon is flexible and customisable:
- Multiple coupon types can be created
- Coupons are fieldable entities, meaning that custom fields can be added to
  each coupon type.
- Rules is used to handle the validation and redemption of coupons. As such, the
  default rules can be fully modified and/or new rules can be added to provide
  further coupon control.

Dependencies
------------

Drupal Commerce and all of its dependencies
Entity Reference


Installation
------------

Commerce Coupon contains two modules:

- Commerce Coupon
- Commerce Coupon UI

Enable both.

Optional
--------

Whilst not required, you will likely want to install either or both of:

- Commerce coupon fixed amount
  http://drupal.org/project/commerce_coupon_fixed_amount

  Generates a:
  - 'Fixed coupon' coupon type
  - 'Redeem a coupon with fixed amount' rule

- Commerce coupon percentage
  http://drupal.org/project/commerce_coupon_pct

  Generates a:
  - 'Percentage coupon' coupon type
  - 'Calculate coupon with percentage amount' rule

and possibly also:

- Commerce coupon batch
  http://drupal.org/project/commerce_coupon_batch

  Allows mass generation of coupons.


Configuration
-------------

- Commerce Coupon permissions

  Home > Administration > People > Permissions
  (admin/people/permissions#module-commerce_coupon)

- The main Commerce Coupon admin page

  Home > Administration > Store > Coupons
  (admin/commerce/coupons)


Coupon logic
------------

Coupons must always be of a coupon type (much like content and content types)

To create a coupon:
- First add a coupon type or install one of the contributed modules that creates
  one for you such as commerce coupon fixed amount or commerce coupon
  percentage.
- Create a coupon of a particular coupon type


Example - Create a '10% off' coupon
-----------------------------------

1) Install modules

   Install:
   - Commerce Coupon
   - Commerce Coupon UI
   - Commerce coupon percentage

2) Create a 'Percentage coupon' coupon

   Go to: Home > Administration > Store > Coupons

   Click 'Create Coupon'

   Choose 'Create Percentage coupon'
   (if you have no other coupon types clicking 'Create Coupon' will redirect to
    the 'Create Percentage coupon' form)

   Create a percentage coupon with the following:

   - Coupon Code: 10%OFF
   - Number of Uses: 1 (change as necessary)
   - Percentage Amount: 10
   - Active: Yes

   Save

3) Enable the 'Coupons' area on one of your Commerce checkout panes

   Go to: Home > Administration > Store > Configuration > Checkout settings

   Move the 'Coupons' item into one of the checkout panes

   Save

4) Test it!
