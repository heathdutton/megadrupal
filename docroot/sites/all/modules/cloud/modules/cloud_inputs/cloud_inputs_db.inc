<?php

/**
 * @file
 * Provides input parameter user interface for Server Templates and Cluster module.
 * Works with Cloud module.
 *
 * Copyright (c) 2010-2011 DOCOMO Innovations, Inc.
 *
 */

function _cloud_inputs_list_get($id = '', $col, $str) {

  return $query =  "SELECT distinct *, a.param_id as param_id,  b.template_id as server_template_id FROM {" . CLOUD_SCRIPTING_INPUT_PARAMETER_TABLE . "} a
               LEFT JOIN {" . CLOUD_INPUTS_PARAMETER_VALUES_TABLE . "} b on a.param_id=b.param_id and template_id='$id' 
               where a.script_id in (select script_id from {" . CLOUD_SERVER_TEMPLATES_SCRIPTS_TABLE . "} where  $col='$str' ) group by a.input_parameter order by b.param_id";

}

function _cloud_inputs_get($id = '') {

  return $query =  "SELECT distinct *, a.param_id as param_id,  b.template_id as server_template_id FROM {" . CLOUD_SCRIPTING_INPUT_PARAMETER_TABLE . "} a
               LEFT JOIN {" . CLOUD_INPUTS_PARAMETER_VALUES_TABLE . "} b on a.param_id=b.param_id and template_id='$id' 
               where a.script_id in (select script_id from {" . CLOUD_SERVER_TEMPLATES_SCRIPTS_TABLE . "} where  server_template_id=:server_template_id) group by a.input_parameter order by b.param_id";


}

function _cloud_inputs_get_cluster($cluster_id, $server_id = -1) {
  $template_ids = _cloud_inputs_get_templates_by_cluster($cluster_id);
  $search_where = ' where 1';
  $join_on = '';
  $cluster_only = '';

  if (_cloud_inputs_get_cluster_count($cluster_id) > 0) {
    $search_where .= ' and cluster_id = ' . $cluster_id;
  }
  else {
    if ($template_ids) {
      $search_where .= ' and a.script_id in (select script_id from {' . CLOUD_SERVER_TEMPLATES_SCRIPTS_TABLE . '} where server_template_id in (' . $template_ids . ') )';
    }
    else {
      $search_where .= ' and cluster_id = ' . $cluster_id;
    }
  }

  $query =  'SELECT distinct *, a.param_id as param_id, b.template_id as server_template_id FROM {' . CLOUD_SCRIPTING_INPUT_PARAMETER_TABLE . '} a
             LEFT JOIN {' . CLOUD_INPUTS_PARAMETER_VALUES_TABLE . '} b on a.param_id=b.param_id and cluster_id=' . $cluster_id;

  if ($server_id != -1) {
    $query .= ' and b.server_id = ' . $server_id;
  }
  else {
    $query .= ' and b.server_id = -1';
  }

  $query .= $search_where . $cluster_only . ' group by a.input_parameter order by b.param_id';

  return $query;
}

