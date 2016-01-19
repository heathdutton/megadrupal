This is an implementation of hungarian shipping method pick-pack pont for drupal
commerce shops.

The pickpackpoints module read from automatic generated validboltlista.xml
and create data of shops.

The pickpack_field is create a field for commerce order.
 
The pickpack_shipping module is "clone" of commerce_flat_rate module.
Able to create pick-pack shipping types and rates.

Pickpack export generate CSV file from orders whitch using pick-pack shipping
method. This CSV file can upload to pick-pack pont site to order the service.

Installation:
- Copy the pickpackpoints folder to sites/all/modules

- Enable modules in admin/modules Pick-pack pont group with all dependencies

- Configure properly the commerce_shipping module.

- Go to admin/commerce/config/shipping page and choose
"add a pick-pack service". You will see now same form as commerce_flat_rate
module, fill data and save. After save you can same rules condition adding to
this shipping mode.

- Add a product to cart and make the shopping procedure.
