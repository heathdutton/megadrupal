------------
Why use it ?
------------

Why to use Commerce Abandoned Cart as email marketing for Drupal Commerce ?

* The view displays the abandoned carts with the user e-mail address associated
* This module is 100% integrated with Commerce Views Bulk Operations

--------------------
How does that work ?
--------------------

All customers that abandoned their cart will receive an email with the details of their order when cron is run.  The view for the abandoned carts is set to get customers that abandoned their shopping cart for more than 30 minutes and their order is with Shopping cart, User notified, Checkout: shipping or Checkout: Review status. You can change that by modifying the abandoned_cart_orders view. The other two views: customers_who_have_not_ordered_for_X_weeks_months and purchase_anniversary_promo_view are used to get the customers that have not been purchasing for some time. The first view gets the customers that have not purchase for more than two weeks; the second view gets all the customers that made their last order a year ago, you can change the settings of those two views to get more desire results.  This module uses the commerce_message module, which means you can go to ‘Message types’ and change the text and structure of the emails for abandoned cart (abandoned_cart_message), customers that have not ordered in x months/weeks(customer_not_ordered_x_months_weeks) or customers that have not purchased for one whole year(anniversary_message). To test those functionalities you should change the create date for an order in the database according to the provided information above and the views settings.The module uses three different reaction rules to send emails respectively: -	rules_nonotify_user_abandoned_cart-	customers_who_have_not_purchase_x_months_weeks-	send_aniversary_emails_to_customersYou can send emails to customers that abandoned their carts only once after that the status of their order is set to user_notified, but you can change that from the settings of the rule.For customers that have not purchase X months/weeks you can send emails on daily bases, and customers that have not purchased for one whole year you can send notification emails per year, but of course you can change that by modifying the rules settings, mostly by modifying the conditions for those rules (the condition is Abandoned card notification comparison) you can set any amount of time to compare against. 
