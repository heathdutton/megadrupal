<?php

/**
 * @file
 * Handles Amazon SDK interactions
 * 
 */

/**
 * Function to build a fully usable AmazonEC2 object,
 * based on a particular cloud_context.  The object
 * returned is ready to use for any SDK calls.
 */
function aws_cloud_sdk_ec2_init($cloud_context) {

  if (isset($cloud_context)) {

    // add check to make sure awssdk is there
    $loaded = libraries_load('awssdk');
    if ($loaded['installed'] == TRUE && $loaded['loaded'] == 1) {

      $cloud_name = cloud_get_info($cloud_context, 'cloud_name');
      $host_uri   = cloud_get_info($cloud_context, 'host_uri');
      $key        = cloud_get_info($cloud_context, 'aws_access_key');
      $secret     = cloud_get_info($cloud_context, 'aws_secret_key');
      $account_id = cloud_get_info($cloud_context, 'user_id');

      // set the credentials for this cloud context
      CFCredentials::set(
        array(
          $cloud_name  => array(
            'key'                   => $key,
            'secret'                => $secret,
            'account_id'            => $account_id,
            'certificate_authority' => FALSE,
          )
        )
      );
      
      // instantiate and set the region
      $ec2 = new AmazonEC2();
      $ec2->set_region($host_uri);

      return $ec2;
    }
    else {
      drupal_set_message(t('AWSSDKforPHP library not installed or loaded properly. Cloud information will not be downloaded.'), 'error', FALSE);
    }
  }
  return FALSE;
}

/**
 * The DescribeImages operation returns information about AMIs, 
 * AKIs, and ARIs available to the user. 
 * @param $cloud_context
 *   Cloud to perform operation on
 * @param $opt
 *   An associative array of parameters.
 */
function aws_cloud_sdk_describe_images($cloud_context, $images = array(), $owners = array(), $users = array()) {

  $ec2 = aws_cloud_sdk_ec2_init($cloud_context);

  if ($ec2) {

    $response = $ec2->describe_images(array(
      'ImageId' => $images,
      'Owner' => $owners,
      'ExecutableBy' => $users,
    ));

    return aws_cloud_sdk_check_response($response, 'Downloading Images');
  }
  return FALSE;
}

/**
 * Creates an Amazon EBS-backed AMI from a �running� or �stopped� instance.
 * @param $cloud_context
 *   Cloud to perform operation on
 * @param $instance_id
 *   The ID of the instance from which to create the new image.
 * @param $name
 *   The name for the new AMI being created.
 * @param $no_reboot
 *   By default this property is set to false, which means Amazon 
 *   EC2 attempts to cleanly shut down the instance before image creation and reboots the instance afterwards
 */
function aws_cloud_sdk_create_image($cloud_context, $instance_id, $name, $description = '', $no_reboot = FALSE) {

  $ec2 = aws_cloud_sdk_ec2_init($cloud_context);

  if ($ec2) {

    $response = $ec2->create_image($instance_id, $name);
    return aws_cloud_sdk_check_response($response, 'Create Image', array('NoReboot' => $no_reboot, 'Description' => $description));
  }
  return FALSE;  
}

/**
 * The RunInstances operation launches a specified number of instances.
 * @param $cloud_context
 *   Cloud to perform operation on
 * @param $image_id    
 *   ID of the AMI with which to launch instances.
 * @param $min         
 *   Minimum number of instances to launch.
 * @param $max         
 *   Maximum number of instances to launch.
 * @param $key_name    
 *   Name of the key pair with which to launch instances.
 * @param $groups      
 *   Names of the security groups with which to associate the instances.
 * @param $data        
 *   The user data available to the launched instances.
 * @param $type        
 *   This specifies the instance type.  Options include m1.small,  m1.large,  and m1.xlarge.
 * @param $zone        
 *   This specifies the placement constraints (Availability Zones) for launching the instances.
 * @param $kernel_id   
 *   This specifies the ID of the kernel with which to launch the instance.
 * @param $ramdisk_id  
 *   This specifies the ID of the RAM disk with which to launch the instance.
 * @param $params
 *   An associative array of additional parameters used to launch the instance. 
 *   This is not used currently.  It is passed for backward compatibility.
 */
