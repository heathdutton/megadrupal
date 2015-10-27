<?php

/**
 * @file
 * New registration forms. Public and admin.
 */

/**
 * Page callback for registration/%node_registration/toggle/ajax/%/%.
 */
function node_registration_toggle_flag($registration, $ajax, $field, $new_value) {
  $ajax = $ajax == 'ajax';

  $registration->$field = (int) (bool) $new_value;
  $registration->save();

  if ($ajax) {
    // What a mess, just to reload the page. CTools doesn't help here either.
    $commands = array(ajax_command_prepend('head', '<script>location.reload()</script>'));
    print drupal_json_encode($commands);
    exit;
  }

  drupal_goto();
}

/**
 * Admin: registration types form.
 *
 * This is a form, because a form can be altered by other modules
 * and then rendered/themed by this module. No buttons or handlers.
 */
function node_registration_types_form($form, $form_state) {
  $module_path = _node_registration_type_to_uri('node_registration');

  $form['#theme'] = 'node_registration_types_form';

  $form['types'] = array('#tree' => TRUE);
  foreach (node_type_get_names() as $type => $name) {
    $enabled = _node_registration_node_type_enabled($type);

    $operations = array();
    if ($enabled) {
      $operations['settings'] = array(
        '#title' => t('settings'),
        '#type' => 'link',
        '#href' => 'admin/structure/' . $module_path . '/manage/' . $type . '/settings',
      );
      $operations['fields'] = array(
        '#title' => t('fields'),
        '#type' => 'link',
        '#href' => 'admin/structure/' . $module_path . '/manage/' . $type . '/fields',
      );
      $operations['display'] = array(
        '#title' => t('display'),
        '#type' => 'link',
        '#href' => 'admin/structure/' . $module_path . '/manage/' . $type . '/display',
      );
    }

    $form['types'][$type] = array(
      '#name' => $name,
      '#enabled' => $enabled,
      '#operations' => $operations,
    );
  }

  return $form;
}

/**
 * Action mail form.
 */
function node_registration_send_mail_action_form($context, $form_state) {
  $values = $form_state['values'];
  $selection = array_filter($values['views_bulk_operations']);

  $form = array();

  $registrations = node_registration_load_multiple($selection);
  $form['#registrations'] = $registrations;

  $form['recipients'] = array(
    '#type' => 'item',
    '#title' => t('Recipients'),
    '#markup' => count($registrations),
  );

  $form['recipientfield'] = array(
    '#type' => 'textfield',
    '#title' => t('Recipient field (token)'),
    '#default_value' => '[node-registration:email]',
    '#required' => TRUE,
  );

  $form['bcc'] = array(
    '#type' => 'textfield',
    '#title' => t('Send BCC to'),
    '#element_validate' => array('value_is_email'),
    '#max' => 20,
  );

  $form['message'] = array(
    '#type' => 'fieldset',
    '#title' => t('Message'),
  );
  $form['message']['subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#required' => TRUE,
  );
  $form['message']['message'] = array(
    '#type' => 'textarea',
    '#title' => t('Message'),
    '#required' => TRUE,
    '#cols' => '80',
    '#rows' => '10',
  );

  /*$form['attachments'] = array(
    '#type' => 'file',
    '#title' => t('Attachments'),
    '#name' => 'attachments[]',
    '#attributes' => array('multiple' => ''),
  );*/

  $form['from'] = array(
    '#type' => 'fieldset',
    '#title' => t('From'),
  );
  $form['from']['from_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#required' => TRUE,
    '#default_value' => _node_registration_default_from_name(),
  );
  $form['from']['from_email'] = array(
    '#type' => 'textfield',
    '#title' => t('E-mail'),
    '#required' => TRUE,
    '#element_validate' => array('value_is_email'),
    '#default_value' => variable_get('site_mail'),
  );

  return $form;
}

/**
 * Submit handler for node_registration_send_mail_action() form.
 *
 * Collects and returns a context to be used in the actual action.
 *
 * @see node_registration_send_mail_action()
 */
function node_registration_send_mail_action_submit($form, &$form_state) {
  $values = $form_state['values'];

  unset($values['submit'], $values['form_build_id'], $values['form_token'], $values['form_id'], $values['op']);

  return $values;
}

/**
 * All registration settings for registration types and nodes.
 */
