This project creates a Pagos Net payment gateway for the Drupal Commerce
payment and checkout systems.

Pagos Net (http://www.pagosnet.com.bo/) is a payment gateway that allows you to
pay through convenience stores (http://www.pagosnet.com.bo/#!/pageEntidades).

Requirements
============

- NuSOAP library. You need to download the nusoap library from:
  https://github.com/deviservi/nusoap/archive/master.zip and unzip it in
  sites/all/libraries module. Then rename the nusoap-master folder to nusoap.

- You need to have add three fields for this module to work:
  - field_nit and field_nombre_razon_social need to be added to your billing
  profile. Pagos Net requires that information in order to work. Field nit
  should be an integer field, and nombre_razon_social a text field.
  - field_codigo_recaudacion needs to be added to the order, and it will
  save the CODE your customer uses to pay on the convenience stores.

- Your order must be in BOB (Boliviano) currency. Pagos net does not allow
  other currencies.

- You need to create a symlink in your webroot to the serverPagosNet.php file
  that creates a SOAP server that pagos net uses to notify your store when a
  payment is received.