function aws_cloud_sdk_run_instances($cloud_context, $image_id, $min_count, $max_count, $key_name = NULL, $groups = array(), $user_data = NULL, $instance_type = NULL, $zone = NULL, $kernel_id  = NULL, $ramdisk_id  = NULL, $params = array()) {

  $ec2 = aws_cloud_sdk_ec2_init($cloud_context);

  if ($ec2) {
    // put configurable parameteres into the $opt array
    $opt = array();
    $opt['InstanceType'] = $instance_type;
    if (isset($user_data)) {
      $opt['UserData'] = $user_data;
    }
    
    if (isset($key_name)) {
      $opt['KeyName'] = $key_name;
    }
    
    if (count($groups) == 0) {
      // set a default group if there are none
      $opt['SecurityGroup'] = 'default';
    }
    else {
      $opt['SecurityGroup'] = $groups;
    }
    
    if ($zone != 'any') {
      $opt['Placement']['AvailabilityZone'] = $zone;
    }
    
    if ($kernel_id != AWS_CLOUD_CLOUD_NONE) {
      $opt['KernelId'] = $kernel_id;
    }
    if ($ramdisk_id != AWS_CLOUD_CLOUD_NONE) {
      $opt['RamdiskId'] = $ramdisk_id;
    }

    $response = $ec2->run_instances($image_id, $min_count, $max_count, $opt);
    return aws_cloud_sdk_check_response($response, 'Launching Instance');
  }
  return FALSE;
}

/**
 * The DescribeInstances operation returns information about instances that you own.
 * @param $cloud_context
 *   Cloud to perform operation on
 * @param $instances
 *   An array of instances to get information on
 */
function aws_cloud_sdk_describe_instance($cloud_context, $instances = array()) {

  $ec2 = aws_cloud_sdk_ec2_init($cloud_context);

  if ($ec2) {
    $response = $ec2->describe_instances(array('InstanceId' => $instances));
    return aws_cloud_sdk_check_response($response, 'Describing Instances');
  }
  return FALSE;
}

/**
 * The RebootInstances operation requests a reboot of one or more instances
 * @param $cloud_context
 *   Cloud to perform operation on
 * @param $instances
 *   An array or string id of instances to be rebooted.  
 */
function aws_cloud_sdk_reboot_instance($cloud_context, $instances) {

  $ec2 = aws_cloud_sdk_ec2_init($cloud_context);

  if ($ec2) {
    $response = $ec2->reboot_instances($instances);
    return aws_cloud_sdk_check_response($response, 'Reboot Instance');
  }
  return FALSE;
}

/**
 * The TerminateInstances operation shuts down one or more instances
 * @param $cloud_context
 *   Cloud to perform operation on
 * @param $instances
 *   An array or string id of instances to be rebooted.  
 */
function aws_cloud_sdk_terminate_instances($cloud_context, $instances) {

  $ec2 = aws_cloud_sdk_ec2_init($cloud_context);

  if ($ec2) {
    $response = $ec2->terminate_instances($instances);
    return aws_cloud_sdk_check_response($response, 'Terminate Instance');
  }
  return FALSE;
}

/**
 * The CreateKeyPair operation creates a new 2048 bit RSA 
 * key pair and returns a unique ID that can be used to 
 * reference this key pair when launching new instances
 * @param $cloud_context
 *   Cloud to perform operation on
 * @param $key_name
 *   The unique name for the new key pair.
 */
function aws_cloud_sdk_create_keypair($cloud_context, $key_name) {

  $ec2 = aws_cloud_sdk_ec2_init($cloud_context);

  if ($ec2) {
    $response = $ec2->create_key_pair($key_name);
    return aws_cloud_sdk_check_response($response, 'Creating Key Pair');
  }
  return FALSE;
}

