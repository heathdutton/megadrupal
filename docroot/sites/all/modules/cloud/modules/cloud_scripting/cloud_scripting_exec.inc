<?php

/**
 * @file
 * Provides scripting feature such as bash, Perl and etc (Mainly bash scripts).
 * Works with Cloud and Server Templates module.
 *
 * Copyright (c) 2010-2011 DOCOMO Innovations, Inc.
 *
 */

$instance_ip                     = $argv[1]  ; // The ip of instance where the script is to be executed
$perm_file                       = $argv[2]  ; // The Key file using which we will ssh on the instance
$log_file_name                   = $argv[3]  ; // write the logs in this file
$script_file                     = $argv[4]  ; // Script which is to be executed on the instance
$script_id                       = $argv[6]  ; // The id for which the script was executed
$instance_id                     = $argv[5]  ; // The id of the instance on which the script is to executed
$SSH_PATH                        = $argv[7]  ;
$SCP_PATH                        = $argv[8]  ;
$CLOUD_SCRIPTING_TEMP_DIRECTORY  = $argv[9]  ;
$CLOUD_SCRIPTING_OUTPUT_LOG_FILE = $argv[10];
$USER                            = $argv[11];
$script_dest_path                = $argv[12]; // This is the destination folder where the script files will be copied

$folder_name = '/tmp/' . $instance_id;

$os_name = php_uname('s');
if ($os_name == 'Linux' || $os_name == 'Unix') {
  $pid = pcntl_fork();
}
else {
  $pid = 0;
}


if (!$pid) {
  $fh = fopen($log_file_name,    'w');
  fwrite($fh,    "Started executing Script:\n" );

  //$folder_name .= '.' . time()   ;
  fwrite($fh,    "FolderName: $folder_name\n");

  $command =  $SSH_PATH . ' ' . $USER . '@' . $instance_ip . ' -i "' . $perm_file . '" -o \'StrictHostKeyChecking no\' "mkdir -p ' . $folder_name . '"';

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

  $command = $SSH_PATH . ' ' . $USER . '@' . $instance_ip . ' -i "' . $perm_file . '" -o \'StrictHostKeyChecking no\' "sh -l ' . $script_dest_path . ' "';

  fwrite($fh, 'Executing command ' . $command  );

  exec(  $command, $output, $retval);

  _cloud_scripting_print_output($fh, $command, $retval, $output );

  $script_execution_log_file = $folder_name . $CLOUD_SCRIPTING_OUTPUT_LOG_FILE;

  if ( $retval != 0 ) {

    fwrite($fh, 'Execution of Script file failed' );
    fclose($fh);

    if (copy($log_file_name, $script_execution_log_file)) {
      echo "Failed to copy $log_file_name...\n";
    }

    return $retval;
  }
  fclose($fh);

  //capture output of script execution to file

  if (copy($log_file_name, $script_execution_log_file)) {

    echo "Failed to copy $log_file_name...\n";
  }

  return 0;
}


return 0;

function _cloud_scripting_print_output($fh, $command, $retval, $output) {

  fwrite($fh, "cmd: $command\n");
  fwrite($fh, "return: $retval\n");

  foreach ($output as $key => $val ) {
    fwrite($fh, 'output:' . $val . "\n" );
  }

  flush();
}
