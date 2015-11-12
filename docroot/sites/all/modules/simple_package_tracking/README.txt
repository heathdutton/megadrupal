-----------------------
Simple Package Tracking
-----------------------

The Simple Package Tracking module makes adding tracking information to a
customer's order a breeze! Simple Package Tracking was developed for online
stores that drop ship and/or manually process a large numbers of orders
with limited staff. With this module, tracking information is tied to the
order, rather than individual packages, which dramatically minimizes the
number of steps required to add tracking information to the order.

Drupal Simple Package for download at
https://www.drupal.org/project/simple_package_tracking

Enable the module as normal.


--------
Overview
--------

The module has two datasets:

 *  Carriers, who might or might not have a package tracking facility,
 *  Package tracking entries attached to customer orders when the order is
    shipped.

The Administration section sets the carriers allowed for your store, whilst
the normal order view screen or the quick-edit screen allows you to attach
the tracking entries. Each tracking entry shows the customer the carrier
name, the tracking number, a link to the tracking service URL to show the
tracking, and a notes field that typically holds the date the parcel was
shipped (but can be anything you like).


--------------
Administration
--------------

Go to: Store > Configuration > Simple Package Tracking
       admin/commerce/config/tracking/carriers

Adding a carrier amounts to specifying the carrier name and the pattern for
calculating the URL at which tracking can be viewed for any given order
number. Three fields are provided:

New carrier name:

    The name of your postal service. This will be visible to the customer.

Example tracking number:

    A sample tracking number of a parcel.

Carrier URL to show tracking for the example tracking number:

    The exact URL that will take one to the web page to view tracking for
    the sample number.

For example, for Australia Post I might add:

---------------------------------------------------------------
New carrier name:
    Australia Post

Example tracking number:
    12345

Carrier URL to show tracking for the example tracking number:
    http://auspost.com.au/track/track.html?exp=b&id=12345
---------------------------------------------------------------

The system calculates the general pattern by looking for the example
number in the URL. Alternatively, you could specify the final pattern
directly. For example:

---------------------------------------------------------------
New carrier name:
    Australia Post

Example tracking number:
    (leave blank)

Carrier URL to show tracking for the example tracking number:
    http://auspost.com.au/track/track.html?exp=b&id=@tracking_number
---------------------------------------------------------------

ALSO PROVIDED (by fugazi):

The file, tracking-urls.txt, contains suitable URLs for many tracking
services around the world. NB: We cannot guarantee that this list will be
accurate, as carriers do change their computer systems from time to time.
As always, test on your testing system before going live. Please report any
discrepancies by posting an issue for this module on drupal.org.

A link is provided to allow you to automatically set the configuration
simply by clicking on your chosen carrier from the list!

--------------------------------
Also on the administration page:
--------------------------------

A short configuration form allows you to set a default text for the note
field that will appear on the order page. This can be made to default to
today's date (in any format you like). For example, if the default text is:

Posted on %e %b, %Y, %l:%M%P

and the time is 3:35 on 10 October 2014, then the Note to Customer field
will be prefilled as:

Posted on 10 Oct, 2014, 3:35pm

The codes available for this feature are as listed on:
http://php.net/manual/en/function.strftime.php


----------------
Order Fulfilment
----------------

Tracking information will be entered under
       admin/commerce/orders

You can add or remove tracking for individual orders on either the order
view screen for an individual order, or on the quickedit panel for an order
in the orders list.

The fields shown are:

Carrier:
    A drop-down list of the carriers you have set up in the administration
    section.

Tracking number:
    The number given to you by the postal service when you posted the
    parcel.

Note to customer:
    This is provided so you can enter the time of posting if you wish, but
    you can enter any short message you please. If you set up the default
    text using the system explained above, this will be pre-filled with the
    current date and/or time (in whatever format you like).

Execute associated actions:
    A checkbox to control whether actions will execute, as explained below.


-----
Buyer
-----

The buyer can view, but not edit, tracking information on their order view
screen.


------------------------------------------------------------------------
Automatically sending email notifications when you add a tracking number
------------------------------------------------------------------------

The module invokes the event 'simple_package_tracking_added' when you add
a tracking number to an order. Any actions that you connect to this event
will be executed. (A checkbox is provided to suppress these actions if they
are not needed on some occasion.)

To get this going, go to admin/config/workflow/rules. Add a new rule and
select the event "A tracking number is added to order". You can add any
actions, but a good idea is to add two:

1) a "Send mail" action. You can include tokens to refer to the carrier,
   the tracking number, and the tracking URL. For example:

    Your parcel has now been sent via [tracking-carrier:value], with
    tracking number [tracking-number:value]. To track your parcel, please
    go to [tracking-url:value] to see your tracking information.

2) Then add a "Show a message on the site" action so you see a
   verification. Your message should allow you to verify that the correct
   action has occurred. For example:

    Email sent to [commerce-order:mail-username] re tracking no.
    [tracking-number:value], url [tracking-url:value], with carrier
    [tracking-carrier:value].

Tokens available are:

[tracking-carrier:value]
    The name of the postal service (carrier) as set up in the admin page.

[tracking-number:value]
    The tracking umber given to you by the carrier.

[tracking-url:value]
    The URL of the carrier tracking service, with the number filled in.

[tracking-note:value]
    The text of the note attached to this tracking entry.


-----------
Limitations
-----------

The module was originally written for both Ubercart and Drupal Commerce.
However, circumstances have prevented any recent testing on Ubercart, and
it is strongly suspected that the module no longer works correctly for
Ubercart. It has been thoroughly tested with Drupal Commerce, and no bugs
are known at the time of writing.


-------
Credits
-------

pyrello
rhouse
fugazi
