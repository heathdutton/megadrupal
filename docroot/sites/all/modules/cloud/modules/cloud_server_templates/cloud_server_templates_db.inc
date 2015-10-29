<?php

/**
 * @file
 * Provides server template functionality for each cloud sub-system.
 * Works with Cloud module.
 *
 * Copyright (c) 2010-2011 DOCOMO Innovations, Inc.
 *
 */

/**
 * Updated by yas   2013/10/31
 * Updated by yas   2011/07/20
 * Updated by jiten 2011/06/20
 */

// scripting functions 

/**
 * Returns a query to get list of server-templates
 * This function build a query and search parameters to sql assign during function call.
 *
 * @param $cloud_context
 *        This is the sub-cloud who's template is being added
 * @return return a sql-query
 */
function _cloud_server_templates_get_all_server_template_query($cloud_context = '') {

  $query = db_select( CLOUD_SERVER_TEMPLATES_TABLE, 'a');
  $query
  ->fields('a');

  return $query;
}

/**
 * Returns a query to get list of server-templates
 * This function build a query and search parameters to sql assign during function call.
 *
 * @param $cloud_context
 *        This is the sub-cloud who's template is being added
 * @return return a sql-query
 */
function _cloud_server_templates_get_server_template($cloud_context = '') {

  $query = db_select( CLOUD_SERVER_TEMPLATES_TABLE, 'a');
  $query->fields('a');

  return $query;
}

/**
 * Returns a query to get list of server-templates by giving template-id
 * This function build a query and search parameters to sql assign during function call.
 *
 * @param $template_id
 *        This is the template-id of template
 * @return return a cloud-type
 */
function _cloud_server_templates_get_cloud($template_id = '') {

  $query = db_select( CLOUD_SERVER_TEMPLATES_TABLE, 'a');
  $result = $query
  ->fields('a')
  ->condition('template_id', $template_id)
  ->execute();

  $template_obj    = $result->fetchObject();
  return $template_obj->cloud_type;
}

// scripting functions 

function _cloud_server_templates_get_server_templatelist_count($cloud_context = '', $params = array()) {

  global $user;
  $search_where = '';
  $group_by = '';
  $order_by = '';

  $query = db_select( CLOUD_SERVER_TEMPLATES_TABLE, 'a');
  $query->addExpression('COUNT(*)', 'template_count');

  if (in_array('actor', $user->roles)) {
    $query->condition('a.uid', $user->uid);
  }
  if ($cloud_context && empty($params['all']) ) {
    $query->condition('a.cloud_type', $cloud_context);
  }
  $result = $query->execute();
  foreach ($result as $tmpl) {

    $tmpl_count = $tmpl->template_count;
  }

  return $tmpl_count;
}


/**
 * Returns a query to get list of server-templates
 * This function build a query and search parameters to sql assign during function call.
 *
 * @param $cloud_context
 *        This is the sub-cloud who's template is being added
 * @return return a sql-query
 */
function _cloud_server_templates_get_server_templatelist($cloud_context = '', $params = array()) {

  global $user;
  $search_where = '';
  $group_by = '';
  $order_by = '';

  $query = db_select(CLOUD_SERVER_TEMPLATES_TABLE, 'a');
  $query->fields('a');
  $query->addExpression('a.template_id', 'servertemplateid');
  $query->addExpression('a.status', 'template_status');
  $query->addExpression('a.created', 'template_created');
  $query->addExpression('a.updated', 'template_updated');


  if (in_array('actor', $user->roles)) {
    $query->condition('a.uid', $user->uid);
  }
  if ($cloud_context && empty($params['all']) ) {
    $query->condition('a.cloud_type', $cloud_context);
  }
  return $query;
}

/**
 * delete a server template from database
 * This function deletes entry from database and updates audit logs.
 *  *
 * @param $template_id
 *        This is the template-id which is to be deleted
 * @param $cloud_context
 *        This is the sub-cloud who's template is being deleted
 * @return return
 */
