<?php

/**
 * @file
 * alias_preview.admin.inc
 */

/**
 * Form builder; returns module settings form for Alias Preview module.
 */
function alias_preview_settings() {
  $form = array();

  $settings = array(
    'new' => t('When creating new nodes'),
    'existing' => t('When editing existing nodes'),
  );

  foreach ($settings as $state => $title) {
    $group = "settings_{$state}";
    $form[$group] = array(
      '#type' => 'fieldset',
      '#title' => $title,
      '#weight' => 2,
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
    $name = "alias_preview_enable_ajax_{$state}";
    $default = ($state == 'new') ? 1 : 0;
    $description = t('Select to enable AJAX updating while editing titles');
    $form[$group][$name] = array(
      '#type' => 'checkbox',
      '#title' => t('Enable live AJAX updates'),
      '#default_value' => variable_get($name, $default),
      '#description' => $description,
    );

    $name = "alias_preview_show_jump_link_{$state}";
    $default = 1;
    $form[$group][$name] = array(
      '#type' => 'checkbox',
      '#title' => t('Show "edit" link after preview'),
      '#default_value' => variable_get($name, $default),
      '#description' => 'Allows users to easily jump to the URL alias settings',
    );

    // Adds the node type setting in a collapsed fieldset.
    $name = "alias_preview_enabled_types_{$state}";
    $default = array();
    $form[$group]['types'] = array(
      '#type' => 'fieldset',
      '#title' => t('Node types'),
      '#weight' => 2,
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form[$group]['types'][$name] = array(
      '#type' => 'checkboxes',
      '#title' => t('Enabled types'),
      '#options' => node_type_get_names(),
      '#default_value' => variable_get($name, $default),
      '#description' => t('Select which node types should display an alias preview below the title field.'),
    );
  }

  $prefix = t('<p>The <strong>Alias Preview</strong> module may change the '
    . 'behavior of node forms in 2 ways:<ul><li>Adds a "URL alias" preview '
    . 'directly below the title field that updates when the value changes.</li>'
    . '<li>Any form on which this preview displays may also provide an "edit" '
    . 'link for users to easily find the URL alias settings in the node form\'s '
    . 'Additional settings area.</li></ul><p>Additionally, you may set which '
    . 'node types should display these behaviors.');
  $form['settings_new']['#prefix'] = $prefix;

  return system_settings_form($form);
}
