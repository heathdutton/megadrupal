<?php
/**
 * @file
 * A not-so-packing algorithm available as default for all shipping methods.
 * This method is included as part of the AusPost PAC module core since it is
 * the coded default for all shipping methods prior to configuration by admin.
 */

/**
 * The Do Not Pack Packing method callback. Split the products by source
 * address and then every single product as a separate product. Still keep
 * products drouped with qty > 1 together since the quote will be same for all
 * of them.
 *
 * @param $method
 *   The method array supplied to the quote, plus the settings.
 * @param $products
 *   An array of product objects as they are represented in the cart or order.
 * @param &$addresses
 *   Reference to an array of addresses which are the pickup locations of each
 *   package. They are determined from the shipping addresses of their
 *   component products.
 *
 * @return
 *   Array of packaged products. Packages are separated by shipping address.
 */
function uc_auspost_pac_packing_do_not_pack($method, $products, &$addresses) {
  $last_key = 0;
  $packages = array();

  foreach ($products as $product) {
    // Packages are grouped by the address from which they will be shipped. The
    // postcode of the address is required by Aus Post for the source address.
    $key = FALSE;
    $address = uc_quote_get_default_shipping_address($product->nid);
    foreach ($addresses as $index => $value) {
      if ($address->isSamePhysicalLocation($value)) {
        // This is an existing address.
        $key = $index;
        break;
      }
    }
    if ($key === FALSE) {
      // New address.
      $key = $last_key++;
      $addresses[$key] = $address;
      $packages[$key] = array();
    }

    $package = _uc_auspost_pac_new_package();
    $package->qty = $product->qty;
    $package->price = $product->price;
    if (strpos($method['id'], 'letter') !== FALSE) {
      list($package->height, $package->length, $package->width) = uc_auspost_pac_convert_sort_dimensions($product);
    }
    else {
      list($package->width, $package->height, $package->length) = uc_auspost_pac_convert_sort_dimensions($product);
    }
    $package->grams = $product->weight * uc_weight_conversion($product->weight_units, 'g');

    $packages[$key][] = $package;
  }

  return $packages;
}
