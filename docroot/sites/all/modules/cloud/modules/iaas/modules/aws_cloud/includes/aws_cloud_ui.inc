<?php

/**
 * @file
 * AWS cloud ui functions related to cloud management
 *
 * Copyright (c) 2010-2011 DOCOMO Innovations, Inc.
 *
 */

// Updated by yas 2015/06/15
// Updated by yas 2015/05/08
// Updated by yas 2015/05/07
// Updated by yas 2015/03/26
// Updated by yas 2015/03/25
// fixed a bug (aws_cloud_list) by yas 2013/10/29

/**
 * Listing page for all the clouds in the system
 */
function aws_cloud_list() {

  drupal_add_js(cloud_get_module_base() . 'js/cloud.js', 'module');

  $form = drupal_get_form('aws_cloud_add_button');

  $output  = '';
  $output .=  drupal_render($form);
  $output .=  theme('aws_cloud_list');

  return $output;
}

/**
 * Add cloud button
 */
function aws_cloud_add_button() {

  $form['submit'] = array(
    '#type'   => 'submit',
    '#value'  => t('Add Cloud'),
    '#submit' => array('aws_cloud_add_cloud'),
  );

  return $form;
}

/**
 * Helper function that gets a list of all the
 * subclouds defined and presents an array back
 * to the Add Cloud Form.
 */
function _aws_cloud_base_cloud_options() {

  $data = aws_cloud_get_cloud_data();
  $return = array();

  foreach ($data as $key => $value) {

    $return[$key] = $value['name'];
  }

  return $return;
}

/**
 * Function renders the cloud add/edit form
 */
function aws_cloud_edit($cloud_name = NULL) {

  $output = '';

  if ($cloud_name) {

    $cloud_data = aws_cloud_get_cloud($cloud_name);
    $output = drupal_get_form('aws_cloud_add_form', $cloud_data, 'edit');
  }
  else {

    $edit = array(//No need to put so many variables.
      'base_cloud'                 => '',
      'cloud_name'                 => '',
      'cloud_display_name'         => '',
      'api_version'                => '',
      'host_uri'                   => '',
      'aws_access_key'             => '',
      'aws_secret_key'             => '',
      'user_id'                    => '',
      'image_upload_url'           => '',
      'image_register_url'         => '',
      'certificate'                => '',
      'monitoring_server_url'      => '',
      'monitoring_server_user_id'  => '',
      'monitoring_server_password' => '',
    );

    $output = drupal_get_form('aws_cloud_add_form', $edit);
  }

  return $output;
}

/**
 * Cloud form
 */
