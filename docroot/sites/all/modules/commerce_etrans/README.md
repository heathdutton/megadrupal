# Commerce Etrans

This module integrates [etrans][0] (Área 54) shipping services for
[Drupal Commerce][1]. It provides both dynamic shipping calculations and
automatic pickup requests when the order is first payed in full.

The module will use etrans web services to calculate the shipping cost of every
shipable product in the order. If selected, your user will have to choose a time
window for delivery and the module will generate the shipping request in etrans.
At the end of the day you will have to manually confirm the shippings in the
etrans website. Etrans will then pickup the package and deliver it to your
customer.

Commerce Etrans needs the Argentina address format provided by
[Addressfield Argentina][5] enabled for the shipping address. It will attempt to
automatically enable it on the default `commerce_customer_address` field.

If you enable [Commerce Shipment Message][3] a message will be added to the
order when the shipping is requested, with the etrans estimated pickup and
delivery times.

## Usage

Once the module is enabled you have to set your credentials, and options in the
shipping method settings page.

You will also have to enable the Argentina address format in the address field
for the shipping and billing customer profiles and add a weight field and a
dimensions field (from [Commerce Physical Product][3]) to your product variation
types (not the product display type). A product is considered shipable when it
has weight and dimensions.

## Requirements

* Commerce Shipping 7.x-2.0 or newer.
* [Commerce Physical Product][4]
* [Addressfield Argentina][5]
* etrans credentials (you can register [here][2])
* [Commerce Shipment Message][3] (optional)


[0]: http://www.etrans.com.ar                         "etrans"
[1]: https://www.drupal.org/project/commerce          "Drupal Commerce"
[2]: http://www.etrans.com.ar/nuevosUsuarios.php      "Register in etrans"
[3]: https://www.drupal.org/sandbox/mpv/2575189       "Commerce Shipment Message"
[4]: https://www.drupal.org/project/commerce_physical "Commerce Physical Product"
[5]: https://www.drupal.org/project/addressfield_ar   "Addressfield Argentina"
