<?php

/**
 * @file
 * Amazon EC2 Library Common file
 *
 * Copyright (c) 2010-2012 DOCOMO Innovations, Inc.
 *
 */

/**
 * Retrieve instances information using REST call.
 *
 * @param $cloud_context
 *   The sub-cloud for which the information is to be retrieved
 * @return
 */
function aws_cloud_fetch_instances_data($cloud_context) {
  if (_cloud_is_update_allowed()) {
    $result = aws_cloud_fetch_data($cloud_context);
  }
  drupal_goto(drupal_get_destination());
}


/**
 * Fetch the volumes related data
 * @param $cloud_context
 *   The sub-cloud for which the information is to retrieved
 * @return
 */
function aws_cloud_fetch_volumes_data($cloud_context) {
  if (_cloud_is_update_allowed()) {
    aws_cloud_fetch_data($cloud_context);
  }
  drupal_goto( cloud_get_menu_path($cloud_context) . '/ebs_volumes' );
}


/**
 *
 * @param $cloud_context
 *              The sub-cloud for which the information is to be retrieved
 * @return
 */
function aws_cloud_fetch_elastic_ips_data($cloud_context) {
  if (_cloud_is_update_allowed()) {
    aws_cloud_fetch_data($cloud_context);
  }
  drupal_goto( cloud_get_menu_path($cloud_context) . '/elastic_ips' );
}


/**
 *
 * @param $cloud_context
 *              The sub-cloud for which the information is to be retrieved
 * @return
 */
function aws_cloud_fetch_security_group_data($cloud_context) {
  if (_cloud_is_update_allowed()) {
    aws_cloud_fetch_data($cloud_context);
  }
  drupal_goto( cloud_get_menu_path($cloud_context) . '/security_groups' );
}


/**
 *
 * @param $cloud_context
 *              The sub-cloud for which the information is to be retrieved
 * @return
 */
function aws_cloud_fetch_snapshots_data($cloud_context) {
  if (_cloud_is_update_allowed()) {
    aws_cloud_fetch_data($cloud_context);
  }
  drupal_goto( cloud_get_menu_path($cloud_context) . '/ebs_snapshots' );
}


/**
 *
 * @param $cloud_context
 *              The sub-cloud for which the information is to be retrieved
 * @return
 */
function aws_cloud_fetch_ssh_keys_data($cloud_context) {
  if (_cloud_is_update_allowed()) {
    aws_cloud_fetch_data($cloud_context);
  }
  drupal_goto( cloud_get_menu_path($cloud_context) . '/ssh_keys' );

}


/**
 *
 * Send request and fetch all the information
 * The purpose of this function is to download all the available information from the sub-cloud
 * HTTP Request is to the sub-cloud server and the information returned is stored in the database
 * It is necessary to download the latest information to keep clanavi updated about the latest information.
 * Information requested is mainly of the following entities:
 *
 *    viz.  Elastic IP Info,
 *          Instances Info,
 *          SSH Keys Info,
 *          Security Group Info,
 *          Zone Info,
 *          Snapshot Info,
 *          Volume Info
 *
 *
 * @param $cloud_context
 *   The sub-cloud for which the information is to be retrieved
 */
function aws_cloud_fetch_data($cloud_context) {
  drupal_set_time_limit(AWS_CLOUD_TIME_LIMIT);
  return _aws_cloud_download_all($cloud_context);
}


/**
 * Retrieve information related to images
 *
 * @param $cloud_context
 *   The sub-cloud for which the information is to be retrieved
 */
function aws_cloud_fetch_image_data($cloud_context) {
  drupal_set_time_limit(AWS_CLOUD_TIME_LIMIT);
  _aws_cloud_download_all_images($cloud_context);
  $destination = drupal_get_destination();

  drupal_goto( $destination );
}


/**
 * Get all zones from the database
 * @param $cloud_context
 *   The sub-cloud for which the information is to be retrieved
 * @return
 */
function aws_cloud_get_zones($cloud_context) {

  $zone_options = array();
  $zone_options['any'] = 'any';

  $query = _aws_cloud_get_describe_zones_query($cloud_context);
  $zones_result = db_query( $query, array(':cloud_type' => $cloud_context) );

  foreach ($zones_result as $type) {
    $zone_options[$type->zone_name] = $type->zone_name;
  }
  asort( $zone_options);
  return $zone_options;
}

/**
 * Checks to see if instances have security group or ssh key
 * @param unknown_type $cloud_context
 */