function _cloud_server_templates_delete($id, $cloud_context = '') {

  $query      = _cloud_server_templates_get_server_template_detail();

  $query->condition('a.template_id', $id);
  $result = $query->execute();

  $count  = 0;
  $template_nickname = '';
  if ($result) {
    $template_obj    = $result->fetchObject();
    $template_nickname = $template_obj->template_nickname;
  }

  //Added - Delete the template based scripting values when
  //a template is deleted
  //THESE SHOULD REALLY BE form_altered into the templates form
  //and be deleted in its respective module
  //instead of using module exist!!! 
  if (module_exists('cloud_scripting')) {

    $query = db_delete( CLOUD_SERVER_TEMPLATES_SCRIPTS_TABLE )
      ->condition('server_template_id', $id)
      ->execute();
  }

  if (module_exists('cloud_inputs')) {
    $query = db_delete( CLOUD_INPUTS_PARAMETER_VALUES_TABLE )
      ->condition('template_id', $id)
      ->execute();

  }

  // User Activity Log
  cloud_audit_user_activity( array(
    'type' => 'user_activity',
    'message' => t('Template has been deleted: @template_nickname', array('@template_nickname' => $template_nickname)),
    'link' => '',
  )
  );

  $query = db_delete( CLOUD_SERVER_TEMPLATES_TABLE )
      ->condition('template_id', $id)
      ->execute();

  return;
}

/**
 * To get first element of image
 * This function uses a split logic to return first element from image_name
 *
 * @param $image_name
 *        This is the image name
 * @return return a first element from name
 */
function _cloud_server_templates_get_first_element_image($image_name) {

  if (isset($image_name) ) {
    $image_name = trim($image_name);
  }

  if ( isset($image_name) === FALSE    || empty($image_name) || $image_name === CLOUD_SERVER_TEMPLATES_NONE ) {
    return CLOUD_SERVER_TEMPLATES_NONE;
  }

  $start_brac = strrpos($image_name, '(');
  $end_brac = strrpos($image_name, ')');
  $len = $end_brac - $start_brac;

  $image_id = substr( $image_name, $start_brac + 1, $len - 1  );

  return $image_id;
}

/**
 * Insert a server template in database
 * This function insert entry in database and audit logs as well.
 *
 * @param $form_values
 *        This is the form-values submitted by new server template form
 * @return return a last inserted template-id
 */
