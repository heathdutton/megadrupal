<?php
// $Id: simple_contest_repeat.helper.inc,v 1.1.2.1 2010/07/09 04:36:13 vinoth.3v Exp $

/**
 * @file simple_contest_repeat.helper.inc
 *
 */

/**
 * Function simple_contest_repeat_create_contests.
 *
 */
function simple_contest_repeat_create_contests() {
  
  $fields = field_info_fields();

  foreach (simple_contest_get_types() as $type => $info) {

    $settings = simple_contest_get_settings($type);
    
    if (isset($settings['auto_create']) && $settings['auto_create'] == 1 && !empty($settings['rrule']) ) {
      $date_field = $settings['date_field'];
      $field = $fields[$date_field];
      
      $title = t($info->name) . '!start - !end';
      $body = '';
      
      if (isset($settings['contest_defaults']['node_title'])) {
        $title = $settings['contest_defaults']['node_title'];
      }
      if (isset($settings['contest_defaults']['node_body'])) {
        $body = $settings['contest_defaults']['node_body'];
      }
      
      $timezone = variable_get('date_default_timezone', 0);
      
      $nextday = date('Y-m-d', strtotime("+1 day"));
      
      $sql = "SELECT * FROM {simple_contest_repeat} 
			WHERE nid <= 0 AND type = :type AND DATE_FORMAT(start_date, '%Y-%m-%d') <= '". $nextday ."' 
			ORDER BY start_date ASC";
      
      $res = db_query($sql, array(':type' => $type));
      
      foreach( $res as $row ) {
        
        $def_node = new stdClass();
        
        $def_node->type = $type;
        
        node_object_prepare($def_node);
        
        $def_node->title = t($title, array('!start' => $row->start_date, '!end' => $row->end_date));
        
        $def_node->language = LANGUAGE_NONE;

        $def_node->body[LANGUAGE_NONE][0]['value']   = $body;
        $def_node->body[LANGUAGE_NONE][0]['summary'] = text_summary($body);
        $def_node->body[LANGUAGE_NONE][0]['format']  = 'filtered_html';
        
        $def_node->uid = 1; // admin
        $def_node->is_new = TRUE;
        $def_node->status = NODE_PUBLISHED;
        $def_node->simple_contest = array(
          'status' => SIMPLE_CONTEST_STATUS_ACTIVE,
        );
        
        $date_start = new DateObject($row->start_date, $settings['tz_handling'], DATE_FORMAT_DATETIME);
        $date_end = new DateObject($row->end_date, $settings['tz_handling'], DATE_FORMAT_DATETIME);
        
        $def_node->{$date_field}[LANGUAGE_NONE][0] = array(
          'value' => $date_start->format(date_type_format($field['type'])),
          'value2' => $date_end->format(date_type_format($field['type'])),
          'offset' => date_offset_get($date_start),
          'offset2' => date_offset_get($date_end),
          'timezone' => $settings['tz_handling'],
        );

        node_save($def_node); // save the new node
        
        // ???: UPDATE OR DELETE?
        db_update('simple_contest_repeat')
          ->fields(array(
            'nid' => $def_node->nid,
          ))
          ->condition('rid', $row->rid)
          ->execute();
      }
    }
  }

}


/**
 * Helper function to build repeating dates from a $node_field.
 *
 * Pass in either the RRULE or the $form_values array for the RRULE,
 * whichever is missing will be created when needed.
 */
function simple_contest_repeat_date_repeat_build_dates($type, $settings, $rrule, $rrule_values) {
  
  $fields = field_info_fields();
  
  // delete all nulls for this contest type
  $sql = "DELETE FROM {simple_contest_repeat}
    WHERE type = :type AND nid <= 0";
  db_query($sql, array(':type' => $type));

  if(!$settings['auto_create']) {
    return;
  }
  
  if (empty($settings)) {
    $settings = simple_contest_get_settings($type);
  }
  
  $date_field = $fields[$settings['date_field']];
  
  $db_info = date_api_database_info($date_field);
  $field_name = $date_field['field_name'];

  $tz = $settings['tz_handling'];

  $timezone = date_get_timezone($tz, '');

  $timezone_db = date_get_timezone_db($tz);
  
  $start = date_now();
  
  if ($timezone != $timezone_db) {
    date_timezone_set($start, timezone_open($timezone));
  }

  $item = array(
    'timezone' => $tz,
    'value' => $start->format(date_type_format($date_field['type'])),
    'value2' => $start->format(date_type_format($date_field['type'])),
    'rrule' => $rrule,
    'show_repeat_settings' => 1,
    'offset' => 10800,
    'offset2' => 10800,
    'show_todate' => 1,
  );
  
  if (!empty($rrule_values['UNTIL'])) {
    $rrule_values['UNTIL']['datetime'] .= ' 23:59:59';
    $rrule_values['UNTIL']['granularity'] = serialize(drupal_map_assoc(array('year', 'month', 'day', 'hour', 'minute', 'second')));
    $rrule_values['UNTIL']['all_day'] = 0;
  }
  module_load_include('inc', 'date', 'date_repeat');
  
  $values = date_repeat_build_dates($rrule, $rrule_values, $date_field, $item);
  
  $query = db_insert('simple_contest_repeat')
    ->fields(array('type', 'start_date', 'end_date', 'nid'));
  
  foreach ($values as $delta => $date) {
    $v = format_date($date['value'], 'custom', DATE_FORMAT_DATETIME);
    $v2 = format_date($date['value2'], 'custom', DATE_FORMAT_DATETIME);
    $query->values(array($settings['type'], $v, $v2, 0));
  }
  
  $query->execute();
  
  return $values;
  
}
