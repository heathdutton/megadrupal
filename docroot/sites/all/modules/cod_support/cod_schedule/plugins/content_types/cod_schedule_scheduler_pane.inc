<?php
/**
* This plugin array is more or less self documenting
*/
$plugin = array(
  'title' => t('Scheduler Pane'),
  'single' => TRUE,
  'category' => array(t('Conference Organizing'), -9),
  'render callback' => 'cod_schedule_pane_scheduler_render'
);

function cod_schedule_pane_scheduler_render($subtype, $conf, $context = NULL) {
  $block = new stdClass();
  // $block->title = t('Room/Timeslot Scheduler');

  if($event = node_load(array_shift($context))) {
    $block->content = cod_schedule_schedule_admin_page($event, array_shift($context));
  } else {
    $block->content = t('Invalid Event Type');
  }
  return $block;
}


function cod_schedule_schedule_admin_page($conference, $day) {
  module_load_include('inc', 'cod_schedule', 'includes/schedule.admin');

  // Grab the days.
  //$days = cod_schedule_days($conference);
  if (empty($day)) {
    return t('Your event must have a <a href="@edit">start and end date</a>. <a href="@edit">Edit</a> event.', array('@edit' => url('node/'.$conference->nid.'/edit', array('fragment' => 'edit-field-dates', 'query' => array('destination' => 'admin/conference/' . $conference->nid . '/schedule')))));
  }

  // Grab the Rooms attached to the event.
  $room_query = new EntityFieldQueryExtraFields();
  $rooms = $room_query->entityCondition('entity_type', 'cod_event_room')
    ->addExtraField('', 'name', 'name', 'cod_event_room')
    ->fieldCondition('og_group_ref', 'target_id', $conference->nid)
    // TODO: Weight property isn't fully coded.
    // ->propertyOrderby('weight')
    ->execute();

  // Provide feedback to help administrators setup the site.
  if (empty($rooms)) {
    return t('You must add a room to view the schedule.');
  }

  $session_query = new EntityFieldQueryExtraFields();
  $sessions = $session_query->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', array('session', 'bof_session', 'schedule_item'), 'IN')
    ->fieldCondition('og_group_ref', 'target_id', $conference->nid)
    ->addExtraField('', 'title', 'title', 'node')
    ->execute();

  if (!empty($sessions)) {
    $sessions = $sessions['node'];
  }

  $fields = field_info_instances('node', 'session');
  $field_id = $fields['field_session_timeslot']['field_id'];
  field_attach_load('node', $sessions, FIELD_LOAD_CURRENT, array('field_id' => $field_id));

  $schedule_items = array();
  foreach($sessions as $session) {
    if(!empty($session->field_session_timeslot)) {
      $schedule_items[$session->field_session_timeslot['und'][0]['target_id']][] = $session;
    }
  }

  $tz = new DateTimeZone($conference->field_dates[LANGUAGE_NONE][0]['timezone']);
  $tz_db = new DateTimeZone($conference->field_dates[LANGUAGE_NONE][0]['timezone_db']);

  $tz_time_db = new DateTime("now", $tz_db);
  $tz_time = new DateTime("now", $tz);

  // Get opposite of the offset.
  $offset = $tz->getOffset($tz_time_db) * -1;

  $day_start = strtotime($day . ' 00:00:00') + $offset;
  $day_end = strtotime('+1 day', $day_start);

  $time_query = new EntityFieldQueryExtraFields();
  $times = $time_query->entityCondition('entity_type', 'cod_event_time')
    ->addExtraField('', 'date_label', 'date_label', 'cod_event_time')
    ->fieldCondition('og_group_ref', 'target_id', $conference->nid)
    ->fieldCondition('field_evttime_date', 'value', format_date($day_start, 'custom', 'Y-m-d H:i:s'), '>=')
    ->fieldCondition('field_evttime_date', 'value2', format_date($day_end, 'custom', 'Y-m-d H:i:s'), '<=')
    ->fieldOrderBy('field_evttime_date', 'value')
    ->execute();

  $header = array(t('Times'), t('Rooms'));
  $rows = array();

  if (isset($times['cod_event_time'])) {
    foreach ($times['cod_event_time'] as $evttid => $time) {
      $time_entity = entity_load('cod_event_time', array($evttid));
      $time_label = $time_entity[$evttid]->getTimeLabel();
      $time_edit_uri = 'admin/conference/config/times/manage/' . $evttid;
      $row = array(
        array(
          'data' => l($time_label, $time_edit_uri, array('query' => array('destination' => 'admin/conference/'.$conference->nid.'/schedule'))),
          'class' => 'cod-schedule-time-column',
        )
      );

      $room_table_rows = array();
      foreach ($rooms['cod_event_room'] as $roomid => $room) {
        $room_table_row = array(
          array(
            'data'=> l($room->extraFields->name, 'admin/conference/config/rooms/manage/' . $roomid, array('query' => array('destination' => 'admin/conference/'.$conference->nid.'/schedule')))
            , 'style'=>'width:125px;'));

        $render = cod_schedule_schedule_grid_item($conference, $evttid, $roomid, TRUE, $schedule_items);
        $room_table_row[] = drupal_render($render);
        $room_table_rows[] = $room_table_row;
      }

      if(!empty($room_table_rows)) {
        $table_rows_what_now = array('data' => theme('table', array( 'rows' => $room_table_rows)));
        $row[] = $table_rows_what_now;
      }
      else {
        $row[] = "";
      }

      $rows[] = $row;
    }
  }

  // Add help content to empty tabs.
  if (empty($rows)) {
    $contents =  t('Add a time to build a schedule for this day.');
  }
  else {
    $contents = theme('table', array('header' => $header, 'rows' => $rows));
  }

  drupal_set_title("");
  ctools_include('modal');
  ctools_modal_add_js();
  drupal_add_library('system', 'ui.sortable');
  drupal_add_js(drupal_get_path('module', 'cod_schedule') .'/js/cod_schedule.js');
  return $contents;
}
