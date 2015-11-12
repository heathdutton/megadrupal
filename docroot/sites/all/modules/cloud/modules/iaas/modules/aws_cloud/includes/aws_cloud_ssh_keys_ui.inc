<?php

/**
 * @file
 * Functions for SSH Key UI
 *
 * Copyright (c) 2010-2011 DOCOMO Innovations, Inc.
 *
 */

/**
 * Updated by yas   2011/02/14
 * Updated by yss   2011/02/11
 * Updated by yas   2011/02/08
 * Updated by yas   2011/02/04
 * Updated by yas   2011/02/02
 */

/**
 * Display ssh key details form
 * @param     $form_submit
 * @param     $cloud_context
 * @return
 */
function aws_cloud_display_ssh_key($form, $form_submit = '', $cloud_context) {

  $key_name = isset( $_REQUEST['key_name'] ) ? $_REQUEST['key_name'] : FALSE;

  $form['cloud_context'] = array(
    '#type' => 'hidden',
    '#value' => $cloud_context,
  );

  $form['fieldset_ssh_key_info'] = array(
    '#type' => 'fieldset',
    '#title' => t('Details'),
  );

  $query        = aws_cloud_get_ssh_keys_query($cloud_context);
  $query_args   = array(
    ':key_name' => $key_name,
    ':cloud_type' => $cloud_context,
  );


  $result = db_query( $query, $query_args );

  foreach ($result as $key) {

    $form['keyname_details']['keyname_label'] = array(
      '#type' => 'item',
      '#title' => t('Key Pair Name'),
    );
    if (user_access($cloud_context . ' list key names')) {
      $form['keyname_details']['keyname_text'] = array('#markup' => check_plain($key_name));
    }
    else {
      $form['keyname_details']['keyname_text'] = array('#markup' => '-');
    }

    $form['keyname_details']['key_fingerprint_label'] = array(
      '#type' => 'item',
      '#title' => t('Fingerprint'),
    );
    if (user_access($cloud_context . ' list key fingerprints')) {
      $form['keyname_details']['key_fingerprint_text'] = array('#markup' => t($key->fingerprint));
    }
    else {
      $form['keyname_details']['key_fingerprint_text'] = array('#markup' => '');
    }

    $form['keyname_details']['KeyMaterial_label'] = array(
      '#type' => 'item',
      '#title' => t('Private key'),
    );
    if (user_access($cloud_context . ' update key')) {
      $form['keyname_details']['key_material_text'] = array(
        '#type' => 'textarea',
        '#default_value' => $key->key_material,
        '#rows' => 40,
      );
    }
    else {
      $form['keyname_details']['key_material_text'] = array(
        '#type' => 'textarea',
        '#default_value' => '',
        '#rows' => 40,
        '#disabled' => TRUE,
      );
    }

    $form['ssh_key'] = array(
      '#type' => 'hidden',
      '#value' => $key_name,
    );

    $form['submit_buttons'] = array(
      '#type' => 'fieldset',
      '#prefix' => '<span class="clear"></span><div class="container-inline"><div class="buttons">',
      '#suffix' => '</div></div>',
    );

    if (user_access($cloud_context . ' update key')) {

      $form['submit_buttons']['Update'] = array(
        '#type' => 'submit',
        '#value' => t('Update'),
      );
    }

    break;
  }

  $cloud_menu_path = cloud_get_menu_path($cloud_context );

  if (isset($form['ssh_key']) === FALSE  ) { // Key not found.
    drupal_set_message(  check_plain( t('Key not found: @SSH_key.', array('@SSH_key' => $key_name) ) ), 'error' );
    drupal_goto($cloud_menu_path . '/ssh_keys');
  }

  if (user_access($cloud_context . ' delete key')) {
    $onclick = cloud_get_messagebox('Are you sure you want to delete the Key Pair "' . $key_name . '" ?');
    $form['submit_buttons']['Delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#attributes' => array('onclick' => $onclick),
    );
  }

  $form['submit_buttons']['Cancel'] = array(
    '#type' => 'submit',
    '#value' => t('List SSH Keys'),
  );

  return $form;
}