function _node_registration_type_settings_form($type, $settings, $node = NULL) {
  $strtotime_help = t('Use minutes, hours, days and weeks to provide a period. E.g.: <code>1 day 4 hours</code> or <code>50 hours</code>.');
  $relative_utc = $node ? $node->registration->site_utc() : REQUEST_TIME;

  $form['register_cancel'] = array(
    '#type' => 'fieldset',
    '#title' => t('Register & Cancel'),
    '#collapsible' => TRUE,
  );
  $form['register_cancel']['allow_registration_until'] = array(
    '#type' => 'textfield',
    '#title' => t('Allow registrations until'),
    '#description' => $strtotime_help . ' ' . _node_registration_strtotime_debug($settings->allow_registration_until, $relative_utc),
    '#default_value' => $settings->allow_registration_until,
    '#field_suffix' => ' ' . t('before'),
    '#element_validate' => array('_node_registration_value_is_strtotime'),
    '#weight' => -9,
  );
  $form['register_cancel']['registration_success_message'] = array(
    '#type' => 'textfield',
    '#title' => t('Registration success message'),
    '#default_value' => $settings->registration_success_message,
    '#description' => t('You can use HTML. URL for the new registration: @registration_url.'),
  );
  $form['register_cancel']['allow_cancellation_until'] = array(
    '#type' => 'textfield',
    '#title' => t('Allow cancellations until'),
    '#description' => $strtotime_help . ' ' . _node_registration_strtotime_debug($settings->allow_cancellation_until, $relative_utc),
    '#default_value' => $settings->allow_cancellation_until,
    '#field_suffix' => ' ' . t('before'),
    '#element_validate' => array('_node_registration_value_is_strtotime'),
  );

  $form['numbers'] = array(
    '#type' => 'fieldset',
    '#title' => t('Numbers'),
    '#collapsible' => TRUE,
  );
  $form['numbers']['capacity'] = array(
    '#type' => 'textfield',
    '#title' => t('Capacity'),
    '#size' => 6,
    '#description' => t('This many <strong>slots</strong> can be filled. One registration &gt;= 1 slot.'),
    '#default_value' => $settings->capacity,
    '#element_validate' => array('value_is_integer'),
    '#min' => 0,
    '#required' => TRUE,
  );
  if ($settings->capacity_field) {
    unset($form['numbers']['capacity']);
    $form['numbers']['capacity'] = array(
      '#type' => 'item',
      '#title' => t('Capacity'),
      '#markup' => is_callable(array($settings, 'capacity')) ? $settings->capacity() : '-',
      '#description' => t('This field is not editable and <strong>not used</strong>, because %field has been set as Capacity source field.', array('%field' => $settings->capacity_field)),
    );
  }
  $form['numbers']['capacity']['#description'] .= ' ' . t("Empty or <code>0</code> means capacity is unlimited, waiting list will never be used.");
  $form['numbers']['allow_exceeding_capacity'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow exceeding capacity?'),
    '#description' => t('Allows people to register even after capacity has been reached.'),
    '#default_value' => $settings->allow_exceeding_capacity,
  );
  $form['numbers']['max_slots_per_registration_authenticated'] = array(
    '#type' => 'textfield',
    '#title' => t('Max slots per authenticated registration'),
    '#size' => 6,
    '#description' => t('This many <strong>slots</strong> can be consumed by 1 registration by an <strong>authenticated</strong> user. Set to 0 to disallow authenticated users.'),
    '#default_value' => $settings->max_slots_per_registration_authenticated,
    '#element_validate' => array('value_is_integer'),
    '#min' => 0,
  );
  $form['numbers']['max_slots_per_registration_anonymous'] = array(
    '#type' => 'textfield',
    '#title' => t('Max slots per anonymous registration'),
    '#size' => 6,
    '#description' => t('This many <strong>slots</strong> can be consumed by 1 registration by an <strong>anonymous</strong> user. Set to 0 to disallow anonymous users.'),
    '#default_value' => $settings->max_slots_per_registration_anonymous,
    '#element_validate' => array('value_is_integer'),
    '#min' => 0,
  );
  $form['numbers']['allow_email_change'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow e-mail address change?'),
    '#description' => t('Allows signed in users to change their e-mail address via the registration interface/form.'),
    '#default_value' => $settings->allow_email_change,
  );
  $form['numbers']['require_email_verification'] = array(
    '#type' => 'checkbox',
    '#title' => t('Require e-mail address verification?'),
    '#description' => t('Forces a) authenticated users to verify their changed e-mail address OR b) anonymous users to verify their e-mail address.'),
    '#default_value' => FALSE && $settings->require_email_verification,
    '#disabled' => TRUE,
  );

  $accepts_tokens = ' ' . t('Accepts tokens.');

  $any_mail =
    $settings->send_mail_to_registree +
    $settings->send_mail_to_admin +
    $settings->send_reminder_mail_to_registrees +
    $settings->send_cancel_mail_to_registree +
    $settings->send_cancel_mail_to_admin +
    $settings->send_unwaitinglist_mail_to_registree;
  $form['email'] = array(
    '#type' => 'fieldset',
    '#title' => t('E-mail'),
    '#collapsible' => TRUE,
    '#collapsed' => !$any_mail,
  );
  $form['email']['sender_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Sender name'),
    '#default_value' => $settings->sender_name,
    '#description' => t('Defaults to the site name: %site_name.', array('%site_name' => _node_registration_default_from_name())),
  );
  $form['email']['sender_mail'] = array(
    '#type' => 'textfield',
    '#title' => t('Sender e-mail address'),
    '#default_value' => $settings->sender_mail,
    '#element_validate' => array('value_is_email_or_token'),
    '#description' => t('Defaults to the site e-mail: %site_email.', array('%site_email' => variable_get('site_mail'))) . $accepts_tokens,
  );

  if (module_exists('mimemail')) {
    $form['email']['mail_system'] = array(
      '#type' => 'radios',
      '#title' => t('Mail system'),
      '#default_value' => $settings->mail_system,
      '#required' => TRUE,
      '#options' => array(
        0 => t('Default Drupal mail system'),
        1 => t('MimeMail with attachments support'),
      ),
      '#description' => t('Defaults to the default Drupal mail system, if MimeMail is enabled it allows for sending attachments with mails (mimemail module is necessary)'),
    );
  }

  $form['email']['mail_to_registree'] = array(
    '#type' => 'fieldset',
    '#title' => t('Register e-mail to registree'),
    '#collapsible' => TRUE,
    '#collapsed' => !$settings->send_mail_to_registree,
  );
  $form['email']['mail_to_registree']['send_mail_to_registree'] = array(
    '#type' => 'checkbox',
    '#title' => t('Send e-mail to registree'),
    '#default_value' => $settings->send_mail_to_registree,
  );
  $form['email']['mail_to_registree']['mail_to_registree_bcc'] = array(
    '#type' => 'textfield',
    '#title' => t('Send BCC to'),
    '#default_value' => $settings->mail_to_registree_bcc,
    '#element_validate' => array('value_is_email_or_token'),
    '#max' => 20,
    '#description' => t('A (comma separated) list of e-mail addresses to send this notification to.') . $accepts_tokens,
  );
  $form['email']['mail_to_registree']['mail_to_registree_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#default_value' => $settings->mail_to_registree_subject,
  );
  $form['email']['mail_to_registree']['mail_to_registree_body'] = array(
    '#type' => 'textarea',
    '#title' => t('Body'),
    '#default_value' => $settings->mail_to_registree_body,
  );

  $form['email']['mail_to_registree']['mail_for_waiting_list_registree'] = array(
    '#type' => 'fieldset',
    '#title' => t('Change e-mail for waiting list registrees'),
    '#collapsible' => TRUE,
    '#collapsed' => !$settings->change_mail_for_waiting_list_registree,
  );
  $form['email']['mail_to_registree']['mail_for_waiting_list_registree']['change_mail_for_waiting_list_registree'] = array(
    '#type' => 'checkbox',
    '#title' => t('Yes, different e-mail for waiting list registrees'),
    '#default_value' => $settings->change_mail_for_waiting_list_registree,
  );
  $form['email']['mail_to_registree']['mail_for_waiting_list_registree']['mail_for_waiting_list_registree_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#default_value' => $settings->mail_for_waiting_list_registree_subject,
  );
  $form['email']['mail_to_registree']['mail_for_waiting_list_registree']['mail_for_waiting_list_registree_body'] = array(
    '#type' => 'textarea',
    '#title' => t('Body'),
    '#default_value' => $settings->mail_for_waiting_list_registree_body,
  );

  $form['email']['mail_to_admin'] = array(
    '#type' => 'fieldset',
    '#title' => t('Register e-mail to admin'),
    '#collapsible' => TRUE,
    '#collapsed' => !$settings->send_mail_to_admin,
  );
  $form['email']['mail_to_admin']['send_mail_to_admin'] = array(
    '#type' => 'checkbox',
    '#title' => t('Send e-mail to admin'),
    '#default_value' => $settings->send_mail_to_admin,
  );
  $form['email']['mail_to_admin']['mail_to_admin_recipients'] = array(
    '#type' => 'textfield',
    '#title' => t('Recipients'),
    '#default_value' => $settings->mail_to_admin_recipients,
    '#element_validate' => array('value_is_email_or_token'),
    '#max' => 20,
    '#description' => t('A (comma separated) list of e-mail addresses to send this notification to.') . $accepts_tokens,
  );
  $form['email']['mail_to_admin']['mail_to_admin_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#default_value' => $settings->mail_to_admin_subject,
  );
  $form['email']['mail_to_admin']['mail_to_admin_body'] = array(
    '#type' => 'textarea',
    '#title' => t('Body'),
    '#default_value' => $settings->mail_to_admin_body,
  );

  $form['email']['reminder_mail_to_registrees'] = array(
    '#type' => 'fieldset',
    '#title' => t('Reminder e-mail to registrees'),
    '#collapsible' => TRUE,
    '#collapsed' => !$settings->send_reminder_mail_to_registrees,
  );
  $form['email']['reminder_mail_to_registrees']['send_reminder_mail_to_registrees'] = array(
    '#type' => 'checkbox',
    '#title' => t('Send e-mail to registree'),
    '#default_value' => $settings->send_reminder_mail_to_registrees,
  );
  $form['email']['reminder_mail_to_registrees']['send_reminders_before'] = array(
    '#type' => 'textfield',
    '#title' => t('Send reminder'),
    '#description' => $strtotime_help . ' ' . _node_registration_strtotime_debug($settings->send_reminders_before, $relative_utc),
    '#default_value' => $settings->send_reminders_before,
    '#field_suffix' => ' ' . t('before'),
    '#element_validate' => array('_node_registration_value_is_strtotime'),
  );
  $form['email']['reminder_mail_to_registrees']['reminder_mail_to_registrees_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#default_value' => $settings->reminder_mail_to_registrees_subject,
  );
  $form['email']['reminder_mail_to_registrees']['reminder_mail_to_registrees_body'] = array(
    '#type' => 'textarea',
    '#title' => t('Body'),
    '#default_value' => $settings->reminder_mail_to_registrees_body,
  );

  $form['email']['cancel_mail_to_registree'] = array(
    '#type' => 'fieldset',
    '#title' => t('Cancel e-mail to registree'),
    '#collapsible' => TRUE,
    '#collapsed' => !$settings->send_cancel_mail_to_registree,
  );
  $form['email']['cancel_mail_to_registree']['send_cancel_mail_to_registree'] = array(
    '#type' => 'checkbox',
    '#title' => t('Send e-mail to registree'),
    '#default_value' => $settings->send_cancel_mail_to_registree,
  );
  $form['email']['cancel_mail_to_registree']['cancel_mail_to_registree_bcc'] = array(
    '#type' => 'textfield',
    '#title' => t('Send BCC to'),
    '#default_value' => $settings->cancel_mail_to_registree_bcc,
    '#element_validate' => array('value_is_email_or_token'),
    '#max' => 20,
    '#description' => t('A (comma separated) list of e-mail addresses to send this notification to.') . $accepts_tokens,
  );
  $form['email']['cancel_mail_to_registree']['cancel_mail_to_registree_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#default_value' => $settings->cancel_mail_to_registree_subject,
  );
  $form['email']['cancel_mail_to_registree']['cancel_mail_to_registree_body'] = array(
    '#type' => 'textarea',
    '#title' => t('Body'),
    '#default_value' => $settings->cancel_mail_to_registree_body,
  );

  $form['email']['cancel_mail_to_admin'] = array(
    '#type' => 'fieldset',
    '#title' => t('Cancel e-mail to admin'),
    '#collapsible' => TRUE,
    '#collapsed' => !$settings->send_cancel_mail_to_admin,
  );
  $form['email']['cancel_mail_to_admin']['send_cancel_mail_to_admin'] = array(
    '#type' => 'checkbox',
    '#title' => t('Send e-mail to admin'),
    '#default_value' => $settings->send_cancel_mail_to_admin,
  );
  $form['email']['cancel_mail_to_admin']['cancel_mail_to_admin_recipients'] = array(
    '#type' => 'textfield',
    '#title' => t('Recipients'),
    '#default_value' => $settings->cancel_mail_to_admin_recipients,
    '#element_validate' => array('value_is_email_or_token'),
    '#max' => 20,
    '#description' => t('A (comma separated) list of e-mail addresses to send this notification to.') . $accepts_tokens,
  );
  $form['email']['cancel_mail_to_admin']['cancel_mail_to_admin_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#default_value' => $settings->cancel_mail_to_admin_subject,
  );
  $form['email']['cancel_mail_to_admin']['cancel_mail_to_admin_body'] = array(
    '#type' => 'textarea',
    '#title' => t('Body'),
    '#default_value' => $settings->cancel_mail_to_admin_body,
  );

  $form['email']['unwaitinglist_mail_to_registree'] = array(
    '#type' => 'fieldset',
    '#title' => t('Unwaitinglist e-mail to registree'),
    '#collapsible' => TRUE,
    '#collapsed' => !$settings->send_unwaitinglist_mail_to_registree,
  );
  $form['email']['unwaitinglist_mail_to_registree']['send_unwaitinglist_mail_to_registree'] = array(
    '#type' => 'checkbox',
    '#title' => t('Send e-mail to registree'),
    '#default_value' => $settings->send_unwaitinglist_mail_to_registree,
  );
  $form['email']['unwaitinglist_mail_to_registree']['unwaitinglist_mail_to_registree_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#default_value' => $settings->unwaitinglist_mail_to_registree_subject,
  );
  $form['email']['unwaitinglist_mail_to_registree']['unwaitinglist_mail_to_registree_body'] = array(
    '#type' => 'textarea',
    '#title' => t('Body'),
    '#default_value' => $settings->unwaitinglist_mail_to_registree_body,
  );

  $form['show_messages'] = array(
    '#type' => 'fieldset',
    '#title' => t('Block error messages'),
    '#description' => t("Show a themeable warning message for which scenario's?"),
    '#collapsible' => TRUE,
  );
  $form['show_messages']['show_message_access'] = array(
    '#type' => 'checkbox',
    '#title' => t('User has insufficient access'),
    '#default_value' => $settings->show_message_access,
  );
  $form['show_messages']['show_message_date'] = array(
    '#type' => 'checkbox',
    '#title' => t('Invalid date'),
    '#default_value' => $settings->show_message_date,
  );
  $form['show_messages']['show_message_capacity'] = array(
    '#type' => 'checkbox',
    '#title' => t('No more room'),
    '#default_value' => $settings->show_message_capacity,
  );
  $form['show_messages']['show_message_registered'] = array(
    '#type' => 'checkbox',
    '#title' => t('User already registered'),
    '#default_value' => $settings->show_message_registered,
  );

  _node_registration_add_token_tree($form);

  return $form;
}