function _cloud_inputs_update($form_values) {

  $query_args = array();
  $module     = $form_values['module'];

  //Adding server ID support
  $server_id = -1;
  if (isset($form_values['server_id'])) {
    $server_id = $form_values['server_id'];
  }

  if ($module == 'cloud_server_templates') {

    $query_args = array(
      ':server_template_id' => $form_values['template_id'],
    );

    $query       = _cloud_inputs_get($form_values['template_id']);
    $result      = db_query( $query, $query_args );

    $update_id    = 'template_id';
    $update_val   = 'template_value';
    $update_valID = $form_values['template_id'];
  }
  else {
    $query_args[] = 'template_id';
    $query_args[] = $form_values['template_ids']; //$form_values['cluster_id'];
    $query_args[] = 'cluster_id';
    $query_args[] = $form_values['cluster_id'];
    $update_id    = 'cluster_id';
    $update_val   = 'cluster_value';
    $update_valID = $form_values['cluster_id'];

    $result      = _cloud_inputs_get_templates_inputs($query_args, $module, $server_id);
  }

  //refactored to run one query only.  
  //store objects in an array, and use that for the $total_rows > 0
  $objects = array();
  foreach ($result as $obj) {

    $objects[] = $obj;
  }

  if (count($objects) > 0) {
    foreach ($objects as $input_obj) {

      $param_id = $input_obj->param_id;

      $get_query = "select * from {" . CLOUD_INPUTS_PARAMETER_VALUES_TABLE . "} where " . $update_id . "='$update_valID' and param_id='$param_id' ";

      if ($module == 'cloud_cluster_template') {
        $get_query .= " and server_id = $input_obj->server_id ";
      }


      $result1 = db_query( $get_query)->fetchField();

      if ($result1 != FALSE) {

        $input_type = $form_values[$param_id . '_input_type_select'];
        $query = db_update(CLOUD_INPUTS_PARAMETER_VALUES_TABLE)
          ->fields(array(
          $update_val => $form_values[$param_id . '_' . $input_type],
          'value_of_instance_id' => $form_values[$param_id . '_instance_select'],
        ))
          ->condition($update_id, $update_valID, '=')
          ->condition('param_id', $param_id, '=');

        if ($module == 'cloud_cluster_template') {
          $query->condition('server_id', $input_obj->server_id, '=');
        }
        else {
          $query->condition('server_id', -1, '=');
        }
        $result = $query->execute();


      }
      else {

        $input_type = $form_values[$param_id . '_input_type_select'];
        $insert_query = db_insert(CLOUD_INPUTS_PARAMETER_VALUES_TABLE)
          ->fields(array(
          'param_id' => $param_id,
          'template_id' => (!empty($form_values['template_id']) ? $form_values['template_id'] : $input_obj->server_template_id),
          'template_value' => (( $form_values['module'] == 'cloud_server_templates') ? $form_values[$param_id . '_' . $input_type] : ''),
          'cluster_id' => empty($form_values['cluster_id']) ? -1 : $form_values['cluster_id'],
          'cluster_value' => (( $form_values['module'] == 'cloud_cluster' || $form_values['module'] == 'cloud_cluster_template') ? $form_values[$param_id . '_' . $input_type] : ''),
          'value_of_instance_id' => (( !empty($form_values[$param_id . '_instance_id']) ? $form_values[$param_id . '_instance_id'] : '' )),
          'server_id' => $server_id,
        ))
          ->execute();


      }

      //BALDWIN = Test comment out code
//      $update_query = 'UPDATE {' . CLOUD_SCRIPTING_INPUT_PARAMETER_TABLE . '} 
//                set param_type=\'%s\' 
//                where param_id=\'%s\'
//          '; 
//      $query_args = array(
//        $input_type ,
//        $param_id   ,
//      );
//      db_query( $update_query, $query_args );

      // User Activity Log
      cloud_audit_user_activity( array(
        'type' => 'user_activity',
        'message' => t('Input has been saved: @param_id', array('@param_id' => $param_id)),
        'link' => '', //'design/inputs&template_id=' . $form_values['template_id'] . '&cluster_id=' . $form_values['cluster_id'] . '&module=' . $form_values['module']
      )
      );
    }
  } //existing number of rows
  else {
    if ($form_values['module'] == 'cloud_server_templates') {

      $query_args = array(
        ':server_template_id' => $form_values['template_id'],
      );

      $query = _cloud_inputs_get($form_values['template_id']);
    }
    else {
      $query = _cloud_inputs_get_cluster($form_values['cluster_id'], $server_id);
    }


    $result = db_query( $query, $query_args );
    foreach ($result as $input_obj) {

      $param_id = $input_obj->param_id;
      $input_parameter = $input_obj->input_parameter;

      $input_type = $form_values[$param_id . '_input_type_select'];
      $insert_query = db_insert(CLOUD_INPUTS_PARAMETER_VALUES_TABLE)
        ->fields(
           array(
        'param_id'       ,
        'template_id'    ,
        'template_value',
        'cluster_id'     ,
        'cluster_value'  ,
        'value_of_instance_id',
        'server_id'      ,
      )
       );

      $insert_query
        ->values(
          array(
        'param_id' => $param_id,
        'template_id' => ($form_values['template_id']) ? $form_values['template_id'] : $input_obj->server_template_id,
        'template_value' => ($form_values['module'] == 'cloud_server_templates') ? $form_values[$param_id . '_' . $input_type] : '',
        'cluster_id' => $form_values['cluster_id'],
        'cluster_value' => ($form_values['module'] == 'cloud_cluster' || $form_values['module'] == 'cloud_cluster_template')
                                ? $form_values[$param_id . '_' . $input_type]
                                : '',
        'value_of_instance_id' => (!empty($form_values[$param_id . '_instance_id']) ? $form_values[$param_id . '_instance_id'] : ''),
        'server_id' => $server_id,
      )
        );

      $insert_query->execute();

      //BALDWIN - Test Code
//      $update_query = 'UPDATE {' . CLOUD_SCRIPTING_INPUT_PARAMETER_TABLE . '} 
//                set param_type=\'%s\' 
//                where param_id=\'%s\'
//          '; 
//      $query_args = array(
//        $input_type ,
//        $param_id   ,
//      );
//      db_query( $update_query, $query_args );
//      
      cloud_audit_user_activity( array(
        'type' => 'user_activity',
        'message' => t('Input has been modified: id: @param_id', array('@param_id' => $param_id)),
        'link' => '', // 'design/inputs&template_id=' . $form_values['template_id'] . '&cluster_id=' . $form_values['cluster_id'] . '&module=' . $form_values['module']
      )
      );
    }
  }

  return;
}

