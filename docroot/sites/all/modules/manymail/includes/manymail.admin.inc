<?php
/**
 * @file
 * Admin page callbacks.
 */

/**
 * Form function for options.
 *
 * @ingroup forms
 */
function manymail_config_options_form($form, &$form_state) {
  $form['throttle'] = array(
    '#type' => 'fieldset',
    '#title' => t('E-mail throttling'),
    '#description' => t('This prevents server time-outs when trying to send a lot of e-mails and makes it less likely for your mail to be marked as spam.'),
    '#weight' => 1,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['throttle']['manymail_options_throttle_pause'] = array(
    '#type' => 'textfield',
    '#title' => t('Pause between batches'),
    '#default_value' => variable_get('manymail_options_throttle_pause', 5),
    '#description' => t('The amount of seconds to wait between sending batches.'),
  );

  $form['throttle']['manymail_options_throttle_amount'] = array(
    '#type' => 'textfield',
    '#title' => t('E-mails per batch'),
    '#default_value' => variable_get('manymail_options_throttle_amount', 50),
    '#description' => t('The amount of e-mails to send per batch.'),
  );

  $form['content'] = array(
    '#type' => 'fieldset',
    '#title' => t('Content encoding'),
    '#weight' => 2,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['content']['manymail_options_charset'] = array(
    '#type' => 'textfield',
    '#title' => t('Character set'),
    '#default_value' => variable_get('manymail_options_charset', 'utf-8'),
    '#description' => t('The character set to encode your e-mails with.<br />Common options are utf-8 (recommended) and iso-8859-1.'),
  );

  $form['content']['manymail_options_html_to_text'] = array(
    '#type' => 'checkbox',
    '#title' => t('Convert HTML to marked up plain text'),
    '#default_value' => variable_get('manymail_options_html_to_text', 0),
    '#description' => t('When checked, runs the subject and message body through <a href="@apiref" target="_blank">drupal_html_to_text</a>.', array('@apiref' => 'http://api.drupal.org/api/drupal/includes!mail.inc/function/drupal_html_to_text/7')),
  );

  $form['logging'] = array(
    '#type' => 'fieldset',
    '#title' => t('Logging options'),
    '#weight' => 3,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['logging']['manymail_options_log_per_mail'] = array(
    '#type' => 'checkbox',
    '#title' => t('Log successfully sent e-mails on a per-mail base.'),
    '#default_value' => variable_get('manymail_options_log_per_mail', 1),
    '#description' => t('Having this checked will guarantee that a cancelled e-mailing process will be resumed at the user it last tried to mail.<br />Unchecking this will make ManyMail only log on a per-batch level, which results in slightly less database load.<br />Note: this could lead to some people receiving the e-mail twice after a resume action.'),
  );

  return system_settings_form($form);
}

/**
 * Implements validation from the Form API.
 *
 * Verifies whether the throttle delay and amount are valid numbers
 * and converts them to integers if they are.
 *
 * @see manymail_config_options_form()
 */
function manymail_config_options_form_validate($form, &$form_state) {
  if (!is_numeric($form_state['values']['manymail_options_throttle_pause']) || $form_state['values']['manymail_options_throttle_pause'] < 1) {
    form_set_error('manymail_options_throttle_pause', t('You must enter a positive number for the throttle delay.'));
  }
  else {
    form_set_value($form['throttle']['manymail_options_throttle_pause'], floor($form_state['values']['manymail_options_throttle_pause']), $form_state);
  }

  if (!is_numeric($form_state['values']['manymail_options_throttle_amount']) || $form_state['values']['manymail_options_throttle_amount'] < 1) {
    form_set_error('manymail_options_throttle_amount', t('You must enter a positive number for the throttle e-mail amount.'));
  }
  else {
    form_set_value($form['throttle']['manymail_options_throttle_amount'], floor($form_state['values']['manymail_options_throttle_amount']), $form_state);
  }
}

/**
 * Form function for content defaults.
 *
 * @ingroup forms
 */
function manymail_config_defaults_form($form, &$form_state) {
  $form['default_content'] = array(
    '#type' => 'fieldset',
    '#title' => t('Content'),
    '#weight' => 2,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['default_content']['manymail_default_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#default_value' => variable_get('manymail_default_subject', t('ManyMail e-mail')),
    '#description' => t('The e-mail subject.'),
  );

  $form['default_content']['manymail_default_body'] = array(
    '#type' => 'textarea',
    '#title' => t('Body'),
    '#default_value' => variable_get('manymail_default_body', ''),
    '#description' => t('The message body.'),
  );

  $form['default_content']['manymail_default_signature'] = array(
    '#type' => 'textarea',
    '#title' => t('Signature'),
    '#default_value' => variable_get('manymail_default_signature', "Yours sincerely,\nThe site admin"),
    '#description' => t('Gets appended to every message. Can be disabled in the newsletter itself.'),
  );

  return system_settings_form($form);
}

/**
 * Form function for header defaults.
 *
 * @ingroup forms
 */
function manymail_config_headers_form($form, &$form_state) {
  $form['default_from'] = array(
    '#type' => 'fieldset',
    '#title' => t('From: header'),
    '#weight' => 1,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['default_from']['manymail_default_from'] = array(
    '#type' => 'textfield',
    '#title' => t('From address'),
    '#default_value' => variable_get('manymail_default_from', variable_get('site_mail', 'admin@example.com')),
    '#description' => t('The e-mail address that e-mails appear to be sent from.'),
  );

  $form['default_from']['manymail_default_from_name'] = array(
    '#type' => 'textfield',
    '#title' => t('From name'),
    '#default_value' => variable_get('manymail_default_from_name', 'Site admin'),
    '#description' => t('The name of the person that e-mails appear to be sent from.'),
  );

  $form['default_reply_to'] = array(
    '#type' => 'fieldset',
    '#title' => t('Reply-to: header'),
    '#weight' => 2,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['default_reply_to']['manymail_default_reply_to'] = array(
    '#type' => 'textfield',
    '#title' => t('Reply-to address'),
    '#default_value' => variable_get('manymail_default_reply_to', ''),
    '#description' => t('The e-mail address you want recipients to reply to.<br />Leave empty if you want this to be the same as the <em>From</em> address.'),
  );

  $form['default_reply_to']['manymail_default_reply_to_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Reply-to name'),
    '#default_value' => variable_get('manymail_default_reply_to_name', ''),
    '#description' => t('The name of the person for the reply-to address.<br />Leave empty if you want this to be the same as the <em>From</em> name.'),
  );

  $form['other_address_headers'] = array(
    '#type' => 'fieldset',
    '#title' => t('Other address headers'),
    '#weight' => 3,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['other_address_headers']['manymail_options_sender_address'] = array(
    '#type' => 'textfield',
    '#title' => t('Sender address'),
    '#default_value' => variable_get('manymail_options_sender_address', ''),
    '#description' => t('The e-mail address that you are actually sending from.<br />If you set a <em>From</em> address that does not match the actual sender address, you should fill this out to reduce the likelihood of your message being discarded as spam.'),
  );

  $form['other_address_headers']['manymail_options_return_path_address'] = array(
    '#type' => 'textfield',
    '#title' => t('Return-path address'),
    '#default_value' => variable_get('manymail_options_return_path_address', ''),
    '#description' => t('Also called the <em>Bounce address</em>.<br />If you want to process bounced e-mails, you can specify the address you want those mails to go to here.'),
  );

  return system_settings_form($form);
}

/**
 * Form function for SMTP configuration.
 *
 * @ingroup forms
 */
function manymail_config_smtp_form($form, &$form_state) {
  // Show a warning when the SMTP settings don't add up.
  if (!manymail_validate_smtp()) {
    drupal_set_message(t('Could not connect to the SMTP host. Please verify the settings below.'), 'error');
  }

  $form['server'] = array(
    '#type' => 'fieldset',
    '#title' => t('Server settings'),
    '#weight' => 1,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['server']['manymail_smtp_host'] = array(
    '#type' => 'textfield',
    '#title' => t('Host'),
    '#default_value' => variable_get('manymail_smtp_host', 'localhost'),
    '#description' => t('The server to send mails through.'),
  );

  $form['server']['manymail_smtp_port'] = array(
    '#type' => 'textfield',
    '#title' => t('Port'),
    '#default_value' => variable_get('manymail_smtp_port', 25),
    '#description' => t('The SMTP port for the mail server.'),
  );

  $form['server']['manymail_smtp_timeout'] = array(
    '#type' => 'textfield',
    '#title' => t('Timeout'),
    '#default_value' => variable_get('manymail_smtp_timeout', 10),
    '#description' => t('Maximum amount of seconds to wait for the server to respond.'),
  );

  $form['security'] = array(
    '#attributes' => array(
      'id' => 'smtp-security-fieldset',
    ),
    '#type' => 'fieldset',
    '#title' => t('Security settings'),
    '#weight' => 2,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['security']['manymail_smtp_secure'] = array(
    '#type' => 'radios',
    '#title' => t('Server prefix'),
    '#options' => array(
      'none' => '&lt;none&gt;',
      'ssl' => 'ssl',
      'tls' => 'tls',
    ),
    '#default_value' => variable_get('manymail_smtp_secure', 'none'),
    '#description' => t('The connection prefix to the server.'),
  );

  $smtp_auth = variable_get('manymail_smtp_auth', 0);
  $smtp_auth_set = isset($form_state['values']['manymail_smtp_auth']);

  $form['security']['manymail_smtp_auth'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use SMTP authentication'),
    '#default_value' => $smtp_auth,
    '#description' => t('You will be asked to provide a username and password if you check this.'),
    '#ajax' => array(
      'callback' => '_manymail_config_smtp_form_ajax',
      'wrapper' => 'smtp-security-fieldset',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );

  // If the checkbox was altered: respect that choice.
  // Otherwise respect the previously saved settings.
  if (($smtp_auth_set && $form_state['values']['manymail_smtp_auth'] == 1) || (!$smtp_auth_set && !empty($smtp_auth))) {
    unset($form['security']['manymail_smtp_auth']['#description']);

    $form['security']['credentials'] = array(
      '#type' => 'fieldset',
      '#title' => t('Credentials'),
      '#collapsible' => FALSE,
    );

    $form['security']['credentials']['manymail_smtp_username'] = array(
      '#type' => 'textfield',
      '#title' => t('Username'),
      '#default_value' => variable_get('manymail_smtp_username', ''),
      '#required' => TRUE,
    );

    $form['security']['credentials']['manymail_smtp_password'] = array(
      '#type' => 'password',
      '#title' => t('Password'),
      '#default_value' => variable_get('manymail_smtp_password', ''),
      '#required' => TRUE,
    );
  }

  return system_settings_form($form);
}

/**
 * AJAX handler for the SMTP configuration.
 *
 * Adds the SMTP credentials part of the form.
 *
 * @see manymail_config_smtp_form()
 */
function _manymail_config_smtp_form_ajax($form, &$form_state) {
  return $form['security'];
}

/**
 * Form function for target roles.
 *
 * @ingroup forms
 */
function manymail_targets_roles_form($form, &$form_state) {
  // Make the first $roles checkbox a select-all.
  $js_setting['ManyMailCheckAll'] = array(
    'list' => '#edit-manymail-allowed-roles',
    'key' => '#edit-manymail-allowed-roles-' . DRUPAL_AUTHENTICATED_RID,
  );
  drupal_add_js($js_setting, 'setting');
  drupal_add_js(drupal_get_path('module', 'manymail') . '/misc/checkall.js');

  $form['allowed_roles'] = array(
    '#type' => 'fieldset',
    '#title' => t('Allowed recipient roles'),
    '#description' => t('The roles that may be sent a newsletter.'),
    '#weight' => 1,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  // Select all but anonymous role.
  $roles = user_roles(TRUE);

  array_walk($roles, 'check_plain');
  $form['allowed_roles']['manymail_allowed_roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Target roles'),
    '#title_display' => 'invisible',
    '#options' => $roles,
    '#default_value' => variable_get('manymail_allowed_roles', $roles),
  );

  $form['allowed_roles']['manymail_allowed_roles_override'] = array(
    '#type' => 'checkbox',
    '#title' => t('Certain users may ignore this restriction'),
    '#default_value' => variable_get('manymail_allowed_roles_override', 1),
    '#description' => t('When checked, users with the :permission permission can target any !what regardless of what has been configured above.', array(
      ':permission' => 'Override preset recipient roles',
      '!what' => 'role',
    )),
  );

  return system_settings_form($form);
}

/**
 * Show the e-mail logs.
 *
 * @ingroup forms
 */
function manymail_logs_overview_form($form, &$form_state) {
  // Retrieve the data and recipients for each mail.
  $query = db_select('manymail_log', 'l')->extend('PagerDefault')->extend('TableSort');
  $query->join('manymail_log_recipients', 'r', 'r.mlog=l.mlog');
  $query->groupBy(
    $query->addField('l', 'mlog')
  );
  $query->addField('l', 'uid');
  $query->addField('l', 'send_time');
  $query->addField('l', 'send_state');
  $query->addField('l', 'completed');
  $query->addField('l', 'last_activity');
  $query->addExpression('COUNT(r.mail)', 'recipients');

  // Add the sortable header to the table.
  $header = array(
    'sender' => array(
      'data' => t('Sender'),
      'field' => 'l.uid',
    ),
    'date' => array(
      'data' => t('Date sent'),
      'field' => 'l.send_time',
      'sort' => 'desc',
    ),
    'subject' => t('Subject'),
    'recipients' => t('Recipients'),
    'status' => t('Status'),
    'actions' => t('Actions'),
  );
  $query->limit(10);
  $query->orderByHeader($header);

  // Add the {manymail_log_recipients}.status field to group
  // recipient amounts by send status.
  $status = $query->addField('r', 'status');
  $query->groupBy($status);
  $query->orderBy($status);

  // Used below to store the activity status of a mail.
  $mail_in_progress = array();

  // Used below to store the recipient amounts per status for each mail.
  $recipients = array();

  // Options for the tableselect.
  $options = array();

  foreach ($query->execute() as $record) {
    // Initialize the total recipient amount for this mail.
    //
    // Using -1 because the switch statement later on uses loose
    // comparison, which means 2 == 'total' would return TRUE.
    if (!isset($recipients[$record->mlog][-1])) {
      $recipients[$record->mlog][-1] = 0;
    }

    // Store the recipient amount for this status.
    $recipients[$record->mlog][-1] += $record->recipients;
    $recipients[$record->mlog][$record->status] = $record->recipients;

    // Every log entry can occur multiple times, so check if
    // the log entry's row hasn't been set already.
    if (empty($options[$record->mlog])) {
      // Restore the send state to retrieve the subject.
      $send_state = unserialize($record->send_state);

      if ($record->completed) {
        $mail_in_progress[$record->mlog] = FALSE;
        $status = t('Completed');
      }
      else {
        // Check whether a list has been active within the last minute,
        // increased by the throttle pause duration.
        $throttle_pause = variable_get('manymail_options_throttle_pause', 5);
        $mail_in_progress[$record->mlog] = ($record->last_activity + $throttle_pause + 60 >= REQUEST_TIME);

        $status = ($mail_in_progress[$record->mlog])
          ? t('In progress')
          : t('Interrupted');
      }

      $options[$record->mlog] = array(
        'sender' => user_load($record->uid)->name,
        'date' => format_date($record->send_time, 'medium'),
        'subject' => check_plain($send_state['values']['subject']),
        'status' => $status,
      );
    }
  }

  // Populate the recipients and actions cell for each row.
  foreach ($recipients as $mlog => $recipient_stati) {
    // Used to store the action links. The 'View e-mail' and
    // 'View recipients' links are the same for any list status.
    $actions = array(
      t('<a href="@url">View e-mail</a>', array(
        '@url' => url("admin/config/manymail/logs/$mlog/view")
      )),
      t('<a href="@url">View recipients</a>', array(
        '@url' => url("admin/config/manymail/logs/$mlog/recipients")
      )),
    );

    // Used to store the non-total amounts.
    $special_recipients = array();

    foreach ($recipients[$mlog] as $status => $amount) {
      switch ($status) {
        case -1:
          $options[$mlog]['recipients'] = format_plural($amount, 'One address', '@count addresses');
          break;

        case MANYMAIL_RECIPIENT_STATUS_IN_QUEUE:
          if (!$mail_in_progress[$mlog]) {
            $actions[] = t('<a href="@url">Resume mail</a>',
              array('@url' => url("admin/config/manymail/logs/$mlog/resume"))
            );
          }
          $special_recipients[] = t('@amount still queued', array('@amount' => $amount));
          break;

        case MANYMAIL_RECIPIENT_STATUS_BOUNCED:
          $special_recipients[] = t('@amount bounced', array('@amount' => $amount));
          break;

        case MANYMAIL_RECIPIENT_STATUS_FAILURE:
          if (!$mail_in_progress[$mlog]) {
            $actions[] = t('<a href="@url">Retry failed</a>',
              array('@url' => url("admin/config/manymail/logs/$mlog/retry"))
            );
          }
          $special_recipients[] = t('@amount failed', array('@amount' => $amount));
          break;
      }
    }

    // Add any non-total amounts to the recipients cell.
    if (!empty($special_recipients)) {
      $options[$mlog]['recipients'] .= '<br />' . t('Of which') . ' ' . implode(', ', $special_recipients);
    }

    // Add the action links to the row.
    $options[$mlog]['actions'] = implode('<br />', $actions);
  }

  $form['table'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#empty' => t('No logs available.'),
    '#js_select' => TRUE,
  );

  return $form;
}

/**
 * View a sent e-mail message.
 *
 * @param $form
 *   As passed to drupal_get_form().
 * @param $form_state
 *   As passed to drupal_get_form().
 * @param $mlog
 *   The log id of the sent e-mail.
 *
 * @ingroup forms
 */
function manymail_logs_view_mail_form($form, &$form_state, $mlog) {
  $result = db_select('manymail_log', 'm')
    ->fields('m', array('send_state'))
    ->condition('mlog', $mlog)
    ->execute()
    ->fetchField(0);

  $send_state = unserialize($result);

  $form['e-mail'] = array(
    '#type' => 'fieldset',
    '#title' => check_plain($send_state['values']['subject']),
    '#weight' => 1,
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );

  $form['e-mail']['message'] = array(
    '#markup' => filter_xss_admin($send_state['values']['body']),
  );

  $form['details'] = array(
    '#theme' => 'table',
    '#header' => array(
      'details' => array(
        'data' => t('Details'),
        'colspan' => 2,
      ),
    ),
    '#rows' => array(),
    '#weight' => 2,
  );

  // For readability, populate the Details table rows here.
  $form['details']['#rows'][] = array(
    t('From'),
    check_plain($send_state['values']['from_name']) . ' &lt;' . check_plain($send_state['values']['from']) . '&gt;'
  );

  $form['details']['#rows'][] = array(
    t('Reply to'),
    (!empty($send_state['values']['reply_to_name']) && !empty($send_state['values']['reply_to_address']))
      ? check_plain($send_state['values']['reply_to_name']) . ' &lt;' . check_plain($send_state['values']['reply_to_address']) . '&gt;'
      : t('Not specified, using <em>From</em> address')
  );

  $form['details']['#rows'][] = array(
    t('Include signature'),
    $send_state['values']['from_name'] ? t('Yes') : t('No')
  );

  return $form;
}

/**
 * View a sent e-mail's recipients.
 *
 * @param $form
 *   As passed to drupal_get_form().
 * @param $form_state
 *   As passed to drupal_get_form().
 * @param $mlog
 *   The log id of the sent e-mail.
 *
 * @ingroup forms
 */
function manymail_logs_view_mail_recipients_form($form, &$form_state, $mlog) {
  // Retrieve the recipients for this e-mail.
  $query = db_select('manymail_log_recipients', 'm')->extend('PagerDefault')->extend('TableSort');
  $query->addField('m', 'mail');
  $query->addField('m', 'recipient');
  $query->addField('m', 'status');
  $query->condition('mlog', $mlog);

  // Add the sortable header to the table.
  $header = array(
    'mail' => array(
      'data' => t('E-mail address'),
      'field' => 'mail',
      'sort' => 'asc',
    ),
    'name' => t('Name'),
    'status' => array(
      'data' => t('Status'),
      'field' => 'status',
    ),
  );
  $query->limit(10);
  $query->orderByHeader($header);

  $rows = array();

  foreach ($query->execute() as $record) {
    $recipient = unserialize($record->recipient);

    switch ($record->status) {
      case MANYMAIL_RECIPIENT_STATUS_IN_QUEUE:
        $status = t('In queue');
        break;

      case MANYMAIL_RECIPIENT_STATUS_BOUNCED:
        $status = t('Bounced');
        break;

      case MANYMAIL_RECIPIENT_STATUS_FAILURE:
        $status = t('Failed');
        break;

      case MANYMAIL_RECIPIENT_STATUS_SUCCESS:
        $status = t('Sent');
        break;
    }

    $rows[] = array(
      'mail' => $record->mail,
      'name' => $recipient->name,
      'status' => $status,
    );
  }

  $form['recipients'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
  );

  return $form;
}

/**
 * Resume an aborted e-mail.
 *
 * @param $form
 *   As passed to drupal_get_form().
 * @param $form_state
 *   As passed to drupal_get_form().
 * @param $mlog
 *   The log id of the aborted e-mail.
 *
 * @ingroup forms
 */
function manymail_logs_view_mail_resume_form($form, &$form_state, $mlog) {
  // Load manymail.forms.inc to borrow its AJAX callback.
  form_load_include($form_state, 'inc', 'manymail', 'includes/manymail.forms');

  // Provide a div for the AJAX callback.
  $form['batch'] = array(
    '#markup' => '<div id="manymail-overlay"></div>',
  );

  $form['mlog'] = array(
    '#type' => 'value',
    '#value' => $mlog,
  );

  $form['message'] = array(
    '#markup' => t('Are you sure you want to resume this e-mail?<br />Please verify that you have the correct data by reviewing the <a href="@mail">e-mail</a> and its <a href="@recipients">recipients</a>.', array(
      '@mail' => url('admin/config/manymail/logs/' . $mlog . '/view'),
      '@recipients' => url('admin/config/manymail/logs/' . $mlog . '/recipients'),
    )),
    '#prefix' => '<p>',
    '#suffix' => '</p>',
    '#weight' => 1,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#weight' => 2,
    '#value' => t('Yes I am sure.'),
    '#ajax' => array(
      'callback' => '_manymail_send_form_ajax',
      'wrapper' => 'manymail-overlay',
      'method' => 'html',
    ),
  );

  return $form;
}

/**
 * Implements submission from the Form API.
 */
function manymail_logs_view_mail_resume_form_submit($form, &$form_state) {
  // Retrieve the send_state
  $mlog = $form_state['values']['mlog'];

  $result = db_select('manymail_log', 'm')
    ->fields('m', array('send_state'))
    ->condition('mlog', $mlog)
    ->execute()
    ->fetchField(0);

  $send_state = unserialize($result);

  // Start the send process.
  manymail_setup_mail_batch($mlog, $send_state);
}
