<?php

/**
 * @file
 * Failover inc file
 */
module_load_include('inc', 'cloud_failover', 'cloud_failover_db'       );

/**
 * Failover listing
 * @param type $form_submit
 * @return boolean
 */
function cloud_failover_scenario_list($form_submit, $form_state) {

  drupal_add_js(cloud_get_module_base() . 'js/cloud.js');

  $filter     = isset($_REQUEST['filter'   ]) ? $_REQUEST['filter'   ] : '';
  $filter_col = isset($_REQUEST['operation']) ? $_REQUEST['operation'] : 0  ; // default: Nickname
  $filter     = trim($filter);
  
  $form['options'] = array(
    '#type' => 'fieldset',
    '#prefix' => '<div class="container-inline">',
    '#suffix' => '</div>',
  );

  $options = array();
  $options['nickname'    ] = t('Name');
  $options['action_desc'] = t('Failover Action');
  $options['command_desc'] = t('Failover Command');
  $options['user_name'   ] = t('User');
  
  if ($filter_col == 'nickname' ) {
    $column  = 'Nickname';
    $sql_col = 'a.nickname';
  }
  elseif ( $filter_col == 'action_desc' ) {
    $column = 'Failover Action';
    $sql_col = 'c.description';
  }
  elseif ( $filter_col == 'command_desc' ) {
    $column = 'Failover Command';
    $sql_col = 'b.description';
  }
  elseif ( $filter_col == 'user_name' ) {
    $column = 'User';
    $sql_col = 'd.name';
  }

  if ( isset($filter) ) {
    $query_args[] = $sql_col;
    $query_args[] = $filter  ;
  }
  else {
    $filter = ' 1 ';
    $query_args[] = ' ';
  }

  $form = array();
  $form['options'] = array(
    '#prefix' => '<div class="container-inline">',
    '#type' => 'fieldset',
    '#suffix' => '</div>',
  );

  $scenario_count = _cloud_failover_get_count_db();
  $filter_disabled = '';
  if ( $scenario_count < 2 ) {

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
  if (user_access('create failover scenario') ) {
    $form['options']['cloud_failover'] = array(
      '#type' => 'submit',
      '#value' => t('Create Scenario'),
    );
  }


  $form['header'] = array(
    '#type' => 'value',
    '#value' => array(
      array(
        'data' => t('Nickname'),
        'field' => 'nickname',
        'sort' => 'asc',
        'class' => array('nickname-column'),
      ),
      array(
        'data' => t('Check Interval'),
        'field' => 'check_interval',
      ),
      array(
        'data' => t('Retry Interval'),
        'field' => 'retry_interval',
      ),
      array(
        'data' => t('Max Check Attempts'),
        'field' => 'max_check_attempts',
      ),
      array(
        'data' => t('User'),
        'field' => 'd.name',
      ),
      array(
        'data' => t('Failover Command'),
        'field' => 'b.description',
      ),
      array(
        'data' => t('Failover Action'),
        'field' => 'c.description',
      ),
      array('data' => t('Action'), 'class' => 'action-column'),
    ),
  );

  $sorting_request = isset($_REQUEST['order']) ? filter_xss($_REQUEST['order']) : '';
  $sorting_order   = isset($_REQUEST['sort']) ? filter_xss($_REQUEST['sort']) : 'ASC';

  $header_sorting = $form['header']['#value'];

  $table_sort = FALSE;
  foreach ($header_sorting as $value) {
    if ($value['data'] == $sorting_request) {
      
      $table_sort = " ORDER BY " . $value['field'] . " " . $sorting_order;
    }
  }

  $query = _cloud_failover_get_scenarios($sql_col, $filter, $table_sort);

  $result = db_query($query) ;
  $destination = drupal_get_destination();

  foreach ($result as $failover_obj) {

    $fsid = $failover_obj->fsid;
    $form['Name'                  ][$fsid] = array(array('#markup' => l( $failover_obj->nickname, CLOUD_FAILOVER_PATH . '/view',   array('query' => array('id' => urlencode($fsid))) ))); //,  array(),   'id=' . urlencode($fsid) ,  NULL,  FALSE,  TRUE ) ) );
    $form['hdnName'               ][$fsid] = array(
      '#type' => 'hidden',
      '#value' => addslashes($failover_obj->nickname),
    );
    $form['check_interval'        ][$fsid] = array(array('#markup' => t( (int) $failover_obj->check_interval )));
    $form['retry_interval'        ][$fsid] = array(array('#markup' => t( (int) $failover_obj->retry_interval )));
    $form['max_check_attempts'    ][$fsid] = array(array('#markup' => t( (int) $failover_obj->max_check_attempts )));
    $form['user'                  ][$fsid] = array(array('#markup' => t( $failover_obj->user_name )));
    $form['cloud_failover_command'][$fsid] = array(array('#markup' => t( $failover_obj->command_desc )));
    $form['cloud_failover_action'][$fsid] = array(array('#markup' => t( $failover_obj->action_desc )));
  }
  $form['pager'        ] = array('#value' => theme('pager', array('tags' => NULL, 'element' => 0)));
  $form['#redirect']  = FALSE;
  return $form;
}




function theme_cloud_failover_scenario_list($form) {

  if (isset($form['form'])) { // drupal 7 passed form as an array 
    $form = $form['form'];
  }
  
  //$output = drupal_render($form['options']);
  $prop = array();
  //$form['Name'] = !empty($form['Name']) ? $form['Name'] : '';

  $form['pager']['#value'] = !empty($form['pager']['#value'])
                             ? $form['pager']['#value']
                             : '';
  $rows = array();
  if ( !empty($form['Name']) ) {
    foreach (element_children($form['Name']) as $key) {

      $row = array(
        array(
          'data' => drupal_render($form['Name'                  ][$key]),
          'class' => array('nickname-column'),
        ),
        drupal_render($form['check_interval'        ][$key]),
        drupal_render($form['retry_interval'        ][$key]),
        drupal_render($form['max_check_attempts'    ][$key]),
        drupal_render($form['user'                  ][$key]),
        drupal_render($form['cloud_failover_command'][$key]),
        drupal_render($form['cloud_failover_action'][$key]),
      );

      $prop['onclick'] = cloud_get_messagebox('Are you sure you want to delete the Scenario "' . $form['hdnName'][$key]['#value'] . '" ?');
      $action_data = '';

      if (user_access('delete failover scenario')) {
        $action_data .= cloud_display_action( 'images/icon_delete', t('Delete'), CLOUD_FAILOVER_PATH . '/delete',   array('query' => array('id' => urlencode($key)), 'html' => TRUE), $prop['onclick']);
      }

      if (user_access('edit failover scenario')) {
        $action_data .= cloud_display_action( 'images/icon_clear', t('Edit'), CLOUD_FAILOVER_PATH . '/create',   array('query' => array('id' => urlencode($key)), 'html' => TRUE));
      }

      //$row[] =  $action_data ;
      $row[] = array(
        'data' => $action_data,
        'class' => 'action-column',
      );
      $rows[] = $row;
    }
  }

  $reload_link = l(t('- Refresh Page -'), CLOUD_FAILOVER_PATH . '/scenarios',   array('query' => array('src' => CLOUD_FAILOVER_PATH . '/scenarios')) );
  $ref_link = array(
    '#type' => 'item',
    '#prefix' => '<div id="link_reload" align="right">',
    '#suffix' => '</div>',
    '#markup' => $reload_link,
  );



  $output  = drupal_render($form['options']);
  
  $output .= theme('table', array( 'header' => $form['header']['#value'], 'rows' => $rows) );


  if ($form['pager']['#value']) {
    $output .= drupal_render($form['pager']);
  }

  $output .= drupal_render($ref_link);
  $output .= drupal_render_children($form);

  return $output;
}


/**
 * Form submit for listing page
 * @param unknown_type $form_id
 * @param unknown_type $form_values
 */
function cloud_failover_scenario_list_submit($form_id, &$form_state) {
  $form_values = $form_state['values'];
  if ($form_values['op'] == 'Create Scenario') {
    $form_state['redirect'] = CLOUD_FAILOVER_PATH . '/create';
  }
  else {
    $param_arr = array(
      'filter' => $form_values['filter'],
      'operation' => $form_values['operation'],
    );
    $form_state['redirect'] = array(current_path(), array('query' => $param_arr));
  }
}

/**
 *
 * @param type $cloud_context
 * @return type
 */
function _cloud_failover_update_monitoring_server_entries($cloud_context) {

  $cloud_array = cloud_get_clouds();
  for ($row_counter = 0; $row_counter < count($cloud_array); $row_counter++) {

    $module = $cloud_array[$row_counter]["cloud_name"];
    if ($module == $cloud_context) {

      $cloud  = $cloud_array[$row_counter]["cloud_name"];
      $url    = $cloud_array[$row_counter]["monitoring_server_url"];
      // if no url set, do not start monitoring
      if (!empty($url)) {
        _cloud_failover_start_monitoring_instances($cloud_context, $cloud, $url);
      }
      else {
        // set an error messages
        drupal_set_message(t('No monitoring url declared.  Cannot enable or disable failover'), 'error');
      }
    }
  }

  return;
}

/**
 *
 * @param type $cloud_context
 * @param type $instance_id
 * @return type
 */
function _cloud_failover_get_instance_failover_flag($cloud_context, $instance_id) {

  return _cloud_failover_instance_failover_flag($cloud_context, $instance_id);
}
