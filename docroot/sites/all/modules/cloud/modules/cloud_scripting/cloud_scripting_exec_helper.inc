<?php

/**
 * @file
 * Provides scripting feature such as bash, Perl and etc (Mainly bash scripts).
 * Works with Cloud and Server Templates module.
 *
 * Copyright (c) 2010-2011 DOCOMO Innovations, Inc.
 *
 */

/**
 * Function executes scripts on an instance
 * @param unknown_type $cloud_context
 */
function _cloud_scripting_exec_script_on_instances($cloud_context) {
  // Check the instances for Executed scripts...
  _cloud_scripting_check_instance_script_status($cloud_context);
  // Get scripts that are ready to exectute
  $scripts = _cloud_scripting_get_execute_ready_scripts($cloud_context);
  
  foreach ($scripts as $script) {
    $instance = cloud_get_instance($cloud_context, $script['instance_id']);
    if ($instance) {
      _cloud_scripting_exec_scripts_on_instance($cloud_context, $instance, $script['instance_id'], $script);
    }
  }
}

/**
 * Function that checks if a script has finished executing
 * @param <type> $cloud_context
 */
function _cloud_scripting_check_instance_script_status($cloud_context) {
  $scripts = _cloud_scripting_get_under_progress_scripts($cloud_context);

  foreach ($scripts as $script) {
    $instance_id = $script['instance_id'];
    $script_id   = $script['script_id'];
    $instance = cloud_get_instance($cloud_context, $instance_id);
    if ($instance) {
      $key_name = $instance['key_name'];
      $perm_file = _cloud_scripting_get_perm_file($instance_id, $key_name, $cloud_context);
      $instance_ip = $instance['dns_name'];
  
      if ( _cloud_scripting_is_script_execution_done($cloud_context, $instance_ip, $instance_id, $perm_file, $script_id) == '1') {
        _cloud_scripting_update_scripts_status($instance_id, $script_id, CLOUD_SCRIPTING_SCRIPT_EXECUTED);
      }
    }
  }
}

/**
 * Execute a one off script on the fly.  This doesn't read or load
 * from the cloud_scripting tables or any of the scripts setup from
 * the template.  This is useful for system scripts or arbitrary scripts
 * without going through cloud templates
 */
function _cloud_scripting_exec_one_off_script($cloud_context, $instance_id, $script, $script_name, $background = FALSE) {
  $ssh_user_name = aws_cloud_get_ssh_user( $cloud_context, $instance_id);
  
  $instance = cloud_get_instance($cloud_context, $instance_id);
  $key_name = $instance['key_name'];
  $instance_ip = $instance['dns_name'];
  if (($path = _cloud_scripting_prepare_directory($instance_id)) != FALSE && 
      (($perm_file = _cloud_scripting_get_perm_file($instance_id, $key_name, $cloud_context)) != FALSE)) {
    
    $script_file = drupal_realpath($path) . '/' . $instance_id . '_' . $script_name;

    $fh = fopen($script_file, 'w');
    if ($fh == FALSE) {
      drupal_set_message( t('Unable to create the script file.'), 'error' );
      return;
    }
  
    fwrite($fh, $script);
    fclose($fh);
  
    $cmd = CLOUD_SSH_PATH . ' ' . $ssh_user_name . '@' . $instance_ip . ' -i "' . $perm_file . '" -o \'StrictHostKeyChecking no\' \'nohup bash -s\' < ' . $script_file;
    if ($background == TRUE) {
      $cmd .= ' > /tmp/log.out 2>&1 &';
    }
    $output = array();
    exec($cmd, $output, $retval);
  
    return $retval;
  }
}

/**
 * Helper function that executes a command on a remote instance 
 */
function _cloud_scripting_exec_remote_command($cloud_context, $instance_id, $command, $background = FALSE) {
  $ssh_user_name = aws_cloud_get_ssh_user( $cloud_context, $instance_id);
  $instance = cloud_get_instance($cloud_context, $instance_id);
  $key_name = $instance['key_name'];
  $instance_ip = $instance['dns_name'];
  if ((($perm_file = _cloud_scripting_get_perm_file($instance_id, $key_name, $cloud_context)) != FALSE)) {
    $cmd = CLOUD_SSH_PATH . ' ' . $ssh_user_name . '@' . $instance_ip . ' -i "' . $perm_file . '" -o \'StrictHostKeyChecking no\' ' . $command;
    if ($background == TRUE) {
      $cmd . ' > /tmp/log.out 2>&1 &';
    }
    exec($cmd, $output, $retval);

    return $output;
  }
  return FALSE; 
}

/**
 * Copy a local file onto the remote server
 */
