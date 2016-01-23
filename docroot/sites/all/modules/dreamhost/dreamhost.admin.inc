<?php

/**
 * @file
 * Administration forms for the Dreamhost module.
 */

/**
 * Menu callback; Displays the administration settings for Disqus.
 */
function dreamhost_admin_settings() {
  $form['dreamhost_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Key'),
    '#description' => t('The API Key found from the <a href="@dreamhost">Dreamhost Web Panel API</a>.', array('@dreamhost' => 'https://panel.dreamhost.com/index.cgi?tree=home.api')),
    '#default_value' => variable_get('dreamhost_key', ''),
  );

  // Provide a list of the available commands.
  $form['commands'] = array(
    '#type' => 'fieldset',
    '#title' => t('Available Commands'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => '',
  );
  $commands = dreamhost('api-list_accessible_cmds');
  if (!empty($commands)) {
    $cmds = array();
    foreach ($commands as $command) {
      $cmds[] = '<code>' . $command['cmd'] . '</code>';
    }
    $form['commands']['#description'] = implode(', ', $cmds);
  }
  else {
    $form['commands']['#description'] = t('There are no commands accessible from Dreamhost with the given key.');
  }

  return system_settings_form($form);
}