function aws_cloud_add_form($form, $form_state, $edit = array(), $op = '') {

  $form['cloud'] = array(
    '#type' => 'fieldset',
    '#description' => 'Use this form to add a new cloud here',
  );

  $form['cloud']['destination'] = array(
    '#type'  => 'hidden',
    '#name'  => 'destination',
    '#value' => 'admin/config/clouds',
  );

  $form['cloud']['submit'] = array(
    '#type'  => 'submit',
    '#value' => ($op == 'edit') ? 'Update' : 'Create',
  );

  if ($op == 'edit') {
    $form['cloud']['delete'] = array(
      '#type'   => 'submit',
      '#value'  => t('Delete'),
      '#submit' => array('aws_cloud_delete_submit'),
    );
    $form['cloud']['list'] = array(
      '#type'   => 'submit',
      '#value'  => t('List Clouds'),
      '#submit' => array('aws_cloud_list_submit'),
    );
  }

  if ($op == 'edit') {

    $form['cloud']['base_cloud_display'] = array(
      '#type'        => 'item',
      '#title'       => t('Base Cloud'),
      '#description' => t('This value cannot be editted'),
      '#options'     => _aws_cloud_base_cloud_options(),
      '#value'       => $edit['base_cloud'],
    );

    $form['cloud']['cloud_name_display'] = array(
      '#type'        => 'item',
      '#title'       => t('Cloud Name'),
      '#description' => t('This value cannot be editted'),
      '#value'       => $edit['cloud_name'],
      //'#disabled'    => ($op == 'edit') ? TRUE: FALSE,
    );
    $form['cloud']['base_cloud'] = array(
      '#type'          => 'value',
      '#title'         => t('Base Cloud'),
      '#description'   => t('Select the cloud type'),
      '#required'      => TRUE,
      '#options'       => _aws_cloud_base_cloud_options(),
      '#default_value' => $edit['base_cloud'],
    );

    $form['cloud']['cloud_name'] = array(
      '#type'          => 'value',
      '#title'         => t('Cloud Name'),
      '#description'   => t('Machine name: e.g. amazon_ec2_east'),
      '#required'      => TRUE,
      '#default_value' => $edit['cloud_name'],
    );
  }
  else {

    $form['cloud']['base_cloud'] = array(
      '#type'          => 'select',
      '#title'         => t('Base Cloud'),
      '#description'   => t('Select the cloud type for pre-defined billing model for hourly rate'),
      '#required'      => TRUE,
      '#options'       => _aws_cloud_base_cloud_options(),
      '#default_value' => $edit['base_cloud'],
    );

    $form['cloud']['cloud_name'] = array(
      '#type'          => 'textfield',
      '#title'         => t('Cloud Name'),
      '#description'   => t('Cloud name as an identifier: e.g. amazon_ec2_us_east | amazon_ec2_us_west_1 | amazon_ec2_us_west_2 | amazon_ec2_eu_west_1 | amazon_ec2_ap_northeast_1 | amazon_ec2_ap_southeast_1 | amazon_ec2_ap_southeast_2 | openstack_nova - The Cloud Name must contain only lowercase letters, numbers, and underscores.  Space cannot be included.'),
      '#required'      => TRUE,
      '#default_value' => $edit['cloud_name'],
    );
  }

  $form['cloud']['cloud_display_name'] = array(
    '#title'       => t('Cloud Display Name'),
    '#type'        => 'textfield',
    '#title'       => t('Cloud Display Name'),
    '#description' => t('e.g. US East (Northern Virginia) Region (or AWS Virgina) |  US West (Northern California) Region (or AWS California) | US West (Oregon) Region (or AWS Oregon) | EU (Ireland) Region (or AWS Ireland) | Asia Pacific (Tokyo) Region (or AWS Tokyo) | Asia Pacific (Singapore) Region (or AWS Singapore) | Asia Pacific (Sydney) Region (or AWS Sydney) | South America (São Paulo) Region (or AWS São Paulo) | OpenStack | Eucalyptus'),
    '#required'    => TRUE,
    '#default_value' => $edit['cloud_display_name'],
  );

  $form['cloud']['api_version'] = array(
    '#title'         => t('API Version'),
    '#type'          => 'textfield',
    '#description'   => t('The API Version of REST API (yyyy-mm-dd)'),
    '#size'          => 15,
    '#required'      => TRUE,
    '#default_value' => $edit['api_version'],
  );

  $form['cloud']['host_uri'] = array(
    '#type'          => 'textfield',
    '#title'         => t('API URI'),
    '#description'   => t("'http://' is NOT required: e.g. ec2.amazonaws.com | ec2.us-west-1.amazonaws.com | ec2.us-west-2.amazonaws.com | ec2.eu-west-1.amazonaws.com | ec2.ap-northeast-1.amazonaws.com | ec2.ap-southeast-1.amazonaws.com | ec2.ap-southeast-2.amazonaws.com | ec2.sa-east-1.amazonaws.com | OpenStack: 192.168.0.1:8773/services/Cloud | Cloud(n) JP: comp-apia.jp-e1.cloudn-service.com/awsapi | Cloud(n) US: comp-apia.us-e1.cloudn-service.com/awsapi"),
    '#size'          => 50,
    '#maxlength'     => 255,
    '#required'      => TRUE,
    '#default_value' => $edit['host_uri'],
  );

  $form['cloud']['aws_access_key'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Access Key'),
    '#description'   => t('16-32 Characters, e.g. 12ABCDEFGHIJKVWXYZ89'),
    '#size'          => 32,
    '#required'      => TRUE,
    '#default_value' => $edit['aws_access_key'],
  );

  $form['cloud']['aws_secret_key'] = array(
    '#type'          => 'textfield',
    '#title'         => t('AWS Secret Key'),
    '#size'          => 55,
    '#maxlength'     => 50,
    '#description'   => t('e.g. 123ABC/defGHIjkl34+LMNopq567RSTuvwxYz89Z'),
    '#required'      => TRUE,
    '#default_value' => $edit['aws_secret_key'],
  );

  $form['cloud']['user_id'] = array(
    '#type'          => 'textfield',
    '#title'         => t('User ID'),
    '#description'   => t('e.g. 123456789012'),
    '#size'          => 15,
    '#required'      => TRUE,
    '#default_value' => $edit['user_id'],
  );

  $form['cloud']['image_upload_url'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Image Upload URL'),
    '#default_value' => '',
    '#description'   => t('e.g. https://s3.amazonaws.com'),
    '#size'          => 50,
    '#maxlength'     => 255,
    '#required'      => TRUE,
    '#default_value' => $edit['image_upload_url'],
  );

  $form['cloud']['image_register_url'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Image Register URL'),
    '#default_value' => '',
    '#description'   => t("'https://' is REQUIRED: e.g. https://ec2.amazonaws.com | https://ec2.us-west-1.amazonaws.com | https://ec2.us-west-2.amazonaws.com | https://ec2.eu-west-1.amazonaws.com | https://ec2.ap-northeast-1.amazonaws.com | https://ec2.ap-southeast-1.amazonaws.com | https://ec2.ap-southeast-2.amazonaws.com | https://ec2.sa-east-1.amazonaws.com"),
    '#size'          => 50,
    '#maxlength'     => 255,
    '#required'      => TRUE,
    '#default_value' => $edit['image_register_url'],
  );

  $form['cloud']['certificate'] = array(
    '#type'          => 'textarea',
    '#title'         => t('X.509 Certificate'),
    '#description'   => t('X.509 Certificate to use. You can temporarily put dummy string if you do not use Bundle Image capability.'),
    '#rows'          => 17,
    '#size'          => 66,
    '#required'      => FALSE,
    '#default_value' => $edit['certificate'],
  );

  if (module_exists('cloud_failover')) {
    $form['cloud']['monitoring_server_url'] = array(
      '#type'          => 'textfield',
      '#title'         => t('Monitoring Server URL'),
      '#default_value' => '',
      '#description'   => t('Monitoring Server URL'),
      '#size'          => 50,
      '#maxlength'     => 255,
      '#required'      => FALSE,
      '#default_value' => $edit['monitoring_server_url'],
    );
  
    $form['cloud']['monitoring_server_user_id'] = array(
      '#type'          => 'textfield',
      '#title'         => t('Monitoring Server UserID'),
      '#default_value' => '',
      '#description'   => t('Monitoring Server UserID'),
      '#size'          => 50,
      '#maxlength'     => 255,
      '#required'      => FALSE,
      '#default_value' => $edit['monitoring_server_user_id'],
    );
  
    $form['cloud']['monitoring_server_password'] = array(
      '#type'          => 'password',
      '#title'         => t('Monitoring Server PWD.'),
      '#default_value' => '',
      '#description'   => t('Monitoring Server Password'),
      '#size'          => 50,
      '#maxlength'     => 255,
      '#required'      => FALSE,
      '#default_value' => $edit['monitoring_server_password'],
    );
  }

  $form['cloud']['submit_2'] = array(
    '#type'  => 'submit',
    '#value' => ($op == 'edit') ? 'Update' : 'Create',
  );

  if ($op == 'edit') {
    $form['cloud']['delete_2'] = array(
      '#type'   => 'submit',
      '#value'  => t('Delete'),
      '#submit' => array('aws_cloud_delete_submit'),
    );
  }

  return $form;
}

