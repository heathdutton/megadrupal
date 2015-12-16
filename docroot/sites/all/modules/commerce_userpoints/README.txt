----------------
Summary
----------------
This module makes a bridge between Drupal Commerce module and Userpoints.

It allows users to create commerce line items with negative points value,
behaving like a discount system.
The user can choose how many points he wants to use, they will be automatically
 taken from his userpoints account.

There are 3 modules in this project, they are:
  commerce userpoints
  commerce userpoints payment method
  commerce userpoints discount


Each of them supply different functionality. List in below.
  commerce userpoints
		1. This module let you create a point currency. The created point currency can
    be used to pricing your product, just like USD does:)
    2. To create your own point currency, please go to "admin/commerce/config/currency/userpoints",    Then you will know how to create.
  

  commerce userpoints payment method
    1. This module define a new payment mothed : point payment. Just enable this
  submodule and place an order, on the order payment page, you will see there 
  a new payment mothed in the list.
    2. To config, like the rate etc, please goto "admin/commerce/config/payment-methods",
    find the payment method named "Points", then edit it; in the new page, go to "Actions" 
    section, edit the elements named "Enable payment method: Points", then you can config it.


  commerce userpoints discount
		1. This module act a point discount system. You can use point as discount when you check out.
		2. To config it, go to "admin/commerce/config/commerce-userpoints/discount".

