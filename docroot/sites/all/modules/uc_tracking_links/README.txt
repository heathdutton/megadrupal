About
=====
This module provide a shipment tracking link on the order page for Ubercart.
When the user clicks on that link, the web browser navigates to the tracking
page provided by the carrier.

At the moment only few italian carriers are supported, but it can be easily
expanded to other carriers which provide a "simple" URL containing the tracking
number to access to the relative informations.

It is useful even when the link doesn't contains the tracking number: it simply
redirects the user to the place where to find informations about its shipment.

This module doesn't retrieve shipping tracking information directly: it just
connects the order page to the carrier portal where real-time data is visible.
Nor it accesses carrier's servers where a login account is required.
For those advanced functionality, consider the uc_tracking module instead.


Installing
==========
Download the uc_tracking_link module and install as other contributed modules.
Then enable it at admin/build/modules. No other options are to be set.


Using
=====
When creating a shipment for a package, a list of carriers is shown. Select one
and also specify the tracking number; if it provides a "simple" link then the
user will be able to see it on the order page.

Tracking numbers will show up as active links in the user order history page at
user/#/orders. Clicking these links will redirect to the carrier's server to
display real-time tracking information.

Use "Other" if the carrier doesn't provide a "simple" link, then specify the
tracking number on the usual field but also insert the carrier name on the
shipment options field: they will be displayed together.

For example, SDA Express Courier provides a link similar to the following:
http://wwww.sda.it/SITO_SDA-WEB/dispatcher?id_ldv=$tracking_number&invoker=...
where $tracking_number is a placeholder for the tracking number.

When the user creates a new shipment and indicates SDA as carrier, then on the
order page a link is visible; that link combinates the previous URL with the
tracking number set by the user. Clicking on the link, redirects to the specific
page for that shipments on the SDA portal.

Another example is Poste Italiane which doesn't provide a link including
tracking number, so the link simply redirects to its portal, where user can
enter manually the required informations in order to access to tracking pages.


Implementation details
======================
The module expands the uc_shipping_shipment_edit form by changing the shipment
carrier from textfield to select list.

Also the packages part in the order pane is overriden to show the composite link
instead of plain carrier textfield. For shipments created before activation of
this module, nothing changes.

If anyone knows of carriers which support similar syntax for tracking links, can
suggest it for addition into this module.

As an alternative, to add new custom carriers use
hook_uc_tracking_links_carriers_alter($carriers).

For example, to add a new carrier "test" the hook would look like:

function MYMODULE_uc_tracking_links_carriers_alter($carriers) {
  $carriers['test'] = array(
    'name' => 'Test',
    'link' => 'http://www.test.test',
  );
}

The new carrier Test now will be automatically detected and added in the list.