/**
 * Make sure the cloud_name is unique
 */
function aws_cloud_add_form_validate($form, &$form_state) {

  if ($form_state['values']['op'] == 'Create') {

    $cloud_name = isset($form_state['values']['cloud_name'])
                ? trim( $form_state['values']['cloud_name'])
                : '';

    if (!preg_match('!^[a-z0-9_]+$!', $cloud_name)) {
      form_set_error('cloud_name', t('The Cloud Name must contain only lowercase letters, numbers, and underscores.'));
    }
    $db_check = cloud_check_cloud_name($cloud_name);
    if (count($db_check)) {

      form_set_error('cloud_name', t('The Cloud Name chosen is not unique'));
    }
    //check to make sure
    $uri_check = aws_cloud_check_host_uri($form_state['values']['host_uri'], $form_state['values']['user_id']);
    if ($uri_check != NULL) {

      form_set_error('host_uri', t('The API URI and User ID combination is in use already.'));
    }
  }

  $form_state['values']['api_version'       ] = trim($form_state['values']['api_version'       ]);
  $form_state['values']['host_uri'          ] = trim($form_state['values']['host_uri'          ]);
  $form_state['values']['aws_access_key'    ] = trim($form_state['values']['aws_access_key'    ]);
  $form_state['values']['aws_secret_key'    ] = trim($form_state['values']['aws_secret_key'    ]);
  $form_state['values']['user_id'           ] = trim($form_state['values']['user_id'           ]);
  $form_state['values']['image_upload_url'  ] = trim($form_state['values']['image_upload_url'  ]);
  $form_state['values']['image_register_url'] = trim($form_state['values']['image_register_url']);
}

/**
 * After a cloud form submit, add it to the database.
 * Call cloud_save() to write the main record.
 */
