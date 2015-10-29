<?php

/**
 * @file
 * Buldle Amazon EC2 Image
 *
 * Copyright (c) 2010-2011 DOCOMO Innovations, Inc.
 *
 */

define('SSH_PATH', 'ssh' );
define('SCP_PATH', 'scp' );
define('SSH_USER_NAME', 'root' );

$instance_ip     = $argv[1]  ;
$perm_file       = $argv[2]  ;
$log_file_name      = $argv[3]  ;
$pvt_file_name      = $argv[4]  ;
$pub_file_fame      = $argv[5]  ;
$user_id        = $argv[6]  ;
$size        = $argv[7]  ;
$bucket_id      = $argv[8]  ;
$nick_name      = $argv[9]  ;
$accesskey      = $argv[10]  ;
$seckey        = $argv[11]  ;
$register_image_id   = $argv[12]  ;
$register_url     = $argv[13]  ;

$folder_name = '/mnt/';


$pid = pcntl_fork(); // On Linux
// $pid = 0 ; // Only for Win remove.


// Child Process to continue
if (!$pid) {

  $test_shell = "";
  $fh = fopen($log_file_name,    'w');
  fwrite($fh,    "Started the bundle image process:\n" );

  $command = SSH_PATH . " root@$instance_ip -i $perm_file  -o 'StrictHostKeyChecking no'  \"ec2-bundle-vol --help\" ";


  $command = $test_shell . SSH_PATH . " root@$instance_ip -i $perm_file  -o 'StrictHostKeyChecking no'  \"ec2-bundle-vol --help\" ";
  $output = array();
  exec(  $command, $output, $retval);
  _cloud_scripting_print_output($fh, $command, $retval, $output );

  if ($retval == 255 ) {
    fwrite($fh,    "Unable to connect to $instance_ip");
    fclose($fh);
    return $retval;
  }

  if ($retval == 127 ) {
    fwrite($fh,    "AMI commandline tools not installed on instance" );
    fclose($fh);
    return $retval;
  }

  if ($retval != 0 ) {
    fwrite($fh,    "Some errror" );
    fclose($fh);
    return $retval;
  }

  $folder_name .= '.' . REQUEST_TIME . '/'  ;
  fwrite($fh,    "FolderName: $folder_name\n");

  $command = $test_shell . SSH_PATH . ' ' . SSH_USER_NAME . '@' . $instance_ip . ' -i ' . $perm_file . ' mkdir -p ' . $folder_name  ;
  $output = array();
  exec(  $command, $output, $retval);
  _cloud_scripting_print_output($fh, $command, $retval, $output );

  if ( $retval != 0 ) {
    fwrite($fh,    "unable to create temporary folder" );
    fclose($fh);
    return $retval;
  }


  // $command = $test_shell .SCP_PATH . " -i $perm_file $pvt_file_name root@$instance_ip:$folder_name "  ; 
  $command = $test_shell . SCP_PATH . " -i $perm_file  \\\\172.21.97.72/keys/private_key.pem root@$instance_ip:$folder_name ";
  $output = array();
  exec(  $command, $output, $retval);
  _cloud_scripting_print_output($fh, $command, $retval, $output );

  if ( $retval != 0 ) {
    fwrite($fh, 'Unable to copy the autentication files');
    fclose($fh);
    return $retval;
  }

  $command = $test_shell . SCP_PATH . " -i $perm_file $pub_file_fame root@$instance_ip:$folder_name ";
  $output = array();
  exec(  $command, $output, $retval);
  _cloud_scripting_print_output($fh, $command, $retval, $output );

  if ( $retval != 0 ) {
    fwrite($fh,    "unable to copy the autentication files" );
    fclose($fh);
    return $retval;
  }

  $command = $test_shell . SSH_PATH . ' ' . SSH_USER_NAME . '@' . $instance_ip . ' -i ' . $perm_file . ' uname -m';
  $output = array();
  exec(  $command, $output, $retval);
  _cloud_scripting_print_output($fh, $command, $retval, $output );

  if ( $retval != 0 ) {
    fwrite($fh,    "unable to find the machine architecture" );
    fclose($fh);
    return $retval;
  }

  $arch = $output[0];


  if ( empty($arch) ) {
    $arch = 'i386';
  }
  elseif ( $arch[0] == 'i' && $arch[strlen($arch) - 1] == '6'  ) {
    $arch = 'i386';
  }


  $command = $test_shell . SSH_PATH . " root@$instance_ip  -i $perm_file ec2-bundle-vol -a -e /mnt -d $folder_name -k $folder_name" . "private_key.pem -u $user_id -s $size --arch $arch  -c  $folder_name" . "public_key.pem"  ;


  $output = array();
  exec(  $command, $output, $retval);
  _cloud_scripting_print_output($fh, $command, $retval, $output );

  if ( $retval != 0 ) {
    fwrite($fh,    "ec2-bundle-vol command failed." );
    fclose($fh);
    return $retval;
  }


  $command = $test_shell . SSH_PATH . " root@$instance_ip  -i $perm_file \"yes | ec2-upload-bundle -b $bucket_id -m $folder_name" . "image.manifest.xml -a $accesskey -s $seckey \" ";


  $output = array();
  exec(  $command, $output, $retval);
  _cloud_scripting_print_output($fh, $command, $retval, $output );

  if ( $retval != 0 ) {
    fwrite($fh,    "ec2-upload-bundle command failed." );
    fclose($fh);
    return $retval;
  }


  fwrite($fh,    "URL=$register_url&id=$register_image_id&location=$bucket_id/image.manifest.xml\n");
  $result = file_get_contents("$register_url&id=$register_image_id&location=$bucket_id/image.manifest.xml",    NULL,    NULL,    -1,    40);
  fwrite($fh,    "Result:$result\n");
  fwrite($fh,    "Bundle image process completed" );

  $command = $test_shell . SSH_PATH . " root@$instance_ip  -i $perm_file \" rm -rf $folder_name \" ";
  $output = array();
  exec(  $command, $output, $retval);
  _cloud_scripting_print_output($fh, $command, $retval, $output );

  if ( $retval != 0 ) {
    fwrite($fh,    "removing of tmp directorie $folder_name failed.");
    fclose($fh);
    return $retval;
  }

  fclose($fh);
  return 0;
}


// send reponse back.    
return 0;


function _cloud_scripting_print_output($fh, $command, $retval, $output) {
  fwrite($fh,    "cmd: $command\n");
  fwrite($fh,    "return: $retval\n");

  foreach ($output as $key => $val ) {
    fwrite($fh,    "output:" . $val . "\n" );
  }

  flush();

}



