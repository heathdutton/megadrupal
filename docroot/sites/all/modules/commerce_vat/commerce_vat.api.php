<?php

/**
 * @file
 * Documents hooks provided by the vat module.
 */

/**
 * Defines countries that VAT is active in.
 *
 * @return
 *   An array of information about available vat rates. The returned array
 *   should be an associative array of vat rate arrays keyed by the countries
 *   iso2 code.
 *   Each vat rate array can include the following keys:
 *   - title: the title of the country defaults to that provided by Drupal
 *   - rules_component:
 *   - default_rules_component: boolean indicating whether or not the vat module
 *     should define a default default Rules component using the specified name;
 *     defaults to TRUE.
 */
function hook_commerce_vat_country_info() {
  $vat_countries = array('AU');

  return $vat_countries;
}

/**
 * Allows modules to alter vat countries defined by other modules.
 *
 * @see hook_commerce_vat_country_info()
 */
function hook_commerce_vat_country_info_alter(&$vat_countries) {
  $vat_countries['GB']['title'] = 'United Kingdom';
}

/**
 * Defines vat rates that may be applied to line items.
 *
 * @return
 *   An array of information about available vat rates. The returned array
 *   should be an associative array of vat rate arrays keyed by the vat rate
 *   name. Each vat rate array can include the following keys:
 *   - title: the title of the vat rate
 *   - display_title: a display title for the vat type suitable for presenting
 *     to customers if necessary; defaults to the title
 *   - description: a short description of the vat rate
 *   - rates: an array of rates keyed by name in order of newest first
 *     - name: in the format rate_year e.g. 169_2004
 *     - rate: the actual rate expresed as a decimal e.g. .169
 *     - stat: the date the rate came in to force e.g. 20040101
 *   - rules_component: name of the Rules component (if any) defined for
 *     determining the applicability of the vat to a line item; defaults to
 *     'commerce_vat_rate_[name]'. If the vat rate name is longer than 46
 *     characters, it must have a Rules component name set here that is 64
 *     characters or less.
 *   - default_rules_component: boolean indicating whether or not the vat module
 *     should define a default default Rules component using the specified name;
 *     defaults to TRUE.
 *   - price_component: name of the price component defined for this vat rate
 *     used when the vat is added to a line item; if set to FALSE, no price
 *     component will be defined for this vat rate
 *   - admin_list: boolean defined by the vat UI module determining whether or
 *     not the vat rate should appear in the admin list
 *   - calculation_callback: name of the function used to calculate the vat
 *     amount for a given line item, returning either a vat price array to be
 *     added as a component to the line item's unit price or FALSE to not
 *     include anything; defaults to 'commerce_vat_rate_calculate'.
 */
function hook_commerce_vat_rate_info() {
  $vat_rates = array();

  $vat_rates['au_sales_gst'] = array(
    'country' => 'AU',
    'title' => t('GST'),
    'rates' => array(
      '20_2010' => array(
        'name' => '20_2010',
        'rate' => .2,
        'start' => '20100101',
      ),
    ),
  );

  return $vat_rates;
}

/**
 * Allows modules to alter vat rates defined by other modules.
 *
 * @see hook_commerce_vat_rate_info()
 */
function hook_commerce_vat_rate_info_alter(&$vat_rates) {
  $vat_rates['au_sales_gst']['title'] = 'Australian GST';
}
