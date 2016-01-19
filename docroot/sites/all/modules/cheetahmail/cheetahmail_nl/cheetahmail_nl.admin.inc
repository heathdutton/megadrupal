<?php
/**
 * @file
 * Cheetahmail NL admin forms
 */

module_load_include('inc', 'cheetahmail', 'cheetahmail.api');

/**
 * Provide Cheetahmail general NL settings form
 * Genereal NL configuration page
 * - Maximum number of items to send
 * - Type of elements pushed into the NL
 * - Recurrence of the day sending and sending
 */
function cheetahmail_nl_admin_menu_configs_form() {
  $form = array();

  $form['test_email'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Email for tests'),
    '#description' => t('Email address where to send Test Newsletters'),
    '#weight' => -10,
    '#default_value' => variable_get('experian/cheetahmail/nl/test_email', EMS_CHEETAHMAIL_DEFAULT_SUBSCRIBER),
  );

  $form['send_test'] = array(
    '#type' => 'submit',
    '#value' => t('Send a test'),
    '#submit' => array('cheetahmail_nl_admin_menu_configs_campaign_form_sendtest'),
  );

  $form['sending'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('Sending configuration'),
    '#description'   => t('Newsletter Sending configuration allows to set when and what to send.'),
  );

  $form['sending']['cheetahmail_nl_numitems'] = array(
    '#type'          => 'textfield',
    '#title'         => 'Number of elements',
    '#description'   => t('Maximum number of elements to send. -1 for unlimited'),
    '#default_value' => variable_get('cheetahmail_nl_numitems', 10),
    '#required'      => TRUE,
    '#size'          => 5,
  );

  $form['sending']['cheetahmail_nl_contenttype'] = array(
    '#type'          => 'select',
    '#title'         => t('Type of elements'),
    '#description'   => t('Type of elements pushed into the NL.'),
    '#default_value' => variable_get('cheetahmail_nl_contenttype', CHEETAHMAIL_NL_CONTENT_POSTSANDPAGES),
    '#options'       => array(
      CHEETAHMAIL_NL_CONTENT_POSTS => 'Posts',
      CHEETAHMAIL_NL_CONTENT_PAGES => 'Pages',
      CHEETAHMAIL_NL_CONTENT_POSTSANDPAGES => 'Posts + Pages',
    ),
    '#required'      => TRUE,
  );

  $form['sending']['cheetahmail_nl_send'] = array(
    '#type'          => 'radios',
    '#title'         => t('Send newsletter'),
    '#default_value' => variable_get('cheetahmail_nl_send', CHEETAHMAIL_NL_SEND_STOP),
    '#options'       => array(
      // CHEETAHMAIL_NL_SEND_TEST => t('Send one test Newsletter to the test address'),
      CHEETAHMAIL_NL_SEND_RIGHTNOW => t('Send Newsletter right now'),
      CHEETAHMAIL_NL_SEND_SCHEDULED => t('Send Newsletter according to schedule'),
      CHEETAHMAIL_NL_SEND_STOP => t('Stop Newsletter schedule'),
    ),
    '#required'      => TRUE,
  );

  $form['sending']['cheetahmail_nl_sending_interval'] = array(
    '#type'          => 'select',
    '#title'         => t('Interval'),
    '#description'   => t('Recurrence of the day sending and sending.'),
    '#default_value' => variable_get('cheetahmail_nl_sending_interval'),
    '#options'       => array(
      1 => 'Every day',
      7 => 'Every week',
      30 => 'Every month (each 30 days)',
    ),
    '#required'      => TRUE,
  );

  $cheetahmail_nl_start = array(
    'month_day' => NULL,
    'week_day'=> NULL,
    'start_hour' => NULL,
  );
  $cheetahmail_nl_start = array_merge($cheetahmail_nl_start, variable_get('cheetahmail_nl_start', array()));

  $form['sending']['start'] = array(
    '#type' => 'item',
    '#description' => t('Works by creating a new Newsletter at the
      desired time'),
    '#title' => t('Start sending on'),
  );
  $form['sending']['start']['cheetahmail_nl_start'] = array(
    '#type' => 'container',
    '#tree' => TRUE,
    '#attributes' => array('class' => array('container-inline-date')),
  );
  $form['sending']['#attached']['js'][] = drupal_get_path('module', 'cheetahmail_nl') . '/cheetahmail_nl.js';
  $form['sending']['start']['cheetahmail_nl_start']['month_day'] = array(
    '#type' => 'select',
    '#default_value' => $cheetahmail_nl_start['month_day'],
    '#options'       => array_merge(array(t('-Month Day')), range(1, 28)),
    '#description'   => t('Day of Month'),
  );
  $form['sending']['start']['cheetahmail_nl_start']['week_day'] = array(
    '#type' => 'select',
    '#default_value' => $cheetahmail_nl_start['week_day'],
    '#options'       => array_merge(array(t('-Week day')), date_week_days(TRUE)),
    '#description'   => t('Day of Week'),
  );
  $hours = range(0, 23);
  $form['sending']['start']['cheetahmail_nl_start']['start_hour'] = array(
    '#type' => 'select',
    '#default_value' => $cheetahmail_nl_start['start_hour'],
    '#options'       => array_merge(array(), date_hours('H', TRUE)),
    '#description'   => t('Hour'),
    '#field_suffix'  => ':',
  );
  $form['sending']['start']['cheetahmail_nl_start']['start_minute'] = array(
    '#type' => 'select',
    '#default_value' => '00',
    '#options'       => array('00'),
    '#description'   => t('Minute'),
    '#disabled'  => TRUE,
  );

  $form = system_settings_form($form);

  $form['#submit'][] = 'cheetahmail_nl_admin_menu_configs_form_submit';

  return $form;
}

/**
 * Validate callback for `cheetahmail_nl_admin_menu_configs_form` form
 */
function cheetahmail_nl_admin_menu_configs_form_validate(&$form, &$form_state) {
  if (!valid_email_address($form_state['values']['test_email'])) {
    form_set_error('test_email', 'Please, provide a valid email address.');
  }

  $cheetahmail_nl_numitems = $form_state['values']['cheetahmail_nl_numitems'];
  if (empty($cheetahmail_nl_numitems) || !is_numeric($cheetahmail_nl_numitems)) {
    form_set_error('cheetahmail_nl_numitems', 'Please, set a valid Number of elements. It must be a non-zero integer between -1 (unlimited) to 50.');
  }
  $cheetahmail_nl_numitems = intval($cheetahmail_nl_numitems);
  if (($cheetahmail_nl_numitems != $form_state['values']['cheetahmail_nl_numitems'])
    || !($cheetahmail_nl_numitems >= -1 && $cheetahmail_nl_numitems <= 50)) {
    form_set_error('cheetahmail_nl_numitems', 'Please, set a valid Number of elements. It must be a non-zero integer between -1 (unlimited) to 50.');
  }
}

/**
 * Submit callback for `cheetahmail_nl_admin_menu_configs_form` form
 */
function cheetahmail_nl_admin_menu_configs_form_submit(&$form, &$form_state) {
  switch ($form_state['values']['cheetahmail_nl_send']) {
    case CHEETAHMAIL_NL_SEND_TEST:
      cheetahmail_nl_set_test_email($form_state['values']['test_email']);
      cheetahmail_nl_send_campaign($campaign_id = variable_get('experian/cheetahmail/nl/test_campaign_id'));
      break;

    case CHEETAHMAIL_NL_SEND_RIGHTNOW:
      cheetahmail_nl_send_campaign($campaign_id = variable_get('experian/cheetahmail/nl/campaign_id'), TRUE);
      break;

    case CHEETAHMAIL_NL_SEND_SCHEDULED:
      $campaign_id = variable_get('experian/cheetahmail/nl/campaign_id');
      $record = db_select('cheetahmail_campaigns', 'cc_list')
        ->fields('cc_list', array(
          'campaign_id',
          'status',
          'startedat',
          'is_finished',
          ))
        ->condition('parent_id', $campaign_id, '=')
        ->condition('is_finished', 0, '=')
        ->execute()
        ->fetchAssoc();
      if (!empty($record['campaign_id'])) {
        // Send current campaign.
        cheetahmail_nl_send_campaign($campaign_id, TRUE);
      }

      // Get the time when the next sending is scheduled.
      $record = db_select('cheetahmail_campaigns', 'cc_list')
        ->fields('cc_list', array(
          'campaign_id',
          'status',
          'startedat',
          'is_finished',
          ))
        ->condition('parent_id', $campaign_id, '=')
        ->condition('is_finished', 1, '=')
        ->orderBy('startedat', 'DESC')
        ->execute()
        ->fetchAssoc();
      if (empty($record['startedat'])) {
        watchdog('cheetahmail_nl', 'Error on NL Campaign Scheduling: Nor campaign has started yet.', array(), WATCHDOG_DEBUG);
        return;
      }
      // 24*3600 = 1 day.
      $scheduled_timestamp = $record['startedat'] + $form_state['values']['cheetahmail_nl_sending_interval'] * 24 * 3600;

      $now_time = REQUEST_TIME;
      $cheetahmail_nl_start = array(
        'month_day' => NULL,
        'week_day' => NULL,
        'start_hour' => NULL,
      );
      $cheetahmail_nl_start = array_merge($cheetahmail_nl_start, variable_get('cheetahmail_nl_start', array()));
      $wish_date = $now_time;
      $week_days = date_week_days();
      switch (variable_get('cheetahmail_nl_sending_interval')) {
        case '1':
          $wish_date = mktime($cheetahmail_nl_start['start_hour'], 0, 0, date("m"), date("d"), date("Y"));
          break;

        case '7':
          $wish_date = strtotime("last " . $week_days[$cheetahmail_nl_start['week_day'] - 1] . " " . $cheetahmail_nl_start['start_hour'] . ":00:00");
          break;

        case '30':
          $wish_date = mktime($cheetahmail_nl_start['start_hour'], 0, 0, date("m"), $cheetahmail_nl_start['month_day'], date("Y"));
          break;
      }
      $scheduled_timestamp = $wish_date;

      drupal_set_message(t("The Newsletter scheduled at %date", array('%date' => format_date($scheduled_timestamp, $type = 'medium'))));
      break;

    case CHEETAHMAIL_NL_SEND_STOP:
      cheetahmail_nl_campaign_stop($campaign_id = variable_get('experian/cheetahmail/nl/campaign_id'));
      break;

    default:
      // Here is nothing to do.
      break;

  }
}

/**
 * Callback for `Sent a test` email
 * via cheetahmail_nl_admin_menu_configs_campaign_form
 */
function cheetahmail_nl_admin_menu_configs_campaign_form_sendtest(&$form, &$form_state) {
  cheetahmail_nl_set_test_email($form_state['values']['test_email']);
  $res = cheetahmail_nl_send_campaign($campaign_id = variable_get('experian/cheetahmail/nl/test_campaign_id'));
  if (!empty($res)) {
    drupal_set_message(t("The test was sent to %mail.", array('%mail' => $form_state['values']['test_email'])));
  }
}

/**
 * Set abd subscribe the email address for receiving test email campaign.
 * Helper function subscribes the $email into EMS and updates Filter
 * for test campaign.
 */
function cheetahmail_nl_set_test_email($email) {
  // Add test_email as subscriber if is missing.
  $subscriber_id = cheetahmail_api_get_helper()->ems_subscribers_FindByEmail($email);
  if (!empty($subscriber_id) && EMS_CHEETAHMAIL_UNREGISTERED != $subscriber_id) {
    // Existing Email.
    $subscriber_info = cheetahmail_api_get_helper()->ems_subscribers_Get($subscriber_id);

    if (isset($subscriber_info->ArrayOfString[EMS_CHEETAHMAIL_SUBSCRIBER_STATUS_FIELD_ID]->string[1]) &&
      EMS_CHEETAHMAIL_SUBSCRIBED == $subscriber_info->ArrayOfString[EMS_CHEETAHMAIL_SUBSCRIBER_STATUS_FIELD_ID]->string[1]) {
      // Nothing to do.
    }
    else {
      // Change status of the existing subcriber
      // from Unsubscribed -> to Subscriber here.
      $result = cheetahmail_api_get_helper()->ems_subscribers_Update($subscriber_id, array(array(EMS_CHEETAHMAIL_SUBSCRIBER_STATUS_FIELD_ID, EMS_CHEETAHMAIL_SUBSCRIBED)));
      drupal_set_message(t('The email address `%email` for test emails successfully subscribed.', array('%email' => $email)), $type = 'status');
    }
  }
  else {
    // New email.
    $subscriber_id = cheetahmail_api_get_helper()->ems_subscribers_Add($email);
    drupal_set_message(t('The email address `%email` for test emails successfully subscribed.', array('%email' => $email)), $type = 'status');
  }
  variable_set('experian/cheetahmail/nl/test_email', $email);

  $filter_fields = array(
    'IdField' => EMS_CHEETAHMAIL_SUBSCRIBER_IDUSER_FIELD_ID,
    'Operation' => 'EQUAL',
    'Value' => $email,
  );
  $result = cheetahmail_api_get_helper()->ems_filters_SetFields($test_filter_id = variable_get('experian/cheetahmail/nl/test_filter_id'), $filter_fields);
}

/**
 * Provide Cheetahmail NL campaign settings form
 */
function cheetahmail_nl_admin_menu_configs_campaign_form() {
  $configs = cheetahmail_nl_get_campaign_configs();

  $form = cheetahmail_api_get_campaign_configuration_form($configs);

  $form['vtabs']['messageParams']['cheetahmail_tokens'] = array(
    '#title' => t('Replacement patterns'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['vtabs']['messageParams']['cheetahmail_tokens']['nl_view_content'] = array(
    '#type' => 'markup',
    '#markup' => theme('table', array(
      'header' => array('Token', 'Description'),
      'rows' => array(
        array('[view:CHEETAHMAIL_NL_CONTENT]', t('The content of the `Cheetahmail Newsletter` view embedded accoring to NL configuration into the Campaign content.')),
      ))),
  );
  if (module_exists('token')) {
    $form['vtabs']['messageParams']['token_help'] = array(
      '#title' => t('Replacement patterns'),
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
    $form['vtabs']['messageParams']['token_help']['browser'] = array(
      '#theme' => 'token_tree',
      '#token_types' => array('cheetahmail', 'cheetahmail-nl'),
    );
  }

  $form['actions'] = array(
    '#type' => 'actions',
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );

  return $form;
}

/**
 * Validate callback for `cheetahmail_admin_menu_nlconfigs_form` form
 */
function cheetahmail_nl_admin_menu_configs_campaign_form_validate(&$form, &$form_state) {
  if (!valid_email_address($form_state['values']['mailReply'])) {
    form_set_error('mailReply', 'Please, provide a valid email address.');
  }

  if (!valid_email_address($form_state['values']['mailRetNpai'])) {
    form_set_error('mailRetNpai', 'Please, provide a valid email address.');
  }
  $mail_from_addr = empty($form_state['values']['mailFromAddr']) ? '' : $form_state['values']['mailFromAddr'] . '@example.com';
  if (!valid_email_address($mail_from_addr)) {
    form_set_error('mailFromAddr', 'Please, provide a valid email address.');
  }
}

/**
 * Submit callback for `cheetahmail_admin_menu_nlconfigs_form` form
 */
function cheetahmail_nl_admin_menu_configs_campaign_form_submit(&$form, &$form_state) {
  $configs = array_merge(array(), $form_state['values']);

  unset($configs['op']);
  unset($configs['form_build_id']);
  unset($configs['form_token ']);
  unset($configs['form_id']);
  unset($configs['submit']);

  $config_id = variable_get('experian/cheetahmail/nl/config_id');
  $configs['idConf'] = $config_id;

  // Update Campaign Configs.
  if (cheetahmail_api_get_helper()->ems_configs_UpdateConfig($configs)) {
    $domains = cheetahmail_api_get_helper()->ems_configs_ListDomain();

    if (!empty($domains[$configs['domain']])) {
      $domain_id = cheetahmail_api_get_helper()->ems_configs_setDomain($config_id, $configs['domain']);
    }

    // Update Campaign Message.
    if (!empty($configs['messageParams'])) {
      variable_set('experian/cheetahmail/nl/message_params', $configs['messageParams']);
    }

    drupal_set_message(t('The settings are saved.'), $type = "status");
    return;
  }

  drupal_set_message(t('Failed to save the settings.'), $type = 'error');
}

/**
 * A History list of NL sent
 */
function cheetahmail_nl_admin_menu_reports_campaign_list() {
  $header = array(
    array('data' => t('ID'), 'field' => 'cc_list.campaign_id'),
    t('Description'),
    array('data' => t('Status'), 'field' => 'cc_list.status'),
    array('data' => t('Date created'), 'field' => 'cc_list.createdat'),
    array(
      'data' => t('Date started'),
      'field' => 'cc_list.startedat',
      'sort' => 'desc',
    ),
  );

  // Select with Pagination and Sorting.
  $q = db_select('cheetahmail_campaigns', 'cc_list')->extend('PagerDefault')->extend('TableSort');

  // Fields.
  $q->fields('cc_list', array('campaign_id'));
  $q->fields('cc_list', array('description'));
  $q->fields('cc_list', array('status'));
  $q->fields('cc_list', array('createdat'));
  $q->fields('cc_list', array('startedat'));
  $q->fields('cc_list', array('is_finished'));

  // Conditions.
  $campaigns = array();
  $campaigns[] = variable_get('experian/cheetahmail/nl/test_campaign_id');
  $campaigns[] = variable_get('experian/cheetahmail/nl/campaign_id');
  $q->condition('parent_id', $campaigns, 'IN');

  // Grouping.
  // Sorting.
  // Execute Query.
  $result = $q->limit(20)->orderByHeader($header)->execute();

  $rows = array();

  while ($record = $result->fetchAssoc()) {
    $record['startedat'] = empty($record['startedat']) ? '---' : format_date($record['startedat']);
    $record['status'] = empty($record['is_finished']) ? t('Created') : t('Sent');
    $rows[] = array(
      array('data' => $record['campaign_id']),
      array('data' => $record['description']),
      array('data' => $record['status']),
      array('data' => format_date($record['createdat'])),
      array('data' => $record['startedat']),
    );
  }

  $build['nl_log_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#attributes' => array('id' => 'admin-nllog'),
    '#empty' => t('No log Newsletters available.'),
  );
  $build['nl_log_pager'] = array('#theme' => 'pager');

  return $build;
}
