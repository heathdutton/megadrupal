<?php

/**
 * @file
 * Administration callbacks and forms for the media_theplatform_mpx module.
 */

/**
 * Page callback: Display a list of mpx accounts.
 *
 * @return array
 *   A render array.
 */
function media_theplatform_mpx_account_list() {
  $header = array(
    array('data' => t('ID'), 'field' => 'id'),
    array('data' => t('Username'), 'field' => 'username', 'sort' => 'ASC'),
    array('data' => t('Import account'), 'field' => 'import_account'),
    array('data' => t('Default player')),
    array('data' => t('Imported videos'), 'field' => 'video_count'),
    array('data' => t('Operations')),
  );
  $query = db_select('mpx_accounts', 'mpxa')
    ->extend('TableSort')
    ->extend('PagerDefault');
  $query->fields('mpxa', array('id'));
  $query->leftJoin('mpx_video', 'mpxv', 'mpxv.parent_account = mpxa.id');
  $query->addExpression('COUNT(mpxv.video_id)', 'video_count');
  $query->groupBy('mpxa.id');
  $query->orderByHeader($header);
  $query->limit(25);
  $results = $query->execute()->fetchAllAssoc('id');
  $accounts = MpxAccount::loadMultiple(array_keys($results));

  $rows = array();
  foreach ($results as $id => $result) {
    $account = $accounts[$id];
    $row = array('data' => array());
    $row['data'][] = $id;
    $row['data'][] = check_plain($account->username);

    if (!empty($account->import_account)) {
      $row['data'][] = check_plain($account->import_account);
    }
    else {
      $row['data'][] = l(t('Not configured'), 'admin/config/media/theplatform/accounts/' . $id . '/edit');
      $row['class'][] = 'error';
    }

    if (!empty($account->default_player)) {
      if ($player = media_theplatform_mpx_get_mpx_player_by_player_id($account->default_player)) {
        $row['data'][] = check_plain($player['title']);
      }
      else {
        $row['data'][] = t('Unable to load (file @fid)', array('@fid' => $account->default_player));
        $row['class'][] = 'error';
      }
    }
    elseif (media_theplatform_mpx_get_players_select($account)) {
      $row['data'][] = l(t('Not configured'), 'admin/config/media/theplatform/accounts/' . $id . '/edit');
      $row['class'][] = 'error';
    }
    else {
      $row['data'][] = l(t('Players not imported'), 'admin/content/file/mpxplayer', array('query' => drupal_get_destination()));
      $row['class'][] = 'error';
    }

    // Imported video count.
    $row['data'][] = $result->video_count;

    $operations = array();
    $operations['account-status'] = array(
      'title' => t('Status'),
      'href' => 'admin/config/media/theplatform/accounts/' . $id,
    );
    $operations['ingest'] = array(
      'title' => t('Run ingestion'),
      'href' => 'admin/config/media/theplatform/accounts/' . $id . '/ingest',
      'query' => array(
        'token' => drupal_get_token('admin/config/media/theplatform/accounts/' . $id . '/ingest'),
      ) + drupal_get_destination(),
    );
    $operations['edit'] = array(
      'title' => t('Edit'),
      'href' => 'admin/config/media/theplatform/accounts/' . $id . '/edit',
    );
    $operations['delete'] = array(
      'title' => t('Delete'),
      'href' => 'admin/config/media/theplatform/accounts/' . $id . '/delete',
    );
    foreach ($operations as $id => $operation) {
      if (!drupal_valid_path($operation['href'])) {
        unset($operations[$id]);
      }
    }
    $row['data'][] = array('data' => array(
      '#theme' => 'links',
      '#links' => $operations,
      '#attributes' => array(
        'class' => array('links', 'inline'),
      ),
    ));
    $rows[] = $row;
  }

  $output = array();
  $output['table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No mpx accounts available.'),
  );
  $output['pager'] = array(
    '#theme' => 'pager',
  );
  return $output;
}

/**
 * Page callback: Display a status report for an mpx account.
 *
 * @param MpxAccount $account
 *   The mpx account.
 *
 * @return array
 *   A render array.
 */
