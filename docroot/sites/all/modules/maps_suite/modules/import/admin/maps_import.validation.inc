<?php

/**
 * @file
 * Administrative UI for validators management.
 */

use Drupal\maps_import\Mapping\Validator\Type;

/**
 * Form builder; Validation overview.
 *
 * @see maps_import_validation_overview_submit()
 */
function maps_import_validation_overview($form, &$form_state) {
  drupal_set_title(t('Fields validation'));

  $drupal_types = Type::getDrupalTypes();
  $drupal_types = array_combine($drupal_types, $drupal_types);

  $form['types']['#tree'] = TRUE;
  foreach (Type::getMapsTypes() as $maps_type) {
    $form['types'][$maps_type] = array(
      '#type' => 'fieldset',
      '#title' => $maps_type,
    );

    $form['types'][$maps_type]['validation'] = array(
      '#type' => 'select',
      '#multiple' => TRUE,
      '#options' => $drupal_types,
      '#default_value' => Type::getCriterium($maps_type),
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Form submission handler; validator overview submit.
 *
 * @see maps_import_validation_overview()
 */
function maps_import_validation_overview_submit($form, &$form_state) {
  foreach ($form_state['values']['types'] as $maps_type => $drupal_types) {
    Type::setCriterium($maps_type, $drupal_types['validation']);
  }

  drupal_set_message(t('Fields validation updated.'));
}
