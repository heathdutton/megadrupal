Date: 27th November 2014
Description:

YellowPay is a Bitcoin payment processor catering the Middle Eastern market 
and its merchants. This payment module has been built for e-commerce websites 
powered by Drupal Commerce 1.x and is compatible with both the 
Kickstart distribution and the ShopBuilder.me ready made eCommerce platform

For more information
YellowPay: www.yellowpay.co
Bitcoin: www.shubitcoin.com
ShopBuilder: www.shopbuilder.me


How to Install:

1. Enable the module as you would do with any other contributed module
2. Open the Store payment methods configuration to edit YellowPay Configuration
3. Give the role(s) of your choice the "Manage YellowPay configuration" to 
have the possibility of accessing the YellowPay configuration screen
4. Input the API Key and API Secret provided by YellowPay to communicate 
with payment YellowPay servers  (For security reason you can't view 
the content of those fields you can only re-edit)
5. Enable the Payment method to have it appear on the checkout payment options

Bitcoin Payment Specifics:

When collecting Bitcoin Payments, the payment needs some time to get authorized. 
This delay comes from the very nature of the P2P Bitcoin network that 
validates payments multiple times to get authorized. 
With YellowPay an authorization can take up to 1 hour.

Given the random nature of validation, the delay between the time a payment 
has been received and it has been authorized can vary. 
This is why received payments will take the buyer to the next checkout step 
yet the order balance will remain unchanged until authorized.

Bitcoin Payment delay indication:

We recommend adding a payment status indicator on the Orders List 
to allow your merchant to see if the payment has been authorized by YellowPay 
yet so he/she can know when is it safe to pack and ship the order.

You can do this pretty easily by editing the Orders list view:
1. Add a relationship: Commerce Order: Representative payment transaction
2. Add the field Commerce Payment Transaction: Status (Status) using the 
previously added relationship.
3. Save your changes
