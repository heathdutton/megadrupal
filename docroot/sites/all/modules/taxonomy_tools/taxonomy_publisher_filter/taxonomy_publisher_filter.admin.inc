<?php

/**
 * @file
 * Administrative page callbacks for the Taxonomy Publisher Filter module.
 */

/**
 * Build administration form for Taxonomy Publisher Filter.
 *
 * @ingroup forms
 */
function taxonomy_publisher_filter_admin_form($form, &$form_state) {

  $form['taxonomy_publisher_filter_cache'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use cache'),
    '#default_value' => variable_get('taxonomy_publisher_filter_cache', TRUE),
  );

  $form['taxonomy_publisher_filter_cache_time'] = array(
    '#type' => 'textfield',
    '#title' => t('Cache liftime'),
    '#description' => t('Time in seconds. 1day = 86400'),
    '#default_value' => variable_get('taxonomy_publisher_filter_cache_time', 60 * 60 * 24),
    '#maxlength' => 6,
    '#required' => TRUE,
    '#element_validate' => array('taxonomy_publisher_filter_settings_validate'),
    '#states' => array(
      'invisible' => array(
        ':input[name="taxonomy_publisher_filter_cache"]' => array('checked' => FALSE),
      )),
  );

  $form['link_to_permissions'] = array(
    '#markup' => '<a href="' . url('admin/people/permissions', array('fragment' => 'module-taxonomy_publisher_filter')) . '" >Set permissions</a>',
  );

  $rendered_form = system_settings_form($form);
  /* Add additional submit callback. */
  $rendered_form['#submit'][] = 'taxonomy_publisher_filter_settings_submit';
  return $rendered_form;
}


/**
 * Additional submit handler for Taxonomy Publisher Filter administration form.
 *
 * @see taxonomy_publisher_filter_admin_form()
 */
function taxonomy_publisher_filter_settings_submit($form, &$form_state) {
  variable_set('taxonomy_publisher_filter_cache_time', (int) $form_state['input']['taxonomy_publisher_filter_cache_time']);
  if (isset($form_state['input']['taxonomy_publisher_filter_cache'])) {
    variable_set('taxonomy_publisher_filter_cache', TRUE);
  }
  else {
    variable_set('taxonomy_publisher_filter_cache', FALSE);
    cache_clear_all(NULL, 'cache');
    drupal_set_message('Caches cleared.', 'status');
  }
}

/**
 * Validator for submit handler.
 *
 * @see taxonomy_publisher_filter_admin_form()
 */
function taxonomy_publisher_filter_settings_validate($form, &$form_state) {
  if (is_numeric($form_state['input']['taxonomy_publisher_filter_cache_time'])) {
    return TRUE;
  }
  form_set_error('Taxonomy Publisher Filter SAVE', "Please provide a numeric value.");
}
