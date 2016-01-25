<?php
function shrinktheweb_admin_log($form, $form_state) {
  $form = array();

  if(!isset($form_state['storage']['confirm'])) {
    $log_records = array();

    // format the tableselect header
    $header = array(
      'domain' => array('data' => t('Domain'), 'field' => 'sl.domain'),
      'hash' => array('data' => t('Hash'), 'field' => 'sl.hash'),
      'timestamp' => array('data' => t('Timestamp'), 'field' => 'sl.timestamp'),
      'capturedon' => array('data' => t('Captured on'), 'field' => 'sl.capturedon'),
      'quality' => array('data' => t('Quality'), 'field' => 'sl.quality'),
      'full' => array('data' => t('Full'), 'field' => 'sl.full'),
      'xmax' => array('data' => t('Xmax'), 'field' => 'sl.xmax'),
      'ymax' => array('data' => t('Ymax'), 'field' => 'sl.ymax'),
      'nrx' => array('data' => t('nrX'), 'field' => 'sl.nrx'),
      'nry' => array('data' => t('nrY'), 'field' => 'sl.nry'),
      'invalid' => array('data' => t('Invalid'), 'field' => 'sl.invalid'),
      'stwerrcode' => array('data' => t('STW error code'), 'field' => 'sl.stwerrcode'),
      'error' => array('data' => t('Error'), 'field' => 'sl.error'),
      'errcode' => array('data' => t('Error'), 'field' => 'sl.errcode'),
      'referrer' => array('data' => t('Referrer'), 'field' => 'sl.referrer'),
    );

    // get the log records
    $result = db_select('shrinktheweb_log', 'sl')
      ->fields('sl')
      ->extend('PagerDefault')
      ->limit(25)
      ->extend('TableSort')
      ->orderByHeader($header)
      ->addTag('shrinktheweb_log_records')
      ->execute();

    foreach ($result as $record) {
      $log_records[$record->siteid] = array(
        'domain' => $record->domain,
        'hash' => $record->hash,
        'timestamp' => $record->timestamp,
        'capturedon' => $record->capturedon,
        'quality' => $record->quality,
        'full' => $record->full,
        'xmax' => $record->xmax,
        'ymax' => $record->ymax,
        'nrx' => $record->nrx,
        'nry' => $record->nry,
        'invalid' => $record->invalid,
        'stwerrcode' => $record->stwerrcode,
        'error' => $record->error,
        'errcode' => $record->errcode,
        'referrer' => $record->referrer,
      );
    }

    //check if there are any records in the log
    if (empty($log_records)) {
      return array(
        'no_records' => array('#markup' => t('The log is empty.'))
      );
    }

    $form['log_records'] = array(
      '#type' => 'tableselect',
      '#header' => $header,
      '#options' => $log_records,
      '#tree' => FALSE
    );

    $form['pager'] = array(
      '#theme' => 'pager',
    );

    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Delete selected records')
    );

    return $form;
  }
  else{
    return confirm_form(
      $form,
      t('Do you want to delete the record(s)?'),
      'admin/config/media/shrinktheweb/log'
    );
  }
}

function shrinktheweb_admin_log_validate($form, $form_state) {
  if(!isset($form_state['storage']['confirm'])) {
    $log_recs = array_filter($form_state['values']['log_records']);
    if (empty($log_recs)) {
      form_set_error('log_records', t('Please select a log record!'));
    }
  }
}

function shrinktheweb_admin_log_submit($form, &$form_state) {
  if(!isset($form_state['storage']['confirm'])) {
    $form_state['storage']['confirm'] = TRUE;
    $form_state['storage']['log_records'] = array_filter($form_state['values']['log_records']);
    $form_state['rebuild'] = TRUE;
  }
  else {
    if (!empty($form_state['storage']['log_records'])) {
      $num_deleted = db_delete('shrinktheweb_log')
        ->condition('siteid', $form_state['storage']['log_records'], 'IN')
        ->execute();
      drupal_set_message(t('Deletion successful.'));
    }
  }
}