/**
 * Theme function for ssh key
 * @param     $form
 * @return
 */
function theme_aws_cloud_display_ssh_key($form) {
  $form = $form['form'];

  $rows = array(
    array(
      drupal_render($form['keyname_details']['keyname_label'       ]),
      drupal_render($form['keyname_details']['keyname_text'        ]),
    ),
    array(
      drupal_render($form['keyname_details']['key_fingerprint_label']),
      drupal_render($form['keyname_details']['key_fingerprint_text']),
    ),
    array(
      drupal_render($form['keyname_details']['KeyMaterial_label'   ]),
      drupal_render($form['keyname_details']['key_material_text'    ]),
    ),
  );

  $table = theme('table', array('header' => NULL, 'rows' => $rows));
  $form['fieldset_ssh_key_info']['#children'] = $table;

  $output  = drupal_render($form['submit_buttons'       ]);
  $output .= drupal_render($form['fieldset_ssh_key_info']);
  $output .= drupal_render($form['ssh_key'              ]);
  $output .= drupal_render($form['cloud_context'        ]);
  $output .= drupal_render_children($form);

  return $output;
}


/**
 * Function for ssh key form submit
 * @param     $form_id
 * @param     $form_values
 * @return
 */
function aws_cloud_display_ssh_key_submit($form_id, &$form_state) {

  $form_values   = $form_state['values'       ];
  $cloud_context = $form_values['cloud_context'];
  $ssh_key = $form_values['ssh_key'];
  $cloud_menu_path = cloud_get_menu_path($cloud_context );
  
  if ($form_values['op'] == t('Update')) {
    $key_material = $form_values['key_material_text'];
    _aws_cloud_update_key_material($cloud_context, $ssh_key, $key_material);
    drupal_set_message(  check_plain( t('Updated SSH key: @SSH_key', array('@SSH_key' => $ssh_key) ) ) );
    $form_state['redirect'] = $cloud_menu_path . '/ssh_keys';
  }
  elseif ($form_values['op'] == t('List SSH Keys')) {
    $form_state['redirect'] = $cloud_menu_path . '/ssh_keys';
  }
  elseif ($form_values['op'] == t('Delete')) {
    $form_state['redirect'] = url($cloud_menu_path . '/ssh_keys/delete', array('query' => array('key_name' => $ssh_key), 'absolute' => TRUE));
  }
}


/**
 * Delete SSH Key
 *
 * @param     $cloud_context
 * @return
 */
function aws_cloud_delete_ssh_key($cloud_context) {

  $key_name = isset( $_REQUEST['key_name'] ) ? $_REQUEST['key_name'] : FALSE;
  $result = _aws_cloud_delete_ssh_key($cloud_context, $key_name);
  if ($result) {
    drupal_set_message(t('Key Pair deleted successfully: @key_name', array('@key_name' => $key_name)));
  }
  else {
    drupal_set_message(t('Unable to delete the Key Pair: @key_name', array('@key_name' => $key_name)), 'error');
  }

  $cloud_menu_path = cloud_get_menu_path($cloud_context);
  drupal_goto($cloud_menu_path . '/ssh_keys');
}


/**
 * Create SSH Key
 *
 * @param     $form_submit
 * @param     $cloud_context
 * @return
 *
 */
function aws_cloud_create_ssh_key($form = '', $form_submit, $cloud_context) {
  $form['cloud_context'] = array(
    '#type' => 'hidden',
    '#value' => $cloud_context,
  );

  $form['fieldset_keyname_details'] = array('#type' => 'fieldset');
  $form['fieldset_keyname_details']['keyname_label'] = array(
    '#type' => 'item',
    '#title' => t('Key Pair Name'),
    '#required' => TRUE,
  );
  $form['fieldset_keyname_details']['keyname_text'] = array('#type' => 'textfield');

  $form['submit_buttons'] = array(
    '#type' => 'fieldset',
    '#prefix' => '<span class="clear"></span><div class="container-inline"><div class="buttons">',
    '#suffix' => '</div></div>',
  );

  $form['submit_buttons']['Create'] = array(
    '#type' => 'submit',
    '#value' => t('Create'),
  );
  $form['submit_buttons']['Cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
  );

  return $form;
}


