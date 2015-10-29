<?php

/**
 * @file
 * Provides scripting feature such as bash, Perl and etc (Mainly bash scripts).
 * Works with Cloud and Server Templates module.
 *
 * Copyright (c) 2010-2011 DOCOMO Innovations, Inc.
 *
 */

// scripting functions
//start
function _cloud_scripting_get_scripts() {

  $query = db_select(CLOUD_SCRIPTING_TABLE, 'a');
  $query
  ->fields('a');

  return $query;
}

function _cloud_scripting_get_script_query() {

  $query = db_select(CLOUD_SCRIPTING_TABLE, 'a');
  $query
  ->fields('a');

  return $query;
}


function _cloud_scripting_get_count_db() {

  $script_count = 0;

  $query = db_select(CLOUD_SCRIPTING_TABLE, 'a');
  $query->addExpression('COUNT(*)', 'script_count');

  $result = $query->execute();
  foreach ($result as $script) {

    $script_count = $script->script_count;
  }

  return $script_count;
}


function _cloud_scripting_get_script_by_id($script_id) {

  $query = db_select(CLOUD_SCRIPTING_TABLE, 'a');
  $result = $query
  ->fields('a')
  ->condition('script_id', $script_id)
  ->execute();

  $scripts_obj = $result->fetchObject();

  return $scripts_obj;
}


function _cloud_scripting_delete($id) {

  //// add watchdog log

  $query  = _cloud_scripting_get_scripts();
  $query
    ->condition('script_id', $id, '=');
  $result = $query->execute();
  $count = 0;

  $scripts_obj    = $result->fetchObject();

  cloud_audit_user_activity( array(
    'type' => 'user_activity',
    'message' => t('Script has been deleted: @script_name', array('@script_name' => $scripts_obj->name)),
    'link' => '',
  )
  );

  //delete script

  $query = db_delete(CLOUD_SCRIPTING_TABLE)
  ->condition('script_id', $id)
  ->execute();

  //DELETE FROM scripting template table

  $query = db_delete(CLOUD_SERVER_TEMPLATES_SCRIPTS_TABLE)
  ->condition('script_id', $id)
  ->execute();


  //delete from cloud_inputs table
  if (module_exists('cloud_inputs')) {

    $query = db_select(CLOUD_SCRIPTING_INPUT_PARAMETER_TABLE, 'a');
    $query
      ->fields('a', array('param_id'))
      ->condition('script_id', $id);
    $result = $query->execute();



    while ($param = $result->fetchField()) {

      $query = db_delete(CLOUD_INPUTS_PARAMETER_VALUES_TABLE)
          ->condition('param_id', array($param))
          ->execute();
    }
  }

  //finally delete cloud_scripting_input_parameter
  $query = db_delete(CLOUD_SCRIPTING_INPUT_PARAMETER_TABLE)
          ->condition('script_id', array($id))
          ->execute();

  return;
}


/**
 * Insert a script in database
 * This function insert entry in database and audit logs as well.
 *
 * @param $form_values
 *        This is the form-values submitted by new script form
 * @return return a last inserted script-id
 */
function _cloud_scripting_insert_script($form_values) {
  $id = $insert_query = db_insert(CLOUD_SCRIPTING_TABLE)
  ->fields(array(
    'name' => $form_values['name_text'                 ],
    'description' => $form_values['description_text'                 ],
    'type' => $form_values['script_type_select'                 ],
    'packages' => '', //package not supported right now
    'inputs' => $form_values['inputs_text'                 ],
    'script_template' => $form_values['script_template_text'                 ],
    'created' => date('c'),
    'updated' => date('c'),
  ))
  ->execute();

  if ( module_exists('cloud_inputs')) {

    $inputs = explode(',', $form_values['inputs_text']);

    if (is_array($inputs) && count($inputs) > 0) {

      foreach ($inputs as $input) {

        if (!empty($input)) {
          $insert_input_query = '';

          $insert_input_query = db_insert(CLOUD_SCRIPTING_INPUT_PARAMETER_TABLE)
        ->fields(array(
            'script_id' => $id,
            'input_parameter' => $input,
            'param_type' => 'text',
          ))
        ->execute();
        }
      }
    }
  }
  // User Activity Log
  cloud_audit_user_activity( array(
    'type' => 'user_activity',
    'message' => t('New Script has been added: @script_name', array('@script_name' => $form_values['name_text'])),
    'link' => '', //d'design/scripting/describe/info&id=' . $id);
  )
  );
  return $id;
}