function _cloud_scripting_exec_copy_file($cloud_context, $instance_id, $source, $target) {
  $success = FALSE;
  $ssh_user_name = aws_cloud_get_ssh_user( $cloud_context, $instance_id);
  $instance = cloud_get_instance($cloud_context, $instance_id);
  $key_name = $instance['key_name'];
  $instance_ip = $instance['dns_name'];
  if ((($perm_file = _cloud_scripting_get_perm_file($instance_id, $key_name, $cloud_context)) != FALSE)) {
    $cmd = CLOUD_SSH_PATH . ' ' . $ssh_user_name . '@' . $instance_ip . ' -i "' . $perm_file . '" -o \'StrictHostKeyChecking no\' ' . $command;
    
    $command = $SCP_PATH . " -i $perm_file -o 'StrictHostKeyChecking no' $source $ssh_user_name@$instance_ip:$target ";
    
    exec($cmd, $output, $retval);
    // check the return
    $success = ($retval != 0) ? FALSE : TRUE;
  }
  return $success; 
}

/**
 * Function executes a script on an instance
 */
function _cloud_scripting_exec_scripts_on_instance($cloud_context, $instance, $instance_id, $instance_script_params) {
  $script_id = $instance_script_params['script_id'];
  if (!empty($script_id)) {
    $script_params = _cloud_scripting_get_script($script_id);
    // @todo find a way to not have cross module functions. This should be a hook
    $ssh_user_name = aws_cloud_get_ssh_user($cloud_context, $instance_id);
    
    if (($path = _cloud_scripting_prepare_directory($instance_id)) != FALSE) {
      if (($perm_file = _cloud_scripting_get_perm_file($instance_id, $instance['key_name'], $cloud_context)) != FALSE) {
        $cluster_id  = $instance_script_params['cluster_id'];
        $template_id = $instance_script_params['template_id'];
        $instance_ip = $instance['dns_name'];
        $script = $script_params['script_template'];
        $script = str_replace("\r\n", "\n", $script);
        $script_files = _cloud_scripting_create_temp_script_files($instance_id, $script_id, $script, $instance_ip, $cluster_id, $cloud_context, $template_id);
        $script_dest_path = '/tmp/' . $instance_id . CLOUD_PATH_SEPARATOR . $script_id;
      
        //Here the main logic starts for executing Script on Remote Machine
        if (_cloud_scripting_is_ssh_enabled($cloud_context, $instance_ip, $instance_id, $perm_file) == '0') {
          // Instance is not ready for ssh. Do not execute script right now. Inform the user
          drupal_set_message(t('SSH is not enabled on instance @instance. Remote script cannot be executed', array('@instance' => $instance_id))); 
          return;
        }
        
        $log_file_name = drupal_realpath($path) . CLOUD_PATH_SEPARATOR . REQUEST_TIME . '.log';
        _cloud_scripting_update_scripts_status($instance_id, $script_id, CLOUD_SCRIPTING_SCRIPT_UNDER_EXECUTION); //Change Script Status to Under Progress
    
        $tmp_args = ' ' . $instance_ip . ' ' . $perm_file . ' ' . $log_file_name . ' ' . "\"" . $script_files . "\"" .
                      ' ' . $instance_id . ' ' . $script_id . ' ' . CLOUD_SSH_PATH . ' ' . CLOUD_SCP_PATH .
                      ' ' . drupal_realpath('public://cloud') . ' ' . drupal_realpath('public://cloud') . CLOUD_SCRIPTING_OUTPUT_LOG_FILE . ' ' . $ssh_user_name .
                      ' ' . "\"" . $script_dest_path . "\""  ;
    
        $command     = CLOUD_PHP_PATH . ' -f ' . dirname(__FILE__) . CLOUD_PATH_SEPARATOR . 'cloud_scripting_exec.inc ' . $tmp_args . ' >> ' . drupal_realpath('public://cloud') . '/cloud.log';
    
        exec($command, $output, $retval);
      }
    }
  }
  return;
}

/**
 * Function prepares the URI for a temporary directory
 * for use when executing remote scripts.  If the directory
 * cannot be created, FALSE is returned 
 */
function _cloud_scripting_prepare_directory($directory) {
  $path = CLOUD_SCRIPTING_TEMP_DIRECTORY . '/' . $directory;
  if (!file_prepare_directory($path, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS)) {
    drupal_set_message(t('Unable to create temporary cloud path @path', array('@path' => $path)), 'error');
    return FALSE;
  }
  return $path;
}

/**
 * This function will do the following.
 * a) Remove any magic words
 * b) It will check if cluster id is there then we will use Keywords related to cluster
 * c) If clusterId is blank then we will use Keywords realted to templateId
 * @param <type> $instance_id
 * @param <type> $script_id
 * @param <type> $script
 * @param <type> $instance_ip
 * @param <type> $cluster_id
 * @param <type> $cloud
 * @param <type> $template_id
 * @return <type>
 */
