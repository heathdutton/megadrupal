<?php

/**
 * Database functions
 * @file
 */

/**
 * Returns all monitors and its detailed information
 */
function cloud_monitoring_get_all_monitors($sort_array = array()) {

  $monitors = array();
  $query = "SELECT monitor_id, monitor_name, instance_id, cloud_type, monitor_type from {cloud_monitor}";
  $result = db_query($query);

  foreach ($result as $obj) {
    $instance = cloud_get_instance($obj->cloud_type, $obj->instance_id);
    $monitors[] = array(
      'monitor_id' => $obj->monitor_id,
      'monitor_name' => $obj->monitor_name,
      'monitor_type' => $obj->monitor_type,
      'instance_id' => $instance['instance_id'],
      'hostname' => $instance['hostname'],
    );
  }
  //Added multiarray sorting support
  if ($sort_array) {
    $monitor_name = array();
    $monitor_host = array();
    $monitor_type = array();

    foreach ($monitors as $key => $row) {
      $monitor_name[$key] = $row['monitor_name'];
      $monitor_host[$key] = $row['hostname'];
      $monitor_type[$key] = $row['monitor_type'];
    }
    $sort = ($sort_array['sort'] == 'desc') ? SORT_DESC : SORT_ASC;
    $keys = array();
    switch ($sort_array['sql']) {
      case 'monitor_type':
        $keys = $monitor_type;
        break;
      case 'hostname':
        $keys = $monitor_host;
        break;
      case 'monitor_name':
      default:
        $keys = $monitor_name;
        break;

    }
    array_multisort($keys, $sort, $monitors);
  }
  return $monitors;
}

/**
 * Load monitor information given a monitor_id
 */
function cloud_monitoring_load_monitor($monitor_id) {
  $query = db_select('cloud_monitor', 'c')
    ->fields('c', array('monitor_id', 'monitor_name', 'monitor_type', 'instance_id', 'cloud_type'))
    ->condition('c.monitor_id', $monitor_id);
  return $query->execute()->fetchAssoc();
}


/**
 * Delete a monitor and its corresponding servers
 */
function cloud_monitoring_delete_monitor($monitor_id) {
  db_delete('cloud_monitor_instance')
    ->condition('monitor_id', $monitor_id)
    ->execute();
  
  db_delete('cloud_monitor')
    ->condition('monitor_id', $monitor_id)
    ->execute();
}

/**
 * Query to check if a monitor exists
 */
function cloud_monitoring_monitor_exists($instance_id) {
  $query = db_select('cloud_monitor', 'c')
    ->fields('c', array('instance_id'))
    ->condition('c.instance_id', $instance_id);
  return $query->execute()->fetchField();
}

/**
 * Get monitor id by instance_id
 */
function cloud_monitoring_monitor_by_instance_id($instance_id) {
  $query = db_select('cloud_monitor', 'c')
    ->fields('c', array('monitor_id'))
    ->condition('c.instance_id', $instance_id);
  return $query->execute()->fetchField();
}

/**
 * Look up an instance via private dns name
 */
function cloud_monitoring_instance_by_dns($private_dns_name) {
  $query = db_select('cloud_aws_instances', 'c')
    ->fields('c', array('instance_id', 'cloud_type'))
    ->condition('private_dns_name', $private_dns_name);
  return $query->execute()->fetchAssoc();
}

/**
 * Get a host from instance_id
 */
function cloud_monitoring_dns_by_id($instance_id) {
  $query = db_select('cloud_aws_instances', 'c')
    ->fields('c', array('private_dns_name'))
    ->condition('c.instance_id', $instance_id);
  return $query->execute()->fetchField();
}

/**
 * Check active instances
 */