function _cloud_server_templates_insert_server_template($form_values) {

  global $user;

  $insert_query = db_insert( CLOUD_SERVER_TEMPLATES_TABLE )
  ->fields(array(
    'template_nickname' => $form_values['name_text'                 ],
    'description' => $form_values['description_text'                 ],
    'cloud_type' => $form_values['cloud_type_select'                 ],
    'instance_type' => (isset( $form_values['instance_type_select'] ) ? $form_values['instance_type_select'] : ''),
    'image_id' => ((!empty($form_values['image_id_select']) ) ?  $form_values['image_id_select'] : ( _cloud_server_templates_get_first_element_image($form_values['ec2_image_id_select']))),
    'count_instances' => (isset( $form_values['Count_text'] ) ? $form_values['Count_text'] : ''),
    'user_name' => $form_values['user_name_text'                 ],
    'key_name' => $form_values['ssh_keys_select'                 ],
    'kernel_id' => _cloud_server_templates_get_first_element_image(isset($form_values['kernel_image_id_select']) ? $form_values['kernel_image_id_select'] : '' ),
    'ramdisk_id' => _cloud_server_templates_get_first_element_image(isset( $form_values['ramdisk_image_id_select'] ) ? $form_values['ramdisk_image_id_select'] : '' ),
    'group_id' => (empty($form_values['SG_select']) ? '' : implode(',', $form_values['SG_select'] )),
    'user_data' => (isset( $form_values['user_data_text'] ) ? $form_values['user_data_text'] : ''),
    'placement' => $form_values['zone_select'   ],
    'created' => date('c'),
    'updated' => date('c'),
    'uid' => $user->uid,
  ))
  ->execute();

  $server_template_id = _cloud_server_templates_last_inserted_id();
  if (module_exists('cloud_scripting')) {

    $scripting_options = cloud_server_templates_get_scripting_options();

    foreach ($scripting_options['CLOUD_SCRIPTING_TYPE_OPTIONS'] as $script => $script_val) {
      $i = 0;
      if ( isset($form_values[$script . '_selected']) &&  
              is_array($form_values[$script . '_selected'])
        && count(    $form_values[$script . '_selected']) > 0 ) {
        foreach (  $form_values[$script . '_selected'] as $script_id ) {

          //insert templates scripts

          $_insert_scripts_query = db_insert( CLOUD_SERVER_TEMPLATES_SCRIPTS_TABLE )
                ->fields(array(
            'id' => uuid(),
            'server_template_id' => $server_template_id,
            'script_id' => $script_id,
            '"order"' => $i,
            'created' => date('c'),
            'updated' => date('c'),
          ))
                ->execute();
          $i++;


        }
      }
    }
  }

  if (module_exists('cloud_alerts')) {
    if (isset($form_values['alert_id_selected']) && is_array($form_values['alert_id_selected']) && count($form_values['alert_id_selected']) > 0) {
      foreach ($form_values['alert_id_selected'] as $alert_id) {
        //insert templates alerts


        $_alerts_inserts_query = db_insert( CLOUD_SERVER_TEMPLATES_ALERTS_TABLE )
                ->fields(array(
          'id' => uuid(),
          'server_template_id' => $server_template_id,
          'alert_id' => $alert_id,
          'created' => date('c'),
          'updated' => date('c'),
        ))
                ->execute();
      }
    }
  }


  if (module_exists('cloud_failover')) {
    if (isset($form_values['failover_scenario_id_selected']) && is_array($form_values['failover_scenario_id_selected']) && count($form_values['failover_scenario_id_selected']) > 0) {
      foreach ($form_values['failover_scenario_id_selected'] as $failover_scenario_id) {
        //insert templates alerts

        $_failover_scenario_inserts_query = db_insert( CLOUD_SERVER_TEMPLATES_FAILOVER_SCENARIOS_TABLE )
                ->fields(array(
          'id' => uuid(),
          'server_template_id' => $server_template_id,
          'fsid' => $failover_scenario_id,
          'created' => date('c'),
          'updated' => date('c'),
        ))
                ->execute();
      }
    }
  }

  // User Activity Log
  cloud_audit_user_activity( array(
    'type' => 'user_activity',
    'message' => t('New Template has been added: @server_template_nickname', array('@server_template_nickname' => $form_values['name_text'])),
    'link' => '', // 'design/server_templates/create&id=' . $server_template_id
  )
  );
  //Fixed to actually return the template_id
  return $server_template_id;
}

/**
 * update a server template to database
 * This function update entry in database and audit logs as well.
 *
 * @param $form_values
 *        This is the form-values submitted by edit server template form
 * @param $cloud_context
 *        This is the sub-cloud who's template is being added
 * @return return a updated template-id
 */
