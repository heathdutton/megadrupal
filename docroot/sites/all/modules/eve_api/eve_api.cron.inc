<?php
/**
 * @file
 * Functions related to controlling and adding cron jobs.
 */

/**
 * Implements hook_cronapi().
 */
function eve_api_cronapi($op, $job = NULL) {
  if ($op == 'list') {
    $items = array();

    $items['eve_api_cron_api_mask_sync_job'] = array(
      'description' => 'Get the latest access mask from the EVE API.',
      'rule' => '12 1 * * 0',
    );

    $items['eve_api_cron_api_alliance_fetch_job'] = array(
      'description' => 'Get current alliance standings.',
      'rule' => '12 2 * * *',
    );

    $items['eve_api_cron_api_alliance_sync_job'] = array(
      'description' => 'Get current corporations in the alliance.',
      'rule' => '12 3 * * *',
    );

    $items['eve_api_cron_api_user_sync_job'] = array(
      'description' => 'Verify each characters API.',
      'rule' => '18 * * * *',
    );

    $items['eve_api_cron_api_user_wallet_journal_job'] = array(
      'description' => 'Import users daily wallet journal.',
      'rule' => '37 4 * * *',
    );

    return $items;
  }
  elseif ($op == 'execute') {
    if (!variable_get('eve_api_enable_cron', TRUE)) {
      return;
    }

    switch ($job) {
      case 'eve_api_cron_api_alliance_fetch_job':
        $queue = DrupalQueue::get('eve_api_cron_api_alliance_fetch');
        $queue->createItem(1);
        break;

      case 'eve_api_cron_api_alliance_sync_job':
        $queue = DrupalQueue::get('eve_api_cron_api_alliance_sync');
        $queue->createItem(1);
        break;

      case 'eve_api_cron_api_mask_sync_job':
        $queue = DrupalQueue::get('eve_api_cron_api_mask_sync');
        $queue->createItem(1);
        break;

      case 'eve_api_cron_api_user_sync_job':
        if (variable_get('eve_api_enable', FALSE)) {
          $queue = DrupalQueue::get('eve_api_cron_api_user_sync');
          $result = db_query('SELECT DISTINCT uid FROM {users} WHERE uid != 0');

          if ($result->rowCount()) {
            foreach ($result->fetchAll() as $item) {
              $queue->createItem(array(
                'uid' => $item->uid,
                'runs' => 1,
              ));
            }
          }
        }
        break;

      case 'eve_api_cron_api_user_wallet_journal_job':
        if (variable_get('eve_api_enable', FALSE)) {
          $queue = DrupalQueue::get('eve_api_cron_api_user_wallet_journal');
          $result = db_query('SELECT c.accessMask, c.characterID, k.keyID, k.vCode, k.errorCode FROM {eve_api_characters} c LEFT JOIN {eve_api_keys} k ON k.apiID = c.apiID WHERE c.deleted = 0');

          if ($result->rowCount()) {
            foreach ($result->fetchAll() as $item) {
              if (($item->accessMask & 2097152) != 2097152) {
                continue;
              }

              if (eve_api_error_code_deleted($item->errorCode)) {
                continue;
              }

              $queue->createItem(array(
                'query' => array(
                  'keyID' => $item->keyID,
                  'vCode' => $item->vCode,
                ),
                'characterID' => $item->characterID,
              ));
            }
          }
        }
        break;
    }
  }
}

/**
 * Implements hook_cron().
 */
function eve_api_cron() {
  if (!variable_get('eve_api_enable_cron', TRUE) || function_exists('elysia_cron_run')) {
    return;
  }

  if (time() >= variable_get('eve_api_cron_api_mask_next', 0) && date('w') == 0 && date('G') == 1 && date('i') >= 12) {
    $queue = DrupalQueue::get('eve_api_cron_api_mask_sync');
    $queue->createItem(1);

    variable_set('eve_api_cron_api_mask_next', time() + (1439 * 60));
  }

  if (time() >= variable_get('eve_api_cron_api_alliance_fetch_next', 0) && date('G') == 2 && date('i') >= 12) {
    $queue = DrupalQueue::get('eve_api_cron_api_alliance_fetch');
    $queue->createItem(1);

    variable_set('eve_api_cron_api_alliance_fetch_next', time() + (1439 * 60));
  }

  if (time() >= variable_get('eve_api_cron_api_alliance_sync_next', 0) && date('G') == 3 && date('i') >= 12) {
    $queue = DrupalQueue::get('eve_api_cron_api_alliance_sync');
    $queue->createItem(1);

    variable_set('eve_api_cron_api_alliance_sync_next', time() + (1439 * 60));
  }

  if (variable_get('eve_api_enable', FALSE)) {
    if (time() >= variable_get('eve_api_cron_api_users_next', 0) && date('i') >= 18) {
      $queue = DrupalQueue::get('eve_api_cron_api_user_sync');
      $result = db_query('SELECT DISTINCT uid FROM {users} WHERE uid != 0');

      if ($result->rowCount()) {
        foreach ($result->fetchAll() as $item) {
          $queue->createItem(array(
            'uid' => $item->uid,
            'runs' => 1,
          ));
        }
      }

      variable_set('eve_api_cron_api_users_next', time() + (59 * 60));
    }

    if (time() >= variable_get('eve_api_cron_api_user_wallet_journal_next', 0) && date('G') == 4 && date('i') >= 37) {
      $queue = DrupalQueue::get('eve_api_cron_api_user_wallet_journal');
      $result = db_query('SELECT c.accessMask, c.characterID, k.keyID, k.vCode, k.errorCode FROM {eve_api_characters} c LEFT JOIN {eve_api_keys} k ON k.apiID = c.apiID WHERE c.deleted = 0');

      if ($result->rowCount()) {
        foreach ($result->fetchAll() as $item) {
          if (($item->accessMask & 2097152) != 2097152) {
            continue;
          }

          if (eve_api_error_code_deleted($item->errorCode)) {
            continue;
          }

          $queue->createItem(array(
            'query' => array(
              'keyID' => $item->keyID,
              'vCode' => $item->vCode,
            ),
            'characterID' => $item->characterID,
          ));
        }
      }

      variable_set('eve_api_cron_api_user_wallet_journal_next', time() + (1439 * 60));
    }
  }
}

