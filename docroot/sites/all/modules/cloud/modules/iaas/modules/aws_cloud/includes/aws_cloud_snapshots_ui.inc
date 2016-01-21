<?php

/**
 * @file
 * aws_ec2_lib Snapshot UI
 *
 * Copyright (c) 2010-2011 DOCOMO Innovations, Inc.
 *
 */

/**
 * Updated by yas   2011/02/17
 * Updated by yas   2011/02/14
 * Updated by yas   2011/02/11
 * Updated by yas   2011/02/10
 * Updated by yas   2011/02/02
 */

/**
 * Display list of Snapshots
 *
 * @param     $form_submit
 * @param     $cloud_context
 * @return
 */
function aws_cloud_display_snapshot_list($form_submit = '', $form_state, $cloud_context) {
  drupal_add_js(cloud_get_module_base() . 'js/cloud.js');
  drupal_add_js(drupal_get_path('module', 'aws_cloud')
              . CLOUD_PATH_SEPARATOR
              . 'js/aws_cloud_auto_refresh_list_snapshots.js'
              );

  $cloud_menu_path = cloud_get_menu_path($cloud_context);
  $column  = 'nickname';
  $sql_col = 'c.nickname';

  $options = array(
    t('Nickname'),
    t('ID'      ),
    t('Volume'  ),
    t('Status'  ),
  );


  $filter     = isset($_REQUEST['filter'   ]) ? $_REQUEST['filter'   ] : '';
  $filter_col = isset($_REQUEST['operation']) ? $_REQUEST['operation'] : 0  ; // default: Nickname
  $filter     = trim($filter);

  if ( $filter_col == 0 ) {
    $column  = 'Nickname'    ;
    $sql_col = 'c.nickname'    ;
  }
  elseif ($filter_col == 1 ) {
    $column  = 'AWS-ID'      ;
    $sql_col = 'c.snapshot_id';
  }
  elseif ($filter_col == 2 ) {
    $column  = 'Volume ID'   ;
    $sql_col = 'c.volume_id'   ;
  }
  elseif ($filter_col == 3 ) {
    $column  = 'Status'      ;
    $sql_col = 'c.status'      ;
  }

  $query_args = array();
  if ( isset($filter) ) {

    $query_args[] = $sql_col;
    $query_args[] = $filter  ;
    $query_args[] = $cloud_context;
  }
  else {

    $filter       = ' 1 ';
    $query_args[] = ' ';
    $query_args[] = $cloud_context;
  }

  $form['filter_hdn'] = array(
    '#type' => 'hidden',
    '#value' => $filter,
  );

  $form['operation_hdn'] = array(
    '#type' => 'hidden',
    '#value' => $filter_col,
  );

  $form['cloud_context'] = array(
    '#type' => 'hidden',
    '#value' => $cloud_context,
  );

  $form['options'] = array(
    '#type' => 'fieldset',
    '#prefix' => '<div class="container-inline">',
    '#suffix' => '</div>',
  );

  $snapshot_count = _aws_cloud_get_snapshots_list_count($cloud_context);
  $filter_disabled = '';
  if ( $snapshot_count < 2 ) {
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

  $form['header'] = array(
    '#type' => 'value',
    '#value' => array(
      array(
        'data' => t('Nickname'  ),
        'field' => 'c.nickname',
        'class' => array('nickname-column'),
      ),
      array(
        'data' => t('ID'        ),
        'field' => 'c.snapshot_id',
      ),
      array(
        'data' => t('Volume'    ),
        'field' => 'c.volume_id',
      ),
      array(
        'data' => t('Status'    ),
        'field' => 'c.status',
      ),
      array(
        'data' => t('Created'   ),
        'field' => 'created'          ,
        'sort' => 'desc',
      ),
      array(
        'data' => t('Actions'   ),
        'class' => 'action-column',
      ),
    ),
  );


  $query  = _aws_cloud_get_describe_snapshots_query($cloud_context);
  $query  = $query
      ->condition('c.cloud_type', $cloud_context, '=')
      ->condition($sql_col, '%' . $filter . '%', 'like')
      ->extend('TableSort')
      ->extend('PagerDefault')
      ->orderByHeader($form['header']['#value'])
      ->limit(AWS_CLOUD_PAGER_LIMIT);

  $result = $query->execute();

  foreach ($result as $ss) {

    $form['Nickname'][$ss->snapshot_id] = array('#markup' => l( $ss->nickname, $cloud_menu_path . '/ebs_snapshots/describe', array('query' => array('ss_id' => urlencode($ss->snapshot_id)))));
    $form['AWS_ID'  ][$ss->snapshot_id] = array('#markup' => t( $ss->snapshot_id));

    if ( empty($ss->vol_nickname) === FALSE ) {
      $form['Volume'][$ss->snapshot_id] = array('#markup' => l($ss->vol_nickname, $cloud_menu_path . '/ebs_volumes/describe', array('query' => array('vol_id' => urlencode($ss->volume_id)))));
    }
    else {
      $form['Volume'][$ss->snapshot_id] = array('#markup' => t($ss->volume_id));
    }
    $form['Status'  ][$ss->snapshot_id] = array('#markup' => t($ss->status . ' ' . $ss->progress ));
    $form['Created'][$ss->snapshot_id] = array('#markup' => format_date(strtotime($ss->created), 'short'));
    $form['Actions'][$ss->snapshot_id] = array('#markup' => aws_cloud_snapshot_action($cloud_context, array('snapshot_id' => $ss->snapshot_id)));
  }

  $form['refresh_page'] = array(
    '#type' => 'item',
    '#prefix' => '<div id="link_reload" align="right">',
    '#suffix' => '</div>',
    '#markup' => l(t('- Refresh Page -'), $cloud_menu_path . '/get_snapshots_data', array()),
  );
  $form['pager'] = array('#value' => theme('pager', array('tags' => NULL, 'element' => 0)));
  $form_state['#redirect'] = FALSE;

  return $form;
}


/**
 * Theme function for snapshot listing
 * @param     $form
 * @return
 *
 */
function theme_aws_cloud_display_snapshot_list($form) {
  $form = $form['form'];
  $output = drupal_render($form['options']);
  $cloud_context = $form['cloud_context']['#value'];
  $cloud_menu_path = cloud_get_menu_path($cloud_context);
  $rows = array();
  if (isset($form['Nickname'])) {
    foreach (element_children($form['Nickname']) as $key) {
      $rows[] = array(
        array(
          'data' => drupal_render($form['Nickname'][$key]),
          'class' => array('nickname-column'),
        ),
        drupal_render($form['AWS_ID'][$key]),
        drupal_render($form['Volume'][$key]),
        drupal_render($form['Status'][$key]),
        drupal_render($form['Created'][$key]),
        array(
          'data' => drupal_render($form['Actions'][$key]),
          'class' => 'action-column',
        ),
      );
    }
  }
  $table_attr = array();
  $table_attr['id'] = 'snapshots_list_table';
  $table_attr['autoupdate_url'] = url( $cloud_menu_path . '/callback_get_snapshots_list' );
  $output .= theme('table', array('header' => $form['header']['#value'], 'rows' => $rows, 'attributes' => $table_attr));

  $output .= theme('pager', array('tags' => array()));

  $output .= drupal_render($form['refresh_page']);
  $output .= drupal_render_children($form);
  return $output;
}

function aws_cloud_snapshot_action($cloud_context, $snapshot_info) {
  $action_data = array();
  $cloud_menu_path = cloud_get_menu_path($cloud_context);
  
  if (user_access($cloud_context . ' create volume') ) {
    $action_data[] = cloud_display_action(
      'images/icon_play.png',
      t('create volume'),
      $cloud_menu_path . '/ebs_volumes/create',
      array('query' => array('ss_id' => urlencode($snapshot_info['snapshot_id'])), 'html' => TRUE)
    );
  }
  
  if (user_access($cloud_context . ' delete snapshot') ) {
    $prop['onclick'] = cloud_get_messagebox('Are you sure you want to delete the snapshot "' . $snapshot_info['snapshot_id'] . '" ?');
    $action_data[] = cloud_display_action(
      'images/icon_delete.png',
      t('Delete'),
      $cloud_menu_path . '/ebs_snapshots/delete',
      array('query' => array('ss_id' => urlencode($snapshot_info['snapshot_id'])), 'html' => TRUE),
      $prop['onclick']
    );
    $action_data[] = cloud_display_action( 'images/icon_edit', t('Edit'), $cloud_menu_path . '/ebs_snapshots/describe', array('query' => array('ss_id' => urlencode($snapshot_info['snapshot_id'])), 'html' => TRUE));
  }
  
  $user_actions = module_invoke_all('aws_cloud_snapshot_action_data', $snapshot_info);
  $action_data = array_merge($action_data, $user_actions);
  return implode($action_data);
}

/**
 * AJAX Callback for snapshot listing page 
 */
function _aws_cloud_callback_get_snapshots_list($cloud_context) {
  $form = drupal_get_form('aws_cloud_display_snapshot_list', $cloud_context);
  $output = theme('aws_cloud_display_snapshot_list', $form);
  // Send only the body do not send the headers
  // @todo: Render the form properly, without stripping the rendered output
  $index_start = strrpos( $output, '<tbody>' );
  $index_end   = strrpos( $output, '</tbody>' );
  if ( isset($form['Nickname']) === FALSE || sizeof($form['Nickname']) == 0 ) { // No element present
    $output = 'NULL';
  }
  else {
    $output = substr($output, $index_start, $index_end - $index_start);
    $output .= '</tbody>';
  }
  print drupal_json_encode(array('html' => $output));
  exit();
}

/**
 *
 * @param     $form_id
 * @param     $form_values
 *
 */
function aws_cloud_display_snapshot_list_submit($form_id, &$form_state) {
  $form_values =& $form_state['values'];
  $cloud_context = $form_values['cloud_context'];
  if ($form_values['op'] == t('Apply')) {
    $param_arr = array(
      'filter' => $form_values['filter'],
      'operation' => $form_values['operation'],
    );
    $form_state['redirect'] = array(current_path(), array('query' => $param_arr));
  }
}

/**
 * Delete a Snapshot
 *
 * @param  $cloud_context
 * @return
 */
function aws_cloud_delete_snapshot($cloud_context) {

  $ss_id = isset( $_REQUEST['ss_id'] ) ? $_REQUEST['ss_id'] : '';
  $result = _aws_cloud_delete_snapshot($cloud_context, $ss_id);

  if ($result) {
    drupal_set_message(t('Deleting Snapshot: @snapshot_id. This may take some time.', array('@snapshot_id' => $ss_id)));
  }
  else {
    drupal_set_message(t('Unable to delete the Snapshot: @snapshot_id', array('@snapshot_id' => $ss_id)), 'error');
  }

  $cloud_menu_path = cloud_get_menu_path($cloud_context );
  drupal_goto($cloud_menu_path . '/ebs_snapshots');
}

/**
 * Display Snapshot details info
 *
 * @param     $form_submit
 * @param     $cloud_context
 * @return
 *
 */
function aws_cloud_display_snapshot_info($form, $form_submit = '', $cloud_context) {

  $ss_id = isset( $_REQUEST['ss_id'] ) ? $_REQUEST['ss_id'] : '';
  $cloud_menu_path = cloud_get_menu_path($cloud_context );

  if (empty($ss_id) ) {
    drupal_goto( $cloud_menu_path . '/ebs_snapshots' );
    return;
  }

  $form['fieldset_snapshot_info'] = array(
    '#type' => 'fieldset',
    '#title' => t('Snapshot Info'),
  );

  $form['ss_id'] = array(
    '#type' => 'hidden',
    '#value' => $ss_id,
  );
  $form['cloud_context'] = array(
    '#type' => 'hidden',
    '#value' => $cloud_context,
  );

  $snapshot_data = _aws_cloud_get_snapshots_by_id_db( $cloud_context, $ss_id );

  $form['ss_name_label'] = array(
    '#type' => 'item',
    '#title' => t('Nickname'),
  );
  $ss_name = $snapshot_data['snap_nickname'];
  $form['ss_name_details'] = array(
    '#prefix' => '<div class="container-inline">',
    '#suffix' => '</div>',
  );

  if (user_access($cloud_context . ' create snapshot') ) {

    $form['ss_name_details']['ss_name_text'] = array(
      '#type' => 'textfield',
      '#default_value' => $ss_name,
    );

    $form['ss_name_details']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Update'),
    );
  }
  else {

    $form['ss_name_details']['ss_name_text'] = array(
      '#type' => 'textfield',
      '#default_value' => $ss_name,
      '#disabled' => TRUE,
    );
  }

  $form['ss_label'] = array(
    '#type' => 'item',
    '#title' => t('ID'),
  );
  $form['ss_text'] = array(
      '#markup' => t( $snapshot_data['snapshot_id'] ),
    );

  $form['vol_label']         = array(
    '#type' => 'item',
    '#title' => t('Volume'),
  );
  $vol_nickname = $snapshot_data['vol_nickname'];
  $vol_id = $snapshot_data['snap_volume_id'];
  if ( empty($vol_nickname) ) {

    $vol_nickname     = $vol_id;
    $form['vol_text'] = array('#markup' => t( $vol_nickname ));
  }
  else {

    $form['vol_text']  =  array(
      array(
        '#markup' => l($vol_nickname,
                    $cloud_menu_path . '/ebs_volumes/describe',
                    array('query' => array('vol_id' => urlencode($vol_id)))),
      ),
    );
  }

  $form['ss_vol_size_label'] = array(
    '#type' => 'item',
    '#title' => t('Volume Size'),
  );
  $form['ss_vol_size_text'] = array(
      '#markup' => t( $snapshot_data['vol_size'] ),
    );

  $form['ss_status_label'] = array(
    '#type' => 'item',
    '#title' => t('Status'),
  );
  $form['ss_status_text'] = array(
      '#markup' => t( $snapshot_data['snap_status'] ),
    );

  $form['ss_created_label'] = array(
    '#type' => 'item',
    '#title' => t('Created'),
  );
  $form['ss_created_text'] = array(
      '#markup' => format_date(strtotime($snapshot_data['snap_created']), 'short'),
    );

  $form['submit_buttons'] = array(
    '#type' => 'fieldset',
    '#prefix' => '<span class="clear"></span><div class="container-inline"><div class="buttons">',
    '#suffix' => '</div></div>',
  );

  // Delete Button
  if (user_access($cloud_context . ' create volume') ) {
    $form['submit_buttons']['create'      ] = array(
      '#type' => 'submit',
      '#value' => t('Create Volume'),
    );
  }

  // Delete Button
  if (user_access($cloud_context . ' delete volume') ) {
    $confirm_msg = cloud_get_messagebox('Are you sure you want to delete the Snapshot "' . $ss_name . '" ?');
    $form['submit_buttons']['delete'      ] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#attributes' => array('onclick' => $confirm_msg),
    );
  }

  $form['submit_buttons']['snapshots_list'] = array(
    '#type' => 'submit',
    '#value' => t('List Snapshots'),
  );

  return $form;
}