function _cloud_server_templates_update_server_template($form_values, $server_template_id = '', $cloud_context = '') {


  $update_query = db_update( CLOUD_SERVER_TEMPLATES_TABLE )
  ->fields(array(
    'template_nickname' => $form_values['name_text'                 ],
    'description' => $form_values['description_text'                 ],
    'cloud_type' => ($form_values['cloud_type_select'] ? $form_values['cloud_type_select'] : $form_values['cloud_type_val']),
    'instance_type' => (isset( $form_values['instance_type_select'] ) ? $form_values['instance_type_select'] : ''),
    'image_id' => ((!empty($form_values['image_id_select']) ) ?  $form_values['image_id_select'] : ( _cloud_server_templates_get_first_element_image($form_values['ec2_image_id_select']))),
    'count_instances' => (isset( $form_values['Count_text'] ) ? $form_values['Count_text'] : ''),
    'user_name' => $form_values['user_name_text'                 ],
    'key_name' => $form_values['ssh_keys_select'                 ],
    'kernel_id' => _cloud_server_templates_get_first_element_image(isset($form_values['kernel_image_id_select']) ? $form_values['kernel_image_id_select'] : '' ),
    'ramdisk_id' => _cloud_server_templates_get_first_element_image(isset( $form_values['ramdisk_image_id_select'] ) ? $form_values['ramdisk_image_id_select'] : '' ),
    'group_id' => (empty($form_values['SG_select']) ? '' : implode(',', $form_values['SG_select'] )),
    'user_data' => (isset( $form_values['user_data_text'] ) ? $form_values['user_data_text'] : ''),
    'placement' => $form_values['zone_select'   ],
    'updated' => date('c'),
  ))
  ->condition('template_id', $server_template_id)
  ->execute();

  if (module_exists('cloud_scripting')) {

    //delete old templates scripts

    $delete_scripts_query = db_delete( CLOUD_SERVER_TEMPLATES_SCRIPTS_TABLE )
      ->condition('server_template_id', $server_template_id)
      ->execute();
  }

  if (module_exists('cloud_alerts')) {
    //delete old templates alerts

    $delete_alerts_query = db_delete( CLOUD_SERVER_TEMPLATES_ALERTS_TABLE )
      ->condition('server_template_id', $server_template_id)
      ->execute();
  }

  if (module_exists('cloud_failover')) {

    $delete_failover_scenarios_query = db_delete( CLOUD_SERVER_TEMPLATES_FAILOVER_SCENARIOS_TABLE )
      ->condition('server_template_id', $server_template_id)
      ->execute();
  }

  if (module_exists('cloud_scripting')) {
    $scripting_options = cloud_server_templates_get_scripting_options();
    //insert new templates scripts
    foreach ($scripting_options['CLOUD_SCRIPTING_TYPE_OPTIONS'] as $script => $script_val) {
      $i = 0;
      if (is_array($form_values[$script . '_selected']) && count($form_values[$script . '_selected']) > 0) {
        foreach ($form_values[$script . '_selected'] as $script_id) {

          //insert templates scripts

          $_insert_scripts_query = db_insert( CLOUD_SERVER_TEMPLATES_SCRIPTS_TABLE )
                ->fields(array(
            'id' => uuid(),
            'server_template_id' => $server_template_id,
            'script_id' => $script_id,
            '"order"' => $i,
            'created' => date('c'),
            'updated' => date('c'),
          ))
                ->execute();
          $i++;
        }
      }
    }
    if ( module_exists('cloud_inputs')) {

      $dq = db_query("select param_id from {" . CLOUD_SCRIPTING_INPUT_PARAMETER_TABLE . "} where param_id not in (select param_id from {" . CLOUD_SCRIPTING_INPUT_PARAMETER_TABLE . "})");
      while ($result = $dq->fetchField()) {
        //delete

        $delete_query = db_delete( CLOUD_INPUTS_PARAMETER_VALUES_TABLE )
                ->condition('param_id', array($result))
                ->execute();
      }
    }
  }

  if (module_exists('cloud_alerts')) {
    if (is_array($form_values['alert_id_selected'])
      &&       count($form_values['alert_id_selected']) > 0) {
      foreach ($form_values['alert_id_selected'] as $alert_id) {
        //insert templates alerts

        $_insert_scripts_query = db_insert( CLOUD_SERVER_TEMPLATES_ALERTS_TABLE )
                ->fields(array(
          'id' => uuid(),
          'server_template_id' => $server_template_id,
          'alert_id' => $alert_id,
          'created' => date('c'),
          'updated' => date('c'),
        ))
                ->execute();
      }
    }
  }

  if (module_exists('cloud_failover')) {
    //print count($form_values['failover_scenario_id_selected']);
    //print count($form_values['alert_id_selected']);
    //die;
    if (is_array($form_values['failover_scenario_id_selected']) && count($form_values['failover_scenario_id_selected']) > 0) {
      foreach ($form_values['failover_scenario_id_selected'] as $failover_scenario_id) {

        $_failover_scenario_inserts_query = db_insert( CLOUD_SERVER_TEMPLATES_FAILOVER_SCENARIOS_TABLE )
                ->fields(array(
          'id' => uuid(),
          'server_template_id' => $server_template_id,
          'fsid' => $failover_scenario_id,
          'created' => date('c'),
          'updated' => date('c'),
        ))
                ->execute();
      }
    }
  }

  // User Activity Log
  cloud_audit_user_activity( array(
    'type' => 'user_activity',
    'message' => 'Template "' . $form_values['name_text'] . '" has been modified.',
    'link' => '', //'design/server_templates/create&id=' . $server_template_id
  )
  );

  return;
}

