<?php
// $Id: simple_contest.api.inc,v 1.1.2.1 2010/07/09 04:36:13 vinoth.3v Exp $

define('SIMPLE_CONTEST_STATUS_INACTIVE', 0);
define('SIMPLE_CONTEST_STATUS_ACTIVE', 1);
define('SIMPLE_CONTEST_STATUS_CANCELED', 2);
define('SIMPLE_CONTEST_STATUS_COMPLETED', 3);

/**
 * returns the list of available content types that are marked as simple contest.
 */
function simple_contest_get_types() {
  static $types = array();
  if (!$types) {
    foreach (node_type_get_types() as $type => $info) {      
      if (simple_contest_is_simple_contest($type)) {
        $types[$type] = $info;
      }
    }
  }
  return $types;
}

/**
 * returns the list of available content names that are marked as simple contest.
 */
function simple_contest_get_types_names() {
  static $types = array();
  if (!$types) {
    foreach (simple_contest_get_types() as $type => $info) {
      $types[$type] = $info->name;
    }
  }
  return $types;
}

function simple_contest_is_simple_contest($obj) {
  
  $type = $obj;
  
  if (is_object($type)) {
    $type = $type->type;
  }
  if (is_array($type)) {
    $type = $type['type'];
  }
  return is_array(variable_get('simple_contest_'. $type, FALSE));
}


/**
 * Returns the simple_contest' settings
 * @param $type The content type to return settings for
 * @return array
 * simple contest settings for the content type $type 
 */
function simple_contest_get_settings($type, $refresh = FALSE) {
  static $settings; 
  
  $default_settings = array();
  
  if ($refresh) {
    $default_settings = module_invoke_all('simple_contest', 'default_settings');
  } else if (isset($settings[$type])) {
    return $settings[$type];
  }
  
  $new_settings = variable_get('simple_contest_'. $type, $default_settings);
  
  $new_settings += $default_settings;
  
  $settings[$type] = $new_settings;
  
  return $new_settings;
}

/**
 * Function simple_contest_finalize_results_all
 * 
 */
function simple_contest_finalize_results_all() {

  $fields = field_info_fields();
  //field_info_instances($entity_type = NULL, $bundle_name = NULL)

  $today = date('Y-m-d', time());
  
  

  foreach (simple_contest_get_types() as $type => $info) {
    
    $settings = simple_contest_get_settings($type);

    if(empty($settings['date_field'])) {
      watchdog('simple_contest', 'Date field for the contest %type is not set.'  , array('%type' => $type),  WATCHDOG_WARNING);
      continue;
    }

    // process only for automatic winner selection types
    if($settings['winner_selection_type'] != 'auto') {
      continue;
    }
    
    $field = $fields[$settings['date_field']];
    //print_r($field);
    
    $db_info = date_api_database_info($field);
    
    module_load_include('inc', 'date_api', 'date_api_sql');

    //$db_info = content_database_info($field);
    $table = $db_info['table'];
    $column_name = '';

    foreach ($db_info['columns'] as $column_name => $attributes) {
      $column_name = $attributes['value'];
      $column_name2 = $attributes['value2'];
      break;
    }
    
    // skip if no db info.
    if (empty($column_name)) {
      continue;
    }
    
    
    $query = db_select('node', 'n')
      ->fields('n', array('nid'))
      ->fields('ct', array($column_name, $column_name2));
    
    $query->join('simple_contest_statistics', 'sc', 'n.nid = sc.nid');
    $query->join($table, 'ct', "n.nid = ct.entity_id AND n.vid = ct.revision_id AND ct.entity_type ='node'");
    
    $query->condition('n.status', NODE_PUBLISHED)
      ->condition('n.type', $type)
      ->condition('sc.status', SIMPLE_CONTEST_STATUS_ACTIVE);
    
    //print_r($field);
    //exit();
    
    // TODO
    $def_tz = date_get_timezone($field['settings']['tz_handling']);
    $def_tz_db = date_get_timezone_db($def_tz);
    
    $site_date = new DateObject(REQUEST_TIME, $def_tz_db, date_type_format($field['type']));
    $site_date->limitGranularity($field['settings']['granularity']); 
    date_timezone_set($site_date, timezone_open($def_tz_db));
    
    $site_date_iso = date_format_date($site_date, 'custom', 'Y-m-d\TH:i:s');
    $site_time = strtotime($site_date_iso);
    $site_datetime = date_format_date($site_date, 'custom', 'Y-m-d H:i:s');
    
    if ( $field['type'] == DATE_UNIX ) {
      $query->condition('ct.' . $column_name2, $site_time, '<=');
    } else if ( $field['type'] == DATE_ISO ) {
      $query->condition('ct.' . $column_name2, $site_date_iso, '<=');
    } else {
      $query->condition('ct.' . $column_name2, $site_datetime, '<=');
    }
    
    $res = $query->groupBy('n.nid')->orderBy('ct.' . $column_name)->execute();
    
    foreach( $res as $row ) {
      simple_contest_finalize_results_node(node_load($row->nid));       
    }
     
  }
}