function _cloud_scripting_update_script($form_values, $script_id = '') {

  $update_query = db_update(CLOUD_SCRIPTING_TABLE)
  ->fields(array(
    'name' => $form_values['name_text'                 ],
    'description' => $form_values['description_text'                 ],
    'type' => $form_values['script_type_select'                 ],
    'packages' => '',
    'inputs' => $form_values['inputs_text'                 ],
    'script_template' => $form_values['script_template_text'                 ],
    'updated' => date('c'),
  ))
  ->condition('script_id', (!empty($form_values['script_id']) ? $form_values['script_id'] : $script_id))
  ->execute();

  ///insert into script input parameter
  if ( module_exists('cloud_inputs')) {
    $inputs = explode(',', $form_values['inputs_text']);
    $inputs_old = explode(',', $form_values['inputs_text_old']);


    foreach ($inputs_old as $input_old) {
      //grab a list of old ids - use it to delete the old values
      //in cloud_inputs_parameter_values table. Otherwise they
      //will be orphaned!
      if (module_exists('cloud_inputs')) {

        $old_id = db_query("SELECT param_id from {" . CLOUD_SCRIPTING_INPUT_PARAMETER_TABLE . "} where script_id = ':script_id' and input_parameter = ':input_old'", array(':script_id' => $script_id, ':input_old' => $input_old))->fetchField();
        if ($old_id != FALSE) {

          $query = db_delete(CLOUD_INPUTS_PARAMETER_VALUES_TABLE)
        ->condition('param_id', array($old_id))
        ->execute();
        }
      }


      $query = db_delete(CLOUD_SCRIPTING_INPUT_PARAMETER_TABLE)
      ->condition('script_id', $script_id)
      ->condition('input_parameter', $input_old)
      ->execute();
    }

    foreach ($inputs as $input) {

      if (!empty($input) ) {

        $get_query = db_select(CLOUD_SCRIPTING_INPUT_PARAMETER_TABLE, 'a');
        $get_query
        ->condition('script_id', $script_id)
        ->condition('input_parameter', $input)
        ->fields('a');

        $result = $get_query->execute();


        $total_input_rows = !empty($result->num_rows) ? $result->num_rows : ''; //mysql_num_rows($result1);

        if ($total_input_rows <= 0) {
          //print $total_input_rows;die;

          $nid = db_insert(CLOUD_SCRIPTING_INPUT_PARAMETER_TABLE)
        ->fields(array(
            'script_id' => $script_id,
            'input_parameter' => $input,
            'param_type' => 'text',
          ))
        ->execute();
        }
      }
    }
  }
  // User Activity Log
  cloud_audit_user_activity( array(
    'type' => 'user_activity',
    'message' => t('Script has been modified: @script_name', array('@script_name' => $form_values['name_text'])),
    'link' => '', // 'design/scripting/describe/info&id=' . $script_id);
  )
  );

  return;
}




function _cloud_scripting_get_scripts_of_template() {

  return $query = 'SELECT * FROM {' . CLOUD_SERVER_TEMPLATES_SCRIPTS_TABLE . '} c WHERE server_template_id=:server_template_id ORDER BY `order`';
}


function _cloud_scripting_get_executed_scripts() {


  $query = db_select(CLOUD_SCRIPTING_INSTANCES_TABLE, 'a');
  $query
      ->fields('a')
      ->condition('script_executed', 2);

  return cloud_get_db_results($query);
}


function _cloud_scripting_get_selected_scripts($str_script) {

  $query = db_select(CLOUD_SCRIPTING_TABLE, 'a');
  $query
      ->fields('a')
      ->condition('script_id', '(' . $str_script . ')', 'IN');

  return cloud_get_db_results($query);
}


function _cloud_scripting_get_script($script_id) {

  $query = db_select(CLOUD_SCRIPTING_TABLE, 'a');
  $query
      ->fields('a')
      ->condition('script_id', array($script_id), 'IN');
  $return_val = $query->execute()->fetchAssoc();
  //$return_val = cloud_get_db_results($query);
  //return $return_val[0];
  return $return_val;
}


function _cloud_scripting_register_exec_save($instance_id, $script_id, $status) {

  $query = db_select(CLOUD_SCRIPTING_INSTANCES_TABLE, 'a');
  $query
  ->fields('a')
  ->condition('instance_id', $instance_id)
  ->condition('script_id', $script_id);

  $result = $query->execute();


  $found = FALSE;
  foreach ($result as $key) {

    $found = TRUE;
  }

  drupal_log_to_file('DEPLOYMENT::EXECUTE_SCRIPT', 'DRUPAL_BASE', 'INFO', 'FOUND=' . $found );
  if ($found) { // Update Table
    $update_query = db_update(CLOUD_SCRIPTING_INSTANCES_TABLE)
      ->fields(array(
      'script_executed' => $status,
    ))
      ->condition('instance_id', $instance_id)
      ->condition('script_id', $script_id)
      ->execute();

    drupal_log_to_file('DEPLOYMENT::EXECUTE_SCRIPT', 'DRUPAL_BASE', 'INFO', 'UPDATE=' . $update_query );
  }
  else { // Insert in table
    $insert_query = db_insert(CLOUD_SCRIPTING_INSTANCES_TABLE)
      ->fields(array(
      'instance_id' => $instance_id,
      'script_id' => $script_id,
      'script_executed' => $status,
    ))
      ->execute();
    drupal_log_to_file('DEPLOYMENT::EXECUTE_SCRIPT', 'DRUPAL_BASE', 'INFO', 'INSERT=' . $insert_query );
  }

  return;
}