function media_theplatform_mpx_account_status_report_page(MpxAccount $account) {
  include_once DRUPAL_ROOT . '/includes/install.inc';

  $status = module_invoke_all('media_theplatform_mpx_account_status', $account);
  drupal_alter('media_theplatform_mpx_account_status', $status, $account);

  $output['report'] = array(
    '#markup' => theme('status_report', array('requirements' => $status)),
  );

  // Add a button link to run ingestion and return back here to the status
  // page.
  $output['ingest'] = array(
    '#type' => 'link',
    '#title' => t('Run ingestion'),
    '#href' => 'admin/config/media/theplatform/accounts/' . $account->id . '/ingest',
    '#options' => array(
      'query' => array(
        'token' => drupal_get_token('admin/config/media/theplatform/accounts/' . $account->id . '/ingest'),
      ) + drupal_get_destination(),
    ),
    '#attributes' => array(
      'class' => array('button'),
    ),
  );

  return $output;
}

/**
 * Page callback: Run ingestion once for an mpx account.
 *
 * @param MpxAccount $account
 *   The mpx account.
 */
function media_theplatform_mpx_account_ingest(MpxAccount $account) {
  // Validate the security token.
  if (!isset($_GET['token']) || $_GET['token'] !== drupal_get_token($_GET['q'])) {
    return MENU_ACCESS_DENIED;
  }

  module_load_include('inc', 'media_theplatform_mpx', 'media_theplatform_mpx.batch');
  batch_set(media_theplatform_mpx_batch_ingest_info(array($account)));
  batch_process('admin/config/media/theplatform/accounts/' . $account->id . '/status');
}

/**
 * Form constructor for the add mpx account form.
 *
 * @see media_theplatform_mpx_account_add_form_validate()
 * @see media_theplatform_mpx_account_add_form_submit()
 *
 * @ingroup forms
 */
function media_theplatform_mpx_account_add_form($form, &$form_state) {
  drupal_set_title(t('Add mpx account'));

  $form['username'] = array(
    '#type' => 'textfield',
    '#title' => t('Username'),
    '#description' => t('The username should be prefixed with <em>mpx/</em>. It is best to use an account that has the <em>Admin (Read-Only)</em> role in thePlatform.'),
    '#placeholder' => 'mpx/username@example.com',
    '#required' => TRUE,
    '#size' => 40,
  );

  $form['password'] = array(
    '#type' => 'password',
    '#title' => t('Password'),
    '#required' => TRUE,
    '#size' => 40,
    '#element_validate' => array('media_theplatform_mpx_account_validate_password'),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Continue and set import account'),
  );
  $form['actions']['cancel'] = array(
    '#type' => 'link',
    '#title' => t('Cancel'),
    '#href' => 'admin/config/media/theplatform/accounts',
  );

  return $form;
}

/**
 * Render API callback: Validates the mpx username and password.
 *
 * This function is assigned as an #element_validate callback in
 * media_theplatform_mpx_account_add_form() and
 * media_theplatform_mpx_account_edit_form().
 */
function media_theplatform_mpx_account_validate_password(&$element, &$form_state) {
  if (!empty($element['#value']) && !form_get_errors()) {
    try {
      $token = MpxToken::fetch($form_state['values']['username'], $element['#value']);
      $token->expire();
    }
    catch (Exception $e) {
      form_error($element, t('Unable to validate mpx account credentials.') . ' ' . $e->getMessage());
    }
  }
}

/**
 * Form validation handler for media_theplatform_mpx_account_add_form().
 *
 * @see media_theplatform_mpx_account_add_form_submit()
 */
function media_theplatform_mpx_account_add_form_validate($form, &$form_state) {
  if (!empty($form_state['values']['username']) && !empty($form_state['values']['password']) && !form_get_errors()) {
    $account = MpxAccount::create($form_state['values']);
    // Check if an unused import account can be selected.
    if (!$account->getImportAccountOptions()) {
      form_set_error('', t('The @account does not have any available or unused import accounts.', array('@account' => (string) $account)));
    }
  }
}

/**
 * Form submission handler for media_theplatform_mpx_account_add_form().
 *
 * @see media_theplatform_mpx_account_add_form_validate()
 */
function media_theplatform_mpx_account_add_form_submit($form, &$form_state) {
  form_state_values_clean($form_state);
  $account = MpxAccount::create($form_state['values']);
  $account->save();
  watchdog('media_theplatform_mpx', 'Created @account.', array('@account' => (string) $account), WATCHDOG_INFO);
  // Redirect to the edit form to select import account and default player.
  $form_state['redirect'] = 'admin/config/media/theplatform/accounts/' . $account->id . '/edit';
}

