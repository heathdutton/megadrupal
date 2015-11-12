Commerce Buy One Click Module
=====================

Commerce Buy One Click Module allows customers to checkout directly from the 
product page with just one click, skipping the annoying checkout steps.

 - The module adds "Buy now with 1-Click" button to product page, just next to 
   "Add to cart" button.
 - Clicking on this button opens pop-up with form, prompting user to enter his 
   name and email address.
 - When the user fills the pop-up form out and presses "Make an order" button, 
   the new order with user data is created, and the special configurable 
   "Buy one click" message is sent to site administers.
 
Features of the module:

 - The behavior and fields of the popup form are configurable with hooks.
 - The module allows you to place "Buy now with 1-Click" buttons in views, 
   blocks and any other places of the site.
 - The module provides [commerce-order:commerce-buy-one-click-name], 
   [commerce-order:commerce-buy-one-click-email] and 
   [commerce-order:commerce-buy-one-click-items] tokens.

=====================
Installation, configuration and test-drive.
=====================

1. Install and enable Drupal Commerce and Fancybox modules.
2. Configure your Drupal Commerce store.
3. Install and enable Commerce Buy One Click module.
4. Go /admin/commerce/config/commerce_buy_one_click and configure the module.
5. Go to any product page, find "Buy now with 1-Click" button and click it.
6. Fill the email field in and press "Make an order".
7. Go to /admin/commerce/orders and make sure that one click order was 
   successfully created.
8. Go to mailbox specified on admin/commerce/config/commerce_buy_one_click page 
   and make sure that you received the one click order message.

=====================
Creating your own buy one click button in custom blick.
=====================

1. Go to /admin/structure/block.
2. Create a new block.
3. Put the following content into the block (don't forget about proper text 
   format):

@code
<div class="commerce-buy-one-click-button-wrapper">
  <div class="commerce-buy-one-click-button-product-id element-hidden">1</div>
  <div class="commerce-buy-one-click-button-add-product-to-cart element-hidden">1</div>
  <div class="commerce-buy-one-click-button-quantity element-hidden">1</div>
  <a href="#" class="commerce-buy-one-click-button">Buy one click block</a>
</div>
@endcode

4. Put the block to say "Sidebar first" or any other visible part of your site.
5. Go to any page of your site and press "Buy one click block" link, make sure 
   that you see pop-up.

=====================
Using buy one click button in views.
=====================

1. Go to /admin/structure/views and create a new view of commerce products.
2. Make sure that your view have "Commerce Product: Product ID" in "Fields" 
   section (we need [product_id] token).
3. Add new "Global: Custom text" field to your view.
4. Place the following content into "Text" section of this field:

@code
<div class="commerce-buy-one-click-button-wrapper">
  <div class="commerce-buy-one-click-button-product-id element-hidden">[product_id]</div>
  <div class="commerce-buy-one-click-button-add-product-to-cart element-hidden">1</div>
  <div class="commerce-buy-one-click-button-quantity element-hidden">1</div>
  <a href="#" class="commerce-buy-one-click-button">Buy one click</a>
</div>
@endcode

5. Test "Buy one click" buttons of your view. Make sure that they creates orders
   with different products.

=====================
JS callback after creating the order.
=====================

You can add custom JS code which will be called when order is completed with 
Commerce Buy One Click module.

This code should look like this:

jQuery(function() {
  jQuery.fn.commerceBuyOneClickOrderCompletedCallback = function(order, name, email) {
    // Redirect user to other page.
    window.location = "/my_custom_page";
    // ...
  };
});