function _cloud_server_templates_last_inserted_id($cloud_context = '') {

  $query_args[] = '';
  $query_args[] = '';

  $query = db_select(CLOUD_SERVER_TEMPLATES_TABLE, 'a');
  $query->addExpression('max(template_id)', 'maxtemplate_id');
  $result = $query->execute();

  $db_obj    = $result->fetchObject();
  return $db_obj->maxtemplate_id;

}



function _cloud_server_templates_get_server_template_details($cloud_context = '') {

  $query = db_select(CLOUD_SERVER_TEMPLATES_TABLE, 'a');
  $query->fields('a');

  return $query;
}

function _cloud_server_templates_get_server_template_detail($edit_or_view = FALSE, $cloud_context = '') {
  $search_where = '';
  $query = db_select(CLOUD_SERVER_TEMPLATES_TABLE, 'a');
  $query
  ->fields('a');

  return $query;
}

/**
 * Copy a server template in database
 * This function copy entry in database and update audit logs as well.
 *
 * @param $template_id
 *        This is the template-id which is to be copied
 * @param $cloud_context
 *        This is the sub-cloud who's template is being added
 * @return return a last inserted template-id
 */
function _cloud_server_templates_copy($template_id) {
  $query = _cloud_server_templates_get_server_template_details();
  $query->condition('template_id', $template_id);
  $result = $query->execute()->fetchObject();

  if ($result) {
    $template_nickname = t('Copy of @name_@time', array(
        '@name' => $result->template_nickname,
        '@time' => REQUEST_TIME));
      
    if (strlen($template_nickname) >= 255) {
      drupal_set_message(t('@template nickname is too long.  Truncated it to 255 characters.  Please update the nickname', array('@template' => $template_nickname)));
      $template_nickname = substr($template_nickname, 0, 255);
    }
      
    $insert_query = db_insert( CLOUD_SERVER_TEMPLATES_TABLE)
      ->fields(array(
        'template_nickname' => $template_nickname,
        'description' => $result->description,
        'cloud_type' => $result->cloud_type,
        'instance_type' => $result->instance_type,
        'image_id' => $result->image_id,
        'count_instances' => $result->count_instances,
        'key_name' => $result->key_name,
        'kernel_id' => $result->kernel_id,
        'ramdisk_id' => $result->ramdisk_id,
        'group_id' => $result->group_id,
        'user_data' => $result->user_data,
        'placement' => $result->placement,
        'created' => date('c'),
        'uid' => $result->uid,
        'status' => 1,
      ))
      ->execute();
  
    cloud_audit_user_activity(
      array(
        'type' => 'user_activity',
        'message' => t('Template "@template_nickname" has been Copied.', array('@template_nickname' => $template_nickname)),
        'link' => '',
      )
    );
  
    //add default initialization - otherwise will break the alerts section
    $selected_alertsIds = array();
    if (module_exists('cloud_scripting')) {
      // copy scripts and alerts as well  
      $query = _cloud_server_templates_get_server_for_copy($edit = TRUE);
      $query_args = array(
        ':template_id' => $template_id,
      );
  
      $result = db_query( $query, $query_args );
  
      foreach ($result as $template_obj) {
        $selected_scripts_ids[$template_obj->script_id] = $template_obj->order;
        $selected_alertsIds[$template_obj->alert_id] = $template_obj->alert_id;
      }
  
  
      $newtemplate_id = _cloud_server_templates_last_inserted_id();
      //copy scripts
      $query_args = array();
      $_insert_scripts_params = '';
      foreach ($selected_scripts_ids as $script_id => $order) {
  
        $_insert_scripts_params = db_insert( CLOUD_SERVER_TEMPLATES_SCRIPTS_TABLE )
                  ->fields(array(
          'id' => uuid(),
          'server_template_id' => $newtemplate_id,
          'script_id' => $script_id,
          '"order"' => $order,
          'created' => date('c'),
          'updated' => date('c'),
        ))
                  ->execute();
      }
  
    }
  
    if (module_exists('cloud_alerts')) {
      //copy alerts
      $query_args = array();
      $_alerts_inserts_params = '';
      if (count($selected_alertsIds) > 0) {
        foreach ($selected_alertsIds as $alert_id => $alert_id) {
  
          $_insert_scripts_params = db_insert( CLOUD_SERVER_TEMPLATES_ALERTS_TABLE )
                  ->fields(array(
            'id' => uuid(),
            'server_template_id' => $newtemplate_id,
            'alert_id' => $alert_id,
            'created' => date('c'),
            'updated' => date('c'),
          ))
                  ->execute();
        }
      }
    }
  }
  return TRUE;
}