/**
 * Form constructor for the edit mpx account form.
 *
 * @param MpxAccount $account
 *   The mpx account to edit.
 *
 * @see media_theplatform_mpx_account_edit_form_validate()
 * @see media_theplatform_mpx_account_edit_form_submit()
 *
 * @ingroup forms
 */
function media_theplatform_mpx_account_edit_form($form, &$form_state, MpxAccount $account) {
  drupal_set_title(t('Edit @account', array('@account' => (string) $account)));

  $form['username_display'] = array(
    '#type' => 'item',
    '#title' => t('Username'),
    '#markup' => $account->username,
  );

  $form['password'] = array(
    '#type' => 'password',
    '#title' => t('Password'),
    '#description' => t('For updating the password on your account, otherwise leave blank.'),
    '#size' => 30,
    '#element_validate' => array('media_theplatform_mpx_account_validate_password'),
  );

  $import_account_options = $account->getImportAccountOptions();
  if (!empty($account->import_account)) {
    $form['import_account_display'] = array(
      '#type' => 'item',
      '#title' => t('Import account'),
      '#markup' => $account->import_account,
    );
    if (!isset($import_account_options[$account->import_account])) {
      $form['import_account_display']['#markup'] .= '<div class="messages error">' . t('The import account does not appear to exist.', array('@name' => $account->import_account)) . '</div>';
    }
  }
  else {
    $form['import_account'] = array(
      '#type' => 'select',
      '#title' => t('Import account'),
      '#description' => t('This can only be set once.'),
      '#options' => $import_account_options,
      '#empty_option' => t('- Select -'),
      '#default_value' => $account->import_account,
      '#required' => TRUE,
    );
  }

  $default_player_options = !empty($account->import_account) ? media_theplatform_mpx_get_players_select($account) : array();
  $form['default_player'] = array(
    '#type' => 'select',
    '#title' => t('Default player'),
    '#options' => $default_player_options,
    '#empty_option' => t('- Select -'),
    '#default_value' => $account->default_player,
    '#required' => TRUE,
    '#access' => !empty($default_player_options),
  );

  foreach (get_object_vars($account) as $key => $value) {
    if (!isset($form[$key])) {
      $form[$key] = array(
        '#type' => 'value',
        '#value' => $value,
      );
    }
  }

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => empty($default_player_options) ? t('Continue and set default player') : t('Save'),
  );
  $form['actions']['cancel'] = array(
    '#type' => 'link',
    '#title' => t('Cancel'),
    '#href' => 'admin/config/media/theplatform/accounts',
  );

  return $form;
}

/**
 * Form submission handler for media_theplatform_mpx_account_edit_form().
 *
 * @see media_theplatform_mpx_account_edit_form_validate()
 */
function media_theplatform_mpx_account_edit_form_submit($form, &$form_state) {
  form_state_values_clean($form_state);
  $account = MpxAccount::create($form_state['values']);
  // If the user has not typed in a new password, then we should ensure it does
  // not get set to empty.
  if (empty($account->password)) {
    $account->password = $form_state['build_info']['args'][0]->password;
  }
  // Ensure that the default_player field will not get set to '0'.
  if (empty($account->default_player)) {
    $account->default_player = NULL;
  }
  $account->save();

  if (empty($account->default_player)) {
    watchdog('media_theplatform_mpx', 'Updated @account.', array('@account' => (string) $account), WATCHDOG_INFO);
    $players = media_theplatform_mpx_import_account_players($account);
    if (empty($players)) {
      drupal_set_message(t('Unable to import players for the account.'), 'error');
    }
    $form_state['redirect'] = 'admin/config/media/theplatform/accounts/' . $account->id . '/edit';
  }
  else {
    watchdog('media_theplatform_mpx', 'Updated @account.', array('@account' => (string) $account), WATCHDOG_INFO);
    if (!$account->getDataValue('last_notification')) {
      $methods = array();
      if (variable_get('media_theplatform_mpx__cron_videos', 1)) {
        if (user_access('administer site configuration')) {
          $methods[] = l(t('Running cron'), 'admin/reports/status/run-cron', array('query' => drupal_get_destination()));
        }
        else {
          $methods[] = t('Running cron');
        }
      }
      $methods[] = t('Using the drush mpxi or mpxia commands from the command line');
      $methods[] = t('Manually using the <a href="@link"><em>Run ingestion</em></a> link', array('@link' => url('admin/config/media/theplatform/accounts/' . $account->id . '/ingest', array('query' => array('token' => drupal_get_token('admin/config/media/theplatform/accounts/' . $account->id . '/ingest')) + drupal_get_destination()))));
      drupal_set_message(t('The @account is now ready for video ingestion using any of the following methods: !methods', array('@account' => $account, '!methods' => theme('item_list', array('items' => $methods)))));
    }
    else {
      drupal_set_message(t('The @account has been saved.', array('@account' => $account)));
    }
    $form_state['redirect'] = 'admin/config/media/theplatform/accounts';
  }
}

