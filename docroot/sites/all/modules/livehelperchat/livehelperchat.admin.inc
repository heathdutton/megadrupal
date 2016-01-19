<?php

/**
 * @file
 * Administrative page callbacks for the livehelperchat module.
 */

/**
 * Implementation of hook_admin_settings() for configuring the module
 *
 * @param array $form_state
 *   structure associative drupal form array
 * @return array
 */
function livehelperchat_admin_settings_form() {
  $form['livehelperchat'] = array(
    '#type' => 'vertical_tabs',
  );

  $form['account'] = array(
    '#type' => 'fieldset',
    '#title' => t('General settings'),
    '#collapsible' => FALSE,
    '#group' => 'livehelperchat',
  );
  $form['account']['livehelperchat_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Live Helper Chat base URL'),
    '#default_value' => variable_get('livehelperchat_url', ''),
    '#size' => 40,
    '#maxlength' => 40,
    '#required' => TRUE,
    '#description' => 'The URL should not contain protocol or the tailing slash. Ie. for http://chat.example.org/lhc/ use chat.example.org/lhc',
  );

  $form['widget'] = array(
    '#type' => 'fieldset',
    '#title' => t('Widget settings'),
    '#collapsible' => FALSE,
    '#group' => 'livehelperchat',
  );
  $form['widget']['livehelperchat_widget_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Widget height'),
    '#default_value' => variable_get('livehelperchat_widget_height', '340'),
  );
  $form['widget']['livehelperchat_widget_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Widget width'),
    '#default_value' => variable_get('livehelperchat_widget_width', '300'),
  );
  $form['widget']['livehelperchat_popup_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Popup height'),
    '#default_value' => variable_get('livehelperchat_popup_height', '520'),
  );
  $form['widget']['livehelperchat_popup_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Popup width'),
    '#default_value' => variable_get('livehelperchat_popup_width', '500'),
  );
  $form['widget']['livehelperchat_position_fromtop'] = array(
    '#type' => 'textfield',
    '#title' => t('Position from top'),
    '#description' => t('Only used if position Middle left or the Middle right side is chosen'),
    '#default_value' => variable_get('livehelperchat_position_fromtop', '500'),
  );
  $form['widget']['livehelperchat_position_unit'] = array(
    '#type' => 'select',
    '#title' => t('Position from top unit'),
    '#description' => t('Only used if position Middle left or the Middle right side is chosen'),
    '#default_value' => variable_get('livehelperchat_position_unit', ''),
    '#options' => array(
      'pixels' => t('Pixels'),
      'percents' => t('Percents'),
    ),
  );
  $form['widget']['livehelperchat_click_internal'] = array(
    '#type' => 'checkbox',
    '#title' => t('On a mouse click show the page widget'),
    '#default_value' => variable_get('livehelperchat_click_internal', FALSE),
  );
  $form['widget']['livehelperchat_check_messages'] = array(
    '#type' => 'checkbox',
    '#title' => t('Check automatically for messages from the operator'),
    '#default_value' => variable_get('livehelperchat_check_messages', FALSE),
  );
  $form['widget']['livehelperchat_disable_proactive'] = array(
    '#type' => 'checkbox',
    '#title' => t('Disable pro active invitations'),
    '#default_value' => variable_get('livehelperchat_disable_proactive', FALSE),
  );
  $form['widget']['livehelperchat_hide_offline'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide when operator is offline'),
    '#default_value' => variable_get('livehelperchat_hide_offline', TRUE),
  );
  $form['widget']['livehelperchat_offline_leavemessage'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show a leave message form when operator is offline'),
    '#default_value' => variable_get('livehelperchat_offline_leavemessage', FALSE),
  );
  $form['widget']['livehelperchat_nonresponsive_widget'] = array(
    '#type' => 'checkbox',
    '#title' => t('Disable responsive layout of the status widget'),
    '#default_value' => variable_get('livehelperchat_nonresponsive_widget', FALSE),
  );
  $form['widget']['livehelperchat_department'] = array(
    '#type' => 'textfield',
    '#title' => t('Department ID'),
    '#default_value' => variable_get('livehelperchat_department_id', ''),
  );
  $form['widget']['livehelperchat_identifier'] = array(
    '#type' => 'textfield',
    '#title' => t('Chat identifier'),
    '#default_value' => variable_get('livehelperchat_identifier', ''),
  );
  $form['widget']['livehelperchat_minimize_action'] = array(
    '#type' => 'select',
    '#title' => t('Minimize action'),
    '#description' => t('Applies only if status widget is at the bottom'),
    '#options' => array(
      'keep' => t('Keep where it was'),
      'minimize' => t('Minimize to the bottom of the screen'),
    ),
  );
  $form['widget']['livehelperchat_position'] = array(
    '#type' => 'select',
    '#title' => t('Position'),
    '#options' => array(
//      'original' => t('Native placement - it will be shown where the html is embedded'),
      'bottom_right' => t('Bottom right corner of the screen'),
      'bottom_left' => t('Bottom left corner of the screen'),
      'middle_right' => t('Middle right side of the screen'),
      'middle_left' => t('Middle left side of the screen'),
      'api' => t('Invisible, only JS API will be included'),
    ),
  );

  $form['role_vis_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Role specific script settings'),
    '#collapsible' => TRUE,
    '#group' => 'livehelperchat',
  );

  $roles = user_roles();
  $role_options = array();
  foreach ($roles as $rid => $name) {
    $role_options[$rid] = $name;
  }
  $form['role_vis_settings']['livehelperchat_roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Remove script for specific roles'),
    '#default_value' => variable_get('livehelperchat_roles', array()),
    '#options' => $role_options,
    '#description' => t('Remove script only for the selected role(s). If none of the roles are selected, all roles will have the script. Otherwise, any roles selected here will NOT have the script.'),
  );

  $form['page_vis_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Page specific script settings'),
    '#collapsible' => TRUE,
    '#group' => 'livehelperchat',
  );

  $access = user_access('use PHP for livehelperchat visibility');
  $visibility = variable_get('livehelperchat_visibility', 0);
  $pages = variable_get('livehelperchat_pages', '');

  if ($visibility == 2 && !$access) {
    $form['page_vis_settings'] = array();
    $form['page_vis_settings']['visibility'] = array('#type' => 'value', '#value' => 2);
    $form['page_vis_settings']['pages'] = array('#type' => 'value', '#value' => $pages);
  }
  else {
    $options = array(t('Add to every page except the listed pages.'), t('Add to the listed pages only.'));
    $description = t("Enter one page per line as Drupal paths. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>'));
    if ($access) {
      $options[] = t('Add if the following PHP code returns <code>TRUE</code> (PHP-mode, experts only).');
      $description .= ' '. t('If the PHP-mode is chosen, enter PHP code between %php tags. Note that executing incorrect PHP-code can break your Drupal site.', array('%php' => '<?php ?>'));
    }
    $form['page_vis_settings']['livehelperchat_visibility'] = array(
      '#type' => 'radios',
      '#title' => t('Add script to specific pages'),
      '#options' => $options,
      '#default_value' => $visibility,
    );
    $form['page_vis_settings']['livehelperchat_pages'] = array(
      '#type' => 'textarea',
      '#title' => t('Pages'),
      '#default_value' => $pages,
      '#description' => $description,
      '#wysiwyg' => FALSE,
    );
  }

  return system_settings_form($form);
}

/**
 * Implementation of hook_admin_settings_form_validate().
 *
 * @param array $form
 *   structured associative drupal form array.
 * @param array $form_state
 */
function livehelperchat_admin_settings_form_validate($form, &$form_state) {
  if (empty($form_state['values']['livehelperchat_url'])) {
    form_set_error('livehelperchat_url', t('A valid Live Helper Chat base URL is needed.'));
  }
}
