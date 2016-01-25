<?php
/**
 * @file
 * To configure user popup.
 *
 */

/**
 * Implements menu callback.
 */
function user_popup_info_config($form, &$form_state) {
  $form['user_popup_info_auto_detect'] = array(
    '#title' => t('How should user links be detected'),
    '#type' => 'radios',
    '#options' => array('Manually', 'Automatically'),
    '#description' => t('<i>Manually</i> : To manually detect user links, add class name <i>user-popup</i>.<br><i>Automatically</i> : Detecting links automatically may overload the server with AJAX requests.<br>Therefore, It is advisable to use this option only in case you do not have any server issues.'),
    '#default_value' => variable_get('user_popup_info_auto_detect', '1'),
  );
  $form['user_popup_info_pic_width'] = array(
    '#title' => t('User picture width'),
    '#type' => 'textfield',
    '#description' => t('Input user picture\'s width value. Enter only numerical value.'),
    '#default_value' => variable_get('user_popup_info_pic_width', '80'),
  );
  $form['user_popup_info_pic_height'] = array(
    '#title' => t('User picture height'),
    '#type' => 'textfield',
    '#description' => t('Input user picture\'s height value. Enter only numerical value.'),
    '#default_value' => variable_get('user_popup_info_pic_height', '80'),
  );
  $form['user_popup_info_alias_name'] = array(
    '#title' => t('User alias name (to support user aliases)'),
    '#type' => 'textfield',
    '#description' => t('Just mention what is between <i>base URL</i> and <i>username</i>, that is, <br>- if user alias is like <i>http://www.example.com/<strong>user/</strong>username</i>, then mention <strong>user/</strong> above, <br>- if user alias is like <i>http://www.example.com/<strong>users/</strong>username</i>, then mention <strong>users/</strong> above,<br>- if user alias is like <i>http://www.example.com/<strong>sales/person/</strong>username</i>, then mention <strong>sales/person/</strong> above.'),
    '#default_value' => variable_get('user_popup_info_alias_name', 'users/'),
  );
  $form['user_popup_info_exclude_user_links'] = array(
    '#title' => t('Links that should be excluded'),
    '#type' => 'textarea',
    '#description' => t('Enter relative paths, one path per line or comma separated with a leading slash "/".'),
    '#default_value' => variable_get('user_popup_info_exclude_user_links', USER_POPUP_INFO_EXCLUDED_LINKS),
  );

  // Get user fields.
  $user_fields = field_info_instances('user', 'user');
  if (!empty($user_fields)) {
    foreach ($user_fields as $fname => $fval) {
      $fields[$fval['field_name']] = ucfirst($fval['label']);
    }

    $form['user_popup_info_select_fields'] = array(
      '#title' => t('Additional fields that must be included in mini profile popup'),
      '#type' => 'checkboxes',
      '#description' => t('These fields were detected on ' . l("user account", "admin/config/people/accounts/fields") . '. You may choose to show this information on the mini profile popup.'),
      '#options' => $fields,
      '#default_value' => variable_get('user_popup_info_select_fields', array()),
    );
  }
  else {

    // Force removal of selected field variable, in case fields are deleted.
    variable_del('user_popup_info_select_fields');

    $form['user_popup_info_select_fields'] = array(
      '#title' => t('Additional fields that must be included in mini profile popup'),
      '#type' => 'item',
      '#value' => t('No user fields found.'),
      '#description' => t('<i>No field found.</i> These fields will be detected on ' . l("user account", "admin/config/people/accounts/fields") . '. You may choose to show this additional information on the mini profile popup.'),
    );
  }

  // Display default user info or not.
  $form['user_popup_info_show_default_user_info'] = array(
    '#title' => t('Show default user info'),
    '#type' => 'radios',
    '#description' => t('Tick yes to show the default user info like : <strong>Status</strong> and <strong>Member Since</strong>.'),
    '#options' => array('yes' => t('Yes'), 'no' => t('No')),
    '#default_value' => variable_get('user_popup_info_show_default_user_info', 'yes'),
  );
  return system_settings_form($form);
}