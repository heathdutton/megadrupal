<?php

/**
 * Amazon settings 
 */
function aws_amazon_admin_settings_form($form, &$form_state) {
  if (!file_exists('sites/all/libraries/AWSSDKforPHP/sdk.class.php')) {
    form_set_error('', t('AWS PHP Library not found. Download and place the library in sites/all/libraries/AWSSDKforPHP'));
  }
  global $base_url;
  //generate a unique URL per site that cannot be faked.
  $accesskey = variable_get('aws_amazon_access_key', '');
  if (!empty($accesskey)) {
    $regions = _aws_amazon_get_regions();
    if (empty($regions)) {
      $regions = array('Failed to connect to Amazon.');
    }
    
    $token = hash('sha1', $base_url . $accesskey);
    $form['amazon_cron_url_link'] = array(
      '#type' => 'markup',
      '#markup' => '<div>' . t('URL for usage in cron %url. Note menu cache may need to be flushed.',
        array('%url' => url('aws-amazon-cron/' . $token, array('absolute' => TRUE)))) . '</div>', 
    );

    //set selected regions against volumes
    _aws_amazon_set_regions($regions);
    $form['regs'] = array(
      '#type' => 'markup',
      '#markup' => '<div>' . t('Available regions are !regions', array('!regions' => implode(', ', $regions))) .
        '</div>',
    );
  }
  $form['aaccess'] = array(
    '#type' => 'fieldset',
    '#title' => t('AWS Access Configuration'),
    '#collapsible' => TRUE,
  );
  $key = variable_get('aws_amazon_isecret_key', '');
  if (!empty($key)) {
    if (strpos($key, 'delete') !== FALSE) {
      variable_del('aws_amazon_secret_key');
    }
    else {
      variable_set('aws_amazon_secret_key', $key);
    }
    variable_del('aws_amazon_isecret_key');
  }
  $form['aaccess']['aws_amazon_isecret_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Amazon Secret Key'),
    '#attributes' => array('autocomplete' => 'off'),
    '#description' => t('Set the value to change the existing value. Saved values are not shown. To delete the saved value enter %del', array('%del' => 'delete')),
  );
  $key = variable_get('aws_amazon_iaccess_key', '');
  if (!empty($key)) {
    if (strpos($key, 'delete') !== FALSE) {
      variable_del('aws_amazon_access_key');
    }
    else {
      variable_set('aws_amazon_access_key', $key);
    }
    cache_clear_all('', 'cache_menu');
    variable_del('aws_amazon_iaccess_key');
  }
  $form['aaccess']['aws_amazon_iaccess_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Amazon Access Key'),
    '#attributes' => array('autocomplete' => 'off'),
    '#description' => t('Set the value to change the existing value. Saved values are not shown. To delete the saved value enter %del', array('%del' => 'delete')),
  );
  $form['abackups'] = array(
    '#type' => 'fieldset',
    '#title' => t('EC2 Backup Configuration'),
    '#collapsible' => TRUE,
  );
  $form['abackups']['aws_amazon_backup_cron'] = array(
    '#type' => 'radios',
    '#title' => t('Run Backup and Clean in Cron'),
    '#options' => array(0 => t('Do not run in cron'), 1 => t('Run on each cron run'), 2 => t('Once a day'),),
    '#default_value' => variable_get('aws_amazon_backup_cron', 0),
    '#description' => t('Both the options on Drupal cron run')
  );
  $form['abackups']['aws_amazon_backup_allow'] = array(
    '#type' => 'checkbox',
    '#title' => t('Create backups'),
    '#default_value' => variable_get('aws_amazon_backup_allow', 1),
    '#description' => t('If this is deselected, then no backup will be created'),
  );
  $form['abackups']['aws_amazon_backup_type'] = array(
    '#type' => 'radios',
    '#title' => t('Select Number of Backups to Retain'),
    '#options' => array(0 => t('All Backup'), 1 => t('Number of Days'), 2 => t('Number of Backups')),
    '#default_value' => variable_get('aws_amazon_backup_type', 0),
  );
  $form['abackups']['aws_amazon_backup_no'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of Backups'),
    '#element_validate' => array('aws_amazon_element_validate_numeric'),
    '#description' => t('Number of backups to retain. At least 1 backup will be maintained.'),
    '#default_value' => variable_get('aws_amazon_backup_no', 0),
  );
  $form['abackups']['aws_amazon_backup_vols'] = array(
    '#type' => 'textarea',
    '#title' => t('Volume IDs'),
    '#description' => t('Enter the volume IDs to backup / delete. Enter one per line. If volumes are in different region specify region|volume.'),
    '#default_value' => variable_get('aws_amazon_backup_vols', ''),
  );
  $form['s3backup'] = array(
    '#type' => 'fieldset',
    '#title' => t('S3 Backup'),
    '#collapsible' => TRUE,
  );
  $form['s3backup']['aws_amazon_s3_backup_bucket'] = array(
    '#type' => 'textfield',
    '#title' => t('S3 Backup Bucket Name'),
    '#description' => t('Specify the bucket for S3 backup. If the bucket must exists.'),
    '#default_value' => variable_get('aws_amazon_s3_backup_bucket', ''),
  );
  $form['s3backup']['aws_amazon_s3_backup_days'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of backups'),
    '#element_validate' => array('aws_amazon_element_validate_numeric'),
    '#description' => t('Specify the number of backups to retain. S3 Backups are done once a day when Drupal cron is executed. Specify 0 to retain all backups.'),
    '#default_value' => variable_get('aws_amazon_s3_backup_days', 0),
  );
  $form['s3backup']['aws_amazon_s3_buckets'] = array(
    '#type' => 'textarea',
    '#title' => t('Buckets'),
    '#description' => t('Specify buckets to backup one per line'),
    '#default_value' => variable_get('aws_amazon_s3_buckets', ''),
  );
  $form['cloudfront'] = array(
    '#type' => 'fieldset',
    '#title' => t('AWS Cloud Front Configuration'),
    '#collapsible' => TRUE,
  );
  $form['cloudfront']['aws_amazon_cloudfront_pk'] = array(
    '#type' => 'textfield',
    '#title' => t('Private key path'),
    '#default_value' => variable_get('aws_amazon_cloudfront_pk', ''),
    '#description' => t('Relative path to your Drupal root e.g. sites/default/modules/mymodule/pk.pem')
  );
  $form['cloudfront']['aws_amazon_cloudfront_kpid'] = array(
    '#type' => 'textfield',
    '#title' => t('Key Paid ID'),
    '#default_value' => variable_get('aws_amazon_cloudfront_kpid', ''),
    '#description' => t('Keypair ID of Cloud Front')
  );
  $form['cloudfront']['aws_amazon_cloudfront_dh'] = array(
    '#type' => 'textfield',
    '#title' => t('Distribution Host'),
    '#default_value' => variable_get('aws_amazon_cloudfront_dh', ''),
    '#description' => t('Distribution host name e.g. d3f4tdoki894i0j.cloudfront.net'),
  );
  $form['common'] = array(
    '#type' => 'fieldset',
    '#title' => t('Common settings'),
    '#collapsible' => TRUE,
  );
  $form['common']['aws_amazon_log'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Logging'),
    '#default_value' => variable_get('aws_amazon_log', 0),
    '#description' => t('If this is selected then all actions will be logged in watchdog, otherwise only errors are logged'),
  );
  $form['common']['aws_amazon_enable_backup'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Backup'),
    '#default_value' => variable_get('aws_amazon_enable_backup', 0),
    '#description' => t('If this is not selected then no actual operation is performed. The actions are logged in watchdog for review. Select this for actual backup / cleanup operation to be performed.'),
  );
  $form['awsfs'] = array(
    '#type' => 'fieldset',
    '#title' => t('Field Settings'),
    '#collapsible' => TRUE,
  );
  $form['awsfs']['aws_amazon_field_bucket'] = array(
    '#type' => 'textfield',
    '#title' => t('Bucket'),
    '#default_value' => variable_get('aws_amazon_field_bucket', ''),
    '#description' => t('Bucket to store uploaded files')
  );
  $form['awsfs']['aws_amazon_field_timeout'] = array(
    '#type' => 'textfield',
    '#title' => t('Timeout'),
    '#default_value' => variable_get('aws_amazon_field_timeout', '+1 hour'),
    '#description' => t('Set the validity of the file links to access S3. Use PHP Date format e.g. +1 hour'),
  );
  $form['awsfs']['aws_amazon_direct_upload'] = array(
    '#type' => 'textarea',
    '#title' => t('Direct Upload Buckets'),
    '#default_value' => variable_get('aws_amazon_direct_upload', ''),
    '#description' => t('Specify buckets to which users can directly upload file. Enter one per line. Each bucket will have own permission. Requires menu cache to be flushed after each change.'),
  );
  return system_settings_form($form);
}