/**
 * function for checking if already there is same script ready for execution. *
 * @param unknown_type $instance_id
 * @param unknown_type $script_id
 * @return TRUE/FALSE
 */
function _cloud_scripting_is_ready_for_exec($instance_id, $script_id) {


  $query = db_select('cloud_instance_script', 'a');
  $query
  ->fields('a')
  ->condition('instance_id', $instance_id)
  ->condition('script_id', $script_id)
  ->condition('script_executed', 0);

  $row_count = count(cloud_get_db_results($query));

  if ($row_count > 0) {
    return TRUE;
  }
  else {
    return FALSE;
  }
}


/**
 * function for adding script ready for execution on instance.
 * @return
 */
function _cloud_scripting_add_ready_for_exec_on_instance($instance_id, $script_id, $script_executed, $cloud, $cluster_id, $template_id) {

  if ( _cloud_scripting_is_ready_for_exec($instance_id, $script_id)) {
    return;
  }

  $insert_query = db_insert(CLOUD_SCRIPTING_INSTANCES_TABLE)
      ->fields(array(
    'instance_id' => $instance_id,
    'started' => 'now()',
    'script_id' => $script_id,
    'script_executed' => $script_executed,
    'cluster_id' => $cluster_id,
    'cloud' => $cloud,
    'template_id' => $template_id,
  ))
      ->execute();

  return 0;
}

function _cloud_scripting_register_script_save($instance_id, $script_id, $status, $cloud, $template_id) {

  $insert_query = db_insert(CLOUD_SCRIPTING_INSTANCES_TABLE)
      ->fields(array(
    'instance_id' => $instance_id,
    'started' => 'now()',
    'script_id' => $script_id,
    'script_executed' => $status,
    'cloud' => $cloud,
    'template_id' => $template_id,
  ))
      ->execute();

  return;
}


function _cloud_scripting_get_master_script_id_query() {

  $query = db_select(CLOUD_SCRIPTING_MASTER_TABLE, 'a');
  $query
  ->fields('a');
  return $query;

}


function _cloud_scripting_get_by_script_id_instance_id() {

  return $query = 'SELECT * FROM {' . CLOUD_SCRIPTING_INSTANCES_TABLE . "} c WHERE instance_id=:instance_id AND script_id=:script_id ";

}

/**
 *
 * @param type $instance_id
 * @return type
 */
function _cloud_scripting_under_progress_scripts_for_instance_db($params) {

  $instance_id = $params['instance_id'];
  $result = array();


  $query = db_select(CLOUD_SCRIPTING_INSTANCES_TABLE, 'a');
  $query
  ->fields('a')
  ->condition('instance_id', $params['instance_id'])
  ->condition('script_executed', '2', '!=');
  $result_scripts = $query->execute();

  foreach ($result_scripts as $script) {

    $result[] = $script->script_id;
  }

  return $result;
}


function _cloud_scripting_get_scripts_for_instance($instance_id) {


  $scripts_query      = $query = 'SELECT inst_script.script_id FROM {' . CLOUD_SCRIPTING_INSTANCES_TABLE . "} inst_script LEFT JOIN {" . CLOUD_SCRIPTING_TABLE . "} scr ON inst_script.script_id=scr.id WHERE instance_id=':instance_id' ";
  $result = array();
  $result_scripts = db_query( $scripts_query, array(':instance_id' => $instance_id) );

  foreach ($result_scripts as $script) {

    $result[] = $script->script_id;
  }

  return $result;
}

/**
 *
 * @param $instance_id
 * @return unknown_type
 */
