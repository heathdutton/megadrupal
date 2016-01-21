<?php

/**
 * @file
 * General settings form.
 */

/**
 * FAPI definition for the settings form.
 *
 * @see langcheck_settings_form_submit()
 * @ingroup forms
 */
function langcheck_settings_form($form_state) {
  module_load_include('inc', 'langcheck');
  $settings = variable_get('langcheck_settings', _langcheck_default_values('langcheck_settings'));
  $form = array();
  $form['langcheck_settings'] = array(
    '#tree' => TRUE,
  );
  $form['langcheck_settings']['check_batch'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('Language check batch process settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  // How many files to be checked in one go when running a batch process.
  $form['langcheck_settings']['check_batch']['num_of_files'] = array(
    '#title' => t('Number of files'),
    '#type' => 'select',
    '#options' => drupal_map_assoc(range(1, 25)),
    '#description' => t('Number of files to investigate during one page request of the batch process'),
    '#default_value' => $settings['check_batch']['num_of_files'],
  );
  // How many languages to be checked in one go when running a batch process.
  $form['langcheck_settings']['check_batch']['num_of_langs'] = array(
    '#title' => t('Number of languages'),
    '#type' => 'select',
    '#options' => drupal_map_assoc(range(1, 15)),
    '#description' => t('Number of languages to investigate during one page request of the batch process'),
    '#default_value' => $settings['check_batch']['num_of_langs'],
  );
  // Skip those directores while searching for source files. Relative paths
  // to index.php, one path in a line. No need to specify these directories
  // below as they will be automatically skipped.
  $form['langcheck_settings']['check_batch']['exclusions'] = array(
    '#title' => t('Exclusions'),
    '#type' => 'textarea',
    '#default_value' => implode("\n", $settings['check_batch']['exclusions']),
    '#description' => t('Some module may contain non Drupal files. Please list those directories, use relative paths to the index.php, and separate the paths with new line.') . ' ' . t('The following directories will be ignored by default: !exclusions', array('!exclusions' => theme('item_list', langcheck_default_exclusions()))),
  );
  // Own submit callback should run first.
  $form['#submit'] = array('langcheck_settings_form_submit');
  return system_settings_form($form);
}

/**
 * Convert lines of the textarea to array elements.
 *
 * @see langcheck_settings_form()
 */
function langcheck_settings_form_submit($form, &$form_state) {
  // Hopefully nobody will create a directory whose name starts with a space.
  $form_state['values']['langcheck_settings']['check_batch']['exclusions'] = preg_split('/\s*\n+\s*/', trim($form_state['values']['langcheck_settings']['check_batch']['exclusions']));
}