function _cloud_scripting_create_temp_script_files($instance_id, $script_id, $script, $instance_ip, $cluster_id, $cloud, $template_id) {

  $input_params_flag_set = FALSE;
  $org_script            = $script;

  //see if the instance is part of a cluster.
  //if so, update it with the cluster based values
  //make sure cloud_cluster is enabled
  $cluster_script = FALSE;
  if (module_exists('cloud_cluster')) {
    if (!empty($instance_id)) {
      //check to see if instance is part of a cluster
      $server = cloud_cluster_get_server($instance_id);
      if ($server != FALSE) {
        //grab its parameters and create a script
        $script = _scripting_update_cluster_inputs($script_id, $script, $server['server_id'], $server['cluster_id']);
        $cluster_script = TRUE;
      }
    }
  }

  //Run the template version if cluster doesn't exist
  if ($cluster_script == FALSE) {
    $script = _scripting_update_inputs($template_id, $script_id, $script);
  }
  
  $path = _cloud_scripting_prepare_directory($instance_id);
  $script_file = drupal_realpath($path . CLOUD_PATH_SEPARATOR . $script_id);
  if (file_unmanaged_save_data($script, $script_file, FILE_EXISTS_REPLACE) != FALSE) {
    // add in magic statement which indicates if the script finished
    file_put_contents($script_file, _scripting_get_magic_statement('/tmp/' . $instance_id . CLOUD_PATH_SEPARATOR . $script_id), FILE_APPEND);
  } 
  return $script_file;
}

function _scripting_update_cluster_inputs($script_id, $script, $server_id, $cluster_id) {
  if (!empty($server_id)) {
    $inputs = _cloud_scripting_get_cluster_script_inputs($script_id, $server_id, $cluster_id);
    foreach ($inputs as $input) {
      $keyword = $input['input_parameter'];
      $val = $input['cluster_value'];
      $type = $input['param_type'];

      if ($type == 'text') {
        $script = str_replace(trim($keyword), $val, $script);
      }
    }
  }
  return $script;
}

/**
 * Function to update inputs
 * @param <type> $template_id
 * @param <type> $script
 * @return <type>
 */
function _scripting_update_inputs($template_id, $script_id, $script) {

  if (! empty($template_id)) {

    $inputs    = _cloud_scripting_get_script_inputs($template_id, $script_id);
    $row_count = count($inputs);

    for ($row = 0; $row < $row_count; $row++) {

      $keyword = $inputs[$row]['input_parameter'];
      $val     = $inputs[$row]['template_value'];
      $type    = $inputs[$row]['param_type'];

      if (trim($type) == 'text') {

        $script = str_replace(trim($keyword), $val, $script);
      }
    }
  }

  return $script;
}

/**
 * Writes .done to signify a script is finished
 * @param <type> $script_file
 * @return <type>
 */
function _scripting_get_magic_statement($script_file) {
  return "\n" . "touch $script_file.done";
}

/**
 * Function to check if a script is done executing on an instance
 * @todo: Replace with this function _cloud_scripting_exec_remote_command()
 */
function _cloud_scripting_is_script_execution_done($cloud_context, $instance_ip, $instance_id, $perm_file, $script_id ) {

  $ssh_user_name  = aws_cloud_get_ssh_user( $cloud_context, $instance_id );
  $script_file    = '/tmp/' . $instance_id . CLOUD_PATH_SEPARATOR . $script_id . '.done';
  $remote_command = "[ -f $script_file ] && echo '1' || echo '0'";
  $command        = CLOUD_SSH_PATH . ' ' . $ssh_user_name . '@' . $instance_ip . ' -i "' . $perm_file . '" -o \'StrictHostKeyChecking no\' "' . $remote_command . '"';
  exec($command, $output, $retval);

  $result = isset($output[0]) ? $output[0] : 0;

  return $result;
}



/**
 * Function to check if SSH is enabled on a specific instance
 * @todo: Replace with this function _cloud_scripting_exec_remote_command()
 */
function _cloud_scripting_is_ssh_enabled($cloud_context, $instance_ip, $instance_id, $perm_file) {

  $ssh_user_name  = aws_cloud_get_ssh_user( $cloud_context, $instance_id );
  $remote_command = "[ -d /tmp ] && echo '1' || echo '0'";
  $command        = CLOUD_SSH_PATH . ' ' . $ssh_user_name . '@' . $instance_ip . ' -i "' . $perm_file . '" -o \'StrictHostKeyChecking no\' "' . $remote_command . '"';
  exec($command, $output, $retval);

  $result = isset($output[0]) ? $output[0] : '0';

  return $result;
}

/**
 * Function to get the permission file based on the cloud and keyname.  If the
 * key does not exist, the key will be created.
 * @param <type> $instance_id
 * @param <type> $key_name
 * @return <type>
 */
function _cloud_scripting_get_perm_file($instance_id, $key_name, $cloud_context) {
  $perm_file = '';
  $path = _cloud_scripting_prepare_directory($instance_id);
  
  if ($path != FALSE) {
    $perm_file = drupal_realpath($path) . '/' . $key_name;
    
    if (file_exists($perm_file) == FALSE) {
      $key = cloud_get_ssh_key($cloud_context, $key_name);
      if (file_unmanaged_save_data($key, $perm_file, FILE_EXISTS_REPLACE) !== FALSE) { 
        drupal_chmod($perm_file, 0400);
      }
      else {
        drupal_set_message(t('Unable to save write temporary permissions file @file', array('@file' => $perm_file)), 'error');
        $perm_file = FALSE; // set to false
      }
    }
  }
  return $perm_file;
}