/**
 * Theme ssh key form
 * @param     $form
 * @return
 */
function theme_aws_cloud_create_ssh_key($form) {
  $form = $form['form'];

  $output  = drupal_render($form['fieldset_keyname_details']);
  $output .= drupal_render($form['cloud_context'           ]);
  $output .= drupal_render($form['submit_buttons'          ]);
  $output .= drupal_render_children($form);

  return;
}


/**
 * Validate function for ssh keys
 * @param     $form_id
 * @param     $form_values
 * @return
 */
function aws_cloud_create_ssh_key_validate($form_id, $form_values) {
  $form_values = $form_values['values'];
  if ($form_values['op'] == t('Create')) {
    if (empty($form_values['keyname_text'])) {
      form_set_error('keyname_text', t('You must enter valid Key Pair Name'));
    }
  }
}


/**
 * Form submit for ssh keys
 * @param     $form_id
 * @param     $form_values
 * @return
 */
function aws_cloud_create_ssh_key_submit($form_id, &$form_state) {

  $form_values     = $form_state['values'       ];
  $cloud_context   = $form_values['cloud_context'];
  $cloud_menu_path = cloud_get_menu_path($cloud_context );

  if ($form_values['op'] == t('Create')) {
    $result = _aws_cloud_create_ssh_key($cloud_context, $form_values['keyname_text']);
    if ($result) {
      drupal_set_message(t('Created a Key Pair Successfully.'));
    }
    else {
      drupal_set_message(t('There was a problem while creating Key Pair'), 'error');
    }
  }
  $form_state['redirect'] = $cloud_menu_path . '/ssh_keys';
}

/**
 * Display SSH Key List
 *
 * @global    $user
 * @param     $form_submit
 * @param     $cloud_context
 * @return
 */
