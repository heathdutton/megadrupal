<?php
/**
 * @file 
 * Rules configuration changes to limit the domestic and international methods
 * the appropriate destinations for the Aus Post shipping module.
 */

/**
 * Implements hook_default_rules_configuration_alter().
 */
function uc_auspost_pac_default_rules_configuration_alter(&$configs) {
  if (!isset($configs['get_quote_from_auspost_domestic_letter'])) {
    return;
  }

  // Countries that are considered Australia. It's in an array because this is
  // mostly ripped straight out of uc_uspc.
  $countries = array(
    36 => t('Australia'),
  );

  $domestic_letter = rules_or();
  $domestic_letter->label = t('Order delivers to one of');
  $domestic_parcel = clone $domestic_letter;

  $intnl_letter = rules_or();
  $intnl_letter->negate();
  $intnl_letter->label = t('NOT Order delivers to one of');
  $intnl_parcel = clone $intnl_letter;

  foreach ($countries as $country_code => $country_name) {
    $condition = rules_condition('data_is', array(
      'data:select' => 'order:delivery-address:country',
      'value' => $country_code)
    );
    $condition->label = $country_name;

    $domestic_letter->condition(clone $condition);
    $domestic_parcel->condition(clone $condition);
    $intnl_letter->condition(clone $condition);
    $intnl_parcel->condition(clone $condition);
  }

  $configs['get_quote_from_auspost_domestic_letter']->condition($domestic_letter);
  $configs['get_quote_from_auspost_domestic_parcel']->condition($domestic_parcel);
  $configs['get_quote_from_auspost_intnl_letter']->condition($intnl_letter);
  $configs['get_quote_from_auspost_intnl_parcel']->condition($intnl_parcel);
}