/**
 * The DescribeKeyPairs operation returns information about key pairs available.
 * @param $cloud_context
 *   Cloud to perform operation on
 * @param $key_name
 *   Optional key name array
 */
function aws_cloud_sdk_describe_keypairs($cloud_context, $key_name = array()) {

  $ec2 = aws_cloud_sdk_ec2_init($cloud_context);

  if ($ec2) {

    $response = $ec2->describe_key_pairs(array('KeyName' => $key_name));

    return aws_cloud_sdk_check_response($response, 'Describing Key Pairs');
  }
  return FALSE;
}

/**
 * The DeleteKeyPair operation deletes a key pair.
 * @param $cloud_context
 *   Cloud to perform operation on
 * @param $key_name
 *   The name of the Amazon EC2 key pair to delete.
 */
function aws_cloud_sdk_delete_keypair($cloud_context, $key_name) {

  $ec2 = aws_cloud_sdk_ec2_init($cloud_context);

  if ($ec2) {
    $response = $ec2->delete_key_pair($key_name);
    return aws_cloud_sdk_check_response($response, 'Deleting Key Pair');
  }  
  return FALSE;
}

/**
 * The CreateSecurityGroup operation creates a new security group.
 * @param $cloud_context
 *   Cloud to perform operation on
 * @param $group_name
 *   Name of the security group.
 * @param $group_description
 *   Description of the group. This is informational only.
 */
function aws_cloud_sdk_create_security_group($cloud_context, $group_name, $group_description) {

  $ec2 = aws_cloud_sdk_ec2_init($cloud_context);

  if ($ec2) {
    $response = $ec2->create_security_group($group_name, $group_description);
    return aws_cloud_sdk_check_response($response, 'Creating Security Group');
  }
  return FALSE;
}

/**
 * The DescribeSecurityGroups operation returns information about security groups that you own. 
 * @param $cloud_context
 *   Cloud to perform operation on
 * @param $groups
 *   The optional list of Amazon EC2 security groups to describe. 
 *   Pass a string for a single value, or an indexed array for multiple values.
 */
function aws_cloud_sdk_describe_security_group($cloud_context, $groups = array()) {

  $ec2 = aws_cloud_sdk_ec2_init($cloud_context);

  if ($ec2) {
    $response = $ec2->describe_security_groups(
      array('GroupName' => $groups)
    );
    return aws_cloud_sdk_check_response($response, 'Describing Security Group');    
  }
  return FALSE;
}

/**
 * The DeleteSecurityGroup operation deletes a security group.
 * @param $cloud_context
 *   Cloud to perform operation on
 * @param $group_name
 *   The name of the Amazon EC2 security group to delete.
 */
function aws_cloud_sdk_delete_security_group($cloud_context, $group_name) {

  $ec2 = aws_cloud_sdk_ec2_init($cloud_context);

  if ($ec2) {
    $response = $ec2->delete_security_group(array('GroupName' => $group_name));
    return aws_cloud_sdk_check_response($response, 'Deleting Security Group');
  }
  return FALSE;
}

/**
 * Adds permissions to a security group.
 * @param $cloud_context
 *   Cloud to perform operation on
 * @param $group_name   
 *   Name of the group to edit.
 * @param $src_name   
 *   Name of security group to authorize access to when operating on a user/group pair.
 * @param $src_owner   
 *   Owner of security group to authorize access to when operating on a user/group pair.
 * @param $protocol   
 *   IP protocol to authorize access to when operating on a CIDR IP.  "tcp",  "udp" and "icmp" allowed.
 * @param $from_port   
 *   Bottom of port range to authorize access to when operating on a CIDR IP. This contains the ICMP type if ICMP is being authorized.
 * @param $to_port   
 *   Top of port range to authorize access to when operating on a CIDR IP. This contains the ICMP code if ICMP is being authorized.
 * @param $src_ip   
 *   CIDR IP range to authorize access to when operating on a CIDR IP.
 */
