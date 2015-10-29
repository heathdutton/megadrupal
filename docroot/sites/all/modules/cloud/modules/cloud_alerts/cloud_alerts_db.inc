<?php

/**
 * @file
 * cloud_alerts_db.inc
 * Provides a set of trigger and action for cloud management
 * Works with Cloud module.
 *
 * Copyright (c) 2010-2011 DOCOMO Innovations, Inc.
 *
 */


// alerts functions 

function _cloud_alerts_get() {

  $query = db_select(CLOUD_ALERTS_TABLE, 'a');
  $query
  ->fields('a');

  return $query;
}


function _cloud_alerts_get_count_db() {

  $alerts_count = 0;
  $query = db_select(CLOUD_ALERTS_TABLE, 'a');
  $query->addExpression('COUNT(*)', 'alerts_count');

  $result = $query->execute();
  foreach ($result as $alerts) {
    $alerts_count = $alerts->alerts_count;
  }

  return $alerts_count;
}


function _cloud_alerts_delete($id) {

  //// add watchdog log
  $query        = _cloud_alerts_get();
  $query
  ->condition('alert_id', $id);
  $result = $query->execute();


  $count        = 0;
  $alert_obj    = $result->fetchObject();


  $query = db_delete( CLOUD_ALERTS_TABLE )
  ->condition('alert_id', $id)
  ->execute();

  // User Activity Log
  cloud_audit_user_activity( array(
    'type' => 'user_activity',
    'message' => t('Alert has been deleted: @alert_name', array('@alert_name' => $alert_obj->name)),
    'link' => '',
  )
  );

  $alert_deleted = db_delete( CLOUD_ALERTS_TABLE )
  ->condition('alert_id', $id)
  ->execute();


  $alert_deleted = db_delete(CLOUD_SERVER_TEMPLATES_ALERTS_TABLE)
  ->condition('alert_id', $id)
  ->execute();

  ////
  return;
}

/**
 * Insert a alert in database
 * This function insert entry in database and audit logs as well.
 *
 * @param $form_values
 *        This is the form-values submitted by new alert form
 * @return return a last inserted alert-id
 */
function _cloud_alerts_insert($form_values) {

  $insert_query = db_insert( CLOUD_ALERTS_TABLE )
  ->fields(array(
    'name' => $form_values['name_text'                 ],
    'description' => $form_values['description_text'                 ],
    'metric' => $form_values['metric_select'                 ],
    'variable' => $form_values['variable_select'                 ],
    '`condition`' => $form_values['condition_select'                 ],
    'threshold' => $form_values['threshold_text'                 ],
    'duration_in_minutes' => $form_values['duration_in_minutes_select'                 ],
    'escalate' => $form_values['escalate_select'                 ],
    'created' => date('c'),
    'updated' => date('c'),
  ))
  ->execute();


  // User Activity Log
  cloud_audit_user_activity( array(
    'type' => 'user_activity',
    'message' => t('New Alert has been added: @alert_name', array('@alert_name' => $form_values['name_text'])),
    'link' => '',
  )
  );

  return;
  //watchdog('user_activity',   'New Alert"' . $form_values['name_text'] . '") has been added.',   WATCHDOG_WARNING,   'design/alerts/create&id=' . $id);
}

function _cloud_alerts_update($form_values,     $alert_id = '') {

  $update_query = db_update( CLOUD_ALERTS_TABLE )
  ->fields(array(
    'name' => $form_values['name_text'                 ],
    'description' => $form_values['description_text'                 ],
    'metric' => $form_values['metric_select'                 ],
    'variable' => $form_values['variable_select'                 ],
    '`condition`' => $form_values['condition_select'                 ],
    'threshold' => $form_values['threshold_text'                 ],
    'duration_in_minutes' => $form_values['duration_in_minutes_select'                 ],
    'escalate' => $form_values['escalate_select'                 ],
    'updated' => date('c'),
  ))
  ->condition('alert_id', (!empty($form_values['alert_id']) ? $form_values['alert_id'] : $alert_id))
  ->execute();

  cloud_audit_user_activity( array(
    'type' => 'user_activity',
    'message' => t('Alert has been modified: @alert_name', array('@alert_name' => $form_values['name_text'])),
    'link' => '', // 'design/alerts/create&id=' . $alert_id
  )
  );

  return;
}


function _cloud_alerts_get_all() {

  $query  = _cloud_alerts_get();
  $result = $query->execute();

  $alert_options = array();
  foreach ($result as $alert) {

    $alert_options[$alert->alert_id] = $alert->name;
  }

  //asort($alert_options);
  return $alert_options;
}

//end
