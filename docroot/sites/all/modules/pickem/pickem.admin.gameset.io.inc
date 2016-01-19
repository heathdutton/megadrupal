<?php

/**
 * @file
 * Includes all gameset input/export functions.
*/


function page_admin_data_import() {
  drupal_set_title(t('Pickem Import Data'), PASS_THROUGH);
  return drupal_get_form('admin_import_form');
}


function admin_import_form($form, $form_state) {

  $dirpath = drupal_get_path('module', 'pickem') . "/data/";
  $dir = opendir($dirpath);
  $list = array();
  $skipped = array();
  while ($filename = readdir($dir)) {
    if ($filename != '.' && $filename != '..') {
      $fullpath = drupal_get_path('module', 'pickem') . "/data/" . $filename;
      $f = fopen($fullpath, "r");
      $string = fread($f, filesize($fullpath));
      fclose($f);
      $gs = unserialize($string);

      // Skip if the file is an old 6.x format (which didn't have a time zone).
      if (empty($gs['gameset']->timezone)) {
        $skipped[] = $filename;
        continue;
      }

      $teams = 'no';
      $weeks = 'no';
      $games = 'no';
      if ( count($gs['teams']) > 0 ) {
        $teams = 'yes';
      }
      if ( count($gs['weeks']) > 0 ) {
        $weeks = 'yes';
      }
      if ( count($gs['games']) > 0 ) {
        $games = 'yes';
      }
      $list[] = array('filename' => $filename, 'teams' => $teams, 'weeks' => $weeks, 'games' => $games);
    }
  }
  closedir($dir);
  if (count($skipped) > 0) {
    drupal_set_message(t('Skipped %count data files which are an older format.', array('%count' => count($skipped))));
  }

  $gamesets = array();
  $options = array();
  foreach ( $list as $file ) {
    $tmp = explode(".", $file['filename']);
    array_pop($tmp);
    $setname = implode(".", $tmp);
    $gamesets[$setname] = $file;
    $options[$setname] = $setname . " (teams=" . $file['teams'] . ", weeks=" . $file['weeks'] . ", games=" . $file['games'] . ")";
  }

  $form['pickem_data_import_gameset_name'] = array(
    '#type' => 'radios',
    '#title' => t('Choose game set to load'),
    '#options' => $options,
    '#description' => t('This will create a new game set including the data from the import file.'),
  );

  $form['pickem_load_data_button'] = array(
    '#type' => 'submit',
    '#value' => t('Load data'),
  );

  $form['#gamesets'] = $gamesets;

  return $form;
}


function admin_import_form_submit($form, &$form_state) {
  $gameset_name = $form_state['values']['pickem_data_import_gameset_name'];
  $gameset = $form['#gamesets'][$gameset_name];
  unserialize_gameset($gameset, $gameset_name);
  drupal_set_message(t("Game Set imported"), "status");

  drupal_goto('admin/pickem/gamesets');
}


