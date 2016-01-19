<?php

/**
 * Implements hook_field_formatter_info().
 */
function aws_amazon_field_formatter_info() {
  return array(
    'file_amazon_aws' => array(
      'label' => t('S3 Files'),
      'field types' => array('file'),
    ),
  );
}

/**
 * Implements hook_field_formatter_view().
 */
function aws_amazon_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array();
  global $user;
  switch ($display['type']) {
    case 'file_amazon_aws':
      $widget = $instance['widget'];
      $settings = $widget['settings'];
      //check if these items have been flagged
      $fids = array();
      foreach ($items as $item) {
        $fids[] = $item['fid'];
      }
      foreach ($items as $delta => $item) {
        $element[$delta] = array(
          '#theme' => 'aws_amazon_s3_file',
          '#file' => (object) $item,
          '#entity' => $entity,
          '#entity_type' => $entity_type,
        );
      }
      break;
  }

  return $element;
}

function theme_aws_amazon_s3_file($variables) {
  $output = '';
  $entity = $variables['entity'];
  $entity_type = $variables['entity_type'];
  $file = $variables['file'];
  $fid = $file->fid;
  $value = isset($file->aws_uri)? $file->aws_uri : $file->uri;
  $bucket = variable_get('aws_amazon_field_bucket', '');

  if (entity_access('view', $entity_type, $entity)) {
    $entity_id = entity_id($entity_type, $entity);
    if (strpos($value, 'aws://') !== FALSE) {
      $url = url("aws-amazon/$fid/download/" . $entity_type . '/' . $entity_id, array('absolute' => TRUE));
    }
    else {
      if (!empty($bucket)) {
        //this file needs to be uploaded to s3. track this we will upload it in cron
        $lfid = db_select('aws_amazon_files', 'afs')->fields('afs', array('fid'))
          ->condition('fid', $fid)->execute()->fetchField();
        if (empty($lfid)) {
          db_insert('aws_amazon_files')->fields(array('fid' => $fid, 'bucket' => $bucket, 'uri' => $value, 
           'entity_type' => $entity_type, 'entity_id' => $entity_id, 'status' => 0))
            ->execute();
        }
      }
      else {
        watchdog('aws-amazon-field', 'S3 Link widget configured, but no bucket specified. !click to specify bucket.',
          array('!click' => l(t('Click here'), 'admin/config/system/aws-amazon')), WATCHDOG_ALERT);
      }
      $url = file_create_url($value);
    }
    $output = l(basename($value), $url);
  }
  return $output;
}
