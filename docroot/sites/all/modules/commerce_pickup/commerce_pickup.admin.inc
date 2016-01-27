<?php

/**
 * @file
 * Commerce Pickup - admin pages/forms
 */


/**
 * Commerce pickup settings form.
 */
function commerce_pickup_settings_form($form, &$form_state) {

  // Generate an array of bundles with addressfields.
  $options = array();
  $options = array();
  foreach (entity_get_info() as $entity_type => $entity) {
    if (!empty($entity['fieldable']) && $entity_type != 'group') {
      foreach($entity['bundles'] as $bundle_name => $bundle) {
        // Prefix the bundle name with the entity type.
        $options[$entity_type][$entity_type . '__' .$bundle_name] = check_plain($bundle['label']);
      }
    }
  }

  // Pickup location bundle.
  $form['commerce_pickup_location_entity'] = array(
    '#type' => 'select',
    '#title' => t('Pickup location bundle'),
    '#description' => t('Select the entity bundle you would like to use for pickup locations. WARNING: Changing this after pickup locations have been used in orders will have weird side-effects with the pickup location field in Views.'),
    '#options' => $options,
    '#default_value' => variable_get('commerce_pickup_location_entity', NULL),
    '#required' => TRUE,
  );

  // Pickup shipping service display title.
  $form['commerce_pickup_service_display_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Pickup service display title'),
    '#description' => t('Specify how you would like the pickup service to appear in checkout and in the order line item.'),
    '#default_value' => variable_get('commerce_pickup_service_display_title', 'Pickup'),
    '#required' => TRUE,
  );

  return system_settings_form($form);
}
