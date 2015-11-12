<?php

/**
 * @file
 * Functions for bundling instance
 */

/**
 * Function to bundle a S3 backed instance.  This
 * supports any instances that are running and still
 * uses S3 backed storage.  The recommended storage is
 * EBS backed.  EBS backed storage does not require bundling
 * to backup an instance.  It is a simple SDK call.
 */
function aws_cloud_process_bundle_instance() {

  $output_dir = _aws_cloud_prepare_directory();
  if ($output_dir != FALSE) {
    // initialize any instances that needs to be bundled
    // @todo: think about refactoring the php execute routine
    aws_cloud_init_bundling();

    // Registration Process
    aws_cloud_process_bundle();
    
    // Register image
    aws_cloud_register_bundle_image();
  }
}

/**
 * Helper function to initiate bundling images
 */
function aws_cloud_init_bundling() {
  $instances = aws_cloud_db_bundle_instance_get('initiated');
  
  foreach ($instances as $inst) {
    $bundle_id = $inst['bundle_id'];
    $instance_id = $inst['instance_id'];
    $cloud_context = $inst['cloud_context'];
    $instance = cloud_get_instance($cloud_context, $instance_id);
    
    if ($instance) {
      $ssh_user_name  = aws_cloud_get_ssh_user( $cloud_context, $instance_id );
      $key_name       = $instance['key_name'];
      $instance_ip    = $instance['dns_name'];
      $perm_file = _cloud_scripting_get_perm_file($instance_id, $key_name, $cloud_context);
      
      // check ssh enabled
      if (_cloud_scripting_is_ssh_enabled($cloud_context, $instance_ip, $instance_id, $perm_file) == '1') {
        // prepare bundle_instance directory
        if (($path = _cloud_scripting_prepare_directory($instance_id . CLOUD_PATH_SEPARATOR . 'bundle_instance')) != FALSE) {
          $script_file = drupal_realpath($path . CLOUD_PATH_SEPARATOR . 'bundle_instance.sh');
          $cert_file = drupal_realpath($path . CLOUD_PATH_SEPARATOR . 'cert.pem');
          $perm_file = _cloud_scripting_get_perm_file($instance_id, $key_name, $cloud_context);
          
          // read in the script contents
          $script_filename = drupal_get_path('module', 'aws_cloud') . CLOUD_PATH_SEPARATOR . 'scripts/aws_cloud_bundle_image_ami_tools.sh';
          $script_handle   = fopen( $script_filename, "r" );
          $contents        = fread($script_handle, filesize($script_filename) );
          fclose($script_handle);
          
          
          // return a script with the contents replaced with real values
          $script_str = aws_cloud_replace_script_values($contents, array_merge($inst, $instance));
          
          //write out the script_str and the perm file
          if (file_unmanaged_save_data($script_str, $script_file, FILE_EXISTS_REPLACE) != FALSE && 
            file_unmanaged_save_data(cloud_get_info($cloud_context, 'certificate'), $cert_file, FILE_EXISTS_REPLACE) != FALSE) {
                        
            $script_dest_path = '/tmp/clanavi/' . $instance_id . CLOUD_PATH_SEPARATOR . 'bundle_instance.sh';
            $log_file_name = drupal_realpath($path) . '/' . REQUEST_TIME . '.log';
            
            $tmp_args  = ' ' . $instance_ip . ' ' . $perm_file . ' ' . $log_file_name . ' ' . "\"" . $script_file . "\"" .
                ' ' . $cert_file . ' ' . $perm_file .
                ' ' . $instance_id . ' bundle_instance ' . CLOUD_SSH_PATH . ' ' . CLOUD_SCP_PATH .
                ' ' . drupal_realpath('public://cloud') . ' ' . drupal_realpath('public://cloud') . ' ' . $ssh_user_name .
                ' ' . "\"" . $script_dest_path . "\""  ;
    
            // clean out the remote directories
            _cloud_scripting_exec_remote_command($cloud_context, $instance_id, 'sudo rm -rf /tmp/clanavi');
            
            // run the script in the background. 
            $command = CLOUD_PHP_PATH . ' -f ' . dirname(__FILE__) . CLOUD_PATH_SEPARATOR . 'aws_cloud_bundle_instance_exec.inc ' . $tmp_args . ' >> /tmp/cloud.log &';
            exec($command, $output, $retval);
                        
            // Update status
            aws_cloud_db_bundle_instance_update_status($bundle_id, $instance_id, 'processed');
          }
          else {
            // set watchdog
            $error = t('Unable to write script file/certificate file. Cannot bundle instance: @instance', array('@instance' => $instance_id));
            watchdog('aws_cloud', $error);
          }
        }  
      }
      else {
        // set watchdog where ssh is not enabled
        $error = t('Cannot ssh into instance: @instance. Unable to bundle instance.', array('@instance' => $instance_id));
        watchdog('aws_cloud', $error);
      }
    }
  }
}

