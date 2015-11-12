<?php
/**
 * @file
 * functions for mpxPlayers.
 */

/**
 * Requests all mpxPlayers for specified thePlatform account.
 *
 * @return array
 */
function media_theplatform_mpx_get_players_from_theplatform(MpxAccount $account, $all = TRUE) {
  if (empty($account->import_account)) {
    watchdog('media_theplatform_mpx', 'Failed to retrieve mpx players for @acccount. Import account not available.',
      array('@account' => (string) $account), WATCHDOG_ERROR);
    return array();
  }

  $current_item = 1;
  $size = variable_get('media_theplatform_mpx__cron_videos_per_run', 100);

  $params = array(
    'schema' => '1.3.0',
    'form' => 'cjson',
    'account' => $account->import_account,
  );

  $players = array();

  do {
    $params['range'] = $current_item . '-' . ($current_item + $size - 1);

    // Get the list of players from thePlatform.
    $data = MpxApi::authenticatedRequest(
      $account,
      'https://read.data.player.theplatform.com/player/data/Player',
      $params
    );

    if (!isset($data['entryCount'])) {
      watchdog('media_theplatform_mpx', 'Failed to retrieve mpx players for @acccount.  "entryCount" field value not set.',
        array('@account' => (string) $account), WATCHDOG_ERROR);
      return array();
    }

    foreach ($data['entries'] as $player) {
      // We only want mpxPlayers which are not disabled.
      if (!$player['disabled']) {
        $player_id = basename($player['id']);
        $players[$player_id] = array(
          'id' => $player_id,
          'guid' => $player['guid'],
          'title' => $player['title'],
          'description' => $player['description'],
          'pid' => $player['pid'],
          'parent_account' => $account->id,
          'account' => $account->import_account,
        );
      }
    }

    $current_item += count($data['entries']);

  } while ($all && !empty($data['entryCount']) && count($data['entries']) == $size);

  media_theplatform_mpx_debug($players, count($players) . " players returned for {$account}");
  return $players;
}

/**
 * Returns array of mpxPlayer fid's and Titles.
 */
function media_theplatform_mpx_get_players_select(MpxAccount $account = NULL, $key = 'player_id') {

  // Retrieve players from mpx_player.
  $query = db_select('mpx_player', 'p');
  $query->join('mpx_accounts', 'a', 'a.id = p.parent_account');
  $query->fields('p', array($key, 'title', 'pid', 'id'))
    ->fields('a', array('import_account'))
    ->condition('p.status', 1, '=')
    ->orderBy('title', 'ASC');

  if ($account) {
    $query = $query->condition('parent_account', $account->id, '=');
  }

  $result = $query->execute();
  $num_rows = $query->countQuery()->execute()->fetchField();

  if ($num_rows == 0) {
    return array();
  }

  // Index by file fid.
  while ($record = $result->fetchAssoc()) {
    if (!empty($account)) {
      $players[$record[$key]] = $record['id'] . ' - ' . $record['pid'] . ' - ' . $record['title'];
    }
    else {
      $players[$record['import_account']][$record[$key]] = $record['id'] . ' - ' . $record['pid'] . ' - ' . $record['title'];
    }
  }

  return $players;
}

/**
 * Returns TRUE if given mpxPlayer $fid matches given $account.
 */
function media_theplatform_mpx_is_valid_player_for_account($player_id, $account = NULL) {

  if (!$player_id) {
    return FALSE;
  }

  $player = media_theplatform_mpx_get_mpx_player_by_player_id($player_id);

  if (is_array($player) && !empty($player['player_id'])) {
    return TRUE;
  }

  return FALSE;
}

function media_theplatform_mpx_import_account_players(MpxAccount $account) {
  $players = media_theplatform_mpx_get_players_from_theplatform($account);
  foreach ($players as $player) {
    media_theplatform_mpx_import_player($player, $account);
  }

  // Find all players that exist locally but weren't returned from
  // the recent API request, and mark them as inactive.
  $inactive_query = db_select('mpx_player', 'p')
    ->fields('p', array('pid', 'fid'))
    ->condition('status', 1)
    ->condition('parent_account', $account->id);
  if (!empty($players)) {
    $inactive_query->condition('id', array_keys($players), 'NOT IN');
  }
  $inactive_players = $inactive_query->execute()->fetchAllKeyed();

  if (!empty($inactive_players)) {
    $inactive_fids = array_values($inactive_players);
    watchdog('media_theplatform_mpx', 'Disabling the following players with pid values of @pids and fid values of @fid.',
      array(
        '@pids' => implode(', ', array_keys($inactive_players)),
        '@fids' => implode(', ', array_values($inactive_fids)),
      ),
      WATCHDOG_INFO);
    db_update('mpx_player')
      ->fields(array('status' => 0))
      ->condition('fid', $inactive_fids)
      ->execute();

    if (module_exists('file_admin')) {
      $player_files = file_load_multiple($inactive_fids);
      foreach ($player_files as $player_file) {
        if (!empty($player_file->published)) {
          $player_file->published = 0;
          file_save($player_file);
        }
      }
    }
  }

  return $players;
}

