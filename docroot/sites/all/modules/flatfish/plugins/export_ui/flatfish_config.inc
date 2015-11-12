<?php
$plugin = array(
  'schema' => 'flatfish_config',
  'access' => 'access content', //TODO UPDATE
  'menu' => array(
    'menu item' => 'flatfish',
    'menu title' => 'Flatfish',
    'menu description' => 'Administer Flatfish configurations.',
  ),

  'title singular' => t('configuration'),
  'title plural' => t('configurations'),
  'title singular proper' => t('Flatfish configuration'),
  'title plural proper' => t('Flatfish configurations'),

  'form' => array(
    'settings' => 'flatfish_ctools_export_ui_form',
    'validate' => 'flatfish_ctools_export_ui_form_validate',
    'submit' => 'flatfish_ctools_export_ui_form_submit',
  ),

  'handler' => 'flatfish_config',
);

function flatfish_ctools_export_ui_form(&$form, &$form_state) {
  $config = $form_state['item'];

  $form += array(
    'description' => array(
      '#type' => 'textfield',
      '#title' => t('Description'),
      '#default_value' => $config->description,
    ),
    'group_name' => array(
      '#type' => 'textfield',
      '#title' => t('Group'),
      '#default_value' => (isset($config->group_name))? $config->group_name: '',
    ),
  );
}

function flatfish_ctools_export_ui_form_validate(&$form, &$form_state) {
  foreach ($form_state['values']['options'] as $optgroup => $options) {
    $form_state['values']['options'][$optgroup] = array_filter($options);
  }
}

function flatfish_ctools_export_ui_form_submit(&$form, &$form_state) {
  $form_state['item']->config['components'] = $form_state['values']['options']['components'];
}