function _aws_amazon_set_regions($regions) {
  $volumes = explode("\n", variable_get('aws_amazon_backup_vols', ''));
  $vol_regions = array();
  $endpoints = array_flip($regions);
  foreach ($volumes as $vol) {
    if (strpos($vol, '|') !== FALSE) {
      $token = explode('|', $vol);
      $vol_regions[trim($token[0])] = $endpoints[trim($token[0])];
    }
  }
  variable_set('aws_amazon_regions', $vol_regions);
}


/**
 * Bucket browser
 */
function aws_amazon_backup_browser($bucket = '', $path = '') {
  $output = '';
  drupal_add_css(drupal_get_path('module', 'aws_amazon') . '/aws_amazon.css');
  drupal_add_js(drupal_get_path('module', 'aws_amazon') . '/aws_amazon.js');
  $show_restore = FALSE;
  // aws_amazon_backup_s3();
  if (empty($bucket)) {
    $bucket = variable_get('aws_amazon_s3_backup_bucket', '');
    $show_restore = TRUE;
  }
  else {
    $show_restore = ($bucket == variable_get('aws_amazon_s3_backup_bucket', ''));
  }
  $url_stub = 'admin/config/system/aws-amazon/browse/' . $bucket . '/';

  if (!empty($bucket)) {
    aws_amazon_set_credentials();
    $s3 = new AmazonS3();
    $opts = array();
    $path = urldecode($path);
    if (empty($path)) {
      $opts['delimiter'] = '/';
    }
    else {
      $opts['delimiter'] = '/';
      $opts['prefix'] = $path;
    }
    $response = $s3->list_objects($bucket, $opts);      
    $data = _aws_amazon_get_response_as_json($response);
    $items = array();
    if (isset($data['CommonPrefixes'])) {
      if (isset($data['CommonPrefixes']['Prefix'])) {
        //single item
        $item = $data['CommonPrefixes'];
        $name = str_replace($path, '', $item['Prefix']);
        $items[] = l($name, $url_stub . urlencode($item['Prefix']),
          array('attributes' => array('class' => array('folder', 'search'), 'key' => strtolower($name))));
      }
      else {
        foreach ($data['CommonPrefixes'] as $item) {
          $name = str_replace($path, '', $item['Prefix']);
          $items[] = l($name, $url_stub . urlencode($item['Prefix']),
          array('attributes' => array('class' => array('folder', 'search'), 'key' => strtolower($name))));
        }
      }
    }
    if(isset($data['Contents'])) {
      if (isset($data['Contents']['Key'])) {
        $item = $data['Contents'];
        //there is only one item
        $name = basename($item['Key']);
        $items[] = l($name, $url_stub . urlencode($item['Key']),
          array('attributes' => array('class' => array('download', 'search'), 'title' => t('Download'), 'key' => strtolower($name)))) .
              ($show_restore? l('<span title="' . t('Restore') . '"></span>', "admin/config/system/aws-amazon/restore/" . urlencode($item['Key']),
                array('html' => TRUE, 'attributes' => array('class' => array('restore')))) : '');
      }
      else {
        foreach ($data['Contents'] as $item) {
          if ($item['Key'] !== $path) {
            $name = basename($item['Key']);
            $items[] = l($name, $url_stub . urlencode($item['Key']),
              array('attributes' => array('class' => array('download', 'search'), 'title' => t('Download'), 'key' => strtolower($name)))) .
              ($show_restore? l('', "admin/config/system/aws-amazon/restore/" . urlencode($item['Key']),
                array('html' => TRUE, 'attributes' => array('class' => array('restore'), 'title' => t('Restore')))) : '');
          }
        }
      }
    }
    if (!empty($items)) {
      $item = array(
        '#type' => t('textfield'),
        '#title' => t('Search'),
        '#id' => 'search',
      );
      $output = drupal_render($item);
      $output .= '<div id="bucket-browser">' . theme('item_list', compact('items')) . '</div>';
    }
    else {
      $output = t('Empty Folder.');
    }
    $tokens = explode('/', $path);
    $tokens = array_filter($tokens);
    $tokens = array_slice($tokens, 0, count($tokens) - 1);
    if (!empty($tokens)) {
      $output = '<div>' . l(t('Back'),  $url_stub . urlencode(implode('/', $tokens) . '/'),
        array('attributes' => array('class' => array('back')))) . '</div>' . $output; 
    }
    elseif (!empty($path)) {
      $output = '<div>' . l(t('Back'), $url_stub,
        array('attributes' => array('class' => array('back')))) . '</div>' . $output;
    }
  }
  else {
    $output = t('No backup bucket specified.');
  }
  return $output;
}

