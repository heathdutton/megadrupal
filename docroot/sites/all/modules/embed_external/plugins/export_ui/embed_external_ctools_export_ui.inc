<?php

$plugin = array(
  'schema' => 'embed_external_pluginset',
  'access' => 'administer embeddable services',
  'menu' => array(
    'menu item' => 'embed_external',
    'menu title' => 'Embed External Pluginsets',
    'menu description' => 'Administer embed_external pluginsets',
  ),
  'title singular' => t('pluginset'),
  'title plural' => t('pluginsets'),
  'title singular proper' => t('Pluginset'),
  'title plural proper' => t('Pluginsets'),
  'form' => array(
    'settings' => 'embed_external_ctools_export_ui_form',
  ),
);

/**
 * Implements hook_ctools_export_ui_form().
 */
function embed_external_ctools_export_ui_form(&$form, &$form_state) {
  $pluginset = $form_state['item'];
  $enabled_plugins = array();
  if (isset($pluginset->enabled_plugins)) {
    $enabled_plugins = drupal_map_assoc(unserialize($pluginset->enabled_plugins));
  }

  $plugins = embed_external_get_embeddables();

  $form['enabled_plugins'] = array(
    '#type' => 'vertical_tabs',
    '#tree' => TRUE,
  );

  foreach ($plugins as $machine_name => $plugin) {
    $category_machine_name = preg_replace('@[^a-z0-9_]+@', '_', strtolower($plugin['category']));

    if (!isset($form['enabled_plugins'][$category_machine_name])) {
      $form['enabled_plugins'][$category_machine_name] = array(
        '#type' => 'fieldset',
        '#title' => $plugin['category'],
      );
    }

    $form['enabled_plugins'][$category_machine_name][$machine_name] = array(
      '#type' => 'checkbox',
      '#title' => $plugin['title'],
      '#default_value' => isset($enabled_plugins[$machine_name]) ? TRUE : FALSE,
      '#description' => $plugin['description'],
    );
  }
}

/**
 * Implements hook_ctools_export_ui_form_validate().
 */
function embed_external_ctools_export_ui_form_validate(&$form, &$form_state) {
  $available_plugins = embed_external_get_embeddables();
  $enabled_plugins = array();

  foreach ($form_state['values']['enabled_plugins'] as $category) {
    // There's an enabled_plugins__active_tab key added to the array, so skip that.
    if (is_array($category)) {
      foreach ($category as $plugin_name => $status) {
        // If ctools doesn't know about the plugin, set an error.
        // @todo: Should this be more descriptive?
        if (!isset($available_plugins[$plugin_name])) {
          form_set_error('', 'Invalid plugin type specified');
        }
        else {
          // If we know about the plugin and the box is checked, add it to the
          // enabled plugins list.
          if ($status == 1) {
            $enabled_plugins[] = $plugin_name;
          }
        }
      }
    }
  }

  // If we don't have any errors, then serialize our data and let it be saved.
  $errors = form_get_errors();
  if (empty($errors)) {
    $form_state['values']['enabled_plugins'] = serialize($enabled_plugins);
  }
}