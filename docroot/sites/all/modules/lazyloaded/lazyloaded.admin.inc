<?php

/**
 * Form callback for settings form.
 */
function lazyloaded_settings_form(&$form_state) {
  $form = array();

  $plugins = lazyloaded_get_plugins();

  foreach ($plugins as $name => $plugin) {
    $form["{$name}_types"] = array(
      '#type' => 'fieldset',
      '#title' => t($plugin->title),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );

    $info = entity_get_info('node');
    $options = array();
    foreach ($info['bundles'] as $type => $bundle) {
      $options[$type] = $bundle['label'];
    }

    $form["{$name}_types"]["lazyloaded_{$name}_types"] = array(
      '#type' => 'checkboxes',
      '#title' => t('Nodetypes'),
      '#options' => $options,
      '#default_value' => variable_get("lazyloaded_{$name}_types", array()),
    );
  }

  return system_settings_form($form);
}
