<?php

/**
 * @file
 * aws_ec2_lib Register Image
 *
 * Copyright (c) 2010-2011 DOCOMO Innovations, Inc.
 *
 */

// Start : DB functions Related to Register Image


/**
 *
 * @param   $cloud_context
 * @return
 */
function _aws_cloud_get_register_image_query($cloud_context) {

  $ec2_lib_register_image_table  = aws_cloud_get_table_info( AWS_CLOUD_REGISTER_IMAGE_STATUS_TABLE );
  return $query = 'SELECT c.id,  c.owner,   c.status ,  c.image_id FROM {' . $ec2_lib_register_image_table . '} c WHERE id=:id and cloud_type = :cloud_type '  ;
}

/**
 *
 * @param     $cloud_context
 * @param     $id
 * @param     $status
 * @param     $image_id
 * @return
 */
function _aws_cloud_update_image_register_status($cloud_context, $id, $status, $image_id) {

  $ec2_lib_register_image_table  = aws_cloud_get_table_info( AWS_CLOUD_REGISTER_IMAGE_STATUS_TABLE );

  $num_updated = db_update($ec2_lib_register_image_table)
    ->fields(array(
    'status' => $status,
    'image_id' => $image_id,
  ))
    ->condition('id', $id, '=')
    ->condition('cloud_type', $cloud_context, '=')
    ->execute();

  // User Activity Log
  cloud_audit_user_activity( array(
    'type' => 'user_activity',
    'message' => t('Image has been added: @image_id', array('@image_id' => $id)),
    'link' => '',
  )
  );

  return;
}

// End : DB functions Related to Register Image