/**
 * Implements hook_cron_queue_info().
 */
function eve_api_cron_queue_info() {
  $queue = array();
  $queue['eve_api_cron_api_user_sync'] = array(
    'worker callback' => 'eve_api_cron_api_user_sync',
    'time' => 3600,
  );
  $queue['eve_api_cron_api_alliance_fetch'] = array(
    'worker callback' => 'eve_api_cron_api_alliance_fetch',
    'time' => 3600,
  );
  $queue['eve_api_cron_api_mask_sync'] = array(
    'worker callback' => 'eve_api_cron_api_mask_sync',
    'time' => 3600,
  );
  $queue['eve_api_cron_api_alliance_sync'] = array(
    'worker callback' => 'eve_api_cron_api_alliance_sync',
    'time' => 3600,
  );
  $queue['eve_api_cron_api_skill_tree'] = array(
    'worker callback' => 'eve_api_cron_api_skill_tree',
    'time' => 3600,
  );
  $queue['eve_api_cron_api_error_list'] = array(
    'worker callback' => 'eve_api_cron_api_error_list',
    'time' => 3600,
  );
  $queue['eve_api_cron_api_user_wallet_journal'] = array(
    'worker callback' => 'eve_api_cron_api_user_wallet_journal',
    'time' => 3600,
  );
  return $queue;
}

/**
 * Verify each characters API.
 *
 * @param mixed $item
 *   whatever was added to the queue via queue->createItem()
 *
 * @return bool
 *   TRUE or FALSE.
 */
function eve_api_cron_api_user_sync($item) {
  $uid = (int) $item['uid'];
  $account = user_load($uid);

  if ($uid == 0) {
    return FALSE;
  }

  $result = db_query('SELECT apiID, keyID, vCode, errorCode FROM {eve_api_keys} WHERE uid = :uid', array(
    ':uid' => $uid,
  ));

  $character_found = FALSE;
  $error_found = FALSE;

  if ($result->rowCount()) {
    foreach ($result->fetchAll() as $row) {
      if (eve_api_error_code_deleted($row->errorCode)) {
        continue;
      }

      $characters = eve_api_cron_update_key($account, $row->keyID, $row->vCode, $row->apiID);
      $error_found = ($error_found == TRUE) ? $error_found : $characters['error_found'];
      $character_found = ($character_found == TRUE) ? $character_found : $characters['character_found'];
    }
  }

  if ($error_found) {
    if ($item['runs'] == 2) {
      if (variable_get('eve_api_debug', FALSE)) {
        watchdog('eve_api', 'Cron update User API task has failed 2 times in a row, task was removed. Data: ' . drupal_http_build_query($item), NULL, WATCHDOG_ERROR);
      }

      return FALSE;
    }

    $item['runs'] = $item['runs'] + 1;
    $queue = DrupalQueue::get('eve_api_cron_api_user_sync');
    $queue->createItem($item);
    return FALSE;
  }

  $character_data = eve_api_get_basic_character_info($account->name);
  $character_is_blue = eve_api_verify_blue($character_data);

  if (!$character_is_blue || !$character_found) {
    $result = db_query('SELECT corporationID, corporationName FROM {eve_api_alliance_list_corporations}');

    if ($result->rowCount()) {
      foreach ($result->fetchAll() as $row) {
        if ($corporation_role = user_role_load_by_name($row->corporationName)) {
          user_multiple_role_edit(array($uid), 'remove_role', (int) $corporation_role->rid);
        }
      }
    }

    $blue_role = user_role_load(variable_get('eve_api_blue_role', 2));
    user_multiple_role_edit(array($uid), 'remove_role', $blue_role->rid);

    $alliance_role = user_role_load(variable_get('eve_api_alliance_role', 2));
    user_multiple_role_edit(array($uid), 'remove_role', $alliance_role->rid);

    $director_role = user_role_load(variable_get('eve_api_director_role', 2));
    user_multiple_role_edit(array($uid), 'remove_role', $director_role->rid);

    $ceo_role = user_role_load(variable_get('eve_api_ceo_role', 2));
    user_multiple_role_edit(array($uid), 'remove_role', $ceo_role->rid);

    $default_role = user_role_load(variable_get('eve_api_unverified_role', 2));
    user_multiple_role_edit(array($uid), 'add_role', $default_role->rid);

    module_invoke_all('eve_api_user_delete', $account);

    db_update('users')->fields(array(
        'characterID' => 0,
      ))
      ->condition('uid', $uid, '=')
      ->execute();
  }
  else {
    $result = db_query('SELECT corporationID, corporationName FROM {eve_api_alliance_list_corporations}');

    if ($corporation_role = user_role_load_by_name($character_data['corporationName'])) {
      if ($result->rowCount()) {
        foreach ($result->fetchAll() as $row) {
          if (($corporation_role_2 = user_role_load_by_name($row->corporationName)) && $row->corporationID != $character_data['corporationID']) {
            user_multiple_role_edit(array($uid), 'remove_role', $corporation_role_2->rid);
          }
        }
      }

      user_multiple_role_edit(array($uid), 'add_role', $corporation_role->rid);

      if (eve_api_character_is_director($character_data['characterID'])) {
        $director_role = user_role_load(variable_get('eve_api_director_role', 2));
        user_multiple_role_edit(array($uid), 'add_role', $director_role->rid);
      }
      else {
        $director_role = user_role_load(variable_get('eve_api_director_role', 2));
        user_multiple_role_edit(array($uid), 'remove_role', $director_role->rid);
      }

      if (eve_api_character_is_ceo($character_data['characterID'])) {
        $ceo_role = user_role_load(variable_get('eve_api_ceo_role', 2));
        user_multiple_role_edit(array($uid), 'add_role', $ceo_role->rid);
      }
      else {
        $ceo_role = user_role_load(variable_get('eve_api_ceo_role', 2));
        user_multiple_role_edit(array($uid), 'remove_role', $ceo_role->rid);
      }

      $default_role = user_role_load(variable_get('eve_api_unverified_role', 2));
      user_multiple_role_edit(array($uid), 'remove_role', $default_role->rid);

      $blue_role = user_role_load(variable_get('eve_api_blue_role', 2));
      user_multiple_role_edit(array($uid), 'remove_role', $blue_role->rid);

      $alliance_role = user_role_load(variable_get('eve_api_alliance_role', 2));
      user_multiple_role_edit(array($uid), 'add_role', $alliance_role->rid);
    }
    else {
      if ($result->rowCount()) {
        foreach ($result->fetchAll() as $row) {
          if ($corporation_role = user_role_load_by_name($row->corporationName)) {
            user_multiple_role_edit(array($uid), 'remove_role', $corporation_role->rid);
          }
        }
      }

      $default_role = user_role_load(variable_get('eve_api_unverified_role', 2));
      user_multiple_role_edit(array($uid), 'remove_role', $default_role->rid);

      $alliance_role = user_role_load(variable_get('eve_api_alliance_role', 2));
      user_multiple_role_edit(array($uid), 'remove_role', $alliance_role->rid);

      $director_role = user_role_load(variable_get('eve_api_director_role', 2));
      user_multiple_role_edit(array($uid), 'remove_role', $director_role->rid);

      $ceo_role = user_role_load(variable_get('eve_api_ceo_role', 2));
      user_multiple_role_edit(array($uid), 'remove_role', $ceo_role->rid);

      $blue_role = user_role_load(variable_get('eve_api_blue_role', 2));
      user_multiple_role_edit(array($uid), 'add_role', $blue_role->rid);
    }
  }

  module_invoke_all('eve_api_user_update', array(
    'account' => $account,
    'character' => $character_data,
    'character_is_blue' => $character_is_blue,
  ));

  return TRUE;
}

