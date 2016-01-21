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
 * Updated by Jamir  2011/04/30
 */


$instance_ip                     = $argv[1]  ; // The ip of instance where the script is to be executed
$perm_file                       = $argv[2]  ; // The Key file using which we will ssh on the instance
$script_log_file                 = $argv[3]  ; // write the logs in this file
$script_file                     = $argv[4]  ; // Script which is to be executed on the instance
$cert_file                       = $argv[5]  ; // Certificate file
$key_file                        = $argv[6]  ; // Key File
$instance_id                     = $argv[7]  ; // The id of the instance on which the script is to executed
$script_id                       = $argv[8]  ; // The id for which the script was executed
$SSH_PATH                        = $argv[9]  ; // SSH Path default should be SSH
$SCP_PATH                        = $argv[10]; // SCP Path
$CLOUD_SCRIPTING_TEMP_DIRECTORY  = $argv[11];
$CLOUD_SCRIPTING_OUTPUT_LOG_FILE = $argv[12];
$USER                            = $argv[13];
$script_dest_path                = $argv[14]; // This is the destination folder where the script files will be copied

$folder_name      = '/tmp/clanavi/' . $instance_id;
$key_folder_name  = '/tmp/clanavi/.ec2';

// Check the OS 
$os_name = php_uname('s');
if ($os_name == 'Linux' || $os_name == 'Unix') {
  $pid = pcntl_fork();
}
else {
  $pid = 0;
}


if (!$pid) {

  $fh   = fopen($script_log_file,    'w');
  fwrite( $fh,    "Started executing Script:\n" );
   
  fwrite($fh,    "FolderName: $folder_name\n");
  $command =  $SSH_PATH . ' ' . $USER . '@' . $instance_ip . ' -i "' . $perm_file . '" -o \'StrictHostKeyChecking no\' "mkdir -p ' . $folder_name . '"';

  $output = array();
  print '<br/>' . $command . '<br/>';

  exec(  $command, $output, $retval);
  _cloud_scripting_print_output($fh, $command, $retval, $output );

  if ( $retval != 0 ) {
    fwrite($fh, 'Unable to create temporary directory' );
    fclose($fh);
    return $retval;
  }

  $command =  $SSH_PATH . ' ' . $USER . '@' . $instance_ip . ' -i "' . $perm_file . '" -o \'StrictHostKeyChecking no\' "mkdir -p ' . $key_folder_name . '"';

  $output = array();

  print '<br/>' . $command . '<br/>';
  exec(  $command, $output, $retval);
  _cloud_scripting_print_output($fh, $command, $retval, $output );

  if ( $retval != 0 ) {

    fwrite($fh, 'Unable to create temporary folder' );
    fclose($fh);
    return $retval;
  }

  $command = $SCP_PATH . " -i $perm_file -o 'StrictHostKeyChecking no' $script_file $USER@$instance_ip:$folder_name ";

  $output = array();
  exec(  $command, $output, $retval);
  _cloud_scripting_print_output($fh, $command, $retval, $output );

  if ( $retval != 0 ) {

    fwrite($fh,    'Unable to copy the script file: ' . $script_file );
    fclose($fh);
    return $retval;
  }

  // Copy Certificate file
  $command = $SCP_PATH . " -i $perm_file -o 'StrictHostKeyChecking no' $cert_file $USER@$instance_ip:$key_folder_name/cert.pem ";

  $output = array();
  exec(  $command, $output, $retval);
  _cloud_scripting_print_output($fh, $command, $retval, $output );

  if ( $retval != 0 ) {

    fwrite($fh,    'Unable to copy the certificate file: ' . $script_file );
    fclose($fh);
    return $retval;
  }

  // Copy Key file
  $command = $SCP_PATH . " -i $perm_file -o 'StrictHostKeyChecking no' $key_file $USER@$instance_ip:$key_folder_name/pk.pem ";

  $output = array();
  exec(  $command, $output, $retval);
  _cloud_scripting_print_output($fh, $command, $retval, $output );

  if ( $retval != 0 ) {

    fwrite($fh,    'Unable to copy the certificate file: ' . $script_file );
    fclose($fh);
    return $retval;
  }

  $command = $SSH_PATH . ' ' . $USER . '@' . $instance_ip . ' -i "' . $perm_file . '" -o \'StrictHostKeyChecking no\' "chmod 700 ' . $folder_name . ' "';

  $output = array();
  exec(  $command, $output, $retval);
  _cloud_scripting_print_output($fh, $command, $retval, $output );
  if ( $retval != 0 ) {

    fwrite($fh, 'Unable to change the mode for folder' );
    fclose($fh);

    return $retval;
  }

  $command = $SSH_PATH . ' ' . $USER . '@' . $instance_ip . ' -i "' . $perm_file . '" -o \'StrictHostKeyChecking no\' "chmod 700 ' . $folder_name . '/* "';

  $output = array();
  exec(  $command, $output, $retval);

  _cloud_scripting_print_output($fh, $command, $retval, $output );

  if ( $retval != 0 ) {

    fwrite($fh, 'Unable to change mode of script files' );
    fclose($fh);

    return $retval;
  }
  else {

    fwrite($fh, $script_file );
    $script_file = str_replace(' ', ';',    trim($script_file));
    fwrite($fh, $script_file );
  }

  $command = $SSH_PATH . ' ' . $USER . '@' . $instance_ip . ' -i "' . $perm_file . '" -o \'StrictHostKeyChecking no\' " bash -l ' . $script_dest_path . ' "';

  fwrite($fh, 'Executing command ' . $command  );

  exec(  $command, $output, $retval);

  _cloud_scripting_print_output($fh, $command, $retval, $output );

  $script_execution_log_file = $folder_name . $CLOUD_SCRIPTING_OUTPUT_LOG_FILE;


  if ( $retval != 0 ) {

    fwrite($fh, 'Execution of Script file failed' );
    fclose($fh);

    if (copy($script_log_file, $script_execution_log_file)) {
      echo "Failed to copy log file $script_log_file...\n";
    }

    return $retval;
  }

  fclose($fh);

  //capture output of script execution to file
  if (copy($script_log_file, $script_execution_log_file)) {

    echo "Failed to copy $script_log_file...\n";
  }

  return 0;
}


return 0;


// Write the output to  a file
function _cloud_scripting_print_output($fh, $command, $retval, $output) {

  fwrite($fh, "Command: $command\n");
  fwrite($fh, "Result: $retval\n");

  // Write the output
  foreach ($output as $key => $val ) {
    fwrite($fh, 'Console Output:' . $val . "\n" );
  }

  flush();
}