/**
 * Theme snapshot details form
 * @param     $form
 * @return
 *
 */
function theme_aws_cloud_display_snapshot_info($form) {
  
  $form = $form['form'];

  $cloud_context = $form['cloud_context']['#value'];

  $rows = array(
    array(
      drupal_render($form['ss_name_label'                  ]),
      drupal_render($form['ss_name_details']                )
    . drupal_render($form['ss_name_details']['ss_name_text'])
    . drupal_render($form['ss_name_details']['submit'      ]),
    ),
    array(
      drupal_render($form['ss_label'        ]),
      drupal_render($form['ss_text'         ]),
    ),
    array(
      drupal_render($form['vol_label'       ]),
      drupal_render($form['vol_text'        ]),
    ),
    array(
      drupal_render($form['ss_vol_size_label']),
      drupal_render($form['ss_vol_size_text'  ]),
    ),
    array(
      drupal_render($form['ss_status_label']),
      drupal_render($form['ss_status_text'  ]),
    ),
    array(
      drupal_render($form['ss_created_label']),
      drupal_render($form['ss_created_text']),
    ),
  );

  $table = theme('table', array('header' => NULL, 'rows' => $rows));
  $form['fieldset_snapshot_info']['#children'] = $table;

  $output  = drupal_render($form['submit_buttons'        ]);
  $output .= drupal_render($form['fieldset_snapshot_info']);
  $output .= drupal_render($form['cloud_context'         ]);
  $output .= drupal_render_children($form);

  return $output;
}


