A single page payment form

Provides a single page payment page where a user can 
enter payment information and an amount to pay.

Good for donations, settling invoices for services 
not managed by the current commerce system.

Provides an order with transactions against it 
much like a normal checkout process.

Provides a couple of alter hooks so additional 
form fields can be easily processed along with the order.

Notes:

Only supports a single payment method at a time 
(but you could alter which method to use using provided hooks)

Requires a prodict be set up as the base product of the transaction. 
This is done automatically when you first access the payment form 
(if you have the proper permission)

requires the default setup for drupal commerce such that 
entity relationships exist like this:

$product->commerce_price
$order->commerce_customer_billing->commerce_customer_address

The default product type "product" must exist

**Depends on most of the commerce modules**

INSTRUCTIONS:

1. Install module
2. The form will work at /justpay
3. There is also a Commerce Justpay block which you can 
set up to show the payment form wherever you like
4. Use commerce rules to configure what happens after payment, 
by default you just get returned to the form.
5. Configure the product dummy JUSTPAYMENT sku to 
set the currency of the payment