function _cloud_inputs_get_templates_inputs($query_args, $module = '', $server_id = -1) {

  $search_where = '';
  if ($query_args[2] && $query_args[3]) {
    $search_where = ' and ' . $query_args[2] . ' in (' . $query_args[3] . ')';
  }

  if ($module == 'cloud_cluster_template' && $server_id != -1) {
    $search_where .= ' and server_id = ' . $server_id;
  }
  else {
    $search_where .= ' and server_id = -1';
  }
  $query = 'select *, template_id as server_template_id from {' . CLOUD_INPUTS_PARAMETER_VALUES_TABLE . '} where 1 ' . $search_where . ' order by param_id';

  $result = db_query( $query );

  return $result;
}

function _cloud_inputs_get_template_scripts_inputs_count($query_args) {

  $query = 'select * from {' . CLOUD_SERVER_TEMPLATES_SCRIPTS_TABLE . '} a left join {' . CLOUD_SCRIPTING_INPUT_PARAMETER_TABLE . '} b on a.script_id=b.script_id where a.server_template_id in (' . $query_args[1] . ')';

  $result = db_query( $query );
  return $result;

}

function _cloud_inputs_get_templates_by_cluster($cluster_id) {

  $query = 'select template_id as template_id from {' . CLOUD_CLUSTER_SERVER_TABLE . '} where cluster_id=' . $cluster_id;

  if (module_exists('cloud_auto_scaling')) {
    $query .= ' union select server_template_id as template_id from {' . CLOUD_AUTO_SCALING_INSTANCE_ARRAY_TABLE . '} where cluster_id=' . $cluster_id . '';
  }


  $result       = db_query( $query );
  $template_ids = array();
  foreach ($result as $dep_obj) {

    $template_ids[] =  $dep_obj->template_id;
  }
  $template_ids = implode(',', $template_ids);

  return $template_ids;
}

function _cloud_inputs_get_cluster_count($cluster_id) {

  $query = "select count(*) count from {" . CLOUD_INPUTS_PARAMETER_VALUES_TABLE . "} where cluster_id = $cluster_id";
  $result = db_query( $query )->fetchField();
  if ($result) {

    $num_rows = $result;
  }
  else {

    $num_rows = '';
  }
  return $num_rows;
}