/**
 * Imports all mpxPlayers into Media Library.
 *
 * @param String $type
 *   Import type. Possible values 'cron' or 'manual', for sync.
 *
 * @return Array
 *   $data['total'] - # of players retrieved
 *   $data['num_inserts'] - # of players added to mpx_player table
 *   $data['num_updates'] - # of players updated
 *   $data['num_inactives'] - # of players changed from active to inactive
 */
function media_theplatform_mpx_import_all_players($type = NULL) {
  // This log message may seem redundant, but it's important for detecting if an
  // ingestion process has begun and is currently in progress.
  watchdog('media_theplatform_mpx', 'Beginning player import/update process @method for all accounts.', array('@method' => $type), WATCHDOG_INFO);

  // Initalize our counters.
  $inserts = array();
  $updates = array();
  $inactives = array();
  $num_players = 0;
  $incoming = array();

  // Retrieve list of players for all accounts.
  foreach (MpxAccount::loadAll() as $account_data) {
    // Check if player sync has been turned off for this account.
    if (!variable_get('media_theplatform_mpx__account_' . $account_data->id . '_cron_player_sync', 1)) {
      continue;
    }
    $account_players = media_theplatform_mpx_get_players_from_theplatform($account_data);
    if ($account_players) {
      // Loop through players retrieved.
      foreach ($account_players as $player) {
        // Keep track of the incoming ID.
        $incoming[] = $player['id'];
        // Import this player.
        $op = media_theplatform_mpx_import_player($player, $account_data);
        if ($op == 'insert') {
          $inserts[] = $player['id'];
        }
        elseif ($op == 'update') {
          $updates[] = $player['id'];
        }
        $num_players++;
      }
    }
  }

  if (empty($incoming)) {
    return array(
      'total' => $num_players,
      'inserts' => count($inserts),
      'updates' => count($updates),
      'inactives' => count($inactives),
    );
  }

  // Find all mpx_player records NOT in $incoming with status = 1.
  $inactives_result = db_select('mpx_player', 'p')
    ->fields('p', array('player_id', 'fid', 'id'))
    ->condition('id', $incoming, 'NOT IN')
    ->condition('status', 1, '=')
    ->execute();

  // Loop through results:
  while ($record = $inactives_result->fetchAssoc()) {
    // Set status to inactive.
    $inactive = db_update('mpx_player')
      ->fields(array('status' => 0))
      ->condition('player_id', $record['player_id'], '=')
      ->execute();
    if (!$inactive) {
      watchdog('media_theplatform_mpx', 'Failed to disable player @pid with player_id @player_id by settings its status to 0 in mpx_player.',
        array(
          '@pid' => $record['id'],
          '@player_id' => $record['player_id'],
        ),
        WATCHDOG_ERROR);
    }
    else {
      watchdog('media_theplatform_mpx', 'Successfully disabled player @pid with player_id @player_id by settings its status to 0 in mpx_player.',
        array(
          '@pid' => $record['id'],
          '@player_id' => $record['player_id'],
        ),
        WATCHDOG_NOTICE);
    }
    $inactives[] = $record['id'];
    // Unpublish the file entity if the file_admin module is enabled.
    if (module_exists('file_admin')) {
      $player_file = file_load($record['fid']);
      $player_file->published = 0;
      file_save($player_file);
    }
  }

  watchdog('media_theplatform_mpx', 'Processed players @method for all accounts:'
      . '  @insert_count player(s) created' . (count($inserts) ? ' (@inserts).' : '.')
      . '  @update_count player(s) updated' . (count($updates) ? ' (@updates).' : '.')
      . '  @inactive_count player(s) disabled' . (count($inactives) ? ' (@inactives).' : '.'),
    array(
      '@method' => $type,
      '@insert_count' => count($inserts),
      '@inserts' => implode(', ', $inserts),
      '@update_count' => count($updates),
      '@updates' => implode(', ', $updates),
      '@inactive_count' => count($inactives),
      '@inactives' => implode(', ', $inactives),
    ),
    WATCHDOG_INFO);

  // Return counters as an array.
  return array(
    'total' => $num_players,
    'inserts' => count($inserts),
    'updates' => count($updates),
    'inactives' => count($inactives),
  );
}