/**
 * Get current alliance standings.
 *
 * @param mixed $item
 *   whatever was added to the queue via queue->createItem()
 *
 * @return bool
 *   TRUE or FALSE.
 */
function eve_api_cron_api_alliance_fetch($item = NULL) {
  $key_id = variable_get('eve_api_corp_keyid', '');
  $v_code = variable_get('eve_api_corp_vcode', '');

  if (empty($key_id) || empty($v_code)) {
    return FALSE;
  }

  $query = array(
    'keyID' => $key_id,
    'vCode' => $v_code,
  );

  $response = eve_api_get_xml('ContactList', $query);

  if (!isset($response['xml']->result)) {
    if ($item == 2) {
      if (variable_get('eve_api_debug', FALSE)) {
        watchdog('eve_api', 'Cron Standings task has failed 2 times in a row, task was removed.', NULL, WATCHDOG_ERROR);
      }

      return FALSE;
    }

    $item = $item + 1;
    $queue = DrupalQueue::get('eve_api_cron_api_alliance_fetch');
    $queue->createItem($item);

    return FALSE;
  }

  db_update('eve_api_contact_list')->fields(array('checked' => 1))->condition('manual', 0, '=')->execute();

  foreach ($response['xml']->result->children() as $rowset) {
    foreach ($rowset->children() as $rows) {
      $row = $rows->attributes();

      db_merge('eve_api_contact_list')->key(array('contactID' => (int) $row->contactID))->fields(array(
        'contactID' => (int) $row->contactID,
        'contactName' => (string) $row->contactName,
        'standing' => (float) $row->standing,
        'contactTypeID' => (int) $row->contactTypeID,
        'manual' => 0,
        'checked' => 0,
      ))->execute();
    }
  }

  db_delete('eve_api_contact_list')->condition('checked', 1, '=')->condition('manual', 0, '=')->execute();

  return TRUE;
}

/**
 * Get current corporations in the alliance.
 *
 * @param mixed $item
 *   whatever was added to the queue via queue->createItem()
 *
 * @return bool
 *   TRUE or FALSE.
 */
