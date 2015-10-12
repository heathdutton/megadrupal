<?php

/**
 * @file
 * Administration functionalities of Disable Context Menu.
 */

/**
 * Configuration form for the Disable Context Menu module.
 */
function disable_context_menu_settings($form, &$form_state) {
  $form['disable_context_menu_selectors'] = array(
    '#title' => t('Selectors'),
    '#type' => 'textarea',
    '#description' => t('Choose here what objects to block from context menu. Use CSS Selector syntax.'),
    '#default_value' => variable_get('disable_context_menu_selectors', 'img, audio, video'),
    '#required' => TRUE,
  );

  $form['disable_context_menu_message'] = array(
    '#type' => 'textfield',
    '#title' => t('Block message'),
    '#default_value' => variable_get('disable_context_menu_message'),
    '#description' => t('Here you can enter a message that pops up when the user tries to use the context menu. Like a copyright notice or simple insult. Leave empty for no message.'),
  );

  if (user_access('bypass disabled context menu')) {
    $module_permissions = disable_context_menu_permission();
    $string_options = array(
      '%permission_name' => $module_permissions['bypass disabled context menu']['title'],
      '!permission_link' => url('admin/people/permissions', array('fragment' => 'module-disable_context_menu')),
    );

    $form['bypass_message'] = array(
      '#markup' => '<p>' . t('Please note that <em>you</em> will still be able the use your context menu because you have the permission <a href="!permission_link">%permission_name</a>.', $string_options) . '</p>',
    );
  }
  return system_settings_form($form);
}
