<?php

/**
 * @file
 * DB functions for images
 *
 * Copyright (c) 2010-2011 DOCOMO Innovations, Inc.
 */

/**
 * Updated by yas 2011/02/22
 */

/**
 * Complete function to create an image based on an instance
 */
function aws_cloud_create_image($cloud_context, $instance_id, $image_name, $description, $no_reboot = FALSE) {
  if (($response = aws_cloud_sdk_create_image($cloud_context, $instance_id, $image_name, $no_reboot)) !== FALSE) {
    $image_id = $response->imageId;
    _aws_cloud_download_images($cloud_context, array($image_id));
    drupal_set_message(t('Backup Image is now being created.'));
  }
  else {
    drupal_set_message(t('Error while trying to create an image'), 'error');
  }
}

/**
 * Get images
 * @param   $cloud_context
 * @return
 */
function _aws_cloud_get_describe_images_query($cloud_context) {

  $ec2_lib_images_table = aws_cloud_get_table_info( AWS_CLOUD_IMAGES_TABLE );

  $query = db_select($ec2_lib_images_table, 'c');
  $query->fields('c');
  return $query;
}

/**
 * Get root device type 
 * @param unknown_type $image_id
 */
function _aws_cloud_get_root_device_type($image_id) {
  $root_device_type = FALSE;
  $ec2_lib_images_table = aws_cloud_get_table_info( AWS_CLOUD_IMAGES_TABLE );
  $root_device_type = db_select($ec2_lib_images_table, 'c')
    ->fields('c', array('root_device_type'))
    ->condition('c.image_id', $image_id)
    ->execute()
    ->fetchField();
  return $root_device_type; 
}

/**
 * Return image given the image_id 
 */
function _aws_cloud_get_image_by_id($image_id) {
  $ec2_lib_images_table = aws_cloud_get_table_info( AWS_CLOUD_IMAGES_TABLE );
  return db_select($ec2_lib_images_table, 'c')
    ->fields('c')
    ->execute()->fetchAssoc();
}

/**
 * Get a count of all available images
 * @param   $cloud_context
 * @return
 */
function _aws_cloud_get_images_list_count($cloud_context) {

  $ec2_lib_images_table = aws_cloud_get_table_info( AWS_CLOUD_IMAGES_TABLE );
  $query        = 'SELECT COUNT(*) AS images_count FROM {' . $ec2_lib_images_table . '} c where cloud_type = :cloud_type'  ;
  $result       = db_query($query, array(':cloud_type' => $cloud_context));
  return $result->fetchField();
}


/**
 * Get images for a particular owner id
 * @param   $cloud_context
 * @return
 */
function _aws_cloud_get_describe_owner_images_query($cloud_context) {

  $ec2_lib_images_table = aws_cloud_get_table_info( AWS_CLOUD_IMAGES_TABLE );
  $query = db_select($ec2_lib_images_table, 'c');
  $query
    ->fields('c');

  return $query;
}


/**
 * Get images query
 * @param   $cloud_context
 * @return
 */
function _aws_cloud_get_image_query($cloud_context) {

  $ec2_lib_images_table = aws_cloud_get_table_info( AWS_CLOUD_IMAGES_TABLE );
  return $query = 'SELECT c.image_nickname, c.image_id, c.image_location, c.imageowner_id, c.image_state, c.architecture FROM {' . $ec2_lib_images_table . '} c WHERE image_id=:image_id  '  ;
}



/**
 * Format the image nickname
 * @param   $location
 * @return
 */
function _aws_cloud_get_image_nickname($location) {

  $tok  = explode('/', $location );
  $len  = sizeof($tok);

  $name =  $tok[$len - 1];

  $nickname = explode('.manifest.xml', $name );

  return $nickname[0];
}


/**
 * Download all images and store them in the database
 * If images array is passed, only retrieve images for those ids
 * @param   $cloud_context
 * @return
 */
