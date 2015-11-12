<?php

/**
 * Form definition for the info plugins admin.
 */
function info_plugins_admin_form() {
  info_plugins_include('helpers');
  $types = info_plugins_get_all_plugin_types();
  $options = array();
  foreach ($types as $name => $type) {
    $options[$name] = $type['label'];
  }

  $form = array();

  // Get all the contrib modules for which we have plugin types so we can keep
  // track later.
  $contrib = array();
  foreach ($types as $name => $type) {
    if (!isset($type['provider'])) {
      continue;
    }

    if (!isset($contrib[$type['provider']])) {
      $contrib[$type['provider']] = array($name);
      continue;
    }
    $contrib[$type['provider']][] = $name;
  }

  $form['contrib'] = array(
    '#type' => 'hidden',
    '#value' => serialize($contrib)
  );

  $form['help'] = array(
    '#type' => 'markup',
    '#markup' => t('Please keep in mind that saving this form will perform a full cache clear to make sure all the new plugins and hooks get picked up.'),
  );

  $default_values = variable_get('info_plugins_enabled_types', array());
  $form['plugin_types'] = array(
    '#type' => 'checkboxes',
    '#options' => $options,
    '#title' => t('Plugin types'),
    '#description' => t('Which plugin types would you like to enable?'),
    '#default_value' => $default_values,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save')
  );
  return $form;
}

/**
 * Submit callback for the info_plugins_admin_form().
 *
 * @param array $form
 * @param array $form_state
 */
function info_plugins_admin_form_submit($form, &$form_state) {
  $types = $form_state['values']['plugin_types'];
  $contrib = unserialize($form_state['values']['contrib']);
  $enabled = array_filter($types, function($type) {
    return $type !== 0;
  });

  // Remove from the list of contrib plugin types all the ones that have not been
  // checked.
  foreach ($contrib as $module => &$types) {
    $types = array_filter($types, function ($name) use ($enabled) {
      return isset($enabled[$name]);
    });

    if (!$types) {
      unset($contrib[$module]);
    }
  }

  variable_set('info_plugins_enabled_types', $enabled);
  variable_set('info_plugins_contrib_types', $contrib);
  // Need to clear all the caches because of the various hooks that are cached.
  drupal_flush_all_caches();
  drupal_set_message(t('Your options have been saved.'));
}