<?php

/**
 * @file
 * taxonomy to apachesolr link module file.
 * This file contains functions that implemtns the administration page for this
 * module.
 */

/**
 * Implements the main settings form for the module.
 */
function taxonomy_to_apachesolr_link_settings_form($form, $form_state) {
  $form['taxonomy_to_apachesolr_link']['taxonomy_to_apachesolr_link_default_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Default search path setting'),
    '#default_value' => _taxonomy_to_apachesolr_link_get_settings_default_path_value(),
    '#description' => t('This value is used as the suggested default search path for all fields using the taxonomy apachesolr link<br/><strong>Example:</strong> If your search path is at http://mysite.com/solr you should put solr in the field.'),
  );

  return system_settings_form($form);
}