/**
 * browse any bucket
 */
function aws_amazon_bucket_browser($bucket = '', $path = '') {
  if (empty($bucket)) {
    $form['#attached']['js'] = array(drupal_get_path('module', 'aws_amazon') . '/aws_amazon.js');
    $form['bucket'] = array(
      '#type' => 'textfield',
      '#title' => t('Bucket'),
      '#required' => TRUE,
      '#attributes' => array('id' => 'edit-bucket'),
    );
    $form['btn_browse'] = array(
      '#type' => 'button',
      '#value' => t('Browse'),
      '#attributes' => array('id' => 'edit-btn-browse'),
    );
    drupal_add_js(array('aws_amazon' => array('url' => url('admin/config/system/aws-amazon/bucket-browse/', array('absolute' => TRUE)))), 'setting');
    return drupal_render($form);
  }
  return aws_amazon_backup_browser($bucket, $path);
}

/**
 * downloads the specified file
 */
function aws_amazon_bucket_browser_download($bucket, $path) {
  $path = urldecode($path);
  $url = aws_amazon_get_link($bucket, $path, strtotime('+5 minute'));
  if ($url) {
    drupal_goto($url);
  }
  return t('File not found.');
}

/**
 * restore a file
 */
function aws_amazon_bucket_browser_restore_form($form, &$form_state, $aws_path) {
  $form['aws_path'] = array(
    '#type' => 'hidden',
    '#value' => $aws_path,
  );  
  $file = urldecode($aws_path);
  $path = 'admin/config/system/aws-amazon/browse/' . variable_get('aws_amazon_s3_backup_bucket', '') . '/' .
    urlencode(str_replace(basename($file), '', $file));
  return confirm_form($form, t('Are you sure you want to restore the file !file', array('!file' => basename($file))), $path);
}

/**
 * submit handler
 */
function aws_amazon_bucket_browser_restore_form_submit($form, &$form_state) {
  $file = urldecode($form_state['values']['aws_path']);
  $tokens = explode('/', $file);
  $backup_bucket = variable_get('aws_amazon_s3_backup_bucket', '');
  $restore_bucket = $tokens[0];
  $tokens = array_slice($tokens, 2);
  $restore_path = implode('/', $tokens);
  $response = aws_amazon_copy_file($backup_bucket, $file, $restore_bucket, $restore_path);
  if ($response){
    drupal_set_message(t('File restored.'), 'success');
  }
  else {
    drupal_set_message(t('File restore failed.'), 'error');
  }
  
  $form_state['redirect'] = 'admin/config/system/aws-amazon/browse/' .
    variable_get('aws_amazon_s3_backup_bucket', '') . '/' . urlencode(str_replace(basename($file), '', $file));
}
