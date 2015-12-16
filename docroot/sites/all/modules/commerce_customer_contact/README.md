# Commerce Customer Contact
## Introduction
Have you ever needed a quick and easy way for a site admin to contact a customer
on a Drupal Commerce-powered site to send a message about his or her order? For
example, to ask the customer question about the order, or to provide him or her
an update on the order status?

Introducing *Commerce Customer Contact*, a module that extends and customizes the
Contact module in core to display a contact form in any or all of the following
places:

- In a collapsible fieldset on the "View" tab of the order.
- In a collapsible fieldset on the "Payments" tab of the order.
- On a standalone "Contact customer" tab of the order.

## How to Install
1. Unpack Drupal module on the site as usual, under `sites/all/modules` or
   `sites/SITE_NAME/modules`.
2. Enable the "Commerce Customer Contact" module (as well as "Contact" and
   "Commerce Order" if you haven't enabled them already).
3. Grant appropriate users the `contact customers` and `administer customer
   contact settings` permissions under `admin/people/permissions`.
4. Configure where you want the contact form to appear under
   `admin/commerce/config/customer/contact`.

## How to Use
Based on the options you select during step #4 of installation, you can access
the contact form under the following paths:

- In a collapsible fieldset on the "View" tab of the order
  (`admin/commerce/orders/ORDER_NUMBER`).
- In a collapsible fieldset on the "Payments" tab of the order
  (`admin/commerce/orders/ORDER_NUMBER/payments`).
- On a standalone "Contact customer" tab of the order
  (`admin/commerce/orders/ORDER_NUMBER/contact`).


The form only appears for users with the `contact customers` permission. Users
do not, however, need the `access user contact forms` permission.

Additionally, the message sent out through the customer order form does not
include Drupal's stock verbiage about opting-out of notifications, etc. This
ensure that users can always be contacted about their orders.

The message will automatically include the salutation ("Hello X") and closing /
signature lines of the message ("Sincerely, the X team"), so you only need to
fill-in the subject line and body of the message.