function aws_cloud_has_instances_data($cloud_context) {
  if ( _aws_cloud_get_security_groups_list_count($cloud_context) == 0  ||  _aws_cloud_get_ssh_keys_list_count($cloud_context) == 0) {
    return FALSE;
  }
  return TRUE;
}

/**
 * Checks if there are ssh keys
 */
function aws_cloud_has_ssh_key_data($cloud_context) {
  if ( _aws_cloud_get_ssh_keys_list_count( $cloud_context) == 0 ) {
    return FALSE;
  }
  return TRUE;
}

/**
 * Check to see if there are security group  
 */
function aws_cloud_has_security_group_data($cloud_context) {
  if ( _aws_cloud_get_security_groups_list_count($cloud_context) == 0 ) {
    return FALSE;
  }
  return TRUE;
}

/**
 *  Get the name of database table using the sub-cloud
 *  This is useful since the schema is shared between sub-cloud's but the
 *  actual tables are different
 *
 *  Removed $cloud_context.  Everything is coming from one set of
 *  tables now
 *
 * @param $table_name
 *  The type of table/information which is to be queried
 * @return
 *  Database table name
 */
function aws_cloud_get_table_info($table_name) {
  return AWS_CLOUD_CLOUD_PREFIX . AWS_CLOUD_DB_CONTEXT . '_' . $table_name;
}

/**
 * Aws cluster form 
 */
function aws_cloud_cluster_form($form, $form_state, $params) {
  $cloud_context = $params['cloud_context'];
  $cluster_id = $params['cluster_id'];
  $cluster_nickname = $params['cluster_nickname'];
  
  $form = array(); // New Form
  //pass in cloud_context
  $form['cloud_context'] = array(
    '#type' => 'value',
    '#value' => $cloud_context,
  );

  $form['fieldset_template_info'] = array(
    '#type' => 'fieldset'
  );

  $form['cluster_id']  = array(
    '#type' => 'hidden',
    '#value' => $cluster_id,
  );

  $form['attach_header'] = array(
    '#prefix' => '<h2>',
    '#value' => t('Cluster: @cluster_nickname', array('@cluster_nickname' => $cluster_nickname) ),
    '#suffix' => '</h2>',
  );

  $template_options = cloud_get_server_templates($cloud_context);
  $template_options['-1'] = ' - Select Template - '  ;

  $form['template_details'] = array(
    'template_label' => array(
      '#type' => 'item'    ,
      '#title' => t('Template'),
    ),
    'template_select' => array(
      '#type' => 'select'  ,
      '#options' => $template_options,
      '#default_value' => -1,
    ),
  );

  $form['nickname_details'] = array(
    'nickname_label' => array(
      '#type' => 'item'     ,
      '#title' => t('Nickname'),
    ),
    'nickname_text' => array(
      '#type' => 'textfield',
      '#default_value' => '',
    ),
  );

  $form['ssh_keys_details'] = array(
    'ssh_keys_label' => array(
      '#type' => 'item',
      '#title' => t('SSH Key'),
    ),
    'ssh_keys_select' => array(
      '#type' => 'select'    ,
      '#options' => _aws_cloud_get_ssh_keys($cloud_context),
    ),
  );

  $form['SG_details'] = array(
    'SG_label' => array(
      '#type' => 'item'      ,
      '#title' => t('Security Group(s)'),
    ),
    'SG_select' => array(
      '#type' => 'select',
      '#options' => _aws_cloud_get_security_groups($cloud_context),
      '#multiple' => TRUE,
    ),
  );

  $form['zone_details'] = array(
    'zone_label' => array(
      '#type' => 'item',
      '#title' => t('Availability Zone'),
    ),
    'zone_select' => array(
      '#type' => 'select',
      '#options' => aws_cloud_get_zones($cloud_context),
    ),
  );

  $form['submit_buttons'] = array(
    '#type' => 'fieldset',
    '#prefix' => '<span class="clear"></span><div class="container-inline"><div class="buttons">',
    '#suffix' => '</div></div>',
    'Add_more' => array(
      '#type' => 'submit',
      '#value' => t('Add More'),
    ),
    'Save' => array(
      '#type' => 'submit',
      '#value' => t('Save'),
    ),
    'Cancel' => array(
      '#type' => 'submit',
      '#value' => t('List Servers'),
    ),
  );
  return $form;
}

/**
 * Theme cluster form
 */
function theme_aws_cloud_cluster_form($form) {
  $form = $form['form'];

  $rows = array(
    array(
      drupal_render($form['template_details']['template_label']),
      drupal_render($form['template_details']['template_select']),
    ),
    array(
      drupal_render($form['nickname_details']['nickname_label']),
      drupal_render($form['nickname_details']['nickname_text']),
    ),
    array(
      drupal_render($form['ssh_keys_details']['ssh_keys_label']),
      drupal_render($form['ssh_keys_details']['ssh_keys_select']),
    ),
    array(
      drupal_render($form['SG_details']['SG_label']),
      drupal_render($form['SG_details']['SG_select']),
    ),
    array(
      drupal_render($form['zone_details']['zone_label']),
      drupal_render($form['zone_details']['zone_select']),
    ),
  );

  $table = theme('table', array('header' => NULL, 'rows' => $rows));
  $form['fieldset_template_info']['#children'] = $table;

  $output  = drupal_render($form['attach_header']);
  $output .= drupal_render($form['fieldset_template_info']);
  $output .= drupal_render($form['submit_buttons']);
  $output .= drupal_render($form['cluster_id']);
  $output .= drupal_render_children($form);

  return $output;
}

/**
 * Validate the cluster form 
 */
function aws_cloud_cluster_form_validate($form_id, $form_values) {
  $form_values = $form_values['values'];

  if ($form_values['op'] == t('Save') || $form_values['op'] == t('Add More') ) {
    if ($form_values['template_select'] == -1 ) {
      form_set_error('template_select', t('You must select Template.'));
    }
    if (empty($form_values['nickname_text']) ) {
      form_set_error('nickname_text', t('You must enter valid Nickname.'));
    }

    if (empty($form_values['ssh_keys_select']) || $form_values['ssh_keys_select'] == -1) {
      form_set_error('ssh_keys_select', t('You must enter valid SSH Key.'));
    }
    if (count($form_values['SG_select']) == 0) {
      form_set_error('SG_select', t('You must select Security Group.'));
    }
  }
}

/**
 * Cluster Submit Form 
 */
function aws_cloud_cluster_form_submit($form_id, $form_values) {
  $form_values = $form_values['values'];
  if ($form_values['op'] == t('Save') || $form_values['op'] == t('List Servers') || $form_values['op'] == t('Add More')) {
    cloud_callback_cluster_operation('cluster_form_submit', $form_id, $form_values);
  }
}

/**
 * Function to check that the temporary directory is ready for use.
 * Return the absolute path
 */
function _aws_cloud_prepare_directory() {
  $path = AWS_CLOUD_TEMP_DIR;
  if (file_prepare_directory($path, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS) == FALSE) {
    drupal_set_message(t('Unable to create directory @path', array('@path' => AWS_CLOUD_TEMP_DIR)));
    return FALSE;
  }
  return drupal_realpath($path);
}

/**
 *
 * clear the temporary logs form for a sub-cloud
 *
 * @return
 */
function aws_cloud_clear_temporary_logs() {
  $cloud_list = cloud_get_all_clouds();
  $all_instances_list = array();
  $running_instances = array();
  $filter_params = array('column' => '', 'filer_value' => '');

  foreach ($cloud_list as $cloud_context) {
    $instances_list     = cloud_get_instances($cloud_context, $filter_params  );
    $all_instances_list = array_merge( $all_instances_list  , $instances_list );
  }

  foreach ($all_instances_list as $instance_id => $instance_obj) {
    if ($instance_obj['state'] == 'running') {
      $running_instances[] = $instance_id;
    }
  }
  $output_dir = _aws_cloud_prepare_directory();
  if ($output_dir != FALSE) {
    aws_cloud_delete_dir($output_dir, $running_instances);
    drupal_set_message(t('Temporary Logs have been cleared.'));
    drupal_goto('admin');
  }
}

/**
 * Delete a directory
 */
function aws_cloud_delete_dir($dir, $running_instances = array(), $flag = '') {
  // open the directory
  $dhandle = opendir($dir);

  if ($dhandle) {
    // loop through it
    while (false !== ($fname = readdir($dhandle))) {
      // if the element is a directory, and
      // does not start with a '.' or '..'
      // we call deleteDir function recursively
      // passing this element as a parameter
      if (is_dir( "{$dir}/{$fname}")  ) {
        if ( !in_array($fname, $running_instances)) {
          if (($fname != '.') && ($fname != '..')) {
            aws_cloud_delete_dir("$dir/$fname", $running_instances, 'true');
          }
        }
        // the element is a file, so we delete it
      }
      else {
        unlink("{$dir}/{$fname}");
      }
    }
    closedir($dhandle);
  }
  // now directory is empty, so we can use
  // the rmdir() function to delete it
  if ($flag == 'true') {
    rmdir($dir);
  }
}