function eve_api_cron_api_alliance_sync($item = NULL) {
  $response = eve_api_get_xml('AllianceList');

  if (!isset($response['xml']->result)) {
    if ($item == 2) {
      if (variable_get('eve_api_debug', FALSE)) {
        watchdog('eve_api', 'Cron Import Alliance API task has failed 2 times in a row, task was removed.', NULL, WATCHDOG_ERROR);
      }

      return FALSE;
    }

    $item = $item + 1;
    $queue = DrupalQueue::get('eve_api_cron_api_alliance_sync');
    $queue->createItem($item);

    return FALSE;
  }

  db_update('eve_api_alliance_list_corporations')->fields(array('checked' => 1))->condition('manual', 0, '=')->execute();
  db_truncate('eve_api_alliance_list_tickers')->execute();

  foreach ($response['xml']->result->rowset->children() as $rows_a) {
    $row_a = $rows_a->attributes();

    db_insert('eve_api_alliance_list_tickers')->fields(array(
      'allianceID' => (int) $row_a->allianceID,
      'allianceTicker' => (string) $row_a->shortName,
      'allianceName' => (string) $row_a->name,
    ))->execute();

    if ((int) $row_a->allianceID == variable_get('eve_api_allianceID', 0)) {
      foreach ($rows_a->rowset->children() as $rows_b) {
        $row_b = $rows_b->attributes();

        $result = db_query('SELECT corporationID FROM {eve_api_alliance_list_corporations} WHERE corporationID = :corporationID AND manual = 0', array(
          ':corporationID' => (int) $row_b->corporationID,
        ));

        if ($result->rowCount()) {
          db_update('eve_api_alliance_list_corporations')->fields(array('checked' => 0))->condition('corporationID', (int) $row_b->corporationID, '=')->execute();
          continue;
        }

        $query = array('corporationID' => (int) $row_b->corporationID);
        $corp_info = eve_api_get_corporation_sheet_api($query);

        if (isset($corp_info['error'])) {
          db_update('eve_api_alliance_list_corporations')->fields(array('checked' => 0))->condition('corporationID', (int) $row_b->corporationID, '=')->execute();
          continue;
        }

        if (!($role = user_role_load_by_name($corp_info['corporationName']))) {
          $user_role = new stdClass();
          $user_role->name = $corp_info['corporationName'];

          if (!empty($corp_info['corporationName'])) {
            user_role_save($user_role);
          }

          $role = user_role_load_by_name($corp_info['corporationName']);
        }

        db_merge('eve_api_alliance_list_corporations')->key(array('corporationID' => (int) $row_b->corporationID))->fields(array(
          'corporationID' => (int) $row_b->corporationID,
          'rid' => (int) $role->rid,
          'corporationName' => (string) $corp_info['corporationName'],
          'corporationTicker' => (string) $corp_info['corporationTicker'],
          'ceoID' => (int) $corp_info['ceoID'],
          'ceoName' => (string) $corp_info['ceoName'],
          'checked' => 0,
          'manual' => 0,
        ))->execute();
      }
    }
  }

  $result = db_query('SELECT corporationID, corporationName FROM {eve_api_alliance_list_corporations} WHERE checked = 1');

  if ($result->rowCount()) {
    foreach ($result->fetchAll() as $row) {
      if (user_role_load_by_name($row->corporationName) == TRUE) {
        user_role_delete($row->corporationName);
      }
    }
  }

  db_delete('eve_api_alliance_list_corporations')->condition('checked', 1, '=')->execute();

  if (!variable_get('eve_api_enable', FALSE)) {
    variable_set('eve_api_enable', TRUE);
    menu_rebuild();
  }

  return TRUE;
}

/**
 * Get the latest access mask from the EVE API.
 * 
 * @param mixed $item
 *   whatever was added to the queue via queue->createItem()
 *
 * @return bool
 *   TRUE or FALSE.
 */
function eve_api_cron_api_mask_sync($item = NULL) {
  $response = eve_api_get_xml('callList');

  if (!isset($response['xml']->result)) {
    if ($item == 2) {
      if (variable_get('eve_api_debug', FALSE)) {
        watchdog('eve_api', 'Cron Access Mask task has failed 2 times in a row, task was removed.', NULL, WATCHDOG_ERROR);
      }

      return FALSE;
    }

    $item = $item + 1;
    $queue = DrupalQueue::get('eve_api_cron_api_mask_sync');
    $queue->createItem($item);

    return FALSE;
  }

  db_truncate('eve_api_call_list')->execute();
  db_truncate('eve_api_call_list_groups')->execute();

  foreach ($response['xml']->result->children() as $rowset) {
    $row_a = $rowset->attributes();

    switch ((string) $row_a->name) {
      case 'callGroups':
        foreach ($rowset->children() as $rows) {
          $row_b = $rows->attributes();

          db_insert('eve_api_call_list_groups')->fields(array(
            'groupID' => (int) $row_b->groupID,
            'name' => (string) $row_b->name,
            'description' => (string) $row_b->description,
          ))->execute();
        }
        break;

      case 'calls':
        foreach ($rowset->children() as $rows) {
          $row_b = $rows->attributes();

          db_insert('eve_api_call_list')->fields(array(
            'accessmask' => (int) $row_b->accessMask,
            'type' => (string) $row_b->type,
            'name' => (string) $row_b->name,
            'groupID' => (int) $row_b->groupID,
            'description' => (string) $row_b->description,
          ))->execute();
        }
        break;
    }
  }

  return TRUE;
}

/**
 * Get the latest skill tree from the EVE API.
 * 
 * @param mixed $item
 *   whatever was added to the queue via queue->createItem()
 *
 * @return bool
 *   TRUE or FALSE.
 */
function eve_api_cron_api_skill_tree($item = NULL) {
  $api_data = eve_api_get_skill_tree_api();

  if (isset($api_data['error'])) {
    if ($item == 2) {
      if (variable_get('eve_api_debug', FALSE)) {
        watchdog('eve_api', 'Cron Skill Tree task has failed 2 times in a row, task was removed.', NULL, WATCHDOG_ERROR);
      }

      return FALSE;
    }

    $item = $item + 1;
    $queue = DrupalQueue::get('eve_api_cron_api_skill_tree');
    $queue->createItem($item);

    return FALSE;
  }

  db_truncate('eve_api_skill_tree_groups')->execute();
  db_truncate('eve_api_skill_tree')->execute();

  foreach ($api_data['group'] as $rows) {
    db_insert('eve_api_skill_tree_groups')->fields(array(
      'groupID' => (int) $rows['groupID'],
      'groupName' => (string) $rows['groupName'],
    ))->execute();
  }

  foreach ($api_data['type'] as $rows) {
    db_insert('eve_api_skill_tree')->fields(array(
      'groupID' => (int) $rows['groupID'],
      'typeID' => (int) $rows['typeID'],
      'typeName' => (string) $rows['typeName'],
    ))->execute();
  }

  return TRUE;
}

/**
 * Get the latest skill tree from the EVE API.
 * 
 * @param mixed $item
 *   whatever was added to the queue via queue->createItem()
 *
 * @return bool
 *   TRUE or FALSE.
 */
function eve_api_cron_api_error_list($item = NULL) {
  $api_data = eve_api_get_error_list_api();

  if (isset($api_data['error'])) {
    if ($item == 2) {
      if (variable_get('eve_api_debug', FALSE)) {
        watchdog('eve_api', 'Cron Error List task has failed 2 times in a row, task was removed.', NULL, WATCHDOG_ERROR);
      }

      return FALSE;
    }

    $item = $item + 1;
    $queue = DrupalQueue::get('eve_api_cron_api_error_list');
    $queue->createItem($item);

    return FALSE;
  }

  db_truncate('eve_api_error_list')->execute();

  foreach ($api_data['errors'] as $error_code => $error_text) {
    db_insert('eve_api_error_list')->fields(array(
      'errorCode' => (int) $error_code,
      'errorText' => (string) $error_text,
    ))->execute();
  }

  return TRUE;
}

