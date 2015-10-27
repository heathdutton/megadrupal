<?php

/**
 * @file
 *  Administration settings.
 */

/**
 * Page callback. FB Reminder administration page.
 */
function fb_reminder_admin_page() {
  $form = drupal_get_form('fb_reminder_admin_settings_form');
  return $form;
}

/**
 * Building settings form.
 */
function fb_reminder_admin_settings_form($form, &$form_state) {
  //Name settings
  $visibility = 'fb_reminder_template_visibility';
  $access_pages = 'fb_reminder_template_access_pages';
  $roles = 'fb_reminder_template_roles';
  $message = 'fb_reminder_template_message';

  // Message settings.
  $form[$message] = array(
    '#type' => 'textarea',
    '#title' => t('Message'),
    '#default_value' => variable_get($message),
    '#rows' => 3,
    '#required' => TRUE,
    '#wysiwyg' => FALSE,
  );

  // Visibility settings.
  $form['visibility_title'] = array(
    '#type' => 'item',
    '#title' => t('Visibility settings'),
  );
  $form['visibility'] = array(
    '#type' => 'vertical_tabs',
  );

  // Path group
  $form['visibility']['path'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Pages'),
  );
  $form['visibility']['path'][$visibility] = array(
    '#type' => 'radios',
    '#title' => t('Show form on specific pages'),
    '#default_value' => variable_get($visibility, 0),
    '#options' => array(
      t('Show on every page except the listed pages.'),
      t('Show on only the listed pages.')
    ),
  );

  $form['visibility']['path'][$access_pages] = array(
    '#type' => 'textarea',
    '#default_value' => variable_get($access_pages),
    '#rows' => 3,
    '#description' => t("Enter one page per line as Drupal paths. The '*' character is a wildcard. Example paths are  %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>')),
    '#wysiwyg' => FALSE,
  );
  // if php_filter is enable
  $access = user_access('use PHP for settings');
  if ($form['visibility']['path'][$visibility]['#default_value'] == FB_REMINDER_FORM_PHP_FILTER && !$access) {
    $form['visibility']['path'][$visibility] = array(
      '#type' => 'value',
      '#value' => FB_REMINDER_FORM_PHP_FILTER
    );
    $form['visibility']['path'][$access_pages] = array(
      '#type' => 'value',
      '#value' => $form['visibility']['path'][$access_pages]['#default_value'],
    );
  }
  elseif ($access && module_exists('php')) {
    $form['visibility']['path'][$visibility]['#options'][] =
      t('Show if the following PHP code returns <code>TRUE</code> (PHP-mode, experts only).');
    $form['visibility']['path'][$access_pages]['#description'] .=
      ' ' . t('If the PHP-mode is chosen, enter PHP code between %php. Note that executing incorrect PHP-code can break your Drupal site.', array('%php' => '<?php ?>'));
  }

  // Role group
  $role_options = array_map('check_plain', user_roles());
  $form['visibility']['role'] = array(
    '#type' => 'fieldset',
    '#title' => t('Roles'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#group' => 'visibility',
    '#weight' => 10,
  );
  $form['visibility']['role'][$roles] = array(
    '#type' => 'checkboxes',
    '#title' => t('Show block for specific roles'),
    '#options' => $role_options,
    '#description' => t('Show this form only for the selected role(s). If you select no roles, the block will be visible to all users.'),
    '#default_value' => variable_get($roles, array()),
  );

  return system_settings_form($form);
}