function aws_cloud_sdk_authorize_security_group_ingress($cloud_context, $group_name, $src_name, $src_owner, $protocol = NULL, $from_port = NULL, $to_port = NULL, $src_ip = NULL) {

  $ec2 = aws_cloud_sdk_ec2_init($cloud_context);

  if ($ec2) {

    $opt = array();
    $perm = array();
    if (isset($group_name)) {
      $opt['GroupName'] = $group_name;
    }
    
    $perm['Groups'] = array();
    $group = array();   
    if (!empty($src_name) && !empty($src_owner)) {
      $group['GroupName'] = $src_name;
      $group['UserId'] = $src_owner;
    }
    if (count($group)) {
      $perm['Groups'][] = $group;
    }
    // OpenStack's Nova-api requires a more 
    // complicated array data structure with IpPermissions
    // and IpRanges.  The extra
    // array encapsulation is for nova support.
    // The extra array encapsulation works well for EC2

    if (!empty($protocol)) {
      $perm['IpProtocol'] = $protocol;
    }
    if (isset($from_port) && strlen($from_port)) {
      $perm['FromPort'] = $from_port;
    }
    if (isset($to_port) && strlen($to_port)) {
      $perm['ToPort'] = $to_port;
    }
    if (!empty($src_ip)) {
      $perm['IpRanges'] = array(
        array('CidrIp' => $src_ip)
      );
    }
    if (count($perm)) {
      $opt['IpPermissions'] = array($perm);
    }

    $response = $ec2->authorize_security_group_ingress($opt);
    return aws_cloud_sdk_check_response($response, 'Add Permissions to Security Group');
  }
  return FALSE;
}

/**
 * The RevokeSecurityGroupIngress operation revokes permissions from a security group.
 * @param $group_name   
 *   Name of the group to edit.
 * @param $src_name   
 *   Name of security group to authorize access to when operating on a user/group pair.
 * @param $src_owner   
 *   Owner of security group to authorize access to when operating on a user/group pair.
 * @param $protocol   
 *   IP protocol to authorize access to when operating on a CIDR IP.  "tcp",  "udp" and "icmp" allowed.
 * @param $from_port   
 *   Bottom of port range to authorize access to when operating on a CIDR IP. This contains the ICMP type if ICMP is being authorized.
 * @param $to_port   
 *   Top of port range to authorize access to when operating on a CIDR IP. This contains the ICMP code if ICMP is being authorized.
 * @param $src_ip   
 *   CIDR IP range to authorize access to when operating on a CIDR IP.
 */
function aws_cloud_sdk_revoke_security_group_ingress($cloud_context, $group_name, $src_name, $src_owner, $protocol = NULL, $from_port = NULL, $to_port = NULL, $src_ip = NULL) {

  $ec2 = aws_cloud_sdk_ec2_init($cloud_context);

  if ($ec2) {

    $opt = array();
    $opt['GroupName'] = $group_name;
    
    $perm = array();
    $perm['Groups'] = array();
    $group = array();   
    if (!empty($src_name) && !empty($src_owner)) {
      $group['GroupName'] = $src_name;
      $group['UserId'] = $src_owner;
    }
    if (count($group)) {
      $perm['Groups'][] = $group;
    }
    
    // OpenStack's Nova-api requires a more 
    // complicated array data structure with IpPermissions
    // and IpRanges.  The extra
    // array encapsulation is for nova support.
    // The extra array encapsulation works well for EC2
    
    if (!empty($protocol)) {
      $perm['IpProtocol'] = $protocol;
    }
    if (isset($from_port) && strlen($from_port)) {
      $perm['FromPort'] = $from_port;
    }
    if (isset($to_port) && strlen($to_port)) {
      $perm['ToPort'] = $to_port;
    }
    if (!empty($src_ip)) {
      $perm['IpRanges'] = array(
        array('CidrIp' => $src_ip)
      );
    }
    if (count($perm)) {
      $opt['IpPermissions'] = array($perm);
    }
    
    $response = $ec2->revoke_security_group_ingress($opt);
    return aws_cloud_sdk_check_response($response, 'Revoke Permissions to Security Group');
  }
  return FALSE;
}