/**
 * Cron that imports the character sheet of a single character.
 *
 * @param array $item
 *   The characterID and API Key and vCode.
 */
function eve_api_cron_import_character_sheet($item) {
  $character_id = (int) $item['characterID'];
  $query = (array) $item['query'];

  $sheet_query = $query + array('characterID' => (int) $character_id);
  $character_sheet = eve_api_get_character_sheet_api($sheet_query);
  $error = 0;

  if (isset($character_sheet['error'])) {
    $error = (int) $character_sheet['error'];

    db_merge('eve_api_character_sheet')->key(array(
      'characterID' => (int) $character_id,
    ))->fields(array(
      'characterID' => (int) $character_id,
      'errorCode' => $character_sheet['errorCode'],
      'errorText' => $character_sheet['errorText'],
    ))->execute();

    if (variable_get('eve_api_debug', FALSE)) {
      watchdog('eve_api', 'Failed importing Character Sheet. Data: (' . drupal_http_build_query($sheet_query) . ') Error: ' . (int) $error, NULL, WATCHDOG_WARNING);
    }

    return FALSE;
  }
  else {
    db_delete('eve_api_character_sheet_attribute_enhancers')->condition('characterID', (int) $character_id, '=')->execute();
    db_delete('eve_api_character_sheet_roles')->condition('characterID', (int) $character_id, '=')->execute();
    db_delete('eve_api_character_sheet_titles')->condition('characterID', (int) $character_id, '=')->execute();

    foreach ($character_sheet as $type => $data) {
      switch ((string) $type) {
        case 'skills':
          foreach ($data as $skillsdata) {
            db_merge('eve_api_character_sheet_skills')->key(array(
              'characterID' => (int) $character_id,
              'typeID' => (int) $skillsdata['typeID'],
            ))->fields(array(
              'characterID' => (int) $character_id,
              'typeID' => (int) $skillsdata['typeID'],
              'skillpoints' => (int) $skillsdata['skillpoints'],
              'level' => (int) $skillsdata['level'],
              'published' => (int) $skillsdata['published'],
            ))->execute();
          }
          break;

        case 'certificates':
          foreach ($data as $certificatesdata) {
            db_merge('eve_api_character_sheet_certificates')->key(array(
              'characterID' => (int) $character_id,
              'certificateID' => (int) $certificatesdata['certificateID'],
            ))->fields(array(
              'characterID' => (int) $character_id,
              'certificateID' => (int) $certificatesdata['certificateID'],
            ))->execute();
          }
          break;

        case 'corporationTitles':
          foreach ($data as $titlesdata) {
            db_merge('eve_api_character_sheet_titles')->key(array(
              'characterID' => (int) $character_id,
              'titleID' => (int) $titlesdata['titleID'],
            ))->fields(array(
              'characterID' => (int) $character_id,
              'titleID' => (int) $titlesdata['titleID'],
              'titleName' => (string) $titlesdata['titleName'],
            ))->execute();
          }
          break;

        case 'roles':
          foreach ($data as $rolename => $roledata) {
            foreach ($roledata as $roles) {
              db_merge('eve_api_character_sheet_roles')->key(array(
                'characterID' => (int) $character_id,
                'type' => (string) $rolename,
                'roleID' => (int) $roles['roleID'],
              ))->fields(array(
                'characterID' => (int) $character_id,
                'type' => (string) $rolename,
                'roleID' => (int) $roles['roleID'],
                'roleName' => (string) $roles['roleName'],
              ))->execute();
            }
          }
          break;

        case 'attributeEnhancers':
          foreach ($data as $attributename => $attributedata) {
            $query_data = db_insert('eve_api_character_sheet_attribute_enhancers');

            $fields = array(
              'characterID' => (int) $character_id,
              'bonusName' => (string) $attributename,
            );

            foreach ($attributedata as $attribute => $attributes) {
              if (empty($attributes)) {
                continue;
              }

              $fields += array(
                (string) $attribute => (string) $attributes,
              );
            }

            $query_data->fields($fields);
            $query_data->execute();
          }
          break;

        case 'info':
          $query_data = db_merge('eve_api_character_sheet')->key(array(
            'characterID' => (int) $character_id,
          ));

          $fields = array(
            'characterID' => (int) $character_id,
            'errorCode' => $character_sheet['errorCode'],
            'errorText' => $character_sheet['errorText'],
          );

          foreach ($data as $infoname => $infodata) {
            if (empty($infodata)) {
              continue;
            }

            $fields += array(
              (string) $infoname => (string) $infodata,
            );
          }

          $query_data->fields($fields);
          $query_data->execute();
          break;

        default:
          break;
      }
    }
  }

  return TRUE;
}

/**
 * Cron that imports the character info of a single character.
 *
 * @param array $item
 *   The characterID and API Key and vCode.
 *
 * @return bool
 *   TRUE or FALSE.
 */