function shrinktheweb_admin_settings() {
  module_load_include('inc', 'shrinktheweb', 'shrinktheweb.api');
  $aAccountInfo = shrinktheweb_getAccountInfo();
  $response_status = $aAccountInfo['stw_response_status'];
  $inside_pages = $aAccountInfo['stw_inside_pages'];
  $custom_size = $aAccountInfo['stw_custom_size'];
  $full_length = $aAccountInfo['stw_full_length'];
  $custom_delay = $aAccountInfo['stw_custom_delay'];
  $custom_quality = $aAccountInfo['stw_custom_quality'];
  $custom_resolution = $aAccountInfo['stw_custom_resolution'];
  $custom_messages = $aAccountInfo['stw_custom_messages'];
  
  $form = array();

  $form['shrinktheweb_clearcache'] = array(
    '#type' => 'fieldset',
    '#title' => t('ShrinkTheWeb Clear Cache'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['shrinktheweb_clearcache']['shrinktheweb_clear_imagecache'] = array(
    '#type' => 'checkbox',
    '#title' => t('Clear captured shots'),
    '#default_value' => FALSE,
    '#description' => t('By selecting this checkbox all cached thumbshots gets deleted'),
    '#disabled' => FALSE,
  );
  $form['shrinktheweb_clearcache']['shrinktheweb_clear_errorcache'] = array(
    '#type' => 'checkbox',
    '#title' => t('Clear error images'),
    '#default_value' => FALSE,
    '#description' => t('By selecting this checkbox all cached error images gets deleted'),
    '#disabled' => FALSE,
  );

  $form['shrinktheweb_info'] = array(
    '#type' => 'fieldset',
    '#title' => t('ShrinkTheWeb Information'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['shrinktheweb_info']['shrinktheweb_get_account'] = array(
    '#type' => 'item',
    '#title' => t('Get Account'),
    '#description' => t('You can get your "Access Key" and "Secret Key" by signing up for automated screenshots at: <a target="_blank" href="http://www.shrinktheweb.com/">ShrinkTheWeb</a>'),
  );
  $form['shrinktheweb_info']['shrinktheweb_referrer_list'] = array(
    '#type' => 'item',
    '#title' => t('Referrer List'),
    '#description' => t('By default, you must add your server\'s IP address to <a href="http://www.shrinktheweb.com/content/how-do-i-lock-my-account.html" target="_blank">ShrinkTheWeb\'s "Allowed Referrers" list</a>'),
  );

  $form['shrinktheweb_keys'] = array(
    '#type' => 'fieldset',
    '#title' => t('ShrinkTheWeb API keys'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['shrinktheweb_keys']['shrinktheweb_access_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Access key'),
    '#default_value' => variable_get('shrinktheweb_access_key', ''),
    '#required' => TRUE,
  );
  $form['shrinktheweb_keys']['shrinktheweb_secret_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Secret key'),
    '#default_value' => variable_get('shrinktheweb_secret_key', ''),
    '#required' => TRUE,
  );
  if (variable_get('shrinktheweb_access_key', '') == '' || variable_get('shrinktheweb_secret_key', '') == '' || $response_status != 'Success') {
    $form['shrinktheweb_keys']['shrinktheweb_status'] = array(
      '#type' => 'item',
      '#title' => t('Invalid account credentials detected'),
      '#description' => t('Please enter your account credentials and make sure they are correct'),
    );
  } else {

  $form['shrinktheweb_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('ShrinkTheWeb Options'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['shrinktheweb_options']['shrinktheweb_inside_pages'] = array(
    '#type' => 'checkbox',
    '#title' => t('Inside Page Captures'),
    '#default_value' => $inside_pages == 1 ? TRUE : FALSE,
    '#description' => $inside_pages == 0 ? t('Upgrade required to use this feature') : t('i.e. not just homepages and sub-domains, auto selected if you have purchased this pro package'),
    '#disabled' => TRUE,
  );
  $form['shrinktheweb_options']['shrinktheweb_thumb_size'] = array(
    '#type' => 'select',
    '#title' => t('Default Thumbnail size'),
     '#options' => array(
      'mcr' => t('mcr'),
      'tny' => t('tny'),
      'vsm' => t('vsm'),
      'sm' => t('sm'),
      'lg' => t('lg'),
      'xlg' => t('xlg'),
    ),
    '#default_value' => variable_get('shrinktheweb_thumb_size', 'lg'),
    '#required' => TRUE,
    '#description' => t('width: mcr 75px, tny 90px, vsm 100px, sm 120px, lg 200px, xlg 320px'),
  );
  $form['shrinktheweb_options']['shrinktheweb_thumb_size_custom'] = array(
    '#type' => 'textfield',
    '#title' => t('Custom Width'),
    '#default_value' => $custom_size == 0 && $full_length == 0 ? '' : variable_get('shrinktheweb_thumb_size_custom', ''),
    '#description' => $custom_size == 0 && $full_length == 0 ? t('Upgrade required to use this feature') : t('Enter your custom image width, this will override default size'),
    '#size' => 10,
    '#disabled' => $custom_size == 1 || $full_length == 1 ? FALSE : TRUE,
  );
  $form['shrinktheweb_options']['shrinktheweb_full_size'] = array(
    '#type' => 'checkbox',
    '#title' => t('Full-Length capture'),
    '#default_value' => $full_length == 0 ? FALSE : variable_get('shrinktheweb_full_size', FALSE),
    '#description' => $full_length == 0 ? t('Upgrade required to use this feature') : '',
    '#disabled' => $full_length == 1 ? FALSE : TRUE,
  );
  $form['shrinktheweb_options']['shrinktheweb_max_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Max height'),
    '#default_value' => $full_length == 0 ? '' : variable_get('shrinktheweb_max_height', ''),
    '#description' => $full_length == 0 ? t('Upgrade required to use this feature') : t('use if you want to set maxheight for fullsize capture'),
    '#size' => 10,
    '#disabled' => $full_length == 1 ? FALSE : TRUE,
  );
  $form['shrinktheweb_options']['shrinktheweb_native_res'] = array(
    '#type' => 'textfield',
    '#title' => t('Native resolution'),
    '#default_value' => $custom_resolution == 0 ? '' : variable_get('shrinktheweb_native_res', ''),
    '#description' => $custom_resolution == 0 ? t('Upgrade required to use this feature') : t('i.e. 640 for 640x480'),
    '#size' => 10,
    '#disabled' => $custom_resolution == 1 ? FALSE : TRUE,
  );
  $form['shrinktheweb_options']['shrinktheweb_widescreen_y'] = array(
    '#type' => 'textfield',
    '#title' => t('Widescreen resolution Y'),
    '#default_value' => $custom_resolution == 0 ? '' : variable_get('shrinktheweb_widescreen_y', ''),
    '#description' => $custom_resolution == 0 ? t('Upgrade required to use this feature') : t('i.e. 900 for 1440x900 if 1440 is set for Native resolution'),
    '#size' => 10,
    '#disabled' => $custom_resolution == 1 ? FALSE : TRUE,
  );
  $form['shrinktheweb_options']['shrinktheweb_delay'] = array(
    '#type' => 'textfield',
    '#title' => t('Flash delay'),
    '#default_value' => $custom_delay == 0 ? '' : variable_get('shrinktheweb_delay', ''),
    '#description' => $custom_delay == 0 ? t('Upgrade required to use this feature') : t('max. 45'),
    '#size' => 10,
    '#disabled' => $custom_delay == 1 ? FALSE : TRUE,
  );
  $form['shrinktheweb_options']['shrinktheweb_quality'] = array(
    '#type' => 'textfield',
    '#title' => t('Quality'),
    '#default_value' => $custom_quality == 0 ? '' : variable_get('shrinktheweb_quality', ''),
    '#description' => $custom_quality == 0 ? t('Upgrade required to use this feature') : t('0 .. 100'),
    '#size' => 10,
    '#disabled' => $custom_quality == 1 ? FALSE : TRUE,
  );

  $form['shrinktheweb_adv_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('ShrinkTheWeb Advanced Options'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['shrinktheweb_adv_options']['shrinktheweb_cache_days'] = array(
    '#type' => 'textfield',
    '#title' => t('Cache days'),
    '#default_value' => variable_get('shrinktheweb_cache_days', 3),
    '#description' => t('How many days the images are valid in your cache, Enter 0 (zero) to never update screenshots once cached or -1 to disable caching and always use embedded method instead'),
    '#size' => 10,
  );
  $form['shrinktheweb_adv_options']['shrinktheweb_thumbs_folder'] = array(
    '#type' => 'textfield',
    '#title' => t('Thumbnails folder'),
    '#default_value' => variable_get('shrinktheweb_thumbs_folder', 'shrinktheweb_thumbnails'),
    '#required' => TRUE,
    '#description' => t('This is a subfolder of the "File system path" folder.'),
  );
  $form['shrinktheweb_adv_options']['shrinktheweb_custom_msg_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Custom Messages URL'),
    '#default_value' => $custom_messages == 0 ? '' : variable_get('shrinktheweb_custom_msg_url', ''),
    '#description' => $custom_messages == 0 ? t('Upgrade required to use this feature') : t('specify the URL where your custom message images are stored'),
    '#size' => 10,
    '#disabled' => $custom_messages == 1 ? FALSE : TRUE,
  );
  $form['shrinktheweb_adv_options']['shrinktheweb_debug'] = array(
    '#type' => 'checkbox',
    '#title' => t('Debug'),
    '#default_value' => variable_get('shrinktheweb_debug', FALSE),
    '#description' => t('Store debug info in database'),
  );
  }

  $form['#submit'][] = 'shrinktheweb_admin_settings_submit';
  
  return system_settings_form($form);
}

function shrinktheweb_admin_settings_validate($form, &$form_state) {
  $cache_days = $form_state['values']['shrinktheweb_cache_days'];
  if (!empty($cache_days)) {
    if (!is_numeric($cache_days)) {
      form_set_error('shrinktheweb_cache_days', t('You must enter an integer for days in cache.'));
    } else if ($cache_days < -1) {
      form_set_error('shrinktheweb_cache_days', t('Number of days in cache must be greater then -1.'));
    }
  }

  $thumb_size_custom = $form_state['values']['shrinktheweb_thumb_size_custom'];
  if (!empty($thumb_size_custom)) {
    if (!is_numeric($thumb_size_custom)) {
      form_set_error('shrinktheweb_', t('You must enter an integer for custom thumb size.'));
    } else if ($thumb_size_custom < 0) {
      form_set_error('shrinktheweb_', t('Custom thumb size must be positive.'));
    }
  }
  $max_height = $form_state['values']['shrinktheweb_max_height'];
  if (!empty($max_height)) {
    if (!is_numeric($max_height)) {
      form_set_error('shrinktheweb_', t('You must enter an integer for maximum height.'));
    } else if ($max_height < 0) {
      form_set_error('shrinktheweb_', t('Maximum height must be positive.'));
    }
  }
  $native_res = $form_state['values']['shrinktheweb_native_res'];
  if (!empty($native_res)) {
    if (!is_numeric($native_res)) {
      form_set_error('shrinktheweb_', t('You must enter an integer for native resolution.'));
    } else if ($native_res < 0) {
      form_set_error('shrinktheweb_', t('Native resolution must be positive.'));
    }
  }
  $widescreen_y = $form_state['values']['shrinktheweb_widescreen_y'];
  if (!empty($widescreen_y)) {
    if (!is_numeric($widescreen_y)) {
      form_set_error('shrinktheweb_', t('You must enter an integer for widescreen Y.'));
    } else if ($widescreen_y < 0) {
      form_set_error('shrinktheweb_', t('Widescreen Y must be positive.'));
    }
  }
  $delay = $form_state['values']['shrinktheweb_delay'];
  if (!empty($delay)) {
    if (!is_numeric($delay)) {
      form_set_error('shrinktheweb_', t('You must enter an integer for delay.'));
    } else if ($delay < 0 || $delay > 45) {
      form_set_error('shrinktheweb_', t('Delay must be between 0 and 45.'));
    }
  }
  $quality = $form_state['values']['shrinktheweb_quality'];
  if (!empty($quality)) {
    if (!is_numeric($quality)) {
      form_set_error('shrinktheweb_', t('You must enter an integer for quality.'));
    } else if ($quality < 0 || $quality > 100) {
      form_set_error('shrinktheweb_', t('Quality must be between 0 and 100.'));
    }
  }

}

function shrinktheweb_admin_settings_submit($form, &$form_state) {
  module_load_include('inc', 'shrinktheweb', 'shrinktheweb.api');
  $clear_imagecache = $form_state['values']['shrinktheweb_clear_imagecache'];
  if (!empty($clear_imagecache)) {
    if ($clear_imagecache == TRUE) {
      shrinktheweb_deleteAllImages();
      drupal_set_message('All cached thumbshots got deleted');
    }
  }
  $clear_errorcache = $form_state['values']['shrinktheweb_clear_errorcache'];
  if (!empty($clear_errorcache)) {
    if ($clear_errorcache == TRUE) {
      shrinktheweb_deleteErrorImages();
      drupal_set_message('All cached error images got deleted');
    }
  }
}