/**
 * Form constructor for the edit mpx account form.
 *
 * @param MpxAccount $account
 *   The mpx account to delete.
 *
 * @see media_theplatform_mpx_account_delete_form_submit()
 *
 * @ingroup forms
 */
function media_theplatform_mpx_account_delete_form($form, &$form_state, MpxAccount $account) {
  return confirm_form(
    $form,
    t('Are you sure you want to delete the mpx account %account?', array('%account' => $account->username)),
    'admin/config/media/theplatform/accounts',
    t('This only deletes data from the local Drupal install, not from the actual mpx account on thePlatform. This action cannot be undone.')
  );
}

/**
 * Form submission handler for media_theplatform_mpx_account_delete_form().
 */
function media_theplatform_mpx_account_delete_form_submit($form, &$form_state) {
  $account = $form_state['build_info']['args'][0];
  _media_theplatform_mpx_delete_account($account->id);
  //drupal_set_message(t('The mpx account %account has been deleted.', array('%account' => $account->username)));
  watchdog('media_theplatform_mpx', 'Deleted @account.', array('@account' => (string) $account), WATCHDOG_INFO);
  $form_state['redirect'] = 'admin/config/media/theplatform/accounts';
}

/**
 * Form constructor for the mpx settings form.
 *
 * @ingroup forms
 */