function simple_contest_finalize_results_node($node, $settings = NULL) {

  $nid = $node->nid;
  $type = $node->type;

  if ( !$settings ) {
    $settings = simple_contest_get_settings($type);
  }

  $view = views_get_view($settings['view']);

  if(!$view) {
    watchdog('simple_contest', 'Cannot find the view %view for the contest %type.'  , array('%view' => $settings['view'], '%type' => $type),  WATCHDOG_WARNING);
    return array();
  }
  
  $date_field = $settings['date_field'];
  $arg1 = urlencode($node->{$date_field}['und'][0]['value']);
  $arg2 = urlencode($node->{$date_field}['und'][0]['value2']);
  
  $args = array($nid, $arg1, $arg2); // views must have two atleast null arguments

  $view->set_arguments($args);
  
  if (intval($settings['max_winners'])) {
    $view->set_items_per_page($settings['max_winners']);
  }
  
  $view->init_display();
  $view->pre_execute();
  $view->execute();
  
  $winners = array();

  if (empty($view->result) || !is_array($view->result)) {
    return array();
  }

  $count = 0;
  $field = $settings['view_field'];
  
  foreach ($view->result as $i => $row) {
    
    if(!isset($row->$field)) {
      watchdog('simple_contest', 'The view must have the field ' . $field . ' to select winner for the contest ' . $type,  WATCHDOG_WARNING);
      break;
    }
    $winners[] = $row->$field;
    $count++;
    
    if (intval($settings['max_winners'])) {
      if($count >= $settings['max_winners']) {
        break;
      }
    }
  }
  
  simple_contest_save_result($node, $winners);

}

