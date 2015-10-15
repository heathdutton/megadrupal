<?php

/**
 * @file
 * Administrative options for representative_image.
 */

/**
 * Callback for administrative settings.
 */
function representative_image_settings_form($form, &$form_state) {
  $entities = entity_get_info();

  $form['representative_image_default'] = array(
    '#type' => 'select',
    '#title' => t('Default image'),
    '#description' => t('What should happen if no representative image is found?'),
    '#default_value' => variable_get('representative_image_default', NULL),
    '#empty_option' => t('No image'),
    '#options' => array(
      'first' => t('Use the first image found on the given entity'),
      'logo' => t('Use the site logo'),
      'first_or_logo' => t('Use the first image on the given entity. If no image is found, use the site logo.')
    ),
  );

  $form['entities'] = array(
    '#type' => 'vertical_tabs',
    '#default_tab' => 'edit-node',
    '#attached' => array(
      'js' => array(drupal_get_path('module', 'representative_image') . '/js/representative_image.js'),
    ),
  );
  foreach ($entities as $entity_name => $entity) {
    representative_image_entity_settings_form($form, $form_state, $entity_name, $entity);
  }

  // Because we are using system_settings_form() here, the variable
  // 'representative_image' gets set with an array automatically, even
  // though it is never used.
  return system_settings_form($form);
}

function representative_image_entity_settings_form(&$form, &$form_state, $entity_name, $entity, $specific_bundles = array()) {
  // If necessary, filter out the bundles.
  $bundles = empty($specific_bundles) ? $entity['bundles'] : array_intersect_key($entity['bundles'], array_flip($specific_bundles));

  // If the entity is not fieldable, check that there is at least one bundle
  // with an image field.
  if (!$entity['fieldable']) {
    $options = array();
    foreach ($bundles as $bundle_name => $bundle) {
      $options = array_merge($options, representative_image_available_fields($entity_name, $bundle_name));
    }
    if (count($options) == 0) {
      return;
    }
  }

  $form[$entity_name] = array(
    '#type' => 'fieldset',
    '#title' => check_plain($entity['label']),
    '#description' => t('For each bundle listed below, select an image field to use when an image is needed to represent it'),
    '#collapsed' => TRUE,
    '#collapsible' => TRUE,
    '#group' => 'entities',
    '#attributes' => array('id' => 'representative_image_' . $entity_name),
  );

  foreach ($bundles as $bundle_name => $bundle) {
    $options = representative_image_available_fields($entity_name, $bundle_name);

    // For non-fieldable entities, only display bundles that have at least one
    // image field.
    if (count($options) == 0 && !$entity['fieldable']) {
      continue;
    }

    $form[$entity_name][$bundle_name] = array(
      '#type' => 'select',
      '#title' => check_plain($bundle['label']),
      '#options' => $options,
      '#empty_option' => t('None'),
      '#empty_value' => 0,
      '#parents' => array('representative_image', $entity_name, $bundle_name),
      '#default_value' => representative_image_get_field($entity_name, $bundle_name),
    );

    if (!count($options)) {
      $form[$entity_name][$bundle_name]['#disabled'] = TRUE;
      $form[$entity_name][$bundle_name]['#description'] = t('You must define at least one image field for %name bundle to define a representative image.', array('%name' => $bundle['label']));
    }
  }

  $form['#submit'][] = 'representative_image_settings_form_submit';
}

/**
 * Submit handler for administrative settings.
 */
function representative_image_settings_form_submit($form, &$form_state) {
  $entities = entity_get_info();
  foreach ($entities as $entity_name => $entity) {
    foreach ($entity['bundles'] as $bundle_name => $bundle) {
      if (isset($form_state['values']['representative_image'][$entity_name][$bundle_name])) {
        $field = $form_state['values']['representative_image'][$entity_name][$bundle_name];
        representative_image_set_field($entity_name, $bundle_name, $field);
      }
    }
  }
}