function _aws_cloud_download_images($cloud_context, $images = array()) {
  if (empty($cloud_context)) {
    $cloud_list = cloud_get_all_clouds_display_name();
    cloud_set_config_error_message($cloud_list[$cloud_context], $cloud_context);
    return FALSE;
  }
  $ec2_lib_images_table = aws_cloud_get_table_info(AWS_CLOUD_IMAGES_TABLE);
  
  // download all images from SDK
  if (($obj_response = aws_cloud_sdk_describe_images($cloud_context, $images)) !== FALSE) {
    $id_nick_arr = cloud_get_all_nickname('image_id', 'image_nickname', $ec2_lib_images_table);
    // delete all the old images - prep for the refresh
    $query = db_delete($ec2_lib_images_table)
      ->condition('cloud_type', $cloud_context);
    // if images are passed, only delete the images with that id
    if (count($images)) {
      $query->condition('image_id', $images);
    }
    $query->execute();
    
    foreach ($obj_response->imagesSet->item as $item) {
      $query_args = array();
  
      $query_args['image_id'] = $item->imageId;
      $query_args['cloud_type'] = $cloud_context;
      $tmp_nickname = cloud_check_in_array($id_nick_arr, $item->imageId);
      $query_args['image_nickname'] = $tmp_nickname != NULL ? $tmp_nickname : _aws_cloud_get_image_nickname($item->imageLocation);
      $query_args['image_location'] = $item->imageLocation;
      $query_args['image_state'] = $item->imageState;
      $query_args['imageowner_id'] = $item->imageOwnerId;
      $query_args['is_public'] = $item->isPublic;
      $query_args['architecture'] = isset($item->architecture) ? $item->architecture : '';
  
      //get the base_cloud and make sure it is openstack_nova for the switch statement
      $base_cloud = cloud_get_base_cloud($cloud_context);
      switch ($base_cloud) { // Needed for OpenStack
        // That should be imageType.  In Austin release, there are two types of attributes like
        // 'type' and 'imageType'.  However in Baxer release, they left 'type' and removed 'imageType'!
        case AWS_CLOUD_OPENSTACK_NOVA_CONTEXT:
          $query_args['image_type'] =  isset($item->type) ? $item->type
                        : (isset($item->imageType) ? $item->imageType : '');
          break;
        default:
          $query_args['image_type'] =  isset($item->imageType) ? $item->imageType : '';
      }
  
      $query_args['kernel_id'] = isset($item->kernelId) ? $item->kernelId : '';
      $query_args['ramdisk_id'] = isset($item->ramdiskId) ? $item->ramdiskId : '';
      $query_args['region'] = !empty($region_url) ? $region_url : '';
      $query_args['root_device_type'] = $item->rootDeviceType;
      
      db_insert($ec2_lib_images_table)
        ->fields(array(
          'image_id',
          'cloud_type',
          'image_nickname',
          'image_location',
          'image_state',
          'imageowner_id',
          'is_public',
          'architecture',
          'image_type',
          'kernel_id',
          'ramdisk_id',
          'region',
          'root_device_type',
        ))
        ->values(
          $query_args
      )->execute();
    }
    // User Activity Log
    cloud_audit_user_activity(
      array(
        'type' => 'user_activity',
        'message' => 'Update: Image(s)',
        'link' => '',
      )
    );
  }
}


/**
 * Download all images
 * @param   $cloud_context
 * @return
 */
function _aws_cloud_download_describe_images($cloud_context) {

  ini_set('memory_limit', '512M');
  _aws_cloud_download_images($cloud_context);
  return TRUE;
}


/**
 * Get all images from db
 * @param   $cloud_context
 * @param   $image_type
 * @return
 */
function aws_cloud_get_all_images_db($cloud_context, $image_type = '') {

  $query = '';
  $query_args    = array();
  $image_options = array();

  $ec2_lib_images_table    = aws_cloud_get_table_info( AWS_CLOUD_IMAGES_TABLE );

  $query .= ' SELECT distinct * FROM {' . $ec2_lib_images_table . ' }'
         . ' WHERE image_type like :image_type and cloud_type = :cloud_type '  ;

  $query_args = array(
    ':image_type' => $image_type . '%', // % is used for wildcard
    ':cloud_type' => $cloud_context,
  );

  $image_options = array();
  $image_options[AWS_CLOUD_CLOUD_NONE] = AWS_CLOUD_CLOUD_NONE;
  $result = db_query( $query, $query_args );

  foreach ($result as $image) {

    $image_options[$image->image_id] =  $image->image_nickname . ' (' . $image->image_id . ')';
  }

  asort( $image_options);
  return $image_options;
}

/**
 * Get all images by a certain string
 * @param   $cloud_context
 * @param   $image_type
 * @param   $search_str
 * @return
 */
function aws_cloud_get_all_images_by_str_db($cloud_context, $image_type = '', $search_str = '') {

  $ec2_lib_images_table = aws_cloud_get_table_info( AWS_CLOUD_IMAGES_TABLE );

  $query      = 'SELECT distinct * FROM {' . $ec2_lib_images_table . '} WHERE 
  image_type=:image_type and ( image_id like :image_id or image_nickname like :image_nickname ) 
  and cloud_type = :cloud_type
  order by image_id ';

  $query_args = array(
    ':image_type' => $image_type,
    ':image_id' => '%' . $search_str . '%',
    ':image_nickname' => '%' . $search_str . '%',
    ':cloud_type' => $cloud_context,
  );

  $image_options = array();
  $image_options[AWS_CLOUD_CLOUD_NONE] = AWS_CLOUD_CLOUD_NONE;
  $result = db_query( $query, $query_args );

  foreach ($result as $image) {

    $image_options[$image->image_nickname . ' (' . $image->image_id . ')'] =  $image->image_nickname . ' (' . $image->image_id . ')';
  }
  asort($image_options);
  return $image_options;
}


/**
 * Ajax function for Machine images
 * @param   $cloud_context
 * @param   $img_id @
 */
function aws_cloud_callback_list_machine_images($cloud_context, $img_id) {

  $matches = array();
  $matches =  aws_cloud_get_all_images_by_str_db($cloud_context, 'machine', $img_id );
  print drupal_json_encode($matches);

  exit();
}


/**
 * Ajax function for getting kernels
 * @param   $cloud_context
 * @param   $img_id
 */
function aws_cloud_callback_list_kernel_images($cloud_context, $img_id) {

  $matches = array();
  $matches =  aws_cloud_get_all_images_by_str_db($cloud_context, 'kernel'  , $img_id );
  print drupal_json_encode($matches);

  exit();
}



/**
 * AJAX function for getting ramdisks
 * @param   $cloud_context
 * @param   $img_id @
 */
function aws_cloud_callback_list_ramdisk_images($cloud_context, $img_id) {

  $matches = array();
  $matches = aws_cloud_get_all_images_by_str_db($cloud_context, 'ramdisk', $img_id );
  print drupal_json_encode($matches);

  exit();
}