function aws_cloud_display_ssh_keys_list($form_submit = '', $form_state, $cloud_context) {
  
  drupal_add_js(cloud_get_module_base() . 'js/cloud.js');

  $options   = array(
    t('Key Pair Name'),
    t('Fingerprint'  ),
  );

  $query_args = array();

  $filter     = cloud_get_filter_value( $form_submit, 'filter'   );
  $filter     = trim($filter);

  $filter     = isset($_REQUEST['filter'   ]) ? $_REQUEST['filter'   ] : '';
  $filter_col = isset($_REQUEST['operation']) ? $_REQUEST['operation'] : 0  ; // default: Key Name
  $filter     = trim($filter);

  if ($filter_col == 0) {
    $column  = 'Key Name'    ;
    $sql_col = 'key_name'    ;
  }
  elseif ($filter_col == 1) {
    $column  = 'Fingerprint';
    $sql_col = 'fingerprint';
  }

  if (isset($filter)) {
    $query_args[] = $sql_col;
    $query_args[] = $filter  ;
    $query_args[] = $cloud_context;
  }
  else {
    $filter = ' 1 '          ;
    $query_args[] = ' '      ;
    $query_args[] = $cloud_context;
  }

  $form['options'] = array(
    '#type' => 'fieldset',
    '#prefix' => '<div class="container-inline">',
    '#suffix' => '</div>',
  );

  $ssh_key_count = _aws_cloud_get_ssh_keys_list_count($cloud_context);
  $filter_disabled = '';
  if ( $ssh_key_count < 2 ) {
    $filter_disabled = TRUE;
  }

  $form['options']['label'    ] = array(
    '#type' => 'item'     ,
    '#title' => t('Filter'),
  );
  $form['options']['operation'] = array(
    '#type' => 'select'   ,
    '#options' => $options,
    '#default_value' => $filter_col,
    '#disabled' => $filter_disabled,
  );
  $form['options']['filter'   ] = array(
    '#type' => 'textfield',
    '#size' => 40      ,
    '#default_value' => $filter    ,
    '#disabled' => $filter_disabled,
  );
  $form['options']['submit'   ] = array(
    '#type' => 'submit'   ,
    '#value' => t('Apply')                               ,
    '#disabled' => $filter_disabled,
  );

  if (user_access($cloud_context . ' register key')) {

    $form['options']['new_key'] = array(
      '#type' => 'submit',
      '#value' => t('Create'),
    );
  }

  $form['header'] = array(
    '#type' => 'value',
    '#value' => array(
      array(
        'data' => t('Key Pair Name'  ),
        'field' => 'key_name'         ,
        'sort' => 'asc'              ,
        'class' => array('nickname-column'),
      ),
      array(
        'data' => t('Key Fingerprint'),
        'field' => 'fingerprint',
      ),
      array(
        'data' => t('Key Material'   ),
        'field' => 'key_material',
      ),
      array(
        'data' => t('Actions'        ),
        'class' => 'action-column',
      ),
    ),
  );

  $query    = _aws_cloud_get_describe_ssh_keys_query($cloud_context);

  $query
      ->condition('cloud_type', $cloud_context, '=')
      ->condition($sql_col, '%' . $filter . '%', 'like')
      ->extend('TableSort')
      ->orderByHeader($form['header']['#value']);

  $result = $query->execute();

  $cloud_menu_path = cloud_get_menu_path($cloud_context);

  foreach ($result as $key) {
    if (user_access($cloud_context . ' list key names')) {
      if (user_access($cloud_context . ' list key fingerprints')) {
        $form['keyname'][$key->key_name] = array('#markup' => l($key->key_name, $cloud_menu_path . '/ssh_keys/describe', array('query' => array('key_name' => $key->key_name))));
      }
      else {
        $form['keyname'][$key->key_name] = array('#markup' => check_plain($key->key_name));
      }
    }
    else {
      $form['keyname'][$key->key_name] = array('#markup' => '-');
    }

    if (user_access($cloud_context . ' list key fingerprints')) {
      $form['key_fingerprint'][$key->key_name] = array('#markup' => t($key->fingerprint));

      $key_material = $key->key_material;
      if (isset($key_material) && strlen($key_material) > 0) {
        $form['key_material'][$key->key_name] = array('#markup' => t('yes'));
      }
      else {
        $form['key_material'][$key->key_name] = array('#markup' => t('no'));
      }
    }
    else {
      $form['key_fingerprint'][$key->key_name] = array('#markup' => '-');
      $form['key_material'][$key->key_name] = array('#markup' => '-');
    }
    $form['Actions'][$key->key_name] = array('#type' => 'markup', '#markup' => aws_cloud_ssh_keys_action($cloud_context, array('key_name' => $key->key_name)));
  }

  $form['cloud_context'] = array(
    '#type' => 'hidden',
    '#value' => $cloud_context,
  );
  $form['refresh_page'] = array(
    '#type' => 'item',
    '#prefix' => '<div id="link_reload" align="right">',
    '#suffix' => '</div>',
    '#markup' => l( t('- Refresh Page -'), $cloud_menu_path . '/get_ssh_keys_data', array()),
  );
  $form['pager'        ] = array('#value' => theme('pager', array('tags' => NULL, 'element' => 0)));
  $form['#redirect'    ]  = FALSE;

  return $form;
}


/**
 * Theme function for ssh key listing page
 */
function theme_aws_cloud_display_ssh_keys_list($form) {
  $output = '';
  $form = $form['form'];
  $cloud_context = $form['cloud_context']['#value'];

  if (user_access($cloud_context . ' register key')) {
    $output .= drupal_render($form['new_key']);
  }

  $output .= drupal_render($form['options']);
  $cloud_menu_path = cloud_get_menu_path($cloud_context );

  $rows = array();
  if (isset($form['keyname'])) {
    foreach (element_children($form['keyname']) as $key) {
      $rows[] = array(
        array('data' => drupal_render($form['keyname'][$key]), 'class' => array('nickname-column')),
        drupal_render($form['key_fingerprint'][$key]),
        drupal_render($form['key_material'][$key]),
        array('data' => drupal_render($form['Actions'][$key]), 'class' => 'action-column'),
      );
    }
  }
  $output .= theme('table', array('header' => $form['header']['#value'], 'rows' => $rows));

  if ($form['pager']['#value']) {
    $output .= drupal_render($form['pager']);
  }
  
  $output .= drupal_render($form['refresh_page']);
  $output .= drupal_render_children($form);

  return $output;
}

