<?php

/**
 * @file
 * Functions, forms and other stuff for Shared Content administration.
 *
 * @ingroup sharedcontent_admin
 */

/**
 * Return form for vocabularies selection.
 *
 * This form allows to select whicht taxonomy vocabularies will be taken into
 * account when building the index keywords.
 */
function sharedcontent_server_configure_keyword_vocabularies($form, &$form_state) {
  if (module_exists('taxonomy')) {
    $defaults = variable_get('sharedcontent_keyword_fields', array());
    $form['hint'] = array(
      '#type' => 'item',
      '#title' => t('Select keyword vocabularies'),
      '#markup' => t('Select fields to be taken into account when building the keywords for the Shared Content Index.'),
    );

    $no_fields = TRUE;

    foreach (field_info_instances() as $entity_type => $entity_values) {
      foreach ($entity_values as $bundle => $bundle_values) {
        $taxonomy_fields = array();
        foreach ($bundle_values as $key => $value) {
          $field = field_info_field($key);
          if ($field['module'] == 'taxonomy') {
            $taxonomy_fields[$key] = empty($value['label']) ? drupal_ucfirst($key) : $value['label'];
          }
        }

        if (!empty($taxonomy_fields)) {
          $no_fields = FALSE;
          $info = entity_get_info($entity_type);

          if (empty($form[$entity_type])) {
            $form[$entity_type] = array(
              '#type' => 'fieldset',
              '#title' => isset($info['label']) ? $info['label'] : drupal_ucfirst($entity_type),
              '#tree' => TRUE,
            );
          }
          $form[$entity_type][$bundle] = array(
            '#type' => 'checkboxes',
            '#options' => $taxonomy_fields,
            '#default_value' => empty($defaults[$entity_type][$bundle]) ? array() : array_values($defaults[$entity_type][$bundle]),
            '#title' => isset($info['bundles'][$bundle]['label']) ? $info['bundles'][$bundle]['label'] : drupal_ucfirst($bundle),
          );
        }
      }
    }

    if ($no_fields) {
      $form['hint']['#markup'] = t('No taxonomy vocabularies availlable.');
    }

    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save keyword fields')
    );
  }
  else {
    $form['no_taxonomy'] = array(
      '#type' => 'item',
      '#title' => t('Taxonomy not availlable'),
      '#markup' => t('Please activate the Taxonomy module in order to use advanced keyword building for the Shared Content Index.'),
    );
  }
  return $form;
}

/**
 * Form submit for vocabularies selection.
 */
function sharedcontent_server_configure_keyword_vocabularies_submit($form, &$form_state) {
  form_state_values_clean($form_state);
  variable_set('sharedcontent_keyword_fields', $form_state['values']);
}


/**
 * Returns a form to enable or disable reason base service access management.
 */
function sharedcontent_server_configure_variables($form, &$form_state) {
  if (module_exists('taxonomy')) {
    $form['sharedcontent_check_reason'] = array(
      '#type' => 'checkbox',
      '#default_value' => variable_get('sharedcontent_check_reason', FALSE),
      '#title' => t('Use Reasons'),
    );
  }
  else {
    $form['no_taxonomy'] = array(
      '#type' => 'item',
      '#title' => t('Taxonomy not availlable'),
      '#markup' => t('Please activate the Taxonomy module in order to use advanced service access management based on reasons.'),
    );
  }

  $themes = list_themes();
  array_walk($themes, '_sharedcontent_server_convert_to_name');

  $form['sharedcontent_overlay_theme'] = array(
    '#type' => 'select',
    '#title' => t('Overlay theme'),
    '#default_value' => variable_get('sharedcontent_overlay_theme', 'seven'),
    '#options' => $themes,
  );

  return system_settings_form($form);
}

/**
 * Convert a theme class to a human readable name.
 *
 * This is a user function targetet for array_walk().
 */
function _sharedcontent_server_convert_to_name(&$item, $key) {
  $item = $item->info['name'] . ' ' . $item->info['version'];
}
