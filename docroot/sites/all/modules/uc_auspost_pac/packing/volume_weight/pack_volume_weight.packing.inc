<?php
/**
 * @file
 * Define and implement the Volume/Weight limited packing method.
 */

/**
 * Implements hook_packing_info().
 */
function pack_volume_weight_packing_info() {
  $info = array();

  $info['volume_weight'] = array(
    'title' => t('Volume and Weight'),
    'callback' => 'pack_volume_weight_pack',
    'file' => 'pack_volume_weight.packing.inc',
    'description' => t('Pack optimal boxes up to volume and weight limits.'),
    'config' => 'pack_volume_weight_config',
    'config file' => 'pack_volume_weight.admin.inc',
  );

  return $info;
}

/**
 * Packing algorithm callback.
 *
 * @see uc_auspost_pac_packing_do_not_pack().
 */
function pack_volume_weight_pack($method, $products, &$addresses) {
  $last_key = 0;
  $packages = array();

  // Get the settings for this quote method.
  $settings = pack_volume_weight_get_settings($method['id']);

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
    }
    if (!isset($packages[$key])) {
      $packages[$key] = array();
    }

    // Calc the volume and weight.
    $weight = $product->weight * uc_weight_conversion($product->weight_units, 'g');
    $volume = $product->height * $product->width * $product->length * pow(uc_length_conversion($product->length_units, 'mm'), 3);

    // Foreach qty of this package, check to see if it can be added to any of
    // the existing packages. If it does not fit, add it to a brand new one.
    for ($c = 0; $c < $product->qty; $c++) {

      $package_added = FALSE;
      foreach ($packages[$key] as $package_key => $package) {
        if (($settings['volume_disabled'] || $package->volume + $volume <= $settings['volume_limit'])
            && $package->grams + $weight <= $settings['weight_limit']) {

          // Product will fit in this package. Package is an object so has
          // been passed by reference to this point.
          $new_package = clone($package);
          $new_package->volume += $volume;
          $new_package->grams += $weight;
          $new_package->price += $product->price;

          // Volume derived dimensions. If a set is returned, replace the old
          // package with the new one.
          $dims = pack_volume_weight_dimensions_from_volume($new_package->volume, $method['settings']);
          if ($dims) {
            list($new_package->width, $new_package->height, $new_package->length) = $dims;
            $packages[$key][$package_key] = $new_package;

            $package_added = TRUE;
            break;
          }
        }
      }

      if (!$package_added) {
        $package = _uc_auspost_pac_new_package();
        $package->qty = 1;
        $package->price = $product->price;
        $package->grams = $weight;
        $package->volume = $volume;
        // Defaut dimensions.
        list($package->width, $package->height, $package->length) = uc_auspost_pac_convert_sort_dimensions($product, $method);
        // Volume derived dimensions, if they work.
        $dims = pack_volume_weight_dimensions_from_volume($package->volume, $method['settings']);
        if ($dims) {
          list($package->width, $package->height, $package->length) = $dims;
        }
        $packages[$key][] = $package;
      }
    }
  }

  return $packages;
}

/**
 * Convert a given volume into a set of small yet vaild set of dimensions.
 * This is atrocious as it doesn't ever consider the actual real size of the
 * products, but just their volume. The resulting parcel dimensions may be too
 * small to actually fit all of the products.
 */
function pack_volume_weight_dimensions_from_volume($volume, $settings) {
  // If the method does not have the parameters limiting the dimensions, just
  // return the rounded up cube root of the volume.
  if (!isset($settings['limits']['length']['min_safe'])) {
    $cube_root = ceil(pow($volume, 1/3));
    return array($cube_root, $cube_root, $cube_root);
  }

  // If the volume is less that the safe minimum, just return the same min.
  $min_safe_vol = $settings['limits']['length']['min_safe'] * $settings['limits']['width']['min_safe'] * $settings['limits']['height']['min_safe'];
  if ($volume < $min_safe_vol) {
    return array($settings['limits']['height']['min_safe'], $settings['limits']['width']['min_safe'], $settings['limits']['length']['min_safe']);
  }

  // The package is big enough to beat the minimums. The following is not
  // based on any kind of known algorithm and the answers it gives should be
  // considered a wild guess at best.
  else {

    // Start with the cube root to make a square package.
    $cube_root = pow($volume, 1/3);
    $up = ceil($cube_root);

    // Length is always the longest dimension. If it's not up to the minimum
    // sane limit, then return the smallest possible package again.
    if ($up < $settings['limits']['length']['min_safe']) {
      return array($settings['limits']['height']['min_safe'], $settings['limits']['width']['min_safe'], $settings['limits']['length']['min_safe']);
    }

    $dn = floor($cube_root);
    $max_girth = $settings['limits']['girth']['max'];

    // If the largest possible girth is larger than the limit, the package
    // must be made rectangular. Use the maximum efficient girth as a start.
    $uuug = 4 * $up;
    if ($uuug > $max_girth) {
      // Best possible params for girth are a square. We need that one side to
      // determine the 3rd dimension (length) when the size is too large for
      // girth limit.
      $one_side = ceil($max_girth / 4);
      $length = ceil($volume / ($one_side * $one_side));
      if ($length <= $settings['limits']['length']['max']) {
        return array($one_side, $one_side, $length);
      }
      // This package is too big to fit into anything.
      else {
        return FALSE;
      }
    }

    // Since the girth is smaller than the limit, we return the smallest
    // square(ish) package available.
    $ddug = 4 * $dn;
    if (($ddug <= $max_girth) && ($dn * $dn * $up >= $volume)) {
      return array($dn, $dn, $up);
    }
    $duug = (2 * $dn) + (2 * $up);
    if (($duug <= $max_girth) && ($dn * $up * $up >= $volume)) {
      return array($dn, $up, $up);
    }
    else {
      return array($up, $up, $up);
    }
  }
}