/**
 * Function for rendering ssh key actions 
 */
function aws_cloud_ssh_keys_action($cloud_context, $key_info) {
  $cloud_menu_path = cloud_get_menu_path($cloud_context);
  $action_data = array();
  
  if (user_access($cloud_context . ' delete key')) {
    $prop['onclick'] = cloud_get_messagebox('Are you sure you want to delete the Key Pair "' . $key_info['key_name'] . '" ?');
    $action_data[] = cloud_display_action('images/icon_delete', t('Delete'), $cloud_menu_path . '/ssh_keys/delete', array('query' => array('key_name' => $key_info['key_name']), 'html' => TRUE), $prop['onclick']); //,   'key_name=' . urlencode($key) , $prop );
  }
  
  if (user_access($cloud_context . ' update key')) {
      $action_data[] = cloud_display_action( 'images/icon_edit', t('Edit'), $cloud_menu_path . '/ssh_keys/describe', array('query' => array('key_name' => $key_info['key_name']), 'html' => TRUE));
  }
  
  $user_actions = module_invoke_all('aws_cloud_ssh_key_action_data', $key_info);
  $action_data = array_merge($action_data, $user_actions);
  return implode($action_data);
}

/**
 * SSH Key list submit function
 * @param     $form_id
 * @param     $form_values
 * @return
 */
function aws_cloud_display_ssh_keys_list_submit($form_id, &$form_state) {
  $form_values     =& $form_state['values'];
  $cloud_menu_path = cloud_get_menu_path($form_values['cloud_context']);
  
  if ($form_values['op'] == t('Create')) {
    $form_state['redirect'] = $cloud_menu_path . '/ssh_keys/create';
  }
  elseif ( $form_values['op'] == t('Apply') ) {
    $param_arr = array(
      'filter' => $form_values['filter'],
      'operation' => $form_values['operation'],
    );
    $form_state['redirect'] = array(current_path(), array('query' => $param_arr));
  }
}

/**
 * Check that a user has a key
 * @param     $cloud_context
 * @param     $uname
 * @return
 */
function aws_cloud_ssh_keys_check_load($cloud_context, $uname) {

  /*
   // Enable this only in Production environment.
   return ;
   */
  $is_cloud_enabled = cloud_is_settings_done($cloud_context);
  if ($is_cloud_enabled === FALSE ) {
    // The cloud is not enabled so Skip key check
    return;
  }

  $query        = aws_cloud_get_ssh_keys_query($cloud_context);
  $query_args   = array(
    ':key_name' => $uname,
    ':cloud_type' => $cloud_context,
  );

  $result = db_query($query, $query_args);

  if ($result->rowCount() == 0) { 
    // Record does not exists create the Key
    _aws_cloud_create_ssh_key($cloud_context, $uname);
  }
  else { 
    // Check if it exists in the user keys table
    $query = _aws_cloud_get_user_keys_query($cloud_context);
    $query_args = array(
      ':key_name' => $uname,
      ':cloud_type' => $cloud_context,
    );
    $result = db_query($query, $query_args);
    if ($result->rowCount() == 0) {
      _aws_cloud_add_user_key($cloud_context, $uname);
    }
  }
  return TRUE;
}

/**
 * Function fetches key material from database and returns it.
 * This function will just exit because its more of an api call.
 * It does not need to return and have Drupal markup anything.
 */
function aws_cloud_get_key_material($cloud_context, $key_name) {
  $output = '';

  $query_args   = array(
    ':key_name' => $key_name,
    ':cloud_type' => $cloud_context,
  );
  $result = db_query(aws_cloud_get_ssh_keys_query($cloud_context), $query_args)->fetchAssoc();

  $keys = $result;
  if ($keys != FALSE) {
    $output = $keys['key_material'];
  }
  print $output;
  exit;
}