/**
 * Updates or inserts given mpxPlayer within Media Library.
 *
 * @param array $player
 *   Record of mpxPlayer data requested from thePlatform
 *
 * @return string
 *   Returns output of media_theplatform_mpx_update_player() or media_theplatform_mpx_insert_player()
 */
function media_theplatform_mpx_import_player($player, MpxAccount $account) {
  $uri = 'mpx://p/' . $player['id'] . '/a/' . basename($account->account_id);
  $fid = db_select('file_managed', 'f')
    ->fields('f', array('fid'))
    ->condition('uri', $uri, '=')
    ->execute()
    ->fetchField();

  // If fid exists:
  if ($fid) {
    // Check if record already exists in mpx_player.
    $existing_player = db_select('mpx_player', 'p')
      ->fields('p')
      ->condition('fid', $fid, '=')
      ->condition('parent_account', $account->id, '=')
      ->execute()
      ->fetchAll();
    $existing_player = (array) reset($existing_player);
    // If mpx_player record exists, then update record.
    if (!empty($existing_player)) {
      return media_theplatform_mpx_update_player($player, $fid, $existing_player, $account);
    }
    // Else insert new mpx_player record with existing $fid.
    else {
      return media_theplatform_mpx_insert_player($player, $fid, $account);
    }
  }
  // Create new mpx_player and create new file.
  else {
    return media_theplatform_mpx_insert_player($player, NULL, $account);
  }
}

/**
 * Inserts given mpxPlayer and File into Media Library.
 *
 * @param array $player
 *   Record of mpxPlayer data requested from thePlatform
 * @param int $fid
 *   File fid of mpxPlayer's File in file_managed if it already exists
 *   NULL if it doesn't exist
 *
 * @return String
 *   Returns 'insert' for counters in media_theplatform_mpx_import_all_players()
 */
function media_theplatform_mpx_insert_player($player, $fid = NULL, $account = NULL) {

  try {
    // If file doesn't exist, write it to file_managed.
    if (!$fid) {
      // Build embed string to create file:
      // "p" is for player.
      $embed_code = 'mpx://p/' . $player['id'] . '/a/' . basename($account->account_id);
      // Create the file.
      $provider = media_internet_get_provider($embed_code);
      $file = $provider->save($account, $player['title']);
      $fid = $file->fid;
      if ($fid) {
        watchdog('media_theplatform_mpx', 'Successfully created file @fid with uri -- @uri -- for player @pid and @account.',
          array(
            '@fid' => $fid,
            '@uri' => $embed_code,
            '@pid' => $player['pid'],
            '@account' => _media_theplatform_mpx_account_log_string($account),
          ),
          WATCHDOG_INFO);
      }
      else {
        watchdog('media_theplatform_mpx', 'Failed to create file with uri -- @uri -- for player @pid and @account.',
          array(
            '@uri' => $embed_code,
            '@pid' => $player['pid'],
            '@account' => _media_theplatform_mpx_account_log_string($account),
          ),
          WATCHDOG_ERROR);
      }
    }

    $insert_fields = array(
      'title' => $player['title'],
      'id' => $player['id'],
      'pid' => $player['pid'],
      'guid' => $player['guid'],
      'description' => $player['description'],
      'fid' => $fid,
      'parent_account' => $player['parent_account'],
      'account' => $player['account'],
      'created' => REQUEST_TIME,
      'updated' => REQUEST_TIME,
      'status' => 1,
    );

    media_theplatform_mpx_debug($player, "Inserting new player {$player['title']} (pid: {$player['pid']}) associated with file $fid");

    // Insert record into mpx_player.
    $player_id = db_insert('mpx_player')
      ->fields($insert_fields)
      ->execute();

    if ($player_id) {
      // When the file_admin module is enabled, setting the "published" property
      // the save handler or in hook_file_presave() won't work.  The value is
      // overridden and set to zero.  Re-save the file entity to publish it.
      if (module_exists('file_admin')) {
        $file->published = 1;
        file_save($file);
      }
      watchdog('media_theplatform_mpx', 'Successfully created new player @pid - "@title" - associated with file @fid for @account.',
        array(
          '@pid' => $player['pid'],
          '@title' => $player['title'],
          '@fid' => $fid,
          '@account' => _media_theplatform_mpx_account_log_string($account),
        ),
        WATCHDOG_NOTICE);
    }
    else {
      watchdog('media_theplatform_mpx', 'Failed to insert new video @pid - "@title" - associated with file @fid for @account into the mpx_video table.',
        array(
          '@pid' => $player['pid'],
          '@title' => $player['title'],
          '@fid' => $fid,
          '@account' => _media_theplatform_mpx_account_log_string($account),
        ),
        WATCHDOG_ERROR);
    }
  }
  catch (Exception $e) {
    watchdog_exception('media_theplatform_mpx', $e,
      'ERROR occurred while creating player @pid -- @title -- for @account.',
      array(
        '@pid' => $player['pid'],
        '@title' => $player['title'],
        '@account' => _media_theplatform_mpx_account_log_string($account),
      ));
  }
  // Return code to be used by media_theplatform_mpx_import_all_players().
  return 'insert';
}