function eve_api_cron_import_character_info($item) {
  $character_id = (int) $item['characterID'];
  $query = (array) $item['query'];

  $info_query = $query + array('characterID' => (int) $character_id);
  $character_info = eve_api_get_character_info_api($info_query);
  $error = 0;

  if (isset($character_info['error'])) {
    $error = (int) $character_info['error'];

    db_merge('eve_api_character_info')->key(array(
      'characterID' => (int) $character_id,
    ))->fields(array(
      'characterID' => (int) $character_id,
      'errorCode' => $character_info['errorCode'],
      'errorText' => $character_info['errorText'],
    ))->execute();

    if (variable_get('eve_api_debug', FALSE)) {
      watchdog('eve_api', 'Failed importing Character Info. Data: (' . drupal_http_build_query($info_query) . ') Error: ' . (int) $error, NULL, WATCHDOG_WARNING);
    }

    return FALSE;
  }
  else {
    db_delete('eve_api_character_info_corporation_history')->condition('characterID', (int) $character_id, '=')->execute();

    foreach ($character_info as $type => $data) {
      switch ((string) $type) {
        case 'employmentHistory':
          foreach ($data as $historydata) {
            db_merge('eve_api_character_info_corporation_history')->key(array(
              'characterID' => (int) $character_id,
              'recordID' => (int) $historydata['recordID'],
            ))->fields(array(
              'characterID' => (int) $character_id,
              'recordID' => (int) $historydata['recordID'],
              'corporationID' => (int) $historydata['corporationID'],
              'startDate' => (string) $historydata['startDate'],
            ))->execute();
          }
          break;

        case 'info':
          $query_data = db_merge('eve_api_character_info')->key(array(
            'characterID' => (int) $character_id,
          ));

          $fields = array(
            'characterID' => (int) $character_id,
            'errorCode' => $character_info['errorCode'],
            'errorText' => $character_info['errorText'],
          );

          $ignored = array(
            'characterID',
          );

          foreach ($data as $infoname => $infodata) {
            if (in_array($infoname, $ignored)) {
              continue;
            }

            if (empty($infodata)) {
              continue;
            }

            $fields += array(
              (string) $infoname => (string) $infodata,
            );
          }

          $query_data->fields($fields);
          $query_data->execute();
          break;

        default:
          break;
      }
    }
  }

  return TRUE;
}

/**
 * Cron that imports the skills being trained of a single character.
 *
 * @param array $item
 *   The characterID and API Key and vCode.
 *
 * @return bool
 *   TRUE or FALSE.
 */
function eve_api_cron_import_skill_in_training($item) {
  $character_id = (int) $item['characterID'];
  $query = (array) $item['query'];

  $training_query = $query + array('characterID' => (int) $character_id);
  $training_info = eve_api_get_skill_in_training_api($training_query);
  $error = 0;

  if (isset($training_info['errorCode'])) {
    $error = (int) $training_info['errorCode'];
  }

  if (isset($training_info['error'])) {
    $error = (int) $training_info['error'];

    db_merge('eve_api_skill_in_training')->key(array(
      'characterID' => (int) $character_id,
    ))->fields(array(
      'characterID' => (int) $character_id,
      'errorCode' => $training_info['errorCode'],
      'errorText' => $training_info['errorText'],
    ))->execute();

    if (variable_get('eve_api_debug', FALSE)) {
      watchdog('eve_api', 'Failed importing Character Skill Training. Data: (' . drupal_http_build_query($training_query) . ') Error: ' . (int) $error, NULL, WATCHDOG_WARNING);
    }

    return FALSE;
  }
  else {
    foreach ($training_info as $type => $data) {
      switch ((string) $type) {
        case 'info':
          $query_data = db_merge('eve_api_skill_in_training')->key(array(
            'characterID' => (int) $character_id,
          ));

          $fields = array(
            'characterID' => (int) $character_id,
            'errorCode' => $training_info['errorCode'],
            'errorText' => $training_info['errorText'],
          );

          foreach ($data as $infoname => $infodata) {
            if (empty($infodata)) {
              continue;
            }

            $fields += array(
              (string) $infoname => (string) $infodata,
            );
          }

          $query_data->fields($fields);
          $query_data->execute();
          break;

        default:
          break;
      }
    }
  }

  return TRUE;
}

/**
 * Cron that imports the wallet journal of a single character.
 *
 * @param array $item
 *   The characterID and API Key and vCode.
 *
 * @return bool
 *   TRUE or FALSE.
 */
function eve_api_cron_api_user_wallet_journal($item) {
  $character_id = (int) $item['characterID'];
  $query = $item['query'];

  $row_count = 250;
  $from_id = NULL;

  $wallet_query = $query + array(
    'characterID' => (int) $character_id,
    'rowCount' => $row_count,
  );

  $counter = 1000;

  $result = db_query('SELECT MAX(refID) AS maxRefID FROM {eve_api_wallet_journal} WHERE characterID = :characterID', array(
    ':characterID' => $character_id,
  ));

  if (!$result->rowCount()) {
    $max_ref_id = '0';
  }
  else {
    $row = $result->fetchAssoc();
    $max_ref_id = isset($row['maxRefID']) ? (string) $row['maxRefID'] : '0';
  }

  do {
    $wallet_info = eve_api_get_wallet_journal_api($wallet_query);

    if (isset($wallet_info['error'])) {
      if (variable_get('eve_api_debug', FALSE)) {
        watchdog('eve_api', 'Failed importing Character Wallet Journal. Data: (' . drupal_http_build_query($wallet_query) . ') Error: ' . (int) $wallet_info['error'], NULL, WATCHDOG_WARNING);
      }

      return FALSE;
    }
    else {
      $max_ref_id_found = FALSE;
      $count = 0;

      foreach ($wallet_info as $type => $data) {
        switch ((string) $type) {
          case 'wallet_journal':
            foreach ($data as $walletdata) {
              if (eve_api_comp($max_ref_id, (string) $walletdata['refID']) != -1) {
                $max_ref_id_found = TRUE;
                continue;
              }

              if (!isset($from_id)) {
                $from_id = (string) $walletdata['refID'];
              }

              if (eve_api_comp($from_id, (string) $walletdata['refID']) != -1) {
                $from_id = (string) $walletdata['refID'];
              }

              $query_data = db_insert('eve_api_wallet_journal');

              $fields = array(
                'characterID' => $character_id,
              );

              foreach ($walletdata as $infoname => $infodata) {
                if (empty($infodata)) {
                  continue;
                }

                $fields += array(
                  $infoname => (string) $infodata,
                );
              }

              $query_data->fields($fields);
              $query_data->execute();

              $count++;
            }
            break;

          default:
            break;
        }
      }

      if ($max_ref_id_found || !isset($from_id) || $count != $row_count) {
        return TRUE;
      }

      $wallet_query = array(
        'fromID' => $from_id,
      ) + $wallet_query;
    }
  } while ($counter--);

  return TRUE;
}