function simple_contest_save_result($node, $winners, $complete = TRUE) {

  global $language;
  
  if (!count($winners)) {
    drupal_set_message(t('No winners has been choosen.'));
    return;
  }
  
  $settings = simple_contest_get_settings($node->type);

  //delete and complete only if selection_type is not instant.
  if ($settings['winner_selection_type'] != 'instant' ) {
    
    // delete the old
    db_delete('simple_contest_results')
      ->condition('nid', $node->nid)
      ->execute();
  }
  
  if ($complete) {
    db_update('simple_contest_statistics')
      ->condition('nid', $node->nid)
      ->fields(array(
        'status' => SIMPLE_CONTEST_STATUS_COMPLETED,
        'winners_count' => count($winners),
        'timestamp' => time(),
        ))
      ->execute();
  }
  
  $new_winners = array();

  if ($settings['winner_type'] == 'user') { //
    $weight = 1;
    foreach($winners as $uid) { // to keep the weight/rank
      $new_winners[$uid] = array('nid' => NULL, 'weight' => $weight);
      $weight++; //rank
    }
  } else if ($settings['winner_type'] == 'node') {
    // find the user ids
    
    $result = db_select('node', 'n')
      ->fields('n', array(
        'nid',
        'uid',
      ))
      ->where('n.nid IN (:nids)', array(':nids' => $winners))
      ->execute();
  
    foreach( $result as $record ) {
      $tmp_winners[$record->nid] = $record->uid;
    }
    
    $weight = 1;
    
    // if instant type, count from old winners
    if ($settings['winner_selection_type'] == 'instant' ) {
      $weight = db_query("SELECT count(nid) FROM 
        {simple_contest_results}")
          ->condition('nid', $node->nid)
          ->fetchField();
      
      $weight++;
    }
    
    foreach($winners as $nid) { // to keep the weight
      $uid = 0;
      if(isset($tmp_winners[$nid])) {
        $uid = $tmp_winners[$nid];
      }
      $new_winners[$uid] = array('nid' => $nid, 'weight' => $weight);
      $weight++; //rank
    }
  }

  //lets others to change it
  drupal_alter('simple_contest_results', $node, $new_winners);

  # TODO: 
  $sql_values = array();
  
  foreach($new_winners as $uid => $info) {
    $sql_values[] = array(
      'nid' => $node->nid,
      'uid' => $uid,
      'wnid' => $info['nid'],
      'weight' => $info['weight'],
    );
  }
  
  $query = db_insert('simple_contest_results')
    ->fields(array('nid', 'uid', 'wnid', 'weight'));
  foreach ($sql_values as $record) {
    $query->values($record);
  }
  $query->execute();

  
  //invoke hooks
  module_invoke_all('simple_contest', SIMPLE_CONTEST_STATUS_COMPLETED, $node, $new_winners);

  if($settings['email_to_winner']) {
    foreach($new_winners as $uid => $info) {
      
      $account = user_load($uid);
      if (!$account) {
        return;
      }
      if ($info['nid']) {
        $participant_node = node_load($info['nid']);
      } else {
        $participant_node = NULL;
      }
      
      $params = array(
        'contest-node' => $node, 
        'participant-user' => $account,
        'participant-node' => $participant_node, 
        'participant-rank' => $info['weight'], 
        'settings' => $settings,
      );
      
      
      /*
       * // set the new vals of other modules to params. 
      // ???: decent way ?
      
      foreach ( $info as $new_param_key => $new_param_val ) {
        if ( !in_array($new_param_key, array('nid', 'uid', 'wnid', 'weight')) ) {
          $params[$new_param_key] = $new_param_val;
          print_r($new_param_val);    
        }
      }
      */
      
      drupal_mail('simple_contest', 'winners_mail', $account->mail, $language, $params);
      
    }
  }
  
  
  //TODO: email_to_participants
  //Create a new view to get all participants. it should have nid if node contest, uid, uname, user email fields of all participants
  if($settings['email_to_participants']) {
    $participants = simple_contest_get_participants_from_view($node, $settings);
    
    //???: batch process?
  
    foreach( $participants as $data ) {
      
      if ( isset($new_winners[$data['uid']]) ) { // s/he is winner
        continue;
      }
      
      $account = user_load($data['uid']);
      $participant_node = NULL;
      
      if ( $settings['winner_type'] == 'node' ) {
        $participant_node = node_load($data['nid']);
      }
      
      $params = array (
        'contest-node' => $node, 
        'participant-user' => $account,
        'participant-node' => $participant_node, 
        'participant-rank' => -1, // TODO: fix
        'settings' => $settings,
      );
      
      
      drupal_mail('simple_contest', 'winners_mail_participants', $account->mail, $language, $params);
    }
  }
  
  // E-mail to admin
  if($settings['email_to_admin']) {
    $params = array(
      'contest_node' => $node, 
      'settings' => $settings, 
    );
    
    #TODO
    $admin = user_load(1); // ???: multiple admins?, site mail?
    
    drupal_mail('simple_contest', 'admin_winners_mail', $admin->mail, $language, $params);
  }
}

function simple_contest_get_node_result($node) {
  $results = array();
  
  $settings = simple_contest_get_settings($node->type);
  
  $sql = "SELECT cr.uid, u.name, u.mail, cr.wnid, cr.weight 
			FROM {simple_contest_results} cr 
			JOIN {users} u ON u.uid = cr.uid 
			WHERE nid = :nid ORDER BY weight";
  $res = db_query($sql, array(':nid' => $nid));
  
  while($row = fetchAssoc($res)) {
    $results[] = $row;
  }

  return $results;

}


function simple_contest_get_participants_from_view($node, $settings = NULL) {

  $participants = array();
  
  $nid = $node->nid;
  $type = $node->type;

  if(!$settings) {
    $settings = simple_contest_get_settings($type);
  }

  $view = views_get_view($settings['email_to_participants_view']);

  if(!$view) {
    watchdog('simple_contest', 'Cannot find the view %view for the contest %type to get the contest participants.'  , array('%view' => $settings['view'], '%type' => $type),  WATCHDOG_WARNING);
    return array();
  }

  $date_field = $node->{$settings['date_field']};

  //arguments to pass to view %1 = contest node ID, %2 = contest from date, %3 = contest to date 
  $args = array(
    $nid, 
    $date_field['und'][0]['value'],
    $date_field['und'][0]['value2'],
  ); // view must have two atleast null arguments
  
  $view->set_arguments($args);
  $view->init_display();
  
  $view->pre_execute();
  $view->execute();


  if (empty($view->result) || !is_array($view->result)) {
    return $participants;
  }
  
  $vfielduid = $settings['email_to_participants_view_field_uid'];
  $vfieldnid = $settings['email_to_participants_view_field_nid'];
    
  foreach ($view->result as $idx => $row) {
    if($settings['winner_type'] == 'node' && (!isset($row->$vfielduid) || !isset($row->$vfielduid))) {
      watchdog('simple_contest', 'The view must have the fields uid and nid fields for the contest ' . $type,  WATCHDOG_WARNING);
    } else if ( !isset($row->$vfielduid)) {
      watchdog('simple_contest', 'The view must have the fields uid field for the contest ' . $type,  WATCHDOG_WARNING);
    }
    return $participants;
  }
  
  reset($view->result);
  
  //???: what about just returning $view->result ?
  if ($settings['winner_type'] == 'node') {
    foreach ($view->result as $idx => $row) {
      $participants[] = array(
      	'uid' => $row->$vfielduid, 
        'nid' => $row->$vfieldnid,
      );
    }
  } else {
    foreach ($view->result as $idx => $row) {
      $participants[] = array(
      	'uid' => $row->$vfielduid,
      );
    }    
  }
  
  return $participants;

}

/**
 * Function simple_contest_get_active_contests
 * Returns an array of active contest Node IDs of type ctype.
 *
 * @param unknown_type $ctype
 * @param unknown_type $refresh
 *
 * @return array
 * Returns an array of active contest Node IDs of type ctype.
 *
 */
function simple_contest_get_active_contests($ctype = NULL, $refresh = FALSE) {
  static $contests = array();

  if ($ctype && !simple_contest_is_simple_contest($ctype)) {
    return array();
  }

  if ($refresh) {
    if ($ctype) {
      unset($contests[$ctype]);
    } else {
      unset($contests['all']);
    }
  }

  if ($ctype && isset($contests[$ctype])) {
    return $contests[$ctype];
  }else if (isset($contests['all'])) {
    return $contests['all'];
  }

  $results = array();

  //???: should we consider node status here? what about active upcoming contest?
  if ($ctype) {
    $sql = "SELECT scs.nid FROM {simple_contest_statistics} scs
		INNER JOIN {node} n ON n.nid = scs.nid 
		WHERE scs.status = 1 AND n.status <> 0 AND n.type = :type ORDER BY n.title ";
    $res = db_query($sql, array(':type' => $ctype));
  } else {
    $sql = "SELECT scs.nid FROM {simple_contest_statistics} scs
		INNER JOIN {node} n ON n.nid = scs.nid 
		WHERE scs.status = 1 AND n.status <> 0 ORDER BY n.title ";
    $res = db_query($sql);
  }

  foreach( $res as $row ) {
    $results[$row->nid] = $row->nid; // node load ?
  }

  $contests[$ctype] = $results; // save to static

  return $results;

}



/**
 * Helper function to get all fields of a view
 * ???: have any other way to get all field ids?
 * 
 * @param string $view
 * @return array:
 * 
 */
function _simple_contest_get_view_fields($view) {
  $fields = array();
  
  $view = views_get_view($view);
  $view->build();
  
  foreach( $view->field as $fid => $field ) {
    if ( !empty($field->field_alias) && $field->field != 'simple_contest_selector' ) {
      $fields[$field->field_alias] = $field->options['label']?$field->options['label']:$field->field_alias;
    }
  }
  
  return $fields;
}




function simple_contest_status_array() {
 return $options = array(
  SIMPLE_CONTEST_STATUS_INACTIVE => t('Disabled'),
  SIMPLE_CONTEST_STATUS_ACTIVE => t('Active'),
  SIMPLE_CONTEST_STATUS_CANCELED => t('Canceled'),
  SIMPLE_CONTEST_STATUS_COMPLETED => t('Completed'),
 );
}


function simple_contest_status_name($status) {
  
  static $options = array(
    SIMPLE_CONTEST_STATUS_INACTIVE => 'Disabled',
    SIMPLE_CONTEST_STATUS_ACTIVE => 'Active',
    SIMPLE_CONTEST_STATUS_CANCELED => 'Canceled',
    SIMPLE_CONTEST_STATUS_COMPLETED => 'Completed',
 );
  
  if (!isset($options[$status])) {
    return '';
  }
  return $options[$status]; // t() not used here

}

/*
 * Function simple_contest_user_win_count
 * 
 * @param $uid This user ID.
 * @param $type The contest content type (optional)
 * @param $rank The rank of winner (optional)
 * 
 * @return int
 * Win count of a user based on contest type and rank.
 */
function simple_contest_user_win_count($uid, $type = NULL, $rank = NULL) {
  
  $query = db_select('simple_contest_results', 'sc')
    ->fields('sc', array('uid'))
    ->condition('sc.uid', $uid);
  
  if ($rank && !empty($rank)) {
    $query->condition('sc.weight', $rank);
  }
  
  if ($type && !empty($type)) {
    $query->join('node', 'n', 'n.nid = sc.nid');
    $query->condition('n.type', $type);
  }
  
  $total = $query->countQuery()->execute()->fetchField();
  
  return $total;
}

/**
 * function simple_contest_is_instant_winner
 * 
 * select the winner by lucky draw.
 * 
 * @param $cid The contest ndoe ID.
 * @param $wid The winner ID. (User ID if contest type is user, Node ID otherwise.)
 * 
 * @return bool
 * TRUE if wid is selected as winner. false otherwise.
 */ 
function simple_contest_is_instant_winner($contest, $wid) {
  
  if (!$contest) {
    watchdog('simple_contest', 'Contest %cid not found.', array('%cid' => $cid),  WATCHDOG_WARNING);
    return;
  }
  
  $cid = $contest->nid;
  
  if( !simple_contest_is_simple_contest($contest->type)) {
    watchdog('simple_contest', 'Node %cid is not a contest.', array('%cid' => $cid),  WATCHDOG_WARNING);
    return;
  }
  
  if (!isset($contest->simple_contest) || !isset($contest->simple_contest['status'])) {
    watchdog('simple_contest', 'Contest status is not set for the contest %cid.', array('%cid' => $cid),  WATCHDOG_WARNING);
    return;
  }
  
  if ($contest->simple_contest['status'] != SIMPLE_CONTEST_STATUS_ACTIVE ) {
    watchdog('simple_contest', 'Contest %cid is not active to do the lucky draw.', array('%cid' => $cid),  WATCHDOG_WARNING);
    return;
  }
  
  $settings = simple_contest_get_settings($contest->type);
  
  if ($settings['winner_selection_type'] != 'instant' ) {
    watchdog('simple_contest', 'Contest %cid is not in instant mode.', array('%cid' => $cid),  WATCHDOG_WARNING);
    return;
  }
  
  $max = intval($settings['max_winners']);
  
  if ($max > 0 && $max <= $contest->simple_contest['winners_count']) {
    watchdog('simple_contest', 'Contest %cid has reached its max winners limit.', array('%cid' => $cid),  WATCHDOG_WARNING);
    return;
  }
  
  $select1 = mt_rand(1, $settings['winner_select_hard']);
  $select2 = mt_rand(1, $settings['winner_select_hard']);
  
  // winner if random numbers are same.
  // TODO: lets other modules provide the custom logic.
  if ($select1 === $select2) {
    // TODO: limit by interval
    return TRUE;
  }
  
}

/**
 * Unconditionally save the winner to contest.
 * 
 */ 
function simple_contest_save_contest_winner($contest, $wid) {
  
  $winners = array($wid);
  simple_contest_save_result($contest, $winners, FALSE);
  
}

