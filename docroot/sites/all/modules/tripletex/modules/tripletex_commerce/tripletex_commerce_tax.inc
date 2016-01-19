<?php

/**
 * @file
 * Documents hooks provided by the Tax module.
 */

define('TRIPLETEX_MVA_RATE_3',  25.0); // 3: High vat.
define('TRIPLETEX_MVA_RATE_31', 13.0); // 31: Medium vat.
define('TRIPLETEX_MVA_RATE_32',  8.0); // 32: Low vat.
define('TRIPLETEX_MVA_RATE_5',   0.0); // 5: No vat (inside).
define('TRIPLETEX_MVA_RATE_6',   0.0); // 6: No vat (outside).

/**
 * Defines tax types used to categorize tax rates.
 *
 * @return
 *   An array of information about tax types available for use by rates. The
 *   returned array should be an associative array of tax type arrays keyed by
 *   the tax type name. Each tax type array can include the following keys:
 *   - title: the title of the tax type
 *   - display_title: a display title for the tax type suitable for presenting
 *     to customers if necessary; defaults to the title
 *   - description: a short description of the tax type
 *   - display_inclusive: boolean indicating whether or not prices containing
 *     this tax will include the tax amount in the displayed price; defaults
 *     to FALSE
 *   - rule: name to use for a default product pricing rule that calculates
 *     taxes of this type for line items; defaults to 'commerce_tax_type_[name]'
 *     but can be set to NULL to not create any default Rule
 *   - admin_list: boolean defined by the Tax UI module determining whether or
 *     not the tax type should appear in the admin list
 */
function tripletex_commerce_commerce_tax_type_info() {
  $tax_types = array();

  $tax_types['MVA'] = array(
    'title' => t('MVA'),
    'display_inclusive' => TRUE,
  );

  return $tax_types;
}

/**
 * Allows modules to alter tax types defined by other modules.
 *
 * @see hook_commerce_tax_type_info()
 */
function tripletex_commerce_commerce_tax_type_info_alter(&$tax_types) {
  $tax_types['MVA']['display_inclusive'] = TRUE;
}

/**
 * Allows modules to react to the creation of a new tax type via the UI module.
 *
 * @param $tax_type
 *   The tax type info array.
 * @param $skip_reset
 *   Boolean indicating whether or not this insert will trigger a cache reset
 *   and menu rebuild.
 *
 * @see commerce_tax_ui_tax_type_save()
 */
function tripletex_commerce_commerce_tax_type_insert($tax_type, $skip_reset) {
  // No example.
}

/**
 * Allows modules to react to the update of a tax type via the UI module.
 *
 * @param $tax_type
 *   The tax type info array.
 * @param $skip_reset
 *   Boolean indicating whether or not this update will trigger a cache reset
 *   and menu rebuild.
 *
 * @see commerce_tax_ui_tax_type_save()
 */
function tripletex_commerce_commerce_tax_type_update($tax_type, $skip_reset) {
  // No example.
}

/**
 * Allows modules to react to the deletion of a tax type via the UI module.
 *
 * @param $tax_type
 *   The tax type info array.
 * @param $skip_reset
 *   Boolean indicating whether or not this deletion will trigger a cache reset
 *   and menu rebuild.
 *
 * @see commerce_tax_ui_tax_type_delete()
 */
function tripletex_commerce_commerce_tax_type_delete($tax_type, $skip_reset) {
  // No example
}

/**
 * Defines tax rates that may be applied to line items.
 *
 * @return
 *   An array of information about available tax rates. The returned array
 *   should be an associative array of tax rate arrays keyed by the tax rate
 *   name. Each tax rate array can include the following keys:
 *   - title: the title of the tax rate
 *   - display_title: a display title for the tax type suitable for presenting
 *     to customers if necessary; defaults to the title
 *   - description: a short description of the tax rate
 *   - rate: the percentage used to calculate this tax expressed as a decimal
 *   - type: the name of the tax type this rate belongs to
 *   - rules_component: name of the Rules component (if any) defined for
 *     determining the applicability of the tax to a line item; defaults to
 *     'commerce_tax_rate_[name]'.
 *   - default_rules_component: boolean indicating whether or not the Tax module
 *     should define a default default Rules component using the specified name;
 *     defaults to TRUE.
 *   - price_component: name of the price component defined for this tax rate
 *     used when the tax is added to a line item; if set to FALSE, no price
 *     component will be defined for this tax rate
 *   - admin_list: boolean defined by the Tax UI module determining whether or
 *     not the tax rate should appear in the admin list
 *   - calculation_callback: name of the function used to calculate the tax rate
 *     for a given line item, returning either a tax price array to be added as
 *     a component to the line item's unit price or FALSE to not include
 *     anything; defaults to 'commerce_tax_rate_calculate'.
 */