function media_theplatform_mpx_settings_form($form, &$form_state) {
  $form['cron'] = array(
    '#type' => 'fieldset',
    '#title' => t('Cron'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['cron']['media_theplatform_mpx__cron_players'] = array(
    '#type' => 'checkbox',
    '#title' => t('Sync mpxPlayers on Cron'),
    '#default_value' => variable_get('media_theplatform_mpx__cron_players', 1),
  );
  $form['cron']['media_theplatform_mpx__cron_videos'] = array(
    '#type' => 'checkbox',
    '#title' => t('Sync mpxMedia on Cron'),
    '#default_value' => variable_get('media_theplatform_mpx__cron_videos', 1),
  );
  $form['cron']['media_theplatform_mpx__cron_videos_per_run'] = array(
    '#type' => 'numberfield',
    '#title' => t('Media Processing Limit'),
    '#description' => t('The number of videos to pull from the media feed per cron run. Maximum of 500.'),
    '#default_value' => variable_get('media_theplatform_mpx__cron_videos_per_run', 100),
    '#min' => 0,
    '#max' => 500,
  );
  $form['cron']['media_theplatform_mpx__cron_videos_timeout'] = array(
    '#type' => 'numberfield',
    '#title' => t('Media Feed Request Timeout'),
    '#description' => t('The amount in seconds to wait for a response from a media feed request.'),
    '#default_value' => variable_get('media_theplatform_mpx__cron_videos_timeout', 180),
    '#min' => 1,
  );
  $form['cron']['media_theplatform_mpx__cron_queue_processing_time'] = array(
    '#type' => 'numberfield',
    '#title' => t('Cron Queue Processing Time'),
    '#description' => t('The amount of time, in seconds, to allot to the cron queue for processing during each cron run.'),
    '#default_value' => variable_get('media_theplatform_mpx__cron_queue_processing_time', 10),
    '#min' => 1,
  );

  $form['logging'] = array(
    '#type' => 'fieldset',
    '#title' => t('Logging'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['logging']['media_theplatform_mpx__output_message_watchdog_severity'] = array(
    '#type' => 'select',
    '#title' => t('Display MPX Debug Message Level'),
    '#options' => array(
      WATCHDOG_NOTICE => t('Basic'),
      WATCHDOG_INFO => t('Advanced'),
      WATCHDOG_DEBUG => t('Extremely Verbose'),
    ),
    '#description' => t('Determines how much information is reported to the user after MPX operations.
<br />Basic - minimal information provided about player, video and setting changes
<br />Advanced - provides feedback on all calls to thePlatform, database inserts/updates and file system writes
<br />Extremely Verbose - outputs all data received from thePlatform and all data inserted/updated in the database.'),
    '#default_value' => variable_get('media_theplatform_mpx__output_message_watchdog_severity', WATCHDOG_INFO),
  );
  $form['logging']['media_theplatform_mpx__watchdog_severity'] = array(
    '#type' => 'select',
    '#title' => t('MPX Debug Logging Level'),
    '#options' => array(
      WATCHDOG_NOTICE => t('Basic'),
      WATCHDOG_INFO => t('Advanced'),
      WATCHDOG_DEBUG => t('Extremely Verbose'),
    ),
    '#description' => t('Determines how much information is saved in logs during MPX operations.'),
    '#default_value' => variable_get('media_theplatform_mpx__watchdog_severity', WATCHDOG_INFO),
  );

  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['advanced']['media_theplatform_mpx_notification_size'] = array(
    '#type' => 'numberfield',
    '#title' => t('Override maximum number of notifications in a request'),
    '#description' => t('Leave blank to use the default limit, which is 500. Maximum: 500. Only change this if requests to data.media.theplatform.com/media/notify are encountering timeouts.'),
    '#default_value' => variable_get('media_theplatform_mpx_notification_size', 500),
    '#min' => 1,
    '#max' => 500,
  );
  $form['advanced']['media_theplatform_mpx__token_ttl'] = array(
    '#type' => 'numberfield',
    '#title' => t('Override authentication token lifetime'),
    '#description' => t('Leave blank to use the default token lifetime. Minimum: 3 seconds. Maximum: @max (@max-raw seconds)', array('@max' => format_interval(MpxToken::MAX_TTL), '@max-raw' => MpxToken::MAX_TTL)),
    '#default_value' => variable_get('media_theplatform_mpx__token_ttl'),
    '#field_suffix' => t('seconds'),
    '#min' => 3,
    '#max' => MpxToken::MAX_TTL,
  );
  $form['advanced']['media_theplatform_mpx_debug'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable debugging mode'),
    '#default_value' => variable_get('media_theplatform_mpx_debug', 0),
  );

  return system_settings_form($form);
}

/**
 *
 * Page callback: Display a status report for all mpx accounts.
 *
 * @return array
 *   A render array.
 */
function media_theplatform_mpx_status_report_page() {
  $output = array();
  $accounts = MpxAccount::loadAll();
  foreach ($accounts as $account) {
    $output[$account->id] = array(
      '#type' => 'fieldset',
      '#title' => (string) $account,
    );
    $output[$account->id] += media_theplatform_mpx_account_status_report_page($account);
    $output[$account->id]['ingest']['#access'] = FALSE;
  }

  return $output;
}

/**
 * @deprecated
 *
 * Returns TRUE if token and import_account set.  If not, drupal_set_message and returns FALSE.
 */
function media_theplatform_mpx_check_account_settings($account_id = NULL) {

  // If an account ID is passed, check settings for that account.  Otherwise,
  // check all available accounts that at least one account is configured
  // properly.
  $account_configured = FALSE;
  $accounts = $account_id ? MpxAccount::loadMultiple(array($account_id)) : MpxAccount::loadAll();
  foreach ($accounts as $account) {
    if ($account->acquireToken() && !empty($account->import_account)) {
      $account_configured = TRUE;
      break;
    }
  }

  if (!$account_configured) {
    if (user_access('administer mpx account')) {
      drupal_set_message(t('Your mpx Account is not configured.') . ' ' . l(t('Configure mpx Account.'), 'admin/config/media/theplatform'), 'error');
    }
    else {
      drupal_set_message(t('Your mpx Account is not configured.') . ' ' . t('Please contact your System Administrator.'), 'error');
    }
    return FALSE;
  }

  return TRUE;
}

/******************* mpxPlayers *****************************/

/**
 * Page callback to return all mpx_players and forms.
 */
function media_theplatform_mpx_page_mpx_players() {
  $output = array();
  // Display forms if signin and import_account
  if (media_theplatform_mpx_check_account_settings() && (user_access('sync mpx_player') || user_access('administer mpx account'))) {
    $output['sync'] = drupal_get_form('media_theplatform_mpx_form_mpx_player_sync');
  }
  $output += media_theplatform_mpx_get_table_mpx_players();
  return $output;
}

/**
 * Form constructor for mpxPlayer Sync.
 *
 * @ingroup forms
 */
function media_theplatform_mpx_form_mpx_player_sync($form, &$form_state) {
  $form['player_sync'] = array(
    '#type' => 'fieldset',
    '#title' => t('Sync mpxPlayers'),
    '#description' => t('Note: Any mpxPlayer marked as "Disabled" in thePlatform mpx will not be retrieved in the Sync process.'),
    '#collapsible' => TRUE,
    '#collapsed' => !((bool) db_query("SELECT 1 FROM {mpx_accounts} mpxa LEFT JOIN {mpx_player} mpxp ON mpxa.id = mpxp.parent_account WHERE mpxp.player_id IS NULL")->fetchField()),
  );
  $form['player_sync']['actions'] = array('#type' => 'actions');
  $form['player_sync']['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Sync mpxPlayers Now'),
  );
  return $form;
}

/**
 * Form submit handler for media_theplatform_mpx_form_mpx_player_sync().
 */
function media_theplatform_mpx_form_mpx_player_sync_submit($form, &$form_state) {
  media_theplatform_mpx_import_all_players('manually');
}

/**
 * Returns a themed table of mpx_player data.
 */
function media_theplatform_mpx_get_table_mpx_players() {
  $header = array(
    // The header gives the table the information it needs in order to make
    // the query calls for ordering. TableSort uses the field information
    // to know what database column to sort by.
    array('data' => t('ID'), 'field' => 'p.player_id'),
    array('data' => NULL),
    array('data' => t('Title'), 'field' => 'p.title'),
    array('data' => t('Description'), 'field' => 'p.description'),
    // array('data' => t('mpx ID'), 'field' => 'p.id'),
    array('data' => t('Status'), 'field' => 'p.status'),
    array('data' => t('First Imported'), 'field' => 'p.created'),
    array('data' => t('Last Updated'), 'field' => 'p.updated'),
  );

  $query = db_select('mpx_player', 'p')
    ->extend('TableSort')
    ->extend('PagerDefault');
  $query->fields('p');
  $query->orderByHeader($header);
  $query->limit(25);
  $results = $query->execute();

  $rows = array();
  foreach ($results as $player) {
    if ($player->fid == variable_get('media_theplatform_mpx__default_player_fid')) {
      $default = '[default]';
    }
    else {
      $default = NULL;
    }
    $rows[] = array(
      $player->player_id,
      $default,
      l($player->title, 'file/' . $player->fid),
      $player->description,
      // $player->id,
      $player->status ? t('Enabled') : t('Disabled'),
      format_date($player->created, 'short'),
      format_date($player->updated, 'short'),
    );
  }

  $output = array();
  $output['table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No mpxPlayers have been imported.'),
  );
  $output['pager'] = array(
    '#theme' => 'pager',
  );
  return $output;
}


/******************** mpxMedia *******************************/

/**
 * Page callback - display all mpx Video media and forms.
 */
function media_theplatform_mpx_page_mpx_videos() {
  $output = array();
  // If no mpxPlayers, you cant do anything with mpxMedia.
  if (!db_query("SELECT 1 FROM {mpx_player}")->fetchField()) {
    $output['#markup'] = t('No mpxPlayers have been imported.');
    if (user_access('sync mpx_player') || user_access('administer mpx account')) {
      $output['#markup'] .= ' ' . l(t('Sync mpxPlayers.'), 'admin/content/file/mpxplayer');
    }
    else {
      $output['#markup'] .= ' ' . t('Please contact your System Administrator.');
    }
    return $output;
  }
  if (media_theplatform_mpx_check_account_settings() && (user_access('sync mpx_video') || user_access('administer mpx account'))) {
    $output['sync'] = drupal_get_form('media_theplatform_mpx_form_mpx_video_sync');
  }
  $output += media_theplatform_mpx_get_table_mpx_videos();
  return $output;
}

/**
 * Form constructor for mpx_video Sync.
 *
 * @ingroup forms
 */
function media_theplatform_mpx_form_mpx_video_sync($form, &$form_state) {

  $form['video_sync'] = array(
    '#type' => 'fieldset',
    '#title' => t('Sync mpxMedia'),
    '#description' => t('Note: Any mpxMedia which has been unpublished or deleted from thePlatform mpx will be set to "Inactive".'),
    '#collapsible' => TRUE,
    '#collapsed' => media_theplatform_mpx_get_mpx_video_count() > 0 ? TRUE : FALSE,
    '#tree' => TRUE,
  );

  foreach (MpxAccount::loadAll() as $account) {
    $player_select = media_theplatform_mpx_get_players_select($account);
    if (empty($player_select)) {
      continue;
    }
    $form['video_sync']['settings'][ $account->id ]['default_player_id'] = array(
      '#type' => 'select',
      '#title' => t('Import new mpxMedia for account @account with mpxPlayer:',
        array('@account' => '"' . $account->import_account . '" (' . basename($account->account_id) . ')')),
      '#options' => $player_select,
      '#empty_option' => t('- Select -'),
      '#required' => TRUE,
      '#default_value' => $account->default_player,
    );
  }

  $form['video_sync']['actions'] = array('#type' => 'actions');
  $form['video_sync']['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Sync mpxMedia Now'),
  );

  return $form;
}

/**
 * Form submission handler for media_theplatform_mpx_form_mpx_video_sync().
 */
function media_theplatform_mpx_form_mpx_video_sync_submit($form, &$form_state) {

  // Set default mpxPlayer variable.
  foreach ($form_state['values']['video_sync']['settings'] as $account_id => $values) {
    $currently_stored_default_player = db_select('mpx_accounts', 'mpx_accounts')
      ->fields('mpx_accounts', array('default_player'))
      ->condition('id', $account_id, '=')
      ->execute()
      ->fetchField();
    if ($currently_stored_default_player != $values['default_player_id']) {
      $account = MpxAccount::load($account_id);
      $account->default_player = $values['default_player_id'];
      $account->save();
      $player = media_theplatform_mpx_get_mpx_player_by_player_id($values['default_player_id']);
      watchdog('media_theplatform_mpx', 'Player @pid -- @player -- set as default player for account @account.',
        array(
          '@pid' => $player['pid'],
          '@player' => $player['title'],
          '@account' => basename($account->account_id),
        ),
        WATCHDOG_NOTICE);
    }
  }

  // Import videos.
  media_theplatform_mpx_import_all_videos('manually');

  // Only show this message about cron if "Sync mpxMedia on Cron" is enabled.
  if (variable_get('media_theplatform_mpx__cron_videos', 1)) {
    $message = t('Cron must be run to begin the import/update process.');
    if (user_access('administer site configuration')) {
      $cron_url = url('admin/reports/status/run-cron', array('query' => drupal_get_destination()));
      $message .= ' ' . t('<a href="@cron">Run cron now.</a>', array('@cron' => $cron_url));
    }
    drupal_set_message($message);
  }
}

/**
 * Returns themed table of mpx_video data.
 */
function media_theplatform_mpx_get_table_mpx_videos() {
  $header = array(
    // The header gives the table the information it needs in order to make
    // the query calls for ordering. TableSort uses the field information
    // to know what database column to sort by.
    array('data' => t('ID'), 'field' => 'v.video_id'),
    array('data' => NULL),
    array('data' => t('Title'), 'field' => 'v.title'),
    array('data' => t('Description'), 'field' => 'v.description'),
    // array('data' => t('mpx ID (Guid)'), 'field' => 'v.guid'),
    array('data' => t('Status'), 'field' => 'v.status'),
    array('data' => t('First Imported'), 'field' => 'v.created'),
    array('data' => t('Last Updated'), 'field' => 'v.updated'),
  );
  $query = db_select('mpx_video', 'v')
    ->extend('TableSort')
    ->extend('PagerDefault');
  $query->fields('v');
  $query->orderByHeader($header);
  $query->limit(50);
  $results = $query->execute();

  $rows = array();
  foreach ($results as $video) {
    $file = file_load($video->fid);
    $thumbnail = media_get_thumbnail_preview($file);
    $rows[] = array(
      $video->video_id,
      l(drupal_render($thumbnail), 'file/' . $video->fid, array('html' => TRUE, 'attributes' => array('class' => 'mpxmedia'))),
      $video->title,
      $video->description,
      // $video->guid,
      $video->status ? t('Enabled') : t('Disabled'),
      format_date($video->created, 'short'),
      format_date($video->updated, 'short'),
    );
  }

  $output = array();
  $output['table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No mpxMedia has been imported.'),
  );
  $output['pager'] = array(
    '#theme' => 'pager',
  );
  return $output;
}
