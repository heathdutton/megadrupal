<?php

$plugin = array(
  'schema' => 'entity_popup_settings',
  'access' => 'administer entity popup',
  'allowed operations' => array(
    'clone' => FALSE,
  ),
  'menu' => array(
    'menu prefix' => 'admin/config/user-interface',
    'menu item' => 'entity-popup',
    'menu title' => 'Entity Popup ',
    'menu description' => 'Entity Popup settings.',
    'items' => array(
      'add' => array(
        'access callback' => FALSE,
        'access arguments' => array(),
      ),
    ),
  ),
  'title singular' => t('Entity Popup setting'),
  'title singular proper' => t('Entity Popup setting'),
  'title plural' => t('Entity Popup settings'),
  'title plural proper' => t('Entity Popup settings'),
  'handler' => array(
    'class' => 'entity_popup_export_ui'
  )
);

/**
 * Form callback.
 */
function entity_popup_settings_form(&$form, &$form_state) {
  $settings = $form_state['item'];
  $entity_info = entity_get_info($settings->entity_type);
  foreach ($entity_info['view modes'] as $view_mode => $view_mode_info) {
    $view_modes[$view_mode] = $view_mode_info['label'];
  }
  if (!isset($view_modes['full']) && !isset($view_modes['default'])) {
    $view_modes = array('default' => t('Default')) + $view_modes;
  }
  $form['entity_type'] = array(
    '#type' => 'value',
    '#value' => $settings->entity_type,
  );
  $form['bundle'] = array(
    '#type' => 'value',
    '#value' => $settings->bundle,
  );
  $form['view_mode'] = array(
    '#type' => 'select',
    '#title' => t('View mode'),
    '#description' => t('The view mode to render the entity in when displayed inside a popup. Projects like !ds and !evm allow you to add custom view modes.', array('!ds' => l(t('Display Suite'), 'https://www.drupal.org/project/ds'), '!evm' => l(t('Entity View Modes'), 'https://www.drupal.org/project/entity_view_mode'))),
    '#options' => $view_modes,
    '#default_value' => $settings->view_mode,
  );
  return $form;
}