/**
 * Updates given mpxPlayer and File in Media Library.
 *
 * @param array $player
 *   Record of mpxPlayer data requested from thePlatform
 * @param int $fid
 *   File fid of mpxPlayer's File in file_managed
 *
 * @return String
 *   Returns 'update' for counters in media_theplatform_mpx_import_all_players()
 */
function media_theplatform_mpx_update_player($player, $fid, $mpx_player = NULL, $account = NULL) {

  try {
    $update_fields = array(
      'title' => $player['title'],
      'pid' => $player['pid'],
      'guid' => $player['guid'],
      'description' => $player['description'],
      'status' => 1,
    );

    // Update mpx_player record.
    $update = db_update('mpx_player')
      ->fields($update_fields)
      ->condition('id', $player['id'], '=')
      ->condition('fid', $fid, '=')
      ->execute();

    // Update the "updated" field if player data has changed.
    if ($update) {
      $update = db_update('mpx_player')
        ->fields(array('updated' => REQUEST_TIME))
        ->condition('id', $player['id'], '=')
        ->condition('fid', $fid, '=')
        ->execute();
    }

    if ($update) {
      watchdog('media_theplatform_mpx', 'Successfully updated player @pid -- @title -- associated with file @fid for @account.',
        array(
          '@pid' => $player['pid'],
          '@title' => $player['title'],
          '@fid' => $fid,
          '@account' => _media_theplatform_mpx_account_log_string($account),
        ),
        WATCHDOG_NOTICE);
    }
    else {
      watchdog('media_theplatform_mpx', 'Failed to update existing player  @pid -- "@title" -- associated with file @fid for @account in the mpx_player table.  Player data has likely not changed.',
        array(
          '@pid' => $player['pid'],
          '@title' => $player['title'],
          '@fid' => $fid,
          '@account' => _media_theplatform_mpx_account_log_string($account),
        ),
        WATCHDOG_NOTICE);
    }

    // Update file entity with (new) title of player and (un)publish status if
    // the player data has changed.
    if ($update) {
      $player_file = file_load($fid);
      $player_file->status = 1;
      $player_file->filename = $player['title'];
      if (module_exists('file_admin')) {
        $player_file->published = 1;
      }
      file_save($player_file);
    }
  }
  catch (Exception $e) {
    watchdog_exception('media_theplatform_mpx', $e,
      'ERROR occurred while updating player @pid -- @title -- for @account.',
      array(
        '@pid' => $player['pid'],
        '@title' => $player['title'],
        '@account' => _media_theplatform_mpx_account_log_string($account),
      ));
  }
  // Return code to be used by media_theplatform_mpx_import_all_players().
  return 'update';
}

/**
 * Returns associative array of mpx_player data for given field from the
 * mpx_player table.
 */
function media_theplatform_mpx_get_mpx_player_by_field($fid, $field_name, $field_value, $op = '=') {

  return db_select('mpx_player', 'p')
    ->fields('p')
    ->condition($field_name, $field_value, $op)
    ->execute()
    ->fetchAll();
}

/**
 * Returns associative array of mpx_player data for given File $fid.
 */
function media_theplatform_mpx_get_mpx_player_by_fid($fid) {

  return db_query('SELECT * FROM {mpx_player} WHERE fid = :fid',
    array(':fid' => $fid))->fetchAssoc();
}

/**
 * Returns associative array of mpx_player data for given player player_id.
 */
function media_theplatform_mpx_get_mpx_player_by_player_id($player_id) {

  return db_query('SELECT * FROM {mpx_player} WHERE player_id = :player_id',
    array(':player_id' => $player_id))->fetchAssoc();
}
