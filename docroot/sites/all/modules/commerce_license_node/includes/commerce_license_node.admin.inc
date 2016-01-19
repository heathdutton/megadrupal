<?php

/**
 * @file
 * Contains admin menu callbacks for the Commerce License Node module.
 */

/**
 * Settings form callback.
 */
function commerce_license_node_settings_form($form, &$form_state) {
  $help = t('This form allows you to enable node licensing for your product types.') . '<br />';
  $help .= t('Only licensable product types selected on the <em class="placeholder">General</em> tab are available here.');
  $form['help'] = array(
    '#markup' => $help,
  );

  // Create a list of licensable product types and their labels.
  $license_product_types = commerce_license_product_types();
  $product_types = array();
  foreach (commerce_product_type_options_list() as $type => $label) {
    if (in_array($type, $license_product_types)) {
      $product_types[$type] = $label;
    }
  }
  // If no licensable product types were found, stop here.
  if (count($product_types) == 0) {
    return array(
      'error' => array(
        '#markup' => t('No licensable product types found, select at least one on the <em class="placeholder">General</em> tab.'),
      ),
    );
  }

  $form['commerce_license_node_product_types'] = array(
    '#title' => t('Product types'),
    '#type' => 'checkboxes',
    '#default_value' => commerce_license_node_product_types(),
    '#options' => $product_types,
  );

  $node_types = node_type_get_names();
  $form['commerce_license_node_licensed_bundle_types'] = array(
    '#title' => t('Licensed Bundle types'),
    '#type' => 'checkboxes',
    '#default_value' => variable_get('commerce_license_node_licensed_bundle_types', array()),
    '#options' => $node_types,
  );

  $form = system_settings_form($form);
  $form['#submit'][] = 'commerce_license_node_settings_form_submit';
  return $form;
}

/**
 * Submit callback of the file settings form.
 */
function commerce_license_node_settings_form_submit($form, &$form_state) {
  // This will create the commerce_license_node field on any newly selected product
  // types, and remove it from any newly deselected product types.
  commerce_license_node_flush_caches();
}