/**
 * Cron that imports the contracts of a single character.
 *
 * @param array $item
 *   The characterID and API Key and vCode.
 *
 * @return bool
 *   TRUE or FALSE.
 */
function eve_api_cron_import_contracts($item) {
  $character_id = (int) $item['characterID'];
  $query = (array) $item['query'];

  $contract_query = $query + array(
    'characterID' => $character_id,
  );

  $contract_info = eve_api_get_contracts_api($contract_query);

  if (isset($contract_info['error'])) {
    if (variable_get('eve_api_debug', FALSE)) {
      watchdog('eve_api', 'Failed importing Character Contracts. Data: (' . drupal_http_build_query($contract_query) . ') Error: ' . (int) $contract_info['error'], NULL, WATCHDOG_WARNING);
    }

    return FALSE;
  }
  else {
    $result = db_query('SELECT MAX(contractID) AS maxContractID FROM {eve_api_contracts} WHERE characterID = :characterID', array(
      ':characterID' => $character_id,
    ));

    if (!$result->rowCount()) {
      $max_contract_id = 0;
    }
    else {
      $row = $result->fetchAssoc();
      $max_contract_id = isset($row['maxContractID']) ? (string) $row['maxContractID'] : 0;
    }

    foreach ($contract_info as $type => $data) {
      switch ((string) $type) {
        case 'contracts':
          foreach ($data as $contract_data) {
            if (eve_api_comp($max_contract_id, (string) $contract_data['contractID']) != -1) {
              continue;
            }

            $query_data = db_insert('eve_api_contracts');

            $fields = array(
              'characterID' => $character_id,
            );

            foreach ($contract_data as $infoname => $infodata) {
              if (empty($infodata)) {
                continue;
              }

              $fields += array(
                $infoname => (string) $infodata,
              );
            }

            $query_data->fields($fields);
            $query_data->execute();
          }
          break;

        default:
          break;
      }
    }
  }

  return TRUE;
}

/**
 * Cron that imports the mail of a single character.
 *
 * @param array $item
 *   The characterID and API Key and vCode.
 *
 * @return bool
 *   TRUE or FALSE.
 */
function eve_api_cron_import_mail($item) {
  $character_id = (int) $item['characterID'];
  $query = (array) $item['query'];

  $mail_query = $query + array(
    'characterID' => $character_id,
  );

  $result = db_query('SELECT MAX(messageID) AS maxMessageID FROM {eve_api_mail_messages} WHERE characterID = :characterID', array(
    ':characterID' => $character_id,
  ));

  if (!$result->rowCount()) {
    $max_message_id = 0;
  }
  else {
    $row = $result->fetchAssoc();
    $max_message_id = isset($row['maxMessageID']) ? (string) $row['maxMessageID'] : 0;
  }

  $mail_info = eve_api_get_mail_messages_api($mail_query);

  if (isset($mail_info['error'])) {
    if (variable_get('eve_api_debug', FALSE)) {
      watchdog('eve_api', 'Failed importing Mail Headers. Data: (' . drupal_http_build_query($mail_query) . ') Error: ' . (int) $mail_info['error'], NULL, WATCHDOG_WARNING);
    }

    return FALSE;
  }
  else {
    $message_ids = array();

    foreach ($mail_info as $type => $data) {
      switch ((string) $type) {
        case 'mail':
          foreach ($data as $mail_data) {
            if (eve_api_comp($max_message_id, (string) $mail_data['messageID']) != -1) {
              continue;
            }

            $message_ids[] = (string) $mail_data['messageID'];

            $query_data = db_insert('eve_api_mail_messages');

            $fields = array(
              'characterID' => $character_id,
            );

            foreach ($mail_data as $infoname => $infodata) {
              if (empty($infodata)) {
                continue;
              }

              $fields += array(
                $infoname => (string) $infodata,
              );
            }

            $query_data->fields($fields);
            $query_data->execute();
          }
          break;

        default:
          break;
      }
    }

    if (empty($message_ids)) {
      return TRUE;
    }

    $message_ids = array_chunk($message_ids, 5, TRUE);

    foreach ($message_ids as $message_id) {
      $body_query = array(
        'ids' => implode(',', $message_id),
      ) + $mail_query;

      $body_info = eve_api_get_mail_bodies_api($body_query);

      if (isset($body_info['error'])) {
        if (variable_get('eve_api_debug', FALSE)) {
          watchdog('eve_api', 'Failed importing Mail Bodies. Data: (' . drupal_http_build_query($body_query) . ') Error: ' . (int) $body_info['error'], NULL, WATCHDOG_WARNING);
        }

        return FALSE;
      }
      else {
        foreach ($body_info as $type => $data) {
          switch ((string) $type) {
            case 'mail_body':
              foreach ($data as $bodydata) {
                $query_data = db_insert('eve_api_mail_bodies');

                $fields = array(
                  'characterID' => $character_id,
                );

                foreach ($bodydata as $infoname => $infodata) {
                  if (empty($infodata)) {
                    continue;
                  }

                  $fields += array(
                    $infoname => (string) $infodata,
                  );
                }

                $query_data->fields($fields);
                $query_data->execute();
              }
              break;

            default:
              break;
          }
        }
      }
    }
  }

  return TRUE;
}

/**
 * Function that verifies and updates all characters on an API Key.
 *
 * @param object $account
 *   A Drupal user object containing.
 * @param int $key_id
 *   A unique EVE API Key ID.
 * @param string $v_code
 *   A unique EVE API Verification Code.
 * @param int $api_id
 *   A unique API ID.
 *
 * @return mixed
 *   A mixed array of data regarding the success/failure.
 */
