<?php

/**
 * @file
 *   Administrative pages for the Tutorials module.
 */

function tutorials_settings() {
  $form = array();

  $form['servers'] = array(
    '#type' => 'fieldset',
    '#title' => t('Servers'),
    '#description' => t('Set servers for tutorial sources.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['servers']['tutorials_server_tutr'] = array(
    '#type' => 'checkbox',
    '#title' => t('Tutr.tv'),
    '#default_value' => variable_get('tutorials_server_tutr', TUTORIALS_SERVER_TUTR_DEFAULT),
    '#description' => t('Include tutorials from ') . l(t('Tutr.tv'), 'http://tutr.tv'),
  );
  if(module_exists('tutorial')) {
    $form['servers']['tutorials_server_self'] = array(
      '#type' => 'checkbox',
      '#title' => t('Self'),
      '#default_value' => variable_get('tutorials_server_self', TUTORIALS_SERVER_SELF_DEFAULT),
      '#description' => t('Include tutorials from ') . 'http://' . $_SERVER['SERVER_NAME'] . (($_SERVER['SERVER_PORT']) ? ':' . $_SERVER['SERVER_PORT'] : '') . base_path(),
    );
  }
  $form['servers']['tutorials_server_custom'] = array(
    '#type' => 'textfield',
    '#title' => t('Custom'),
    '#default_value' => variable_get('tutorials_server_custom', TUTORIALS_SERVER_CUSTOM_DEFAULT),
    '#description' => t('AJAX callback for getting the list of tutorials. Include trailing slash.'),
  );
  
  $form['selectors'] = array(
    '#type' => 'fieldset',
    '#title' => t('Selectors'),
    '#description' => t('Sets the default target selectors to attach the tutorials context menu.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['selectors']['tutorials_selector_title_wrap'] = array(
    '#type' => 'checkbox',
    '#title' => t('Wrap title'),
    '#default_value' => variable_get('tutorials_selector_title_wrap', TUTORIALS_SELECTOR_TITLE_WRAP_DEFAULT),
    '#description' => t('When enabled this will wrap page titles with a <code>&lt;span id="tutorials-menu"&gt;</code> tag. This provides a anchor for the tutorials menu independent of your theme tags. Only page titles where the user has permissions to view tutorials will be wrapped.'),
  );  
  
  
  $form['selectors']['tutorials_default_selector'] = array(
    '#type' => 'textfield',
    '#title' => t('Default tutorial selector'),
    '#default_value' => variable_get('tutorials_default_selector', TUTORIALS_DEFAULT_SELECTOR_DEFAULT),
    '#description' => t('Attach links to this jquery selector for any page wide videos. If using the Wrap title option, set selector to #tutorial_menu'),
    '#required' => TRUE,
  );
  
  $targets = array(
    'popup' => 'Javascript popup',
    'new_window' => 'New Window',
  );
  $form['tutorials_target'] = array(
    '#type' => 'select',
    '#title' => t('Location of video popups'),
    '#default_value' => variable_get('tutorials_target', TUTORIALS_TARGET_DEFAULT),
    '#options' => $targets,
    '#description' => t('Select where video links will open.'),
  );

  return system_settings_form($form);
}