function cloud_monitoring_is_instance_active($monitor_id, $instance_id) {
  $is_active = FALSE;
  $query = db_select('cloud_monitor_instance', 'c')
    ->fields('c', array('status'))
    ->condition('c.monitor_id', $monitor_id)
    ->condition('c.instance_id', $instance_id);
  $result = $query->execute()->fetchField();
  $status = $result;
  if ($status == 1) {
    $is_active = TRUE;
  }
  return $is_active;
}

/**
 * Look up a nickname from dns
 */
function cloud_monitoring_nickname_by_dns($dns) {
  $query = db_select('cloud_aws_instances', 'c')
    ->fields('c', array('instance_nickname'))
    ->condition('c.private_dns_name', $dns);
  return $query->execute()->fetchField();
}

/**
 * Look up a monitor instance based on DNS address
 */
function cloud_monitoring_monitor_instance_by_dns($dns) {
  $query = "SELECT cmi.instance_id from {cloud_monitor_instance} cmi INNER JOIN {cloud_aws_instances} cai on
            cmi.instance_id = cai.instance_id where cai.private_dns_name = ':dns' and cmi.status = 1";
  $result = db_query($query, array(':dns' => $dns))->fetchField();
  return $result;
}

/**
 * Lookup an instance_id using a nickname
 */
function cloud_monitoring_id_lookup($nickname) {
  $query = db_select('cloud_aws_instances', 'c')
    ->fields('c', array('instance_id'))
    ->condition('c.instance_nickname', $nickname);
  return $query->execute()->fetchField();
}

/**
 * Lookup a nickname using an instance_id
 */
function cloud_monitoring_nickname_lookup($instance_id) {
  $query = db_select('cloud_aws_instances', 'c')
    ->fields('c', array('instance_nickname'))
    ->condition('c.instance_id', $instance_id);
  return $query->execute()->fetchField();
}

/**
 * Update status of a monitoring instance
 */
function cloud_monitoring_update_instance_status($monitor_id, $instance_id, $status) {
  db_update('cloud_monitor_instance', 'c')
    ->fields(array('status' => $status))
    ->condition('c.instance_id', $instance_id)
    ->condition('c.monitor_id', $monitor_id)
    ->execute();
}

/**
 * Get all instances relating to a certain monitor_id
 */
function cloud_monitoring_instances($monitor_id) {
  $query = "SELECT monitor_id, instance_id, cloud_name, status from {cloud_monitor_instance} where monitor_id = :monitor_id";
  return cloud_get_db_results($query, array(':monitor_id' => $monitor_id));
}

/**
 * Find the monitor_id using the instance _id
 */
function cloud_monitoring_monitor_id_by_instance($instance_id) {
  $query = db_select('cloud_monitor_instance', 'c')
    ->fields('c', array('monitor_id'))
    ->condition('c.instance_id', $instance_id);
  return $query->execute()->fetchField();
}

/**
 * Delete instance from the lookup date
 */
function cloud_monitoring_delete_instance($monitor_id, $instance_id) {
  db_delete('cloud_monitor_instance')
    ->condition('monitor_id', $monitor_id)
    ->condition('instance_id', $instance_id)
    ->execute();
}

/**
 * Import a specific script's input parameters into the system
 */
function _cloud_monitoring_save_script_input($template_id, $template, $email) {
  foreach ($template['scripts'] as $key => $value) {
    $query = _cloud_inputs_get($value['id']);
    //$result = db_query($query, array('server_template_id', $template_id));
    $result = db_query($query, array(':server_template_id' => $template_id))->fetchAssoc();

    // $results = db_fetch_array($result);
    $results = $result;
    $param_id = $results['param_id'];
    $data = array();
    $data['module'] = 'cloud_server_templates';
    $data['template_id'] = $template_id;
    $data[$param_id . '_input_type_select'] = 'Text';
    $data[$param_id . '_' . 'Text'] = $email;
    $data['cluster_id'] = '';
    _cloud_inputs_update($data);
  }
}

/**
 * Import a script into the system
 */