function _cloud_scripting_get_scripts_ready_for_instance_query($instance_id, $script_id) {

  $query = '';
  if (empty($script_id)) {
    $query = 'SELECT vis.instance_id, vis.script_id script_id, vis.cluster_id, vis.cloud, vis.template_id, s.script_template script FROM {' . CLOUD_SCRIPTING_INSTANCES_TABLE . '} vis, 
          {' . CLOUD_SCRIPTING_TABLE . "} s WHERE s.id = vis.script_id and vis.instance_id = '" . $instance_id . "' ORDER BY vis.id";
  }
  else {
    $query = 'SELECT vis.instance_id, vis.script_id script_id, vis.cluster_id, vis.cloud, vis.template_id, s.script_template script FROM {' . CLOUD_SCRIPTING_INSTANCES_TABLE . '} vis, 
          {' . CLOUD_SCRIPTING_TABLE . "} s WHERE s.id = vis.script_id and vis.instance_id = '" . $instance_id . "' and vis.script_id = '" . $script_id . "' ORDER BY vis.id";
  }

  return $query;
}


/**
 *
 * @param $cluster_id
 * @param $script_id
 * @return unknown_type
 */
function _cloud_scripting_get_cluster_input_query($cluster_id, $script_id) {

  $query = 'SELECT ip.script_id, ip.param_type, ip.input_parameter, pv.cluster_value, pv.value_of_instance_id FROM {CLOUD_SCRIPTING_INPUT_PARAMETER_TABLE} ip, 
                        {' . CLOUD_INPUTS_PARAMETER_VALUES_TABLE . " } pv WHERE ip.param_id = pv.param_id AND pv.cluster_id = " . $cluster_id . " and ip.script_id = '" . $script_id . "'";
  return $query;
}


/**
 *
 * @param <type> $template_id
 * @param <type> $script_id
 * @return array()
 */
function _cloud_scripting_get_script_inputs($template_id, $script_id) {

  $query = 'SELECT ip.script_id, ip.param_type, ip.input_parameter, pv.template_value, pv.value_of_instance_id FROM {' . CLOUD_SCRIPTING_INPUT_PARAMETER_TABLE . '} ip,
                        {' . CLOUD_INPUTS_PARAMETER_VALUES_TABLE . "} pv WHERE ip.param_id = pv.param_id AND pv.template_id = :template_id and ip.script_id = :script_id";

  return cloud_get_db_results($query, array(':template_id' => $template_id, ':script_id' => $script_id));
}

function _cloud_scripting_get_cluster_script_inputs($script_id, $server_id, $cluster_id) {
  $cluster_query = 'SELECT ip.script_id, ip.param_type, ip.input_parameter, pv.cluster_value FROM {' . CLOUD_SCRIPTING_INPUT_PARAMETER_TABLE . '} ip,
            {' . CLOUD_INPUTS_PARAMETER_VALUES_TABLE . "} pv WHERE ip.param_id = pv.param_id AND pv.server_id= :server_id and ip.script_id = ':script_id' and pv.cluster_id =:cluster_id ";
  return cloud_get_db_results($cluster_query, array('$server_id' => $server_id, ':script_id' => $script_id, 'cluster_id' => $cluster_id));

}
/**
 * function for getting script id from master scripts table.
 * @return script_id(varchar)
 */
function _cloud_scripting_get_id_from_master($script) {

  $master_script_query      = _cloud_scripting_get_master_script_id_query();
  $master_script_query
    ->condition('script_name', $script);
  $result_master_script = $query->execute();
  $script_id = '';

  if ($script_data = $result_master_script->fetchObject()) {

    $script_id = $script_data->script_id;
  }

  return $script_id;
}

/*
 * Required functions
 */

/**
 * Function that updates scripting status
 * @param <type> $instance_id
 * @param <type> $script_ids
 * @param <type> $script_executed
 * @return <type>
 * function for Updating the current status for Script.
 */
function _cloud_scripting_update_scripts_status($instance_id, $script_id, $script_execution_status) {
  $update_query = db_update(CLOUD_SCRIPTING_INSTANCES_TABLE)
    ->fields(array(
      'script_executed' => $script_execution_status,
    ))
    ->condition('instance_id', $instance_id)
    ->condition('script_id', $script_id)
    ->execute();
  return;
}

/***NEW FUNCTIONS ADDED****/

/**
 * Database function to get scripts that are ready to execute
 * @param <type> $cloud_context
 * @return <type>
 */
function _cloud_scripting_get_execute_ready_scripts($cloud_context) {

  $query  = 'SELECT * FROM {' . CLOUD_SCRIPTING_INSTANCES_TABLE . "} WHERE `script_executed` = '" . CLOUD_SCRIPTING_SCRIPT_READY_FOR_EXECUTION . "' AND instance_id NOT IN ";
  $query .= ' ( SELECT instance_id FROM {' . CLOUD_SCRIPTING_INSTANCES_TABLE . "} WHERE `script_executed` = '" . CLOUD_SCRIPTING_SCRIPT_UNDER_EXECUTION . "') ORDER BY instance_id, id";

  return cloud_get_db_results($query);
}

/**
 * Database function to check a script's execution progress 
 */
function _cloud_scripting_get_under_progress_scripts() {
  $scripts = array();
  $result = db_select(CLOUD_SCRIPTING_INSTANCES_TABLE, 'a')
    ->fields('a')
    ->condition('script_executed', CLOUD_SCRIPTING_SCRIPT_UNDER_EXECUTION)
    ->execute();
  while ($record = $result->fetchAssoc()) {
    $scripts[] = $record;
  }
  return $scripts;
}