/**
 * Returns a query to get list of server-templates
 * This function build a query and search parameters to sql assign during function call.
 *
 * @param $cloud_context
 *        This is the sub-cloud who's template is being added
 * @return return a sql-query
 */
function _cloud_server_templates_get_describe_all_templates_query($cloud_context = '') {

  $query = db_select(CLOUD_SERVER_TEMPLATES_TABLE, 'a');
  $query
  ->fields('a');
  return $query;
}

/**
 * Returns a query to get list of server-templates by giving template-id
 * This function build a query and search parameters to sql assign during function call.
 *
 * @param $template_id
 *        This is the template-id of template
 * @return return a sql query
 */
function _cloud_server_templates_get_info($template_id) {

  $query = db_select(CLOUD_SERVER_TEMPLATES_TABLE, 'a');
  $result = $query
    ->fields('a')
    ->condition('template_id', $template_id)
    ->execute();
  $template_obj    = $result->fetchObject();

  return $template_obj;
}

/**
 * Returns a query to get list of server-templates
 * This function build a query and search parameters to sql assign during function call.
 *
 * @param $cloud_context
 *        This is the sub-cloud who's template is being added
 * @return return a sql-query
 */
function cloud_server_templates_get_server_templates_by_cloud_db($cloud_context = '') {

  $query = db_select(CLOUD_SERVER_TEMPLATES_TABLE, 'a');
  $query = $query
      ->extend('TableSort')
      ->orderBy('template_nickname', 'ASC');

  $query
      ->condition('cloud_type', $cloud_context)
      ->fields('a');

  $result = $query->execute();

  $template_options = array();
  foreach ($result as $tmpl) {

    $template_options[$tmpl->template_id] = $tmpl->template_nickname;
  }
  asort( $template_options);
  return $template_options;
}

/**
 * Returns a query to get list of server-templates
 * This function build a query and search parameters to sql assign during function call.
 *
 * @param $cloud_context
 *        This is the sub-cloud who's template is being added
 * @return return a sql-query
 */
function _cloud_server_templates_get_server_template_details_by_id($cloud_context = '') {

  $query = db_select(CLOUD_SERVER_TEMPLATES_TABLE, 'a');
  $query
  ->fields('a');
  return $query;
}

/**
 * Returns a query to get list of server-templates by giving template-id
 * This function build a query and search parameters to sql assign during function call.
 *
 * @param $template_id
 *        This is the template-id of template
 * @param $cloud_context
 *        This is the sub-cloud who's template is being added
 * @return return a sql query
 */
function _cloud_server_templates_get_server_template_details_by_cloud($template_id = '', $cloud_context = '') {
  $query = db_select(CLOUD_SERVER_TEMPLATES_TABLE, 'a');
  $query
  ->fields('a')
  ->condition('template_id', $template_id);
  return $query;

}

/**
 * Copy a server template in database
 * This function copy entry in database and update audit logs as well.
 *
 * @param $cloud_context
 *        This is the sub-cloud who's template is being added
 * @return return a last inserted template-id
 */