function _cloud_monitoring_save_scripts(&$template) {
  $script_ids = array();
  $scripts = $template['scripts'];
  if (count($scripts)) {
    //loop the scripts and insert them into the database, return the script ids, and store it
    //in the array
    foreach ($scripts as $key => $value) {
      $values = array();
      $values['name_text'] = $value['name'];
      $values['description_text'] = $value['description'];
      $values['script_type_select'] = 'boot';
      $values['inputs_text'] = $value['inputs'];
      $values['packages_text'] = '';
      $values['cluster_id'] = '';
      $values['script_template_text'] = $value['script_template'];
      $id = _cloud_scripting_insert_script($values);
      if ($id) {
        $template['scripts'][$key]['id'] = $id; //store the ID for further use
      }
    }
  }
}

/**
 * Import a server template into the system
 */
function _cloud_monitoring_save_template(&$template) {
  //in order to use _cloud_server_templates_insert_server_template, it expect form values,
  //take them from server_template, and re-format it
  $values = array();
  $values['name_text'] = $template['template_nickname'];
  $values['description_text'] = $template['description'];
  $values['cloud_type_select'] = $template['cloud_type'];
  $values['instance_type_select'] = $template['instance_type'];
  $values['image_id_select'] = $template['image_id'];
  $values['user_name_text'] = $template['user_name'];
  $values['ssh_keys_select'] = $template['key_name'];
  $values['SG_select'] = array($template['group_id']);
  $values['Count_text'] = $template['count_instances'];
  $values['zone_select'] = $template['zone_select'];

  //map the boot scripts
  foreach ($template['scripts'] as $script) {
    if ($script['id']) {
      $values['boot_selected'][] = $script['id'];
    }
  }
  $template_id = _cloud_server_templates_insert_server_template($values);
  $template['template_id'] = $template_id;
  return $template_id;
}

/**
 * Write the monitor record
 */
function cloud_monitoring_save_monitor_record($instance_id, $monitor_name, $monitor_type, $cloud_type) {
  $data = array();
  $data['monitor_name'] = $monitor_name;
  $data['instance_id'] = $instance_id;
  $data['monitor_type'] = $monitor_type;
  $data['cloud_type'] = $cloud_type;
  $data['created'] = time();
  drupal_write_record('cloud_monitor', $data);
}

/**
 * Write a record to the cloud_monitor_instance table. This maps the instance to
 * a particular monitoring server.
 */
function cloud_monitoring_save_monitor_instance_record($monitor_id, $instance_id) {
  $data = array();
  //map the servers in the database table
  $data['monitor_id'] = $monitor_id;
  $data['instance_id'] = $instance_id;
  $data['cloud_name'] = '';
  $data['status'] = TRUE;
  drupal_write_record('cloud_monitor_instance', $data);
}

/**
 * Save a template to monitor lookup record
 */
function cloud_monitoring_save_lookup_record($template_id, $monitor_type) {
  $monitor_server_map = array(
    'template_id' => $template_id,
    'monitor_type' => $monitor_type,
  );
  drupal_write_record('cloud_monitor_server_template', $monitor_server_map);
}

/**
 * Delete a lookup record
 */
function cloud_monitoring_delete_lookup_record($template_id) {
  db_delete('cloud_monitor_server_template')
    ->condition('template_id', $template_id)
    ->execute();
}

/**
 * Get all server templates
 */
function cloud_monitoring_get_templates() {
  $query = 'select cloud_server_templates.template_id, cloud_server_templates.template_nickname, cloud_server_templates.cloud_type from {cloud_monitor_server_template} inner join {cloud_server_templates}
            on cloud_server_templates.template_id = cloud_monitor_server_template.template_id';
  return cloud_get_db_results($query);
}

/**
 * Get a lookup record
 */
function cloud_monitoring_get_lookup_record($template_id) {
  $query = "select template_id, monitor_type from {cloud_monitor_server_template} where template_id = :template_id";
  return cloud_get_db_results($query, array(':template_id' => $template_id));
}

