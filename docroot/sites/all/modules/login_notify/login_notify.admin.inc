<?php

/**
 * @file
 * Contains menu callbacks for administrative pages.
 */

/**
 * Menu callback for admin/config/people/login-notify.
 *
 * Creates a system settings form for variables that control which users are
 * required to name their browser, and the text of the email that is sent to
 * users when a new browser accessed the site from their account.
 */
function login_notify_admin_form($form, &$form_state) {
  $form = array();

  $form['login_notify_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable login notification'),
    '#default_value' => variable_get('login_notify_enabled', 0),
    '#description' => t('This will require users with the below roles to register their browser by giving it a unique name. When a new browser is registered a notification email will be sent to the user.'),
  );

  // Exclude anonymous.
  $roles = array_values(user_roles(TRUE));
  sort($roles);
  $roles = drupal_map_assoc($roles);
  // Also exclude 'authenticated user'.
  unset($roles['authenticated user']);

  // The default setting for roles mode is 'exclude' and for roles is an empty
  // array, which will include all users with any role other than 'anonymous'
  // and 'authenticated user'.
  $form['roles'] = array(
    '#type' => 'fieldset',
    '#title' => t('Roles'),
    'login_notify_roles_mode' => array(
      '#type' => 'radios',
      '#title' => t('Roles which require login notification'),
      '#options' => array(
        'all' => t('All authenticated users'),
        'include' => t('Only the roles selected below'),
        'exclude' => t('All roles except those selected below'),
      ),
      '#default_value' => variable_get('login_notify_roles_mode', 'exclude'),
    ),
    'login_notify_roles' => array(
      '#type' => 'checkboxes',
      '#title' => t('Roles'),
      '#options' => drupal_map_assoc($roles),
      '#default_value' => variable_get('login_notify_roles', array('')),
      '#states' => array(
        'invisible' => array(
          ':input[name="login_notify_roles_exclude"]' => array('value' => 'all'),
        ),
      ),
    ),
  );

  $form['email_title'] = array(
    '#type' => 'item',
    '#title' => t('E-mails'),
  );
  $form['email'] = array(
    '#type' => 'vertical_tabs',
  );

  $form['email']['new'] = array(
    '#type' => 'fieldset',
    '#title' => t('New browser'),
    '#description' => t('Edit the notification sent when an unrecognized browser is registered.'),
    'login_notify_email_subject' => array(
      '#type' => 'textfield',
      '#title' => t('Subject'),
      '#required' => TRUE,
      '#default_value' => _login_notify_mail_text('login_notify_email_subject', NULL, FALSE),
    ),
    'login_notify_email_body' => array(
      '#type' => 'textarea',
      '#title' => t('Body'),
      '#required' => TRUE,
      '#default_value' => _login_notify_mail_text('login_notify_email_body', NULL, FALSE),
    ),
  );

  $form['email']['bypass_user_agent'] = array(
    '#type' => 'fieldset',
    '#title' => t('Unregistered browser'),
    '#description' => t('Edit the notification sent when an unregistered browser is allowed to access the site without being named.'),
    'login_notify_email_bypass_user_agent_subject' => array(
      '#type' => 'textfield',
      '#title' => t('Subject'),
      '#required' => TRUE,
      '#default_value' => _login_notify_mail_text('login_notify_email_bypass_user_agent_subject', NULL, FALSE),
    ),
    'login_notify_email_bypass_user_agent_body' => array(
      '#type' => 'textarea',
      '#title' => t('Body'),
      '#required' => TRUE,
      '#default_value' => _login_notify_mail_text('login_notify_email_bypass_user_agent_body', NULL, FALSE),
    ),
  );

  $form['email']['blocked'] = array(
    '#type' => 'fieldset',
    '#title' => t('Browser blocked'),
    '#description' => t('Edit the notification sent when a locked browser attempts to access the site.'),
    'login_notify_email_blocked_subject' => array(
      '#type' => 'textfield',
      '#title' => t('Subject'),
      '#required' => TRUE,
      '#default_value' => _login_notify_mail_text('login_notify_email_blocked_subject', NULL, FALSE),
    ),
    'login_notify_email_blocked_body' => array(
      '#type' => 'textarea',
      '#title' => t('Body'),
      '#required' => TRUE,
      '#default_value' => _login_notify_mail_text('login_notify_email_blocked_body', NULL, FALSE),
    ),
  );

  // @todo: Some way to display the available tokens if the token module isn't
  // installed.
  if (module_exists('token')) {
    // Add the token tree UI.
    $form['email']['token_tree'] = array(
      '#theme' => 'token_tree',
      '#token_types' => array('user', 'browser'),
      '#show_restricted' => TRUE,
      '#dialog' => TRUE,
      '#weight' => 90,
    );
  }

  return system_settings_form($form);
}