/**
 * Function to process bundle.  Checks the status 
 * on the remote server.  Updates database if it 
 * failed or succeeded.
 */
function aws_cloud_process_bundle() {
  // Check the in progress bundle_instances
  $instances = aws_cloud_db_bundle_instance_get('processed');

  foreach ($instances as $instance) {
    $cloud_context = $instance['cloud_context'];
    $instance_id = $instance['instance_id'];
    $bundle_id = $instance['bundle_id'];

    // check for success
    if (_aws_cloud_bundle_instance_check_status($cloud_context, $instance_id, 'success')) {
      aws_cloud_db_bundle_instance_update_status($bundle_id, $instance_id, 'success');
    }
    // check if it failed
    if (_aws_cloud_bundle_instance_check_status($cloud_context, $instance_id, 'failed')) {
      aws_cloud_db_bundle_instance_update_status($bundle_id, $instance_id, 'failed');
    }
  }
}

/**
 * Register the bundled image
 */
function aws_cloud_register_bundle_image() {
  $instances = aws_cloud_db_bundle_instance_get('success');
  
  foreach ($instances as $inst) {
    $cloud_context = $inst['cloud_context'];
    $bucket = $inst['bucketname'];
    $bundle_id = $inst['bundle_id'];
    $instance_id = $inst['instance_id'];
          
    if (isset($inst['foldername']) & !empty($inst['foldername'])) {
       $bucket .= '/' . $inst['foldername']; 
    }
    $bucket .= '/' . $inst['nickname'] . '.manifest.xml';
    
    if (($obj_response = aws_cloud_sdk_register_image($cloud_context, $bucket)) !== FALSE) {
      if (isset($obj_response->imageId) && empty($obj_response->imageId) == FALSE) {
        aws_cloud_db_bundle_instance_update_status($bundle_id, $instance_id, 'complete:' . $obj_response->imageId);
      }
    }
    else {
      aws_cloud_db_bundle_instance_update_status($bundle_id, $instance_id, 'register-failed');
    }
  }
}

/**
 * Check the bundle instance flag on the remote instance
 */
function _aws_cloud_bundle_instance_check_status($cloud_context, $instance_id, $status) {
  $status_file    = '/tmp/clanavi/bundle_instance.' . $status;
  $output = _cloud_scripting_exec_remote_command($cloud_context, $instance_id, "[ -f $status_file ] && echo '1' || echo '0'");
  $result = isset($output[0]) ? $output[0] : 0;
  return $result;
}

/**
 * Helper function to substitute user specified values
 * about the bundle.
 */
function aws_cloud_replace_script_values($contents, $instance) {
  $cloud_context = $instance['cloud_context'];
  $script = str_replace("\r\n", "\n", $contents);
  
  // Update bucketname
  $nickname = str_replace( array('-', '_', ':', ' '), '', $instance['nickname']);
  
  // Replace the Bundle Values
  $script = str_replace("@CLANAVI_BUCKET_NAME", $instance['bucketname'], $script);
  $script = str_replace("@CLANAVI_FOLDER_NAME", $instance['foldername'], $script);
  $script = str_replace("@CLANAVI_IMAGE_NAME", $instance['nickname'], $script);
  $script = str_replace("@CLANAVI_SIZE", $instance['volume_size'], $script);
  
  // Replace Credentials
  $script = str_replace("@CLANAVI_AWS_USER_ID", cloud_get_info($cloud_context, 'user_id'), $script);
  $script = str_replace("@CLANAVI_AWS_ACCESS_KEY_ID", cloud_get_info($cloud_context, 'aws_access_key'), $script);
  $script = str_replace("@CLANAVI_AWS_SECRET_ACCESS_KEY", cloud_get_info($cloud_context, 'aws_secret_key'), $script);
  $script = str_replace('@CLANAVI_REGION', _aws_cloud_get_region(cloud_get_info($cloud_context, 'host_uri')), $script);
  
  return $script;
}

/**
 * The ec2-upload-bundle command requires a region.
 * The SDK does not really provide the region so 
 * deriving it from the host_uri
 */
function _aws_cloud_get_region($host_uri) {
  $region = 'us-east-1';
  $uri = explode('.', $host_uri);
  // if the supplied host_uri is ec2.amazonaws.com
  // it will default to us-east
  if (count($uri) > 3) {
    $region = $uri[1];
  }
  return $region;
}