function _cloud_server_templates_get_server_for_copy($edit_or_view = FALSE, $cloud_context = '') {

  global $user;

  if (in_array('actor', $user->roles)) {
    //$search_where = ' and a.uid=' . $user->uid;
  }

  $search_where = '';
  $group_by = '';
  $order_by = '';

  if ($cloud_context) {
    $search_where = " and a.cloud_type='$cloud_context' ";
  }

  if (!$edit_or_view) {
    $group_by = 'group by a.template_id';
    $search_where .= ' and a.template_id like :template_id ';
  }
  else {
    $order_by = 'order by b.order';
    $search_where .= ' and a.template_id = :template_id ';
  }

  if ( module_exists('cloud_scripting') && module_exists('cloud_alerts')) {
    return $query =  'SELECT distinct a.template_id as serverTemplateId, d.name as script_name, d.script_template as script_template, e.name as alert_name, a.status as template_status,  a.created as template_created,  a.updated as template_updated,    a.*, b.*, c.*, d.type
                    FROM {' . CLOUD_SERVER_TEMPLATES_TABLE . '} a
               LEFT JOIN {' . CLOUD_SERVER_TEMPLATES_SCRIPTS_TABLE . '} b on a.template_id=b.server_template_id
               LEFT JOIN {' . CLOUD_SCRIPTING_TABLE . '} d on b.script_id=d.script_id
               LEFT JOIN {' . CLOUD_SERVER_TEMPLATES_ALERTS_TABLE . '} c on a.template_id=c.server_template_id
               LEFT JOIN {' . CLOUD_ALERTS_TABLE . '} e on c.alert_id=e.alert_id
           WHERE 1 ' . $search_where . '  ' . $group_by . $order_by;
  }

  if ( module_exists('cloud_scripting')) {
    return $query =  'SELECT distinct a.template_id as serverTemplateId, d.name as script_name, d.script_template as script_template, a.status as template_status,  a.created as template_created,  a.updated as template_updated, a.*, b.*, d.type
                    FROM {' . CLOUD_SERVER_TEMPLATES_TABLE . '} a
               LEFT JOIN {' . CLOUD_SERVER_TEMPLATES_SCRIPTS_TABLE . '} b on a.template_id=b.server_template_id 
               LEFT JOIN {' . CLOUD_SCRIPTING_TABLE . '} d on b.script_id=d.script_id
           WHERE 1 ' . $search_where . '  ' . $group_by . $order_by;
  }
}


/**
 * Returns a query to get list of server-template's scripts
 * Generate a Sql - query for template script list
 * @param
 * @return return a sql-query
 */
function _cloud_server_templates_get_server_templates_scripts($server_template_id) {

  global $user;

  $group_by = '';
  $order_by = '';

  if (in_array('actor', $user->roles)) {
    //$search_where = ' and a.uid='.$user->uid;
  }

  $edit_or_view = isset($edit_or_view) ? $edit_or_view :  FALSE;
  $search_where = '';

  if (!$edit_or_view) {
    $search_where .= " and a.template_id like '%$server_template_id%' ";
  }
  else {
    $search_where .= " and a.template_id = '$server_template_id' ";
  }

  return $query =  'SELECT distinct a.template_id as serverTemplateId,d.name as script_name,d.script_template as script_template,a.status as template_status, a.created as template_created, a.updated as template_updated,   a.*,b.*,d.type FROM {' . CLOUD_SERVER_TEMPLATES_TABLE . '} a
               LEFT JOIN {' . CLOUD_SERVER_TEMPLATES_SCRIPTS_TABLE . '} b on a.template_id=b.server_template_id 
               LEFT JOIN {' . CLOUD_SCRIPTING_TABLE . '} d on b.script_id=d.script_id
               WHERE 1 ' . $search_where . ' ' . $group_by . $order_by;
}

/**
 * Returns a query to get list of server-template's alerts
 * Generate a Sql - query for template alerts list
 * @param
 * @return return a sql-query
 */
function _cloud_server_templates_get_server_templates_alerts() {

  //Function not used. to remove?
  global $user;

  $group_by = '';
  $order_by = '';

  if (in_array('actor', $user->roles)) {
    //$search_where = ' and a.uid='.$user->uid;
  }

  $edit_or_view = isset($edit_or_view) ? $edit_or_view : FALSE;
  $group_by = '';
  $search_where = '';

  if (!$edit_or_view) {
    $group_by = 'group by a.template_id';
    $search_where .= ' and %s like \'%%%s%%\' ';
  }
  else {
    $order_by = 'order by b.order';
    $search_where .= ' and %s = \'%s\' ';
  }

  return $query =  'SELECT distinct a.template_id as serverTemplateId,e.name as alert_name,a.status as template_status, a.created as template_created, a.updated as template_updated,   a.*,c.* FROM {' . CLOUD_SERVER_TEMPLATES_TABLE . '} a
               LEFT JOIN {' . CLOUD_SERVER_TEMPLATES_ALERTS_TABLE . '} c on a.template_id=c.server_template_id 
               LEFT JOIN {' . CLOUD_ALERTS_TABLE . '} e on c.alert_id=e.alert_id
               WHERE 1 ' . $search_where . ' ' . $group_by . $order_by;
}


