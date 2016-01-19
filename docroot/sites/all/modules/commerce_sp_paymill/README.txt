Commerce SP Paymill
*******************

This module provides a Paymill recurring subscriptions integration for the
Commerce subscription product module.
This allows you to automatically bill bought subscription products on each
billing cycle via Paymill.


Prerequisite:
*************
Get familiar how Paymill subscriptions and the Commerce subscription products
module work.
Basic understanding will ease the configuration and administration.


Installation
************
Dependencies:
  * Commerce subscription products (2.x branch necessary)
  * Commerce Paymill (tested with 1.x, 2.x support should be possible)
  * Features (components exported with 1.x)
  * Message module (for logging)
  * Entity Reference

After the module has been activated, ensure that the included feature components
are in the default state.


Configuration
*************
Basic settings have to be filled in at:
  Store > Configuration > Paymill subscriptions
  (/admin/commerce/config/commerce_sp_paymill)

There you need to enter your Paymill API keys. This is a duplication to the
Paymill payment configurations of the Commerce module, but there the settings
are located in Rules components and are therefore difficult to extract.

Once the API keys are set, a Paymill webhook will be registered at a private
URL. The private URL to is based on a random key and in addition to that,
validation is performed based on the submitted subscription id.

After setting the API keys, edit your subscription products and set the option
"Paymill recurring billing". This will register Paymill offers.


Order Checkout
**************
If a user now checks out a single subscription product, the system will try to
register a subscription at Paymill.
After the payment has been validated a subscription object is created. This does
not directly create a Paymill transaction, as it happens with the standard
Paymill payment. The transaction is returned via a webhook invocation,
independent of the user interaction. Therefore the order balance is negative
after the payment has been finished.
The webhook invocation will later insert the transaction and complete the order.

To have a correct checkout workflow, do not set the order status to completed
on the checkout completion event, if the order balance is negative.
Still you can redirect the user after the payment for the subscription has been
completed.


Webhooks - Rules events
***********************
A webhook invocation happens on following Paymill events:
  - subscription.created
  - subscription.updated
  - subscription.deleted
  - subscription.succeeded
  - subscription.failed

You can react upon these events by either implementing the hook
'commerce_sp_paymill_webhook_event' or by using the Rules event 'Commerce SP
Paymill webhook event'.


Subscription Failure
********************
If the Drupal system receives a subscription.failed notification, the
subscription will get canceled and expires as soon as the validity date is
reached.
There is currently no real dunning management by Paymill, so that it would
retry the payment after a certain time.
It is recommended to send out an email to the client via Rules, informing him
about the failed payment and suggesting a new order checkout.


Subscription Entity Type
************************
The implementation uses a custom entity type called 'Commerce SP Paymill
subscription'.
This entity type stores a reference to the user, to the order, the Paymill
subscription ID and whether the subscription is active.

You can use Views to list the current Paymill subscription.


Webhooks Logging
****************
The Message module is used to log all webhook invocations. This is very useful
information when something goes wrong.
The Message type is called 'Commerce SP Paymill Webhook log' and can again be
listed via Views.


Cancellation Link
*****************
Users need to cancel their subscriptions via Drupal. The cancellation URL
(commerce_sp_paymill/cancel/[subscription entity id]) can either be manually
constructed in code or in Views (e.g. by adding a custom field and using the ID
token).


Administration
**************
Use the subscription entity type and the message log to get an overview on
what's happing on your website. Furthermore users with correct recurring Paymill
subscription need a validity date in the future and the field 'Skip automatic
expiration' activated.


Credits
*******
Matthias Hutterer (mh86)