/**
 * Add token tree to form array.
 */
function _node_registration_add_token_tree(&$form) {
  if (module_exists('token')) {
    $form['token_tree'] = array(
      '#type' => 'fieldset',
      '#title' => t('Tokens'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      'tokens' => array(
        '#theme' => 'token_tree',
        '#token_types' => array('node-registration', 'node', 'current-date'),
        '#global_types' => FALSE,
      ),
    );
  }
}

/**
 * Form for registration type settings.
 */
function node_registration_type_settings($form, &$form_state, $registration_type) {
  $form['#registration_type'] = $registration_type;
  $type = $registration_type->type;

  // This might be a 'private fields' bundle and that can't have settings.
  if (preg_match('/^node_(\d+)$/', $type, $match)) {
    $node = node_load($match[1]);
    $goto = 'admin/structure/node_registration' . ( $node ? '/manage/' . $node->type : '' );
    return drupal_goto($goto);
  }

  $settings = _node_registration_node_type_settings($type);

  $form['events'] = array(
    '#type' => 'fieldset',
    '#title' => t('Events'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['events']['toggle_enabled_in_node_form'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show registration status checkbox in node form'),
    '#default_value' => $settings->toggle_enabled_in_node_form,
    '#description' => t('Will add a vertical tab with enabled checkbox to the node form.'),
  );
  $form['events']['no_register_page'] = array(
    '#type' => 'checkbox',
    '#title' => t('Disable register page'),
    '#default_value' => $settings->no_register_page,
    '#description' => t('Will hide the register tab on the node page and deny access to the register page for all people without Administer registration permission.'),
  );

  $form += _node_registration_type_settings_form($type, $settings);

  $form['register_cancel']['help'] = array(
    '#markup' => '<p>' . t('To disable registration on this <strong>content type</strong>, or to change the <strong>date field</strong>, go to the !settings_link.', array(
      '!settings_link' => l(t('content type settings page'), 'admin/structure/types/manage/' . $type),
    )) . '</p>',
    '#weight' => -10,
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Submit handler for node_registration_type_settings().
 */
function node_registration_type_settings_submit($form, &$form_state) {
  $registration_type = $form['#registration_type'];
  $type = $registration_type->type;

  // Allowed settings.
  $old_settings = (array) _node_registration_node_type_settings($type);

  $values = $form_state['values'];

  // Remove foreign stuff from values.
  $new_settings = array_intersect_key($values, $old_settings);

  // Add allowed `extra_` stuff from values.
  foreach ($values as $name => $value) {
    if (0 === strpos($name, 'extra_')) {
      $new_settings[$name] = $value;
    }
  }

  _node_registration_node_type_settings($type, $new_settings);

  drupal_set_message(t('Registration type settings saved.'));
}


/**
 * Helper to get available users (names) from the node_registration_users view.
 */
function _node_registration_assoc_users_from_view() {
  $view = views_get_view('node_registration_users');
  if ($view && empty($view->disabled)) {
    $view->execute_display('master');

    if ($view->executed && $view->result) {
      $values = $view->result;
      $labels = $view->style_plugin->rendered_fields;

      $options = array();
      foreach ($values as $i => $value) {
        $options[$value->users_name] = strip_tags($labels[$i]['name']);
      }

      if ($options) {
        return $options;
      }
    }
  }
}


/**
 * The registration form. It takes a (mandatory) existing or empty
 * registration object.
 */
function node_registration_form($form, &$form_state, $registration, $user_to_register = NULL) {
  global $user;

  $account = $user_to_register ? $user_to_register : $user;
  $account_email = isset($account->mail) ? $account->mail : '';

  $form['#registration'] = $registration;
  $form['#node'] = $node = $registration->node ?: node_load($registration->nid);
  $form['#account'] = $account;

  $admin = user_access('administer node registration');
  $can_register_others = $admin || node_registration_node_access($node, 'register others');

  $auth_type = $account->uid ? 'authenticated' : 'anonymous';

  $settings = $node->registration;
  $capacity = $settings->capacity();
  $is_new = (bool) $registration->is_new;

  // Verify the event hasn't sold out.
  if ($is_new && 'GET' == $_SERVER['REQUEST_METHOD']) {
    if (!_node_registration_event_has_room($node)) {
      $waitinglist = _node_registration_waitinglist_size($node);
      drupal_set_message(t("This event is at capacity. You can register for the waiting list. There are @num people waiting...", array('@num' => $waitinglist)), 'warning');
    }
  }

  // Different user context
  if ($can_register_others && $is_new && !$user_to_register) {
    // Allow changing the context user (=registree).
    $form['account'] = array(
      '#type' => 'textfield',
      '#title' => t('Account'),
      '#element_validate' => array('value_is_username'),
      '#exists' => TRUE,
      '#autocomplete_path' => 'user/autocomplete',
      '#default_value' => $account->uid ? $account->name : '',
      '#empty_option' => t('-- Anonymous'),
      '#description' => t('Leaving this field empty, means the registree will be anonymous.'),
    );

    // Get users from a view.
    if ($options = _node_registration_assoc_users_from_view()) {
      $form['account']['#type'] = 'select';
      $form['account']['#options'] = $options;
      $form['account']['#description'] = t('Choose Anonymous to disconnect this registration from any Drupal user.');
    }
  }

  // Field: attended.
  if (!$is_new) {
    $form['attended'] = array(
      '#type' => 'checkbox',
      '#title' => t('Attended?'),
      '#default_value' => !empty($registration->attended),
      '#access' => $admin,
      '#weight' => -11,
    );
  }

  // Field: slots.
  $setting_name = 'max_slots_per_registration_' . $auth_type;
  $max_slots = min($settings->$setting_name, ($capacity ?: 999) - node_registration_event_count($node));
  $options = range(1, max(1, min(9, $max_slots)));
  $form['slots'] = array(
    '#type' => 'select',
    '#title' => t('Slots'),
    '#description' => t('Select the number of slots your registration will consume.'),
    '#options' => array_combine($options, $options),
    '#default_value' => $registration->slots,
    '#access' => 1 < $max_slots,
    '#real_max_slots' => $max_slots,
  );

  // Field fields.
  _node_registration_fields_callback('field_attach_form', $registration, $form, $form_state);

  // E-mail field/value.
  $default_email = !empty($registration->email) ? $registration->email : $account_email;

  // Show e-mail field.
  if (!$default_email || $settings->allow_email_change || $can_register_others) {
    $form['email'] = array(
      '#type' => 'textfield',
      '#title' => t('E-mail address'),
      '#default_value' => $default_email,
      '#required' => TRUE,
      '#element_validate' => array('value_is_email'),
    );

    // Admins are allowed to leave this empty, so it's autofilled via the registree user.
    // If the registree user is Anonymous, this field will be required (in the validation handler).
    if ($can_register_others && $is_new) {
      $form['email']['#default_value'] = '';
      $form['email']['#required'] = FALSE;
      $form['email']['#description'] = t("You can leave this field empty to autofill it with the registree's e-mail address.");
    }
  }
  // Don't show e-mail field.
  else {
    $form['#fixed_email'] = $default_email;

    $form['email'] = array(
      '#type' => 'item',
      '#title' => t('E-mail address'),
      '#markup' => '<div class="value">' . $default_email . '</div>',
      '#value' => $default_email,
    );
  }

  // Default values.
  if ($is_new) {
    $values = array();
    $context = array(
      'node' => $node,
      'registration' => $registration,
      'user' => $account,
    );
    drupal_alter('node_registration_default_values', $form, $values, $context);

    // Simple default values.
    $lang = $registration->language;
    foreach ($values as $field_name => $value) {
      if ($field_info = field_info_field($field_name)) {
        if (isset($field_info['columns']['value'])) {
          $form[$field_name][$lang][0]['value']['#default_value'] = $value;
        }
        elseif (isset($field_info['columns']['email'])) {
          $form[$field_name][$lang][0]['email']['#default_value'] = $value;
        }
      }
    }
  }

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => $is_new ? t('Create registration') : t('Update registration'),
  );

  // Cancel registration.
  if ($registration->registration_id) {
    $uri = node_registration_uri($registration);
    $form['actions']['cancel'] = array(
      '#type' => 'link',
      '#title' => t('Cancel registration'),
      '#href' => $uri['path'] . '/cancel',
      '#access' => node_registration_access($registration, 'cancel'),
    );
  }

  // Don't register (cancel registering / return).
  else {
    $form['actions']['return'] = array(
      '#type' => 'link',
      '#title' => t('Cancel'),
      '#href' => 'node/' . $registration->nid,
    );
  }

  $form['#validate'][] = 'node_registration_form_validate';
  $form['#submit'][] = 'node_registration_form_submit';

  return $form;
}

/**
 * Validation handler for node_registration_form().
 */
function node_registration_form_validate($form, &$form_state) {
  $registration = $form['#registration'];
  $is_new = (bool) $registration->is_new;

  $node = $form['#node'];
  $account = $form['#account'];
  $settings = $node->registration;

  $admin = user_access('administer node registration');
  $can_register_others = node_registration_node_access($node, 'register others');

  $values = &$form_state['values'];

  // Verify the event hasn't sold out.
  if ($is_new && !_node_registration_event_has_room($node) && !$settings->allow_exceeding_capacity) {
    return form_set_error('email', t("Sorry, this event's capacity during your registration."));
  }

  // Change user context for admins.
  if ($can_register_others && isset($form['account'])) {
    $account = '' == $values['account'] ? drupal_anonymous_user() : user_load_by_name($values['account']);
  }

  // E-mail not editable.
  if (isset($form['#fixed_email'])) {
    $values['email'] = $form['#fixed_email'];
  }

  if (empty($values['email'])) {
    if ($account->uid) {
      $values['email'] = $account->mail;
    }
    else {
      return form_set_error('email', t("You have to submit an e-mail address for anonymous registrations."));
    }
  }
  else {
    $existing_account = user_load_by_mail($values['email']);
    if ($existing_account && $existing_account->uid != $account->uid && !$admin) {
      return form_set_error('email', t('This e-mail address belongs to a registered user. If you are that user, please <a href="!url">log in</a> first.', array('!url' => url('user/login'))));
    }
  }

  // Used values / flags.
  $email = $values['email'];

  // Notify field widgets to validate their data.
  _node_registration_fields_callback('field_attach_form_validate', $registration, $form, $form_state);

  $errors = form_get_errors();

  if (!$errors) {
    // E-mail address must be unique per event.
    $registered = _node_registration_email_registered($node, $email);
    if ($registered) {
      if (!$registration->registration_id || $registration->registration_id != $registered->registration_id) {
        form_set_error('email', t('%email is already registered for this event.', array('%email' => $email)));
      }
    }

    // Alter registration object and collect custom errors.
    $context = array(
      'form' => $form,
      'form_state' => $form_state,
      'node' => $node,
      'user' => $account,
    );
    $errors = array();
    drupal_alter('node_registration_submit', $registration, $context, $errors);

    // Set custom form errors.
    foreach ($errors as $field => $error) {
      form_set_error($field, $error);
    }
  }
}

/**
 * Submit handler for node_registration_form().
 */
function node_registration_form_submit($form, &$form_state) {
  global $user;

  $account = $form['#account'];
  $registration = $form['#registration'];
  $node = $form['#node'];
  $settings = $node->registration;

  $can_register_others = node_registration_node_access($node, 'register others');

  $values = &$form_state['values'];

  // Change user context for admins.
  if ($can_register_others && isset($form['account'])) {
    $account = '' == $values['account'] ? drupal_anonymous_user() : user_load_by_name($values['account']);
  }

  // E-mail not editable.
  if (isset($form['#fixed_email'])) {
    $values['email'] = $form['#fixed_email'];
  }

  // Used values / flags.
  $email = $values['email'];
  $is_new = (bool) $registration->is_new;

  // {node_registration} values.
  $registration->email = $email;
  $registration->slots = (int) $values['slots'];
  isset($values['attended']) && $registration->attended = (int) $values['attended'];

  // Only set for new registration.
  if ($is_new) {
    $registration->uid = $account->uid;
    $registration->author_uid = $user->uid;
    $registration->secret = _node_registration_secret();

    // Auto verify for authenticated users and admins.
    if ($account->uid || $can_register_others) {
      $registration->verified = 1;
    }
  }

  // Notify field widgets.
  _node_registration_fields_callback('field_attach_submit', $registration, $form, $form_state);

  // Alter registration object.
  $context = array(
    'form' => $form,
    'form_state' => $form_state,
    'node' => $node,
    'user' => $account,
  );
  $errors = array();
  drupal_alter('node_registration_submit', $registration, $context, $errors);

  // Very last pre-flight check to make sure this is a unique e-mail address.
  if ($is_new) {
    $exists = db_query('SELECT registration_id FROM {node_registration} WHERE nid = :nid AND cancelled = :cancelled AND email = :email', array(
      ':nid' => $node->nid,
      ':cancelled' => 0,
      ':email' => $email,
    ))->fetchField();
    if ($exists) {
      $form_state['rebuild'] = TRUE;
      return form_set_error('email', t('%email is already registered for this event.', array('%email' => $email)));
    }
  }

  // Save the registration.
  if (node_registration_save($registration)) {
    $uri = node_registration_uri($registration);

    if ($is_new) {
      drupal_set_message(t($settings->registration_success_message, array(
        '@registration_url' => url($uri['path']),
      )));

      // Token param.
      $registration->node = $node;

      // Waitinglist? then re-fetch object.
      $waitinglist = FALSE;
      if (_node_registration_reset_waitinglist($node)) {
        $registration = node_registration_load($registration->registration_id, TRUE);
        $waitinglist = TRUE;
      }

      $token_data = array(
        'node' => $node,
        'node-registration' => $registration,
      );

      // Send e-mail to new registree.
      if ($settings->send_mail_to_registree) {
        $subject = $waitinglist && $settings->change_mail_for_waiting_list_registree && $settings->mail_for_waiting_list_registree_subject ? $settings->mail_for_waiting_list_registree_subject : $settings->mail_to_registree_subject;
        $message = $waitinglist && $settings->change_mail_for_waiting_list_registree && $settings->mail_for_waiting_list_registree_body ? $settings->mail_for_waiting_list_registree_body : $settings->mail_to_registree_body;
        $options = array(
          'bcc' => token_replace($settings->mail_to_registree_bcc, $token_data, array('clear' => TRUE)),
          'alter' => 'new_registration',
        );
        node_registration_send_broadcast($node, $subject, $message, array($registration), $options);
      }

      // Send notification e-mail to admin.
      if ($settings->send_mail_to_admin) {
        $subject = token_replace($settings->mail_to_admin_subject, $token_data);
        $message = token_replace($settings->mail_to_admin_body, $token_data);
        $recipients = token_replace($settings->mail_to_admin_recipients, $token_data);
        _node_registration_send_email($recipients, $subject, $message, $token_data);
      }
    }
    else {
      // Trigger: verify / unverify.
      $op = $registration->verified ? 'verify' : 'unverify';
      module_invoke_all('node_registration_' . $op, $registration);

      drupal_set_message(t('Your registration has been updated.'));
    }

    // Redirect to event node.
    $form_state['redirect'] = $is_new ? 'node/' . $registration->nid : $uri['path'];
  }
  else {
    drupal_set_message(t('Sorry, there has been a problem submitting your registration.'));
  }
}

/**
 * Menu callback -- ask for confirmation of registration cancellation.
 */
function node_registration_cancel_confirm($form, &$form_state, $registration) {
  // Always provide entity id in the same form key as in the entity edit form.
  $form['#registration'] = $registration;

  $form['#submit'][] = 'node_registration_cancel_confirm_submit';

  $uri = node_registration_uri($registration);

  return confirm_form($form,
    t('Are you sure you want to cancel registration %title?', array('%title' => $registration->registration_id)),
    $uri['path'],
    t('This action cannot be undone.'),
    t('Cancel registration'),
    t('Cancel')
  );
}

/**
* Menu callback -- ask for confirmation of registration cancellation
*/
function node_registration_cancel($form, &$form_state, $registration) {
  // Always provide entity id in the same form key as in the entity edit form.
  $form['#registration'] = $registration;

  $form['description'] = array(
    '#markup' => t('Are you sure you want to cancel registration %title?', array('%title' => $registration->registration_id))
  );

  $form['confirm'] = array(
    '#type' => 'submit',
    '#value' => t('Yes'),
    '#submit' => array('node_registration_cancel_confirm_submit')
  );

  $form['no'] = array(
    '#type' => 'submit',
    '#value' => t('No')
  );

  return $form;
}

/**
 * Execute node registration cancellation.
 */
function node_registration_cancel_confirm_submit($form, &$form_state) {
  $registration = $form['#registration'];
  $node = node_load($registration->nid);

  if ($form_state['values']['confirm']) {
    node_registration_cancel_action($registration, TRUE);

    // Notify user.
    drupal_set_message(t('Registration %id has been cancelled.', array('%id' => $registration->registration_id)));
  }

  $form_state['redirect'] = 'node/' . $registration->nid;
}

/**
 * Confirm delete registration.
 */
function node_registration_delete_confirm($form, &$form_state, $registration) {
  $form['#registration'] = $registration;

  $form['#submit'][] = 'node_registration_delete_confirm_submit';

  $uri = node_registration_uri($registration);

  return confirm_form($form,
    t('Are you sure you want to delete registration %title?', array('%title' => $registration->registration_id)),
    $uri['path'],
    t('This action cannot be undone.'),
    t('Delete registration'),
    t('Cancel')
  );
}

/**
 * Execute delete registration.
 */
function node_registration_delete_confirm_submit($form, &$form_state) {
  $registration = $form['#registration'];

  if ($form_state['values']['confirm']) {
    // Delete.
    node_registration_delete_multiple(array($registration->registration_id));

    // Notify.
    watchdog('node_registration', 'Registration %id deleted.', array('%id' => $registration->registration_id));
    drupal_set_message(t('Registration %id has been deleted.', array('%id' => $registration->registration_id)));
  }

  $form_state['redirect'] = 'node/' . $registration->nid;
}

/**
 * Return a form for sending a broadcast email to participants.
 */
function node_registration_registrations_broadcast_form($form, &$form_state, $node) {
  $form['#node'] = $node;

  $registrations = node_registration_load_multiple(FALSE, array('nid' => $node->nid, 'cancelled' => 0));

  $form['recipients'] = array(
    '#type' => 'item',
    '#title' => t('Recipients'),
    '#markup' => count($registrations),
  );

  $form['bcc'] = array(
    '#type' => 'textfield',
    '#title' => t('Send BCC to'),
    '#element_validate' => array('value_is_email'),
    '#max' => 20,
  );

  $form['subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#required' => TRUE,
    '#maxlength' => 150,
  );
  $form['message'] = array(
    '#type' => 'textarea',
    '#title' => t('Message'),
    '#required' => TRUE,
    '#cols' => 60,
    '#rows' => 6,
  );

  if ($node->registration->mail_system) {
    $form['attachment'] = array(
      '#type' => 'file',
      '#title' => t('Attachment'),
      '#required' => FALSE,
    );
  }

  $form['actions'] = array(
    '#type' => 'actions',
  );
  $form['actions']['send'] = array(
    '#type' => 'submit',
    '#value' => t('Send'),
  );

  _node_registration_add_token_tree($form);

  return $form;
}

/**
 * Validate handler for registration_registrations_broadcast_form.
 */
function node_registration_registrations_broadcast_form_validate($form, &$form_state) {
  $validators = array('file_validate_extensions' => array('jpg gif png pdf doc docx xls xlsx ppt pptx txt zip rar gz'));
  $file = file_save_upload('attachment', $validators);
  if (isset($file)) {
    // File upload was attempted.
    if ($file) {
      // Put the temporary file in form_values so we can save it on submit.
      $form_state['values']['attachment'] = $file;
    }
    else {
      // File upload failed.
      form_set_error('attachment', t('The attachment must be of these types: !types.', array('!types' => $validators['file_validate_extensions'][0])));
    }
  }
}

/**
 * Submit handler for registration_registrations_broadcast_form.
 */
function node_registration_registrations_broadcast_form_submit($form, &$form_state) {
  $options = array(
    'notify' => TRUE,
    'bcc' => $form_state['values']['bcc'],
  );

  if (isset($form_state['values']['attachment']) && isset($form_state['values']['attachment']->uri)) {
    $options['attachments'] = array($form_state['values']['attachment']);
  }

  node_registration_send_broadcast($form['#node'], $form_state['values']['subject'], $form_state['values']['message'], NULL, $options);
}

/**
 * Return a form for a node's registration settings.
 */
function node_registration_registrations_settings_form($form, &$form_state, $node) {
  $form['#node'] = $node;

  $form['#attributes']['class'][] = 'node-registration';

  $type = $node->type;
  $settings = $node->registration;

  $form += _node_registration_type_settings_form($type, $settings, $node);

  $status = $settings->enabled() ?
    '<span class="registration-status enabled">' . t('Enabled') . '</span>' :
    '<span class="registration-status disabled">' . t('Disabled') . '</span>';
  $form['register_cancel']['prestatus'] = array(
    '#markup' => '<div class="form-item">',
    '#weight' => -11,
  );
  $form['register_cancel']['status'] = array(
    '#type' => 'submit',
    '#value' => $settings->enabled() ? t('Disable registrations') : t('Enable registrations'),
    '#weight' => -10,
    '#submit' => array('node_registration_node_settings_status_submit'),
    '#prefix' => '<span>' . t('Current status: !status', array('!status' => $status)) . '</span> ',
  );
  $form['register_cancel']['poststatus'] = array(
    '#markup' => '</div>',
    '#weight' => -9,
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Settings'),
  );

  $form['#submit'][] = 'node_registration_registrations_settings_form_submit';

  return $form;
}

/**
 * Submit handler for registration_registrations_settings_form().
 *
 * Saves actual settings into {node_registration_node}.
 */
function node_registration_registrations_settings_form_submit($form, &$form_state) {
  $node = $form['#node'];
  $nid = $node->nid;

  $values = &$form_state['values'];

  unset($values['status']);
  $node->registration->update($values);

  drupal_set_message(t('Registration node settings saved.'));
}

/**
 * Submit handler for node_registration_registrations_settings_form().
 *
 * Toggles registration status for a node/event.
 */
function node_registration_node_settings_status_submit($form, &$form_state) {
  $node = $form['#node'];
  $settings = $node->registration;

  // Update these values.
  $update = array(
    // Toggle status.
    'status' => $settings->enabled() ? 0 : 1,
  );

  $settings->update($update);
}

/**
 * Form element validator for simple strtotime() format.
 *
 * Possibly interesting regex: #(^|[^\+\d\-])(\d+)#
 */
function _node_registration_value_is_strtotime($element, &$form_state, $form) {
  $value = $element['#value'];

  if ('' != $value) {
    if (!preg_match('#^(?:\d+ (?:minute|hour|day|week|month)s? ?)+$#i', $value)) {
      form_error($element, t('%title must be a valid period.', array('%title' => $element['#title'])));
    }
  }
}
