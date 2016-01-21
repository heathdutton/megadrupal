<?php

/**
* This plugin array is more or less self documenting
*/
$plugin = array(
  'title' => t('Unscheduled Sessions Pane'),
  'single' => TRUE,
  'category' => array(t('Conference Organizing'), -9),
  'render callback' => 'cod_schedule_pane_sessions_render'
);



function cod_schedule_pane_sessions_render($subtype, $conf, $context = NULL) {
  $block = new stdClass();
  $block->title = t('Content Types');

  if($event = node_load(array_shift($context))) {
    $block->content = cod_schedule_sessions_admin_page($event);
  } else {
    $block->content = t('Invalid Event Type');
  }
  return $block;
}

function cod_schedule_sessions_admin_page($node) {
  $content = array();
  $content['type_selector'] = array(
    '#type' => 'select',
    '#options' => array(
      'session' => t('Session'),
      'bof_session' => t('BoF Session'),
      'schedule_item' => t('Schedule Item'),
    ),
    '#attributes' => array(
      'id' => 'schedule-items-type-select',
    ),
    '#default_value' => 'session',
  );

  $content['add_new'] = array(
    '#type' => 'button',
    '#value' => t('+'),
    '#attributes' => array(
      'id' => 'schedule-items-add-new',
    ),
  );

  $content['search'] = array(
    '#type' => 'textfield',
    '#attributes' => array(
      'id' => 'schedule-items-filter',
      'placeholder' => 'Search',
    ),
  );

  // Get the unprocessed sessions.
  $sessions = cod_schedule_get_unscheduled_sessions($node->nid, 'session');
  $session_items = cod_schedule_format_unscheduled_sessions($sessions);

   // Build the render array.
  $content['list'] = array(
    '#theme' => 'item_list',
    '#items' =>  !empty($session_items) ? $session_items : array(array('data' => '', 'data-nid' => 'empty')),
    '#attributes' => array(
      'class' => array(
        'sortable-sessions'
      ),
    ),
    '#attached' => array(
      'css' => array(
        drupal_get_path('module', 'cod_schedule') . '/css/cod_schedule.css'
       )
    ),
 );
  return $content;
}