/**
 * The DescribeAvailabilityZones operation describes availability zones 
 * that are currently available to the account and their states.
 * @param $cloud_context
 *   Cloud to perform operation on
 * @param $zones
 *   A list of the availability zone names to describe. 
 *   Pass a string for a single value, or an indexed array for multiple values.
 */
function aws_cloud_sdk_describe_zones($cloud_context, $zones = array()) {

  $ec2 = aws_cloud_sdk_ec2_init($cloud_context);

  if ($ec2) {
    $response = $ec2->describe_availability_zones(array('ZoneName' => $zones));
    return aws_cloud_sdk_check_response($response, 'Describing Availability Zones');
  }
  return FALSE;
}

/**
 * The DescribeAddresses operation lists elastic IP addresses assigned to your account.
 * @param $cloud_context
 *   Cloud to perform operation on
 */
function aws_cloud_sdk_describe_addresses($cloud_context) {

  $ec2 = aws_cloud_sdk_ec2_init($cloud_context);

  if ($ec2) {
    $response = $ec2->describe_addresses();
    return aws_cloud_sdk_check_response($response, 'Describing Addresses');
  }
  return FALSE;  
}

/**
 * The AllocateAddress operation acquires an elastic IP address for use with your account.
 * @param $cloud_context
 *   Cloud to perform operation on
 */
function aws_cloud_sdk_allocate_address($cloud_context) {

  $ec2 = aws_cloud_sdk_ec2_init($cloud_context);

  if ($ec2) {
    $response = $ec2->allocate_address();
    return aws_cloud_sdk_check_response($response, 'Allocating Address');
  } 
  return FALSE;
}

/**
 * The ReleaseAddress operation releases an elastic IP address associated with your account.
 * @param $cloud_context
 *   Cloud to perform operation on
 * @param $public_ip
 *   The elastic IP address that you are releasing from your account.
 */
function aws_cloud_sdk_release_address($cloud_context, $public_ip) {

  $ec2 = aws_cloud_sdk_ec2_init($cloud_context);

  if ($ec2) {
    $response = $ec2->release_address(array('PublicIp' => $public_ip));
    return aws_cloud_sdk_check_response($response, 'Releasing Address');
  }
  return FALSE;
}

/**
 * The AssociateAddress operation associates an elastic IP address with an instance.
 * @param $cloud_context
 *   Cloud to perform operation on
 * @param $instance_id
 *   The instance to associate with the IP address.
 * @param $public_ip
 *   IP address that you are assigning to the instance.
 */
function aws_cloud_sdk_associate_address($cloud_context, $instance_id, $public_ip) {

  $ec2 = aws_cloud_sdk_ec2_init($cloud_context);

  if ($ec2) {
    $response = $ec2->associate_address($instance_id, $public_ip);
    return aws_cloud_sdk_check_response($response, 'Associating Address');
  }
  return FALSE;
}

/**
 * The DisassociateAddress operation disassociates the specified elastic IP address 
 * from the instance to which it is assigned. This is an idempotent operation. 
 * If you enter it more than once, Amazon EC2 does not return an error.
 * @param $cloud_context
 *   Cloud to perform operation on
 * @param $public_ip
 *   The elastic IP address that you are disassociating from the instance.
 */
function aws_cloud_sdk_disassociate_address($cloud_context, $public_ip) {

  $ec2 = aws_cloud_sdk_ec2_init($cloud_context);

  if ($ec2) {

    $response = $ec2->disassociate_address($public_ip);
    return aws_cloud_sdk_check_response($response, 'Disassociating Address');
  }
  return FALSE;
}

/**
 * Describes the status of the indicated volume or, in lieu of any specified, 
 * all volumes belonging to the caller. Volumes that have been deleted are not described.
 * @param $cloud_context
 *   Cloud to perform operation on
 */
function aws_cloud_sdk_describe_volumes($cloud_context) {

  $ec2 = aws_cloud_sdk_ec2_init($cloud_context);

  if ($ec2) {

    $response = $ec2->describe_volumes();
    return aws_cloud_sdk_check_response($response, 'Describing Volumes');
  }
  return FALSE;
}

