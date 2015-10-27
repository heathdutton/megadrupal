# Commerce Urbano

This module integrates [Urbano][0] shipping services with [Drupal Commerce][1].
It provides both dynamic shipping cost calculations and automatic pickup
requests when the order is first payed in full.

The module will use Urbano web services to calculate the shipping cost of every
shipable product in the order. When the order is first payed in full the module
will automatically create the pickup request in Urbano.

Commerce Urbano needs the Argentina address format provided by
[Addressfield Argentina][4] enabled for the shipping address. It will attempt to
automatically enable it on the default `commerce_customer_address` field.

If you enable [Commerce Shipment Message][2] a message will be added to the
order log when the pickup is requested.

## Usage

Once the module is enabled you have to set your credentials and options in the
shipping method settings page. You can choose between Urbano's live and sandbox
servers. Use the latter to make tests and switch to the live server when you are
ready.

You will also have to enable the Argentina address format in the address field
for the shipping and billing customer profiles and add a weight field and a
dimensions field (from [Commerce Physical Product][3]) to your product variation
types (not the product display type). A product is considered shipable when it
has weight and dimensions.

## Requirements

* Commerce Shipping 7.x-2.0 or newer
* [Commerce Physical Product][3]
* [Addressfield Argentina][4]
* Urbano credentials
* [Commerce Shipment Message][2] (optional)



[0]: http://www.urbano.com.ar/home-delivery.html      "Urbano home delivery"
[1]: https://www.drupal.org/project/commerce          "Drupal Commerce"
[2]: https://www.drupal.org/sandbox/mpv/2575189       "Commerce Shipment Message"
[3]: https://www.drupal.org/project/commerce_physical "Commerce Physical Product"
[4]: https://www.drupal.org/project/addressfield_ar   "Addressfield Argentina"