/**
 *
 * @param     $form_id
 * @param     $form_values
 * @return
 *
 */
function aws_cloud_display_snapshot_info_validate($form_id, $form_values) {
  $form_values = $form_values['values'];
  if ( $form_values['op'] == t('Update') ) {
    if ( empty( $form_values['ss_name_text']) == TRUE) {
      form_set_error('ss_name_text', t('Please specify Snapshot Nickname'));
    }
  }
}



/**
 * Snapshot form submit
 * @param     $form_id
 * @param     $form_values
 * @return
 *
 */
function aws_cloud_display_snapshot_info_submit($form_id, &$form_state) {

  $form_values     = $form_state['values'];
  $cloud_context   = $form_values['cloud_context'];
  $cloud_menu_path = cloud_get_menu_path($cloud_context );

  if ($form_values['op'] == t('Create Volume')) {
    $form_state['redirect'] = url($cloud_menu_path . '/ebs_volumes/create', array('query' => array('ss_id' => $form_values['ss_id']), 'absolute' => TRUE));
  }
  elseif ($form_values['op'] == t('Update')) {
    _aws_cloud_snapshot_update($cloud_context, $form_values['ss_id'], 'nickname', $form_values['ss_name_text']);

    // User Activity Log
    cloud_audit_user_activity( 
      array(
        'type' => 'user_activity',
        'message' => t('Snapshot has been modified: @snapshot_id', array('@snapshot_id' => $form_values['ss_id'])),
        'link' => '',
      )
    );

    drupal_set_message(t('Updated nickname: @snapshot_nickname (@snapshot_id)', array(
      '@snapshot_nickname' => $form_values['ss_name_text'],
      '@snapshot_id' => $form_values['ss_id'],
    )));

    $form_state['redirect'] = $cloud_menu_path . '/ebs_snapshots';
  }
  elseif ($form_values['op'] == t('List Snapshots')) {
    $form_state['redirect'] = $cloud_menu_path . '/ebs_snapshots';
  }
  elseif ($form_values['op'] == t('Delete')) {
    $form_state['redirect'] = url($cloud_menu_path . '/ebs_snapshots/delete', array('query' => array('ss_id' => $form_values['ss_id']), 'absolute' => TRUE));
  }
}
