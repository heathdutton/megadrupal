<?php
/**
 * @file
 * Admin settings form for vk_openapi module
 */

/**
 * Admin settings form.
 */
function vk_openapi_admin_settings() {
  global $base_url;

  $form['global'] = array(
    '#type' => 'fieldset',
    '#title' => t('vKontakte API settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['global']['vk_openapi_app_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Application ID'),
    '#description' => t('Enter your Application ID'),
    '#default_value' => variable_get('vk_openapi_app_id', ''),
  );

  $form['global']['vk_openapi_secret_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Secret key'),
    '#description' => t('Enter your secret key'),
    '#default_value' => variable_get('vk_openapi_secret_key', ''),
  );

  $form['global']['vk_openapi_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Site URL'),
    '#default_value' => variable_get('vk_openapi_url', $base_url . base_path()),
  );

  $form['global']['vk_openapi_base_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Base URL'),
    '#default_value' => variable_get('vk_openapi_base_url', $base_url . base_path()),
  );

  $form['global']['vk_openapi_redirect_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Redirect URL'),
    '#default_value' => variable_get('vk_openapi_redirect_url', ""),
  );

  $form['global']['vk_openapi_alter_login_form'] = array(
    '#type' => 'select',
    '#title' => t('Show vKontakte button on login form'),
    '#default_value' => variable_get('vk_openapi_alter_login_form', 1),
    '#options' => array(
      '0' => t('Do not add vKontakte button to login form'),
      '1' => t('Add vKontakte button to login form'),
    ),
  );

  $form['global']['vk_openapi_update_freq'] = array(
    '#type' => 'select',
    '#title' => t('Frequency of user data refresh'),
    '#default_value' => variable_get('vk_openapi_update_freq', 60),
    '#options' => array(
      60 => t('1 minute'),
      300 => t('5 minutes'),
      900 => t('15 minutes'),
      3600 => t('1 hour'),
    ),
  );

  $form['global']['vk_openapi_add_js'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add VKontakte js to all pages'),
    '#description' => t('Mark this checkbox if you have the intention to use VKontekte widgets.'),
    '#default_value' => variable_get('vk_openapi_add_js', 0),
  );

  $list_themes = array(1 => t('all'));
  $themes = list_themes();
  foreach ($themes as $theme_name => $theme) {
    $list_themes[$theme_name] = $theme->info['name'];
  }

  $form['global']['vk_openapi_themes_list'] = array(
    '#type' => 'select',
    '#title' => t('Show button in listed themes'),
    '#default_value' => variable_get('vk_openapi_themes_list', 1),
    '#options' => $list_themes,
    '#multiple' => TRUE,
  );

  $form['new_user_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('New user settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['new_user_settings']['vk_openapi_username'] = array(
    '#type' => 'textfield',
    '#title' => t('Mask for new user name.'),
    '#description' => t('You may use following tokens: %first_name, %last_name, %nickname (and %nickname_lq, %nickname_rq for quotes).'),
    '#default_value' => variable_get('vk_openapi_username', '%first_name %nickname_lq%nickname%nickname_rq %last_name'),
  );

  $form['new_user_settings']['vk_openapi_nickname_lq'] = array(
    '#type' => 'textfield',
    '#title' => t('Left quote for nickname.'),
    '#default_value' => variable_get('vk_openapi_nickname_lq', '«'),
  );

  $form['new_user_settings']['vk_openapi_nickname_rq'] = array(
    '#type' => 'textfield',
    '#title' => t('Right quote for nickname.'),
    '#default_value' => variable_get('vk_openapi_nickname_rq', '»'),
  );

  $form['new_user_settings']['vk_openapi_allow_users_unite_accounts'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow users unite accounts.'),
    '#description' => t('Allow users unite own vkontakte account account on Drupal-site.'),
    '#default_value' => variable_get('vk_openapi_allow_users_unite_accounts', 1),
  );

  $form['new_user_settings']['vk_openapi_oblige_users_to_enter_email'] = array(
    '#type' => 'checkbox',
    '#title' => t('Oblige users to enter e-mail.'),
    '#description' => t('If checked then user will receive message with request to enter e-mail.'),
    '#default_value' => variable_get('vk_openapi_oblige_users_to_enter_email', 0),
  );

  $disabled = TRUE;
  if (variable_get('user_pictures', 0)) {
    $disabled = FALSE;
  }
  $form['new_user_settings']['vk_openapi_avatar'] = array(
    '#type' => 'select',
    '#title' => t('Use vkontakte avatar'),
    '#default_value' => variable_get('vk_openapi_avatar', 0),
    '#options' => array(
      '0' => t('Don\'t use'),
      '1' => t('Use small avatar'),
      '2' => t('Use medium avatar'),
      '3' => t('Use big avatar'),
    ),
    '#disabled' => $disabled,
    '#description' => t('You should enable pictures support on <a href="@usp">user settings page</a>.', array('@usp' => base_path() . 'admin/config/people/accounts#edit-personalization')),
  );

  $roles = user_roles();
  $roles[0] = t('None');

  $form['new_user_settings']['vk_openapi_role_for_new_user'] = array(
    '#type' => 'select',
    '#title' => t('Role for new vkontakte user'),
    '#default_value' => variable_get('vk_openapi_role_for_new_user', 0),
    '#options' => $roles,
  );

  if (variable_get('vk_openapi_avatar', 0) && !function_exists('curl_init')) {
    variable_set('vk_openapi_avatar', 0);
    drupal_set_message(t('You can\'t use vkontakte avatars because  your PHP don\'t support <a href="http://ru2.php.net/curl">CURL functions</a>.'), 'error');
    drupal_goto('admin/settings/vk_openapi');
  }

  $form['new_user_settings']['vk_openapi_confirm_time'] = array(
    '#type' => 'select',
    '#title' => t('Time for e-mail confirmation'),
    '#default_value' => variable_get('vk_openapi_confirm_time', 86400),
    '#options' => array(
      86400 => t('1 day'),
      604800 => t('7 days'),
      2592000 => t('1 month'),
      0 => t('Never'),
    ),
  );

  return system_settings_form($form);
}