function unserialize_gameset($gameset, $gameset_name) {
  $return = TRUE;

  $fullpath = drupal_get_path('module', 'pickem') . "/data/" . $gameset['filename'];
  $file = fopen($fullpath, "r");
  $string = fread($file, filesize($fullpath));
  fclose($file);
  $gs = unserialize($string);

  // Insert the new gameset entry.
  $gsid = db_insert('pickem_gamesets')
    ->fields(array(
      'gs_name' => $gameset_name,
      'sport' => !empty($gs['gameset']->sport) ? $gs['gameset']->sport : '',
      'timezone' => !empty($gs['gameset']->timezone) ? $gs['gameset']->timezone : variable_get('pickem_time_zone', 'US/Eastern')
    ))
    ->execute();

  // Append the unique id to the gameset name.  We don't want duplicate names where you can't tell the difference.
  if (!empty($gsid)) {
    $result = db_query("UPDATE {pickem_gamesets} set gs_name=:gs_name where gsid=:gsid", array(':gs_name' => $gameset_name . '-' . $gsid, ':gsid' => $gsid));
  }

  // TEAMS
  foreach ( $gs['teams'] as $tid => $team ) {

    $new_tid = db_insert('pickem_teams')
      ->fields(array(
        'gsid' => $gsid,
        'tm_city' => $team->tm_city,
        'tm_abbrev' => $team->tm_abbrev,
        'tm_name' => $team->tm_name,
        'tm_nick' => $team->tm_nick,
        'conference' => !empty($team->conference) ? $team->conference : '',
        'division' => !empty($team->division) ? $team->division : '',
        'assetid' => empty($team->assetid)?'':$team->assetid
      ))
      ->execute();

    if (empty($new_tid)) {
      drupal_set_message(t("Insert teams failed"), "error");
      $return = FALSE;
    }
    // save the new tid into the datastructure for the games inserts
    $gs['teams'][$tid]->tid = $new_tid;
  }

  // WEEKS
  foreach ( $gs['weeks'] as $wid => $week ) {
    $new_wid = db_insert('pickem_weeks')
      ->fields(array(
        'wk_number' => $week->wk_number,
        'gsid' => $gsid,
        'wk_name' => $week->wk_name,
        'wk_abbrev' => $week->wk_abbrev,
        'first_gamestart' => !empty($week->first_gamestart) ? $week->first_gamestart : NULL,
        'last_gamestart' => !empty($week->last_gamestart) ? $week->last_gamestart : NULL,
        'wk_points' => $week->wk_points,
        'playoffs' => $week->playoffs,
      ))
      ->execute();

    if (empty($new_wid)) {
      drupal_set_message(t("Insert weeks failed"), "error");
      $return = FALSE;
    }
    // save the new wid into the datastructure for the games inserts
    $gs['weeks'][$wid]->wid = $new_wid;
  }

  // GAMES, use the new tid and new wid
  foreach ( $gs['games'] as $gid => $game ) {

    $id = db_insert('pickem_games')
      ->fields(array(
        'wid' => $gs['weeks'][$game->wid]->wid,
        'gsid' => $gsid,
        'gamestart' => !empty($game->gamestart) ? $game->gamestart : NULL,
        'h_id' => $gs['teams'][$game->h_id]->tid,
        'h_spread' => $game->h_spread,
        'h_score' => $game->h_score,
        'v_id' => $gs['teams'][$game->v_id]->tid,
        'v_spread' => $game->v_spread,
        'v_score' => $game->v_score,
      ))
      ->execute();

    if (!isset($id)) {
      drupal_set_message(t("Insert games failed"), "error");
      $return = FALSE;
    }
  }

  return $return;
}


/**
 * Export function.
 */
function export_gameset($gameset) {

  $gs = array();
  $gs['gameset'] = $gameset;
  $gs['teams'] = get_teams(array('gameset_id' => $gameset->gsid));
  $gs['weeks'] = get_weeks(array('gameset_id' => $gameset->gsid));
  $gs['games'] = get_games(array('gameset_id' => $gameset->gsid));

  $gameset_name = $gameset->gs_name;
  $gameset_name = str_replace(" ", "_", $gameset_name);
  $filename = $gameset_name . '.gameset';
  $fullpath = drupal_get_path('module', 'pickem') . '/data/' . $filename;
  $i=65;  // uppercase A
  // try not to overwrite existing export files.
  while (file_exists($fullpath)) {
    $gameset_name = $gameset->gs_name;
    $gameset_name = str_replace(" ", "_", $gameset_name);
    $filename = $gameset_name . '-' . chr($i) . '.gameset';
    $fullpath = drupal_get_path('module', 'pickem') . '/data/' . $filename;
    if ( $i >= 90 ) {
      break;
    }
    $i++;
  }

  $data = serialize($gs);
  file_unmanaged_save_data($data, $fullpath, FILE_EXISTS_REPLACE);

  $t_string = t('Exported game set %gameset-name to %export-dir', array('%gameset-name' => $gameset_name, '%export-dir' => $fullpath));
  drupal_set_message($t_string);
  drupal_goto('admin/pickem/gamesets');
}