/**
 * The GetConsoleOutput operation retrieves console output for the specified instance.
 * @param $cloud_context
 *   Cloud to perform operation on
 * @param $instance_id
 *   The ID of the instance for which you want console output.
 */
function aws_cloud_sdk_get_console_output($cloud_context, $instance_id) {

  $ec2 = aws_cloud_sdk_ec2_init($cloud_context);

  if ($ec2) {

    $response = $ec2->get_console_output($instance_id);
    return aws_cloud_sdk_check_response($response, 'Getting Console Output');
  }
  return FALSE;
}

/**
 * Initializes an empty volume of a given size.
 * @param $cloud_context
 *   Cloud to perform operation on
 * @param $size
 *   The size of the volume, in gigabytes. Required 
 *   if you are not creating a volume from a snapshot.
 * @param $zone
 *   The Availability Zone in which to create the new volume.
 * @param $snapshot_id
 *   The ID of the snapshot from which to create the new volume.
 */
function aws_cloud_sdk_create_volume($cloud_context, $size, $zone, $snapshot_id = NULL) {

  $ec2 = aws_cloud_sdk_ec2_init($cloud_context);

  if ($ec2) {

    $opt = array(
      'Size' => $size,
    );
    if (!empty($snapshot_id) && $snapshot_id != AWS_CLOUD_CLOUD_NONE) {
      $opt['SnapshotId'] = $snapshot_id;
    }
    var_dump($opt);
    $response = $ec2->create_volume($zone, $opt);
    return aws_cloud_sdk_check_response($response, 'Creating Volume');
  }
  return FALSE;
}

/**
 * Deletes a previously created volume. Once successfully deleted, a new volume can be created with the same name.
 * @param $cloud_context
 *   Cloud to perform operation on
 * @param $volume_id
 *   The ID of the EBS volume to delete.
 */
function aws_cloud_sdk_delete_volume($cloud_context, $volume_id) {

  $ec2 = aws_cloud_sdk_ec2_init($cloud_context);

  if ($ec2) {
    $response = $ec2->delete_volume($volume_id);
    return aws_cloud_sdk_check_response($response, 'Deleting Volume');
  }
  return FALSE;
}

/**
 * Attach a previously created volume to a running instance.
 * @param $cloud_context
 *   Cloud to perform operation on
 * @param $volume_id
 * The ID of the Amazon EBS volume. The volume and instance 
 * must be within the same Availability Zone and the instance must be running.
 * @param $instance_id
 *   The ID of the instance to which the volume attaches. The volume and 
 *   instance must be within the same Availability Zone and the instance must be running.
 * @param $device
 *   Specifies how the device is exposed to the instance (e.g., /dev/sdh).
 */
function aws_cloud_sdk_attach_volume($cloud_context, $volume_id, $instance_id, $device) {

  $ec2 = aws_cloud_sdk_ec2_init($cloud_context);

  if ($ec2) {

    $response = $ec2->attach_volume($volume_id, $instance_id, $device);
    return aws_cloud_sdk_check_response($response, 'Attaching Volume');
  }
  return FALSE;
}

/**
 * Detach a previously attached volume from a running instance.
 * @param $cloud_context
 *   Cloud to perform operation on
 * @param $volume_id
 *   The ID of the volume to detach.
 */
function aws_cloud_sdk_detach_volume($cloud_context, $volume_id) {

  $ec2 = aws_cloud_sdk_ec2_init($cloud_context);

  if ($ec2) {

    $response = $ec2->detach_volume($volume_id);
    return aws_cloud_sdk_check_response($response, 'Detaching Volume');
  }
  return FALSE;
}

/**
 * Returns information about the Amazon EBS snapshots available to you. 
 * @param $cloud_context
 *   Cloud to perform operation on
 */
