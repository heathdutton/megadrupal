
This is a new generation Product Key module for Drupal 7.

Requires Views, Rules 2, Feeds.
Integrates with Advanced Help, Rules 2.


BASIC - SETUP


- First Create your products. IMPORTANT! 
	- do this under http://www.examplesite.com/admin/commerce/products/add/product
	
- Next edit the default sequence 'Voucher' and select the product(s) you wish to assign the keys to. You can set other up at this time as well.
  - do this under http://www.examplesite.com/admin/commerce/product-keys/types
  
- At this point in time we are leaving it to the administrator to setup the actions to assign the product keys to the user / order.
  Step 1. Create a New Rule "Assign product key to user and order" the event to attach it to would be what ever is moving your product to completed.
  Step 2. Add an action and select the action "Assign a product key to a order and user".
  
- Load your product keys into the system.
  - do this under http://www.examplesite.com/admin/commerce/product-keys/add


And you are ready to go.

