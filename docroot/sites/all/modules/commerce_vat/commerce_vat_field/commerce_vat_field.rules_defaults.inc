<?php

/**
 * @file
 * Alters default vat components and rules.
 */

/**
 * Implimentation of hook_default_rules_configuration_alter().
 */
function commerce_vat_field_default_rules_configuration_alter(&$configs) {
  // Loop over every vat rate looking for rates requiring a default component
  // that specify a a component name.
  foreach (commerce_vat_rates() as $name => $vat_rate) {
    if ($vat_rate['default_rules_component'] && !empty($vat_rate['rules_component']) && !empty($configs[$vat_rate['rules_component']])) {
      $field = 'commerce_' . drupal_strtolower($vat_rate['country']) . '_vat';
      $configs[$vat_rate['rules_component']]->condition('entity_has_field', array('entity:select' => 'line-item', 'field' => 'commerce_product'))
        ->condition('entity_has_field', array('entity:select' => 'line-item:commerce-product', 'field' => 'commerce_' . drupal_strtolower($vat_rate['country']) . '_vat'))
        ->condition('data_is', array('data:select' => 'line-item:commerce-product:' . 'commerce-' . drupal_strtolower($vat_rate['country']) . '-vat', 'value' => $name));
    }
  }
}
