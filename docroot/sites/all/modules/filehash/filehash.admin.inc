<?php

/**
 * @file
 * File hash configuration.
 */

/**
 * Builds the file hash settings form.
 */
function filehash_settings() {
  $form['filehash_algos'] = array(
    '#default_value' => variable_get('filehash_algos', array()),
    '#description' => t('The checked hash algorithm(s) will be calculated when a file is saved. For optimum performance, only enable the hash algorithm(s) you need.'),
    '#options' => filehash_names(),
    '#title' => t('Enabled hash algorithms'),
    '#type' => 'checkboxes',
  );
  $form['filehash_dedupe'] = array(
    '#default_value' => variable_get('filehash_dedupe', 0),
    '#description' => t('If checked, prevent duplicate uploaded files from being saved. Note, enabling this setting has privacy implications, as it allows users to determine if a particular file has been uploaded to the site.'),
    '#title' => t('Disallow duplicate files'),
    '#type' => 'checkbox',
  );
  return system_settings_form($form);
}