function aws_cloud_add_form_submit($form, &$form_state) {

  $message      = '';
  $op           = &$form_state['values']['op'                ];
  $display_name =  $form_state['values']['cloud_display_name'];

  // Set the extra data only on create
  if ($op == 'Create') {

    // Get all the extra data from the base cloud
    $extra_data = aws_cloud_get_cloud_data($form_state['values']['base_cloud']);
    $data = array();
    if (isset($extra_data)) {
      $data = array_merge($form_state['values'], $extra_data);
    }
    $data['is_new'] = TRUE;
    $message = t('Cloud @cloud_name has been created.',
                 array(
      '@cloud_name' => $form_state['values']['cloud_name'],
    ));
  }
  else {
    $data = $form_state['values'];
    $data['is_new'] = FALSE;
    $message = t('Cloud @cloud_name has been updated.',
                 array(
      '@cloud_name' => $form_state['values']['cloud_name'],
    ));
  }

  $message .= t('Refresh the data and go to: !display_name',
                                      array(
    '!display_name' => l($display_name, 'clouds/'
                       . $form_state['values']['cloud_name'] . '/get_instances_data',
                         array('query' => array('destination' => 'clouds/'
                                              . $form_state['values']['cloud_name']))),
  ));

  // Take the cloud object, and add extra stuff to it
  $cloud = (object) $data;

  if (!isset($cloud->host_entries_refresh_time)) {

    $cloud->host_entries_refresh_time = CLOUD_HOST_ENTRIES_REFRESH_TIME;
  }

  $cloud->last_update_time = REQUEST_TIME - 10;
  $cloud->module = AWS_CLOUD_MODULE_NAME;

  cloud_save($cloud);
  drupal_set_message($message);
}

function aws_cloud_list_submit($form, &$form_state) {

  $form_state['redirect'] = array(AWS_CLOUD_LIST_PAGE);
}
/**
 * Handler for the "Add" button on the main listing page
 */
function aws_cloud_add_cloud($form, &$form_state) {

  $form_state['redirect'] = array(AWS_CLOUD_ADD_PAGE);
}

/**
 * Confirm the deletion of the cloud
 */
function aws_cloud_delete_submit($form, &$form_state) {

  $destination = '';

  if (isset($_REQUEST['destination'])) {

    $destination = drupal_get_destination();
    unset($_REQUEST['destination']);
  }

  $form_state['redirect'] = array('admin/config/clouds/'
. $form_state['values']['cloud_name'] . '/delete', $destination);
}

/**
 * Confirm delete form
 */
function aws_cloud_confirm_delete($form, &$form_state, $cloud_name) {

  $form['cloud_name'] = array(
    '#type' => 'value',
    '#value' => $cloud_name,
  );

  return confirm_form($form, t('Are you sure you want to delete the cloud %cloud?', array('%cloud' => $cloud_name)),
    'admin/config/clouds/' . $cloud_name,
    t('All related cloud information will be deleted.  This action cannot be undone.'),
    t('Delete'), t('Cancel'));
}

/**
 * Delete the cloud and its data
 */
function aws_cloud_confirm_delete_submit($form, &$form_state) {

  cloud_delete($form_state['values']['cloud_name']);
  drupal_set_message(t('%cloud has been deleted.', array('%cloud' => $form_state['values']['cloud_name'])));
  $form_state['redirect'] = AWS_CLOUD_LIST_PAGE;
}

/**
 * Theme the clouds stored in the database
 */
function theme_aws_cloud_list() {

  $header = array(
    array(
      'data'  => t('Cloud Display Name'),
      'field' => 'cloud_display_name',
      'sort'  => 'asc',
    ),
    array(
      'data'  => t('Cloud Name'        ),
      'field' => 'cloud_name',
    ),
    array(
      'data'  => t('Host Uri'          ),
      'field' => 'host_uri',
    ),
    array(
      'data'  => t('User Id'           ),
      'field' => 'user_id',
    ),
    array(
      'data'  => t('Action'            ),
      'class' => 'action-column',
    ),
  );

  $query  = aws_cloud_get_all_clouds_query();
  $query->extend('PagerDefault')
        ->extend('TableSort')
        ->orderByHeader($header);

  $clouds = $query->execute();

  $rows = array();

  foreach ($clouds as $cloud) {

    $action_data = cloud_display_action('images/edit', t('Edit'), 'admin/config/clouds/'
                 . $cloud->cloud_name . '/edit'  , array('query' => array(), 'html' => TRUE));
    $action_data .= cloud_display_action('images/icon_delete', t('Delete'), 'admin/config/clouds/'
                 . $cloud->cloud_name . '/delete', array('query' => array(), 'html' => TRUE));
    $row =
      array('data' => array(
        l($cloud->cloud_display_name, 'admin/config/clouds/' . $cloud->cloud_name . '/edit'),
          $cloud->cloud_name        ,
          $cloud->host_uri          ,
          $cloud->user_id           ,
          $action_data,
      ));
    $rows[] = $row;
  }

  $output = theme('table', array('header' => $header, 'rows' => $rows) );

  return $output;
}