function aws_cloud_sdk_describe_snapshots($cloud_context) {

  $ec2 = aws_cloud_sdk_ec2_init($cloud_context);

  if ($ec2) {

    $response = $ec2->describe_snapshots(array('RestorableBy' => cloud_get_info($cloud_context, 'user_id')));
    return aws_cloud_sdk_check_response($response, 'Describing Snapshot');
  }
  return FALSE;
}

/**
 * Create a snapshot of the volume identified by volume ID. 
 * @param $cloud_context
 *   Cloud to perform operation on
 * @param $volume_id
 *   The ID of the volume from which to create the snapshot.
 * @param $description
 *   The description for the new snapshot.
 */
function aws_cloud_sdk_create_snapshot($cloud_context, $volume_id, $description) {

  $ec2 = aws_cloud_sdk_ec2_init($cloud_context);

  if ($ec2) {

    $response = $ec2->create_snapshot($volume_id, array('Description' => $description));
    return aws_cloud_sdk_check_response($response, 'Creating Snapshot');
  }
  return FALSE;  
}

/**
 * Deletes the snapshot identified by snapshotId.
 * @param $cloud_context
 *   Cloud to perform operation on
 * @param $snapshot_id
 *   The ID of the snapshot to delete.
 */
function aws_cloud_sdk_delete_snapshot($cloud_context, $snapshot_id) {

  $ec2 = aws_cloud_sdk_ec2_init($cloud_context);

  if ($ec2) {
    $response = $ec2->delete_snapshot($snapshot_id);
    return aws_cloud_sdk_check_response($response, 'Deleting Snapshot');
  }
  return FALSE;
}

/**
 * The RegisterImage operation registers an AMI with Amazon EC2. 
 * Images must be registered before they can be launched. 
 * @param $cloud_context
 *   Cloud to perform operation on
 * @param $opt
 *   Array with image options.  
 *   See: http://docs.amazonwebservices.com/AWSSDKforPHP/latest/index.html#m=AmazonEC2/register_image
 *   for available parameters
 */
function aws_cloud_sdk_register_image($cloud_context, $image_location) {

  // implement when fixing bundle image
  // decide if $opt is needed
  $ec2 = aws_cloud_sdk_ec2_init($cloud_context);

  if ($ec2) {
    //NOT FULLY TESTED YET
    $response = $ec2->register_image(array('ImageLocation' => $image_location));
    return aws_cloud_sdk_check_response($response, 'Registering Image');
  }
  return FALSE;
}

/**
 * The DeregisterImage operation deregisters an AMI. Once deregistered, 
 * instances of the AMI can no longer be launched.
 * @param $cloud_context
 *   Cloud to perform operation on
 * @param $image_id
 *   The ID of the AMI to deregister.
 * @param $opt
 *   An associative array of parameters.  
 *   See: http://docs.amazonwebservices.com/AWSSDKforPHP/latest/index.html#m=AmazonEC2/deregister_image
 *   for available parameters
 */
function aws_cloud_sdk_deregister_image($cloud_context, $image_id, $opt = array()) {
  // implement when fixing bundle image
  // decide if $opt is needed
}

/**
 * Helper function that checks response. If the response is ok
 * return it. Print the error message if the response has errors.
 * @param $response
 *   Response object from the AWS SDK
 * @param $operation
 *   The operation being performed.  The operation is set in the
 *   error message.
 */
function aws_cloud_sdk_check_response($response, $operation) {

  if ($response->isOK()) {
    try { // for Cloud(n)
      @$body = new SimpleXMLElement($response->body);
      return $body;
    } catch (Exception $e) { // This is normal for AWS
      return $response->body;
    }
  }
  else {
    // Loop the errors and set a message. 
    // return false so that calling functions can handle the error
    $error_message = array();
    foreach ($response->body->Errors as $error) {
      $error_message[] = $error->Error->Message;
    }
    drupal_set_message(t('Error performing "@operation"', array('@operation' => $operation)), 'error');
    drupal_set_message(t('The errors are: @error', array('@error' => implode(',', $error_message))), 'error');
    return FALSE;
  }
}
