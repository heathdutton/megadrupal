<?php

/**
 * @file
 * Administrative pages for Adform.
 */

/**
 * Settings page callback.
 */
function adform_admin_settings_form($form_state) {
  $form['adform_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Adform ID'),
    '#default_value' => variable_get('adform_id', ''),
  );

  $form['adform_hostnames'] = array(
    '#type' => 'textarea',
    '#title' => t('Production hostnames'),
    '#default_value' => join("\n", variable_get('adform_hostnames', array())) . "\n",
    '#description' => t("Hostnames that's considered production, one per line. If the current hostname doesn't match any on this list, the tracking it marked as test."),
  );

  // Transform the variable format back into the human friendly format.
  $mappings = array();
  foreach (variable_get('adform_mappings', array()) as $pattern => $path) {
    $mappings[] = preg_replace(array('/\\.\\*/', '/\\\\\//'), array('*', '/'), $pattern) . '|' . $path;
  }

  $form['adform_mappings'] = array(
    '#type' => 'textarea',
    '#title' => t('Mapped paths'),
    '#default_value' => join("\n", $mappings) . "\n",
    '#description' => t("Patterns of paths that should be mapped to a static path in the format pattern|path, one per line. For example, search/node/*|search/node will map all search pages to search/node. Adform doesn't deal well with highly dynamic paths."),
  );

  return system_settings_form($form);
}

/**
 * Validate handler for settings form.
 *
 * Cleans up the hostnames and path mappings.
 */
function adform_admin_settings_form_validate($form, &$form_state) {
  $hostnames = explode("\n", trim($form_state['values']['adform_hostnames']));
  foreach ($hostnames as &$hostname) {
    $hostname = trim($hostname);
  }
  // Set the value to the split and trimmed array.
  form_set_value($form['adform_hostnames'], $hostnames, $form_state);

  // Process the mapping into an array easier to deal with.
  $mappings = array_filter(explode("\n", trim($form_state['values']['adform_mappings'])));
  if (!empty($mappings)) {
    $mappings_var = array();
    foreach ($mappings as $mapping) {
      $parts = explode('|', trim($mapping));
      if (sizeof($parts) != 2) {
        form_set_error('adform_mappings', t('Invalid mapping %mapping', array('%mapping' => $mapping)));
      }
      else {
        list($pattern, $path) = $parts;
        $pattern = preg_replace('/\\\\\*/', '.*', preg_quote($pattern, '/'));
        $mappings_var[$pattern] = $path;
      }
    }
    form_set_value($form['adform_mappings'], $mappings_var, $form_state);
  }
}
