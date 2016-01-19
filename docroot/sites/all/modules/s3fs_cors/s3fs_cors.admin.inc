<?php

/**
 * @file
 * Administration form setup for S3 File System CORS Upload.
 */

/**
 * Builds the Admin form.
 */
function s3fs_cors_admin_form() {
  $config = _s3fs_get_config();
  $form = array();
  $form['s3fs_cors_origin'] = array(
    '#type'          => 'textfield',
    '#title'         => t('CORS Origin'),
    '#description'   => t(
      'Please enter the URL from which your users access this website, e.g. <i>www.example.com</i>.
      You may optionally specifiy up to one wildcard, e.g. <i>*.example.com</i>.<br>
      Upon submitting this form, if this field is filled, your S3 bucket will be configured to allow CORS
      requests from the specified origin. If the field is empty, your bucket\'s CORS config will be deleted.'),
    '#default_value' => !empty($config['cors_origin']) ? $config['cors_origin'] : '',
  );

  $form = system_settings_form($form);
  $form['#submit'][] = '_s3fs_cors_admin_form_submit';
  $form['actions']['submit']['#value'] = 'Apply CORS Config';
  return $form;
}

 /**
 * Enables CORS on the configured bucket when the form is submitted.
 */
function _s3fs_cors_admin_form_submit($form, &$form_state) {
  $config = _s3fs_get_config();
  $s3 = _s3fs_get_amazons3_client($config);
  if (!empty($form_state['values']['s3fs_cors_origin'])) {
    $cors_origin = $form_state['values']['s3fs_cors_origin'];
    $s3->putBucketCors(array(
      'Bucket' => $config['bucket'],
      'CORSRules' => array(
        array(
          'AllowedHeaders' => array('*'),
          'ExposeHeaders' => array('x-amz-version-id'),
          'AllowedMethods' => array('POST'),
          'MaxAgeSeconds' => 3000,
          'AllowedOrigins' => array("http://$cors_origin", "https://$cors_origin"),
        ),
        array(
          'AllowedMethods' => array('GET'),
          'AllowedOrigins' => array('*'),
        ),
      ),
    ));
  }
  else {
    // If $form_state['values']['s3fs_cors_origin'] is empty, that means we
    // need to delete their bucket's CORS config.
    $s3->deleteBucketCors(array(
      'Bucket' => $config['bucket'],
    ));
  }
}