function tripletex_commerce_commerce_tax_rate_info() {
  $tax_rates = array();

  $tax_rates['MVA_lav'] = array(
    'title' => t('MVA Lav sats'),
    'description' => t('Lav sats - 8%. Ofte aktuelt for reiser.'),
    'rate' => .08,
    'type' => 'MVA',
  );

  $tax_rates['MVA_medium'] = array(
    'title' => t('MVA Medium sats'),
    'description' => t('Merverdiavgift, medium sats - 12%. Stort sett matvarer.'),
    'rate' => .12,
    'type' => 'MVA',
  );

  $tax_rates['MVA_hoy_intern'] = array(
    'title' => t('MVA Høy sats'),
    'description' => t('Merverdiavgift, høy sats - 25%.'),
    'rate' => .25,
    'type' => 'MVA',
  );

  $tax_rates['MVA_hoy_extern'] = array(
    'title' => t('MVA Høy sats (ekstern)'),
    'description' => t('Merverdiavgift, høy sats - 25%.'),
    'rate' => .25,
    'type' => 'MVA',
  );

  $tax_rates['MVA_fritt'] = array(
      'title' => t('MVA fritt'),
      'description' => t('Unntatt merverdiavgift'),
      'rate' => .0,
      'type' => 'MVA',
  );


  return $tax_rates;
}

/**
 * Allows modules to alter tax rates defined by other modules.
 *
 * @see hook_commerce_tax_rate_info()
 */
function tripletex_commerce_commerce_tax_rate_info_alter(&$tax_rates) {
  //$tax_rates['ky_sales_tax']['rate'] = .06;
}

/**
 * Allows modules to react to the creation of a new tax rate via the UI module.
 *
 * @param $tax_rate
 *   The tax rate info array.
 * @param $skip_reset
 *   Boolean indicating whether or not this insert will trigger a cache reset
 *   and menu rebuild.
 *
 * @see commerce_tax_ui_tax_rate_save()
 */
function tripletex_commerce_commerce_tax_rate_insert($tax_rate, $skip_reset) {
  // No example.
}

/**
 * Allows modules to react to the update of a tax rate via the UI module.
 *
 * @param $tax_rate
 *   The tax rate info array.
 * @param $skip_reset
 *   Boolean indicating whether or not this update will trigger a cache reset
 *   and menu rebuild.
 *
 * @see commerce_tax_ui_tax_rate_save()
 */
function tripletex_commerce_commerce_tax_rate_update($tax_rate, $skip_reset) {
  // No example.
}

/**
 * Allows modules to react to the deletion of a tax rate via the UI module.
 *
 * @param $tax_rate
 *   The tax rate info array.
 * @param $skip_reset
 *   Boolean indicating whether or not this deletion will trigger a cache reset
 *   and menu rebuild.
 *
 * @see commerce_tax_ui_tax_rate_delete()
 */
function tripletex_commerce_commerce_tax_rate_delete($tax_rate, $skip_reset) {
  // No example.
}

/**
 * Allows modules to calculate taxes that don't determine applicability through
 *   default Rules components.
 *
 * @param $tax_type
 *   The tax type object whose rates should be calculated.
 * @param $line_item
 *   The line item to which the taxes should be applied.
 *
 * @see commerce_tax_type_calculate_rates()
 */
function tripletex_commerce_commerce_tax_type_calculate_rates($tax_type, $line_item) {
  // An implementation might contact a web service and apply the tax to the unit
  // price of the line item based on the returned data.
}