function eve_api_cron_update_key($account, $key_id, $v_code, $api_id) {
  $updated = date('Y-m-d H:i:s', time());
  $uid = (int) $account->uid;

  $character_found = FALSE;
  $error_found = FALSE;
  $character_data = FALSE;

  $query = array(
    'keyID' => $key_id,
    'vCode' => $v_code,
  );

  $characters = eve_api_get_api_key_info_api($query);

  $error = 0;

  if (isset($characters['error'])) {
    if ($api_id == 0) {
      return array(
        'error' => TRUE,
        'error_found' => $error_found,
        'character_data' => $character_data,
        'character_found' => $character_found,
      );
    }

    db_merge('eve_api_keys')->key(array('apiID' => (int) $api_id))->fields(array(
      'apiID' => (int) $api_id,
      'errorCode' => $characters['errorCode'],
      'errorText' => $characters['errorText'],
      'updated' => (string) $updated,
    ))->execute();

    if (eve_api_error_code_deleted($characters['errorCode'])) {
      db_update('eve_api_characters')->fields(array(
        'errorCode' => $characters['errorCode'],
        'errorText' => $characters['errorText'],
        'deleted' => 1,
      ))->condition('apiID', (int) $api_id, '=')->execute();
    }
    else {
      db_update('eve_api_characters')->fields(array(
        'errorCode' => $characters['errorCode'],
        'errorText' => $characters['errorText'],
      ))->condition('apiID', (int) $api_id, '=')->execute();
      $error_found = TRUE;
    }

    return array(
      'error' => FALSE,
      'error_found' => $error_found,
      'character_data' => $character_data,
      'character_found' => $character_found,
    );
  }

  if (!isset($characters['characters'])) {
    if (variable_get('eve_api_debug', FALSE)) {
      watchdog('eve_api', 'Error occured, should never happen. (' . drupal_http_build_query($query) . ')<br /><pre>' . print_r($characters, TRUE) . '</pre>', NULL, WATCHDOG_WARNING);
    }

    return array(
      'error' => TRUE,
      'error_found' => $error_found,
      'character_data' => $character_data,
      'character_found' => $character_found,
    );
  }

  $whitelist = array();

  if (!empty($characters)) {
    foreach ($characters['characters'] as $character) {
      $whitelist[] = (int) $character['characterID'];
    }
  }

  $result = db_query('SELECT characterID FROM {eve_api_whitelist} WHERE characterID IN (:characterIDs)', array(
    ':characterIDs' => $whitelist,
  ));

  $allow_expires = variable_get('eve_api_require_expires', FALSE) ? FALSE : !empty($characters['expires']);
  $allow_type = variable_get('eve_api_require_type', TRUE) ? $characters['type'] != 'Account' : FALSE;

  if ($result->rowCount()) {
    if ($allow_expires || ($characters['accessMask'] & 8388680) != 8388680) {
      $error = 1;
    }
  }
  else {
    if ($allow_expires || $allow_type || ($characters['accessMask'] & variable_get('eve_api_access_mask', 268435455)) != variable_get('eve_api_access_mask', 268435455)) {
      $error = 1;
    }
  }

  db_update('eve_api_characters')->fields(array(
    'errorCode' => $characters['errorCode'],
    'errorText' => $characters['errorText'],
    'deleted' => 1,
  ))->condition('apiID', (int) $api_id)->execute();
  db_merge('eve_api_keys')->key(array('apiID' => (int) $api_id))->fields(array(
    'apiID' => (int) $api_id,
    'uid' => (int) $uid,
    'keyID' => (int) $key_id,
    'vCode' => (string) $v_code,
    'updated' => (string) $updated,
    'errorCode' => $characters['errorCode'],
    'errorText' => $characters['errorText'],
  ))->execute();

  if ($error == 1) {
    return array(
      'error' => FALSE,
      'error_found' => $error_found,
      'character_data' => $character_data,
      'character_found' => $character_found,
    );
  }

  foreach ($characters['characters'] as $character) {
    $error = 0;

    db_merge('eve_api_characters')->key(array('characterID' => (int) $character['characterID']))->fields(array(
      'characterID' => (int) $character['characterID'],
      'uid' => (int) $uid,
      'apiID' => (int) $api_id,
      'characterName' => (string) $character['characterName'],
      'corporationID' => (int) $character['corporationID'],
      'corporationName' => (string) $character['corporationName'],
      'corporationTicker' => (string) $character['corporationTicker'],
      'allianceID' => (int) $character['allianceID'],
      'allianceName' => (string) $character['allianceName'],
      'allianceTicker' => (string) $character['allianceTicker'],
      'accessMask' => (int) $characters['accessMask'],
      'errorCode' => $characters['errorCode'],
      'errorText' => $characters['errorText'],
      'deleted' => 0,
    ))->execute();

    if ($account->name == (string) $character['characterName']) {
      $character_found = TRUE;
      $character_data = $character;
    }

    $item = array(
      'query' => $query,
      'characterID' => (int) $character['characterID'],
    );

    eve_api_cron_import_character_sheet($item);

    eve_api_cron_import_character_info($item);

    if (($characters['accessMask'] & 131072) == 131072) {
      eve_api_cron_import_skill_in_training($item);
    }
    else {
      db_merge('eve_api_skill_in_training')->key(array(
        'characterID' => (int) $character['characterID'],
      ))->fields(array(
        'characterID' => (int) $character['characterID'],
        'errorCode' => 0,
        'errorText' => '',
      ))->execute();
    }

    if (($characters['accessMask'] & 67108864) == 67108864) {
      eve_api_cron_import_contracts($item);
    }

    if (($characters['accessMask'] & 2048) == 2048 && ($characters['accessMask'] & 512) == 512) {
      eve_api_cron_import_mail($item);
    }
  }

  return array(
    'error' => FALSE,
    'error_found' => $error_found,
    'character_data' => $character_data,
    'character_found' => $character_found,
  );
}