function _cloud_server_templates_get_alerts_by_server_templates() {

  global $user;

  $group_by = '';
  $order_by = '';

  if (in_array('actor', $user->roles)) {
    //$search_where = ' and a.uid='.$user->uid;
  }

  $edit_or_view = isset($edit_or_view) ? $edit_or_view : FALSE;
  $group_by = '';
  $search_where = '';

  if (!$edit_or_view) {
    $search_where .= ' and a.template_id like :template_id ';

  }
  else {
    $search_where .= ' and a.template_id = :template_id ';
  }


  return $query =  'SELECT a.template_id as serverTemplateId,e.name as alert_name,a.status as template_status, a.created as template_created, a.updated as template_updated,   a.*,c.* FROM
               {' . CLOUD_SERVER_TEMPLATES_ALERTS_TABLE . '} c
               LEFT JOIN  {' . CLOUD_SERVER_TEMPLATES_TABLE . '} a  on a.template_id=c.server_template_id 
               LEFT JOIN {' . CLOUD_ALERTS_TABLE . '} e on c.alert_id=e.alert_id
               WHERE 1 ' . $search_where . ' ' . $order_by;
}

function _cloud_server_templates_get_active_template_count_by_cloud() {

  $cloud_list = cloud_get_all_clouds(); // Get all enabled clouds
  $cloud_count_arr = array();
  $query = db_select(CLOUD_SERVER_TEMPLATES_TABLE, 'a');
  $query
  ->fields('a', array('cloud_type'));
  $query->addExpression('COUNT(*)', 'active_count');

  $result = $query->execute();

  foreach ($result as $cnt_row) {

    if (isset($cloud_list[$cnt_row->cloud_type]) ) {

      $cloud_count_arr[$cnt_row->cloud_type] = $cnt_row->active_count;
    }
  }

  return $cloud_count_arr;
}


function _cloud_server_templates_get_failover_scanarios_by_server_templates() {

  global $user;

  $group_by = '';
  $order_by = '';

  if (in_array('actor', $user->roles)) {
    //$search_where = ' and a.uid='.$user->uid;
  }

  $edit_or_view = isset($edit_or_view) ? $edit_or_view : FALSE;
  $group_by = '';
  $search_where = '';

  if (!$edit_or_view) {
    $search_where .= ' and a.template_id like :template_id ';
  }
  else {
    $search_where .= ' and a.template_id = :template_id ';
  }


  $query =  'SELECT e.nickname nickname, a.*, c.* FROM
               {' . CLOUD_SERVER_TEMPLATES_FAILOVER_SCENARIOS_TABLE . '} c
               LEFT JOIN  {' . CLOUD_SERVER_TEMPLATES_TABLE . '} a  on a.template_id=c.server_template_id 
               LEFT JOIN {' . CLOUD_FAILOVER_SCENARIO_TABLE . '} e on c.fsid=e.fsid
               WHERE 1 ' . $search_where . ' ' . $order_by;
  //print $query; die;
  return $query;
}


/**
 *
 * @param type $cloud_context
 * @param type $instance_id
 * @param type $template_id
 * @param type $old_instance_id
 * @return type
 */

function _cloud_server_templates_insert_instance_info($cloud_context, $instance_id, $template_id) {

  $insert_query = db_insert( CLOUD_SERVER_TEMPLATES_INSTANCE_INFO_TABLE )
  ->fields(array(
    'cloud_context' => $cloud_context,
    'instance_id' => $instance_id,
    'template_id' => $template_id,
  ))
  ->execute();

  return;
}


/**
 *
 * @param type $cloud_context
 * @param type $instance_id
 * @return type
 */
function _cloud_server_templates_get_instance_info($cloud_context, $instance_id) {

  $query = db_select(CLOUD_SERVER_TEMPLATES_INSTANCE_INFO_TABLE, 'a');
  $result = $query
  ->fields('a')
  ->condition('instance_id', $instance_id)
  ->condition('cloud_context', $cloud_context)
  ->execute();

  if ($row = $result->fetchObject()) {

    return $row->template_id;
  }

  return;
}
