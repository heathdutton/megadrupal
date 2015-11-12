<?php

/**
 * @file
 * Provides cluster feature which enables to bundle server templates.
 * Works with Cloud and Server Templates module.
 *
 * Copyright (c) 2010-2011 DOCOMO Innovations, Inc.
 *
 */

/**
 * Updated   by yas   2013/10/31
 * Updated   by yas   2011/03/03
 * Updated   by yas   2011/02/17
 * Updated   by yas   2011/02/14
 * Updated   by yas   2011/02/10
 * Updated   by yas   2011/02/02
 * Updated   by yas   2011/02/01
 * Updated   by Jamir 2011/02/01
 * Updated   by Jamir 2011/01/29
 * Formatted by yas   2011/01/28
 * Updated   by Jamir 2011/01/27
 * Updated   by Jamir 2011/01/26
 */

/**
 * Display Cluster List
 *
 * @param $form_submit
 * @return
 */
function cloud_cluster_display_list($form, $form_submit) {

  drupal_add_js(cloud_get_module_base() . 'js/cloud.js');
  drupal_add_js(cloud_get_module_base() . 'js/cloud_activity_audit.js');

  $column = 'cluster_nickname';

  $options = array(
    t('Nickname'),
    t('Creator' ),
  );

  $filter     = cloud_get_filter_value( $form_submit, 'filter'   );
  $filter     = trim($filter);

  $filter_col = cloud_get_filter_value( $form_submit, 'operation');
  $filter_col = isset($filter_col) && $filter_col ? $filter_col : 0; // default value: Nickname

  $sql_col = NULL;

  if ( $filter_col == 0 ) {
    $column  = 'Nickname';
    $sql_col = 'cluster_nickname';
  }
  elseif ($filter_col == 1 ) {
    $column  = 'Creator';
    $sql_col = 'creator';
  }

  $query_args = array();
  if ( isset($filter) ) {

    $query_args[] = $sql_col;
    $query_args[] = $filter  ;
  }
  else {
    $filter       = ' 1 ';
    $query_args[] = ' '   ;
  }

  $cluster_count = _cloud_cluster_get_count_db();
  $filter_disabled = '';
  if ( $cluster_count < 2 ) {

    $filter_disabled = TRUE;
  }

  $form = array();
  $form['operations'] = array(
    '#prefix' => '<div class="container-inline">',
    '#type' => 'fieldset',
    '#suffix' => '</div>',
  );

  $form['operations']['label'    ] = array(
    '#type' => 'item'     ,
    '#title' => t('Filter'),
  );
  $form['operations']['operation'] = array(
    '#type' => 'select'   ,
    '#options' => $options  ,
    '#disabled' => $filter_disabled,
    '#default_value' => $filter_col,
  );
  $form['operations']['filter'   ] = array(
    '#type' => 'textfield',
    '#size' => 40        ,
    '#default_value' => $filter   ,
    '#disabled' => $filter_disabled,
  );
  $form['operations']['submit'   ] = array(
    '#type' => 'submit'   ,
    '#value' => t('Apply'),
    '#disabled' => $filter_disabled,
  );

  if (user_access('create cluster') ) {

    $tmpl_active_count = _cloud_server_templates_get_active_template_count_by_cloud();

    if (isset($tmpl_active_count) && count($tmpl_active_count) > 0 ) {

      $form['operations']['cloud_cluster'] = array(
        '#type' => 'submit',
        '#value' => t('Create'),
      );
    }
    else {

      $form['operations']['cloud_cluster'] = array(
        '#type' => 'submit',
        '#value' => t('Create'),
        '#disabled' => TRUE,
      );
      $template_url = filter_xss( l( t('Design - Template'), 'design/server_templates' ) );
      drupal_set_message(check_plain(t('There are no Templates > ', array())) . $template_url );
    }
  }

  $form['header'] = array(
    '#type' => 'value',
    '#value' => array(
      array(
        'data' => t('Nickname'),
        'field' => 'cluster_nickname',
        'sort' => 'asc',
        'class' => array('nickname-column'),
      ),
      array(
        'data' => t('Description'),
        'field' => 'description',
      ),
      array(
        'data' => t('Creator'    ),
        'field' => 'creator',
      ),
      array('data' => t('Servers'    )),
      array('data' => t('Running'    )),
      array('data' => t('Action'), 'class' => 'action-column'),
    ),
  );

  $query = _cloud_cluster_get_clusters_query();
  $query = $query
      ->extend('PagerDefault')
      ->extend('TableSort')
      ->orderByHeader($form['header']['#value']);

  $query
      ->condition($query_args[0], '%%' . $query_args[1] . '%%', 'like')
      ->fields('a')
      ->limit(CLOUD_CLUSTER_PAGER_LIMIT);

  $result = $query->execute();

  $destination = drupal_get_destination();

  $filter_params = array();
  $filter_params['filter_state'] = 'all';
  $all_instances_list = cloud_get_all_instances_list($filter_params);
  foreach ($result as $cluster) {
    $run_count = _cloud_cluster_get_running_instance_count($cluster->cluster_id, '', $all_instances_list);
    if ($run_count > 0) {
      $form['Nickname'][$cluster->cluster_id] = array('#markup' => l( $cluster->cluster_nickname, CLOUD_CLUSTER_PATH . '/describe/servers/' . $cluster->cluster_id, array()));
    }
    else {
      $form['Nickname'][$cluster->cluster_id] = array('#markup' => l( $cluster->cluster_nickname, CLOUD_CLUSTER_PATH . '/describe/cluster_templates/' . $cluster->cluster_id, array()));
    }

    $form['Description'][$cluster->cluster_id] = array('#markup' => check_plain( $cluster->description));
    $form['Creator'][$cluster->cluster_id] = array('#markup' => t( $cluster->creator));

    $servers_count = _cloud_cluster_get_servers_count($cluster->cluster_id, $all_instances_list);
    $form['Servers'][$cluster->cluster_id] = array('#markup' => t( $servers_count));

    $form['Running'][$cluster->cluster_id] = array('#markup' => t( $run_count));
    $form['Actions'][$cluster->cluster_id] = array('#markup' => cloud_cluster_action(array('cluster_name' => $cluster->cluster_nickname, 'cluster_id' => $cluster->cluster_id)));
  }
  $form['#redirect'] = FALSE;

  return $form;
}

/**
 * Theme function for cluster listing page 
 */
function theme_cloud_cluster_display_list($form) {
  $form = $form['form'];
  $rows = array();

  if (isset($form['Nickname'])) {
    foreach (element_children($form['Nickname']) as $key) {
      $rows[] = array(
        array(
          'data' => drupal_render($form['Nickname'   ][$key]),
          'class' => array('nickname-column'), 
        ),
        drupal_render($form['Description'][$key]),
        drupal_render($form['Creator'    ][$key]),
        drupal_render($form['Servers'    ][$key]),
        drupal_render($form['Running'    ][$key]),
        array(
          'data' => drupal_render($form['Actions'][$key]),
          'class' => 'action-column',
        ),
      );
    }
  }
  $output  = drupal_render($form['operations']);
  $output .= theme('table', array('header' => $form['header']['#value'], 'rows' => $rows, 'attributes' => array()));
  $output .= theme('pager', array('tags' => array()));
  $output .= drupal_render_children($form);
  return $output;
}

/**
 * Function to render the actions for a cluster 
 */
function cloud_cluster_action($cluster_info) {
  $action_data = array();
  
  if (user_access('delete cluster')) {
    $prop['onclick'] = cloud_get_messagebox(t('Are you sure you want to delete the cluster "@cluster_name" ?', array('@cluster_name' => $cluster_info['cluster_name'])));
    $action_data[] = cloud_display_action('images/icon_delete.png', t('Delete'), CLOUD_CLUSTER_PATH . '/delete/' . $cluster_info['cluster_id'], array('html' => TRUE), $prop['onclick']);
  }
  
  if (user_access('update cluster')) {
    $action_data[] = cloud_display_action('images/icon_edit.png', t('Edit'), CLOUD_CLUSTER_PATH . '/describe/info/' . $cluster_info['cluster_id'], array('html' => TRUE));
  }
  
  if (module_exists('cloud_inputs')) { // Check if inputs exists
    if (user_access('update cluster')) {
      $action_data[] = cloud_display_action('images/icon_table_inputs.png', 'Inputs', 'design/cloud_cluster/' . $cluster_info['cluster_id'] . '/inputs',   array('query' => array('cluster_id' => $cluster_info['cluster_id'], 'module' => 'cluster'), 'html' => TRUE));
    }
  }

  // allow other modules to add action icons
  $user_actions = module_invoke_all('cloud_cluster_action_data', $cluster_info);
  $action_data = array_merge($action_data, $user_actions);
  return implode($action_data);
}

/**
 * Function that submits the cluster display listing form 
 */
function cloud_cluster_display_list_submit($form_id, &$form_state) {

  $form_values =& $form_state['values'];

  if ($form_values['op'] == t('Create')) {
    $form_state['redirect'] = CLOUD_CLUSTER_PATH . '/create';
  }
  elseif ($form_values['op'] == t('Apply')) {
    $param_arr = array(
      'filter' => $form_values['filter'],
      'operation' => $form_values['operation'],
    );
    $form_state['redirect'] = array(current_path(), array('query' => $param_arr));
  }
}

/**
 * Create a new Cluster
 *
 * @param $page_submit
 * @return
 */
function cloud_cluster_create_new_cluster($form, $page_submit = '') {

  $form = array();

  $form['fieldset_cluster_info'             ] = array('#type' => 'fieldset' /* '#title' => t('Cluster Info') */);
  $form['nickname_details']['nickname_label'] = array(
    '#type' => 'item',
    '#title' => t('Nickname'),
    '#required' => TRUE,
  );
  $form['nickname_details']['nickname_text'] = array('#type' => 'textfield');

  $form['Description_details']['Description_label'] = array(
    '#type' => 'item',
    '#title' => t('Description'),
  );
  $form['Description_details']['description_text'] = array('#type' => 'textarea');

  $form['submit_buttons'] = array(
    '#type' => 'fieldset',
    '#prefix' => '<span class="clear"></span><div class="container-inline"><div class="buttons">',
    '#suffix' => '</div></div>',
  );

  $form['submit_buttons']['Next'  ] = array(
    '#type' => 'submit',
    '#value' => t('Next'  ),
  );
  $form['submit_buttons']['Cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
  );

  return $form;
}


function theme_cloud_cluster_create_new_cluster($form) {

  if (isset($form['form'])) {

    $form = $form['form'];
  }

  $rows = array(
    array(
      drupal_render($form['nickname_details']['nickname_label'] ),
      drupal_render($form['nickname_details']['nickname_text'] ),
    ),
    array(
      drupal_render($form['Description_details']['Description_label'] ),
      drupal_render($form['Description_details']['description_text'] ),
    ),
  );


  $table = theme('table', array('header' => array(), 'rows' => $rows, 'attributes' => array()));
  $form['fieldset_cluster_info']['#children'] = $table;

  $output  = '';
  $output .= drupal_render($form['fieldset_cluster_info']);
  $output .= drupal_render($form['submit_buttons'       ]);
  $output .= drupal_render_children($form);

  return $output;
}

/**
 * Form validation for creating new cluster
 */
function cloud_cluster_create_new_cluster_validate($form_id, $form_values) {

  $form_values = $form_values['values'];
  if ( $form_values['op'] == t('Next') ) {
    if ( empty( $form_values['nickname_text']) == TRUE) {
      form_set_error('nickname_text',   t('Please enter Nickname'));
      return;
    }
    else {
      // Check whether same name cluster exists
      $query = _cloud_cluster_get_cluster_by_filter_query();
      $query_args = array(
        ':cluster_nickname' => $form_values['nickname_text'],
      );

      $result = db_query( $query, $query_args );
      foreach ($result as $db_obj) {
        $cluster_id = $db_obj->cluster_id;
        if (isset($form_values['cluster_id']) === FALSE || $cluster_id != $form_values['cluster_id']) {
          form_set_error('cluster_id',   t('Name is already used by a Cluster.'));
        }
      }
    }
  }
}

/**
 * Create new cluster submit 
 */
function cloud_cluster_create_new_cluster_submit($form_id, &$form_state) {

  $form_values = $form_state['values'];

  if ($form_values['op'] == t('Next')) {
    global $user;
    $owner      = $user->name;
    $cluster_id = _cloud_cluster_save_cluster_db($form_values, $owner);

    if ($cluster_id !== NULL ) {
      drupal_set_message(t('Cluster successfully created. For the next step, add Server(s) based on Template to Cluster.'));
      $enabled_cloud_list = cloud_get_clouds();
      $tmpl_active_count  = _cloud_server_templates_get_active_template_count_by_cloud();
      for ($row_counter   = 0; $row_counter < count($enabled_cloud_list); $row_counter++) {
        $cloud_name = $enabled_cloud_list[$row_counter]["cloud_name"];
        if (isset($tmpl_active_count) == FALSE || isset($tmpl_active_count[$cloud_name]) == FALSE ) {
          // No templates repsent
          continue;
        }
        // Goto the first cloud in the list
        $form_state['redirect'] = CLOUD_CLUSTER_PATH . '/describe/servers/add_server/' . $cloud_name . '/' . $cluster_id;
        return;
      }
    }
    else {
      drupal_set_message(t('There was an error while creating Cluster'), 'error');
    }
  }
  $form_state['redirect'] = CLOUD_CLUSTER_PATH;
}


/**
 * Display Cluster details information
 *
 * @param $page_submit
 * @param $cluster_id
 *          Cluster Id which information is to be displayed
 * @return
 */
function cloud_cluster_display_detail_info($form, $page_submit = '', $cluster_id = '' ) {

  if (empty($cluster_id) || strlen($cluster_id) == 0  ) {

    drupal_goto( CLOUD_CLUSTER_PATH  );

    return;
  }

  $query      = _cloud_cluster_get_cluster_by_id_query();
  $query_args = array(
    ':cluster_id' => $cluster_id,
  );

  $result = db_query( $query, $query_args );
  $count  = 0;

  $filter_params = array(
    'filter_state' => 'all',
  );

  $all_instances_list = cloud_get_all_instances_list($filter_params);

  $form = array();
  $form['fieldset_cluster_info'] = array(
    '#type' => 'fieldset',
    '#title' => t('Details'),
  );
  $form['cluster_info_header'  ] = array(
    '#prefix' => '<h2>',
    '#markup' => t('Cluster Info'),
    '#suffix' => '</h2>',
  );

  $txt_disabled = '';
  if (user_access('update cluster') === FALSE ) {

    $txt_disabled = TRUE;
  }


  foreach ($result as $key) {

    $form['nickname_details'   ]['nickname_label'   ] = array(
      '#type' => 'item',
      '#title' => t('Nickname'),
    );
    $form['nickname_details'   ]['nickname_text'    ] = array(
      '#type' => 'textfield',
      '#default_value' => $key->cluster_nickname,
      '#disabled' => $txt_disabled,
    );

    $form['Description_details']['Description_label'] = array(
      '#type' => 'item',
      '#title' => t('Description'),
    );
    $form['Description_details']['description_text'] = array(
      '#type' => 'textarea',
      '#default_value' => $key->description       ,
      '#disabled' => $txt_disabled,
    );

    $form['Creator_details'    ]['creator_label'    ] = array(
      '#type' => 'item',
      '#title' => t('Creator'),
    );
    $form['Creator_details'    ]['creator_text'     ] = array('#markup' => t( $key->creator));

    $servers_count = _cloud_cluster_get_servers_count($key->cluster_id, $all_instances_list);
    $form['Servers_details'    ]['servers_label'    ] = array(
      '#type' => 'item',
      '#title' => t('Servers'),
    );
    $form['Servers_details'    ]['servers_text'     ] = array('#markup' => t( $servers_count));

    $run_count = _cloud_cluster_get_running_instance_count($key->cluster_id, '', $all_instances_list);
    $form['Running_details'    ]['running_label'    ] = array(
      '#type' => 'item',
      '#title' => t('Running'),
    );
    $form['Running_details'    ]['running_text'     ] = array('#markup' => t( $run_count));

    $form['cluster_id'] = array(
      '#type' => 'hidden',
      '#value' => $key->cluster_id,
    );

    $form['submit_buttons'] = array(
      '#prefix' => '<span class="clear"></span><div class="container-inline"><div class="buttons">',
      '#type' => 'fieldset',
      '#suffix' => '</div></div>',
    );

    if (user_access('update cluster') ) {

      // Update button
      $form['submit_buttons']['Update'] = array(
        '#type' => 'submit',
        '#value' => t('Update'),
      );
      $confirm_msg = cloud_get_messagebox('Are you sure you want to delete the Cluster?');
      $form['submit_buttons']['Delete'] = array(
        '#type' => 'submit',
        '#value' => t('Delete'),
        '#attributes' => array('onclick' => $confirm_msg),
      );
    }

    $form['submit_buttons']['List Clusters'] = array(
      '#type' => 'submit',
      '#value' => t('List Clusters'),
    );

    $count++;

    break;
  }

  if ( $count == 0 ) {

    drupal_goto( CLOUD_CLUSTER_PATH );

    return;
  }

  return $form;
}


function theme_cloud_cluster_display_detail_info($form) {

  if (isset($form['form'])) {

    $form = $form['form'];
  }

  $rows = array(
    array(
      drupal_render($form['nickname_details'   ]['nickname_label'   ] ),
      drupal_render($form['nickname_details'   ]['nickname_text'    ] ),
    ),
    array(
      drupal_render($form['Description_details']['Description_label'] ),
      drupal_render($form['Description_details']['description_text'] ),
    ),
    array(
      drupal_render($form['Creator_details'    ]['creator_label'    ] ),
      drupal_render($form['Creator_details'    ]['creator_text'     ] ),
    ),
    array(
      drupal_render($form['Servers_details'    ]['servers_label'    ] ),
      drupal_render($form['Servers_details'    ]['servers_text'     ] ),
    ),
    array(
      drupal_render($form['Running_details'    ]['running_label'    ] ),
      drupal_render($form['Running_details'    ]['running_text'     ] ),
    ),
  );


  $table = theme('table', array('header' => array(), 'rows' => $rows, 'attributes' => array()));

  $form['fieldset_cluster_info']['#children'] = $table;

  $output  = drupal_render($form['cluster_info_header'] );
  $output .= drupal_render($form['submit_buttons'     ] );
  $output .= drupal_render($form['cluster_id'         ] );
  $output .= drupal_render($form['fieldset_cluster_info']);

  $output .= drupal_render_children($form);

  return $output;
}


function cloud_cluster_display_detail_info_validate($form_id, $form_values) {

  $form_values = $form_values['values'];
  if ( $form_values['op'] == t('Update') ) {

    if ( empty( $form_values['nickname_text']) == TRUE) {

      form_set_error('nickname_text', t('Please enter Nickname'));

      return;
    }
    else {

      $query      = _cloud_cluster_get_cluster_by_filter_query();
      $query_args = array(
        ':cluster_nickname' => $form_values['nickname_text'],
      );

      $result = db_query( $query, $query_args )->fetchObject();
      if ($result !== FALSE) {

        $db_obj     = $result;
        $cluster_id = $db_obj->cluster_id;

        if ($cluster_id != $form_values['cluster_id']) {
          form_set_error('cluster_id',   t('Name is already used by a Cluster.'));
        }
      }
    }
  }
}


function cloud_cluster_display_detail_info_submit($form_id, &$form_state) {

  $form_values = $form_state['values'];

  if ( $form_values['op'] == t('List Clusters') ) {
    $form_state['redirect'] = CLOUD_CLUSTER_PATH;
  }
  elseif ($form_values['op'] == t('Update')) {
    $cluster_id = $form_values['cluster_id'];
    _cloud_cluster_update_cluster_db($form_values, $cluster_id );
    drupal_set_message(t('Cluster has been updated.'));
    $form_state['redirect'] = CLOUD_CLUSTER_PATH . '/describe/info/' . $cluster_id;
  }
  elseif ($form_values['op'] == t('Delete')) {
    $cluster_id = $form_values['cluster_id'];
    _cloud_cluster_delete_cluster(array(), $form_values, $cluster_id);
    $form_state['redirect'] = CLOUD_CLUSTER_PATH;
  }
}


function _cloud_cluster_stop_all_servers($cluster_id, $action, $all_instances_list = array()) {

  $run_count = _cloud_cluster_get_running_instance_count($cluster_id, '', $all_instances_list);
  if ( $run_count < 1 ) {

    return NULL; //No instances running. No action to be taken
  }

  if (empty($all_instances_list)) {

    $filter_params = array();
    $filter_params['filter_state'] = 'all';
    $all_instances_list = cloud_get_all_instances_list($filter_params);
  }


  $running_instance_count = 0;
  $query      = _cloud_cluster_servers_get_query();
  $query->condition('cluster_id', $cluster_id, '=');
  $result     = $query->execute();


  foreach ($result as $key) {

    $instance_ids_str = $key->instance_id;
    if (empty($instance_ids_str)) {

      continue;
    }

    $instance_ids = explode( ',', $instance_ids_str );
    foreach ($instance_ids as $instance_id) {

      $instance_id = trim($instance_id);
      if (isset($all_instances_list[$instance_id])) {

        if (    isset($all_instances_list[$instance_id]['state'])
           && drupal_strtolower($all_instances_list[$instance_id]['state']) !== 'terminated'
           && drupal_strtolower($all_instances_list[$instance_id]['state']) !== 'halted'
           && drupal_strtolower($all_instances_list[$instance_id]['state']) !== 'stopped'
           && drupal_strtolower($all_instances_list[$instance_id]['state']) !== 'shutting-down') {

          if ($action == 'stop_all' ) {

            $result_terminate = cloud_perform_action( '', 'terminate', $all_instances_list[$instance_id]['cloud_context'], array($instance_id) );
          }
          elseif ($action == 'count_running' ) {

            $running_instance_count++;
          }
        }
      }
    }
  }

  return $running_instance_count;
}


function _cloud_cluster_get_servers_count($cluster_id, $all_instances_list = array()) {

  $servers_count = 0;
  $run_count  = _cloud_cluster_get_running_instance_count($cluster_id, '', $all_instances_list);
  $query      = _cloud_cluster_templates_get_query();
  $query_args = array(
    ':cluster_id' => $cluster_id,
  );

  $result     = db_query( $query, $query_args );

  if ( $run_count < 1 ) { // No servers running

    foreach ($result as $clust) { // Cluster server object
      $servers_count += $clust->count_instances;
    }
  }
  else { // Servers running : Count the number of instances present
    foreach ($result as $clust) { // Cluster server object
      $instance_ids_str =  $clust->instance_id;
      $instance_ids     =  explode( ',', $instance_ids_str);
      $servers_count +=  sizeof($instance_ids);
    }
  }

  return $servers_count;
}

function _cloud_cluster_get_servers_count_by_id($cluster_id, $server_id, $all_instances_list = array()) {

  $servers_count = 0;
  $run_count  = _cloud_cluster_get_running_instance_count($cluster_id, '', $all_instances_list);
  $query      = _cloud_cluster_templates_server_get_query();
  $query_args = array(
    ':cluster_id' => $cluster_id,
    ':server_id' => $server_id,
  );

  $result     = db_query( $query, $query_args );

  if ( $run_count < 1 ) { // No servers running
    foreach ($result as $clust) { // Cluster server object
      $servers_count += $clust->count_instances;
    }
  }
  else { // Servers running : Count the number of instances present
    foreach ($result as $clust) { // Cluster server object
      $instance_ids_str =  $clust->instance_id;
      $instance_ids     =  explode( ',', $instance_ids_str);
      $servers_count +=  sizeof($instance_ids);
    }
  }

  return $servers_count;
}


function _cloud_cluster_get_running_instance_count($cluster_id, $server_id = '', $all_instances_list = array()) {

  $filter_params = array();
  $filter_params['filter_state'] = 'all';
  if (empty($all_instances_list)) {

    $all_instances_list = cloud_get_all_instances_list($filter_params);
  }

  $query  = '';
  $query_args = array();

  if ( empty($server_id) ) {
    $query      = _cloud_cluster_servers_get_query();
    $query->condition('cluster_id', $cluster_id, '=');

    $result     = $query->execute();
  }
  else {

    $query      = _cloud_cluster_server_get_query();
    $query_args = array(
      ':cluster_id' => $cluster_id,
      ':server_id' => $server_id,
    );

    $result     = db_query( $query, $query_args );
  }

  $running_instance_count = 0;

  foreach ($result as $key) {

    $instance_ids_str = $key->instance_id;
    if (empty($instance_ids_str)) {

      continue;
    }

    $instance_ids = explode( ',', $instance_ids_str );
    foreach ($instance_ids as $instance_id) {

      $instance_id = trim($instance_id);
      if (isset($all_instances_list[$instance_id])) {

        if (    isset($all_instances_list[$instance_id]['state'])
         && drupal_strtolower($all_instances_list[$instance_id]['state']) !== 'terminated'
         && drupal_strtolower($all_instances_list[$instance_id]['state']) !== 'halted'
         && drupal_strtolower($all_instances_list[$instance_id]['state']) !== 'stopped'   ) {

          $running_instance_count++;
        }
      }
    }
  }

  return $running_instance_count;
}


/**
 * Display List of Templates present in a cluster
 *
 * @param $page_submit
 * @param $cluster_id
 *              Cluster ID who's server's are to be displayed
 * @return
 */
function cloud_cluster_display_templates_list($form, $page_submit = '', $cluster_id = '' ) {
  $base = cloud_get_module_base();
  drupal_add_js(cloud_get_module_base() . 'js/cloud.js');

  if (empty($cluster_id) ) {
    drupal_goto( CLOUD_CLUSTER_PATH );
    return;
  }

  $filter_params = array();
  $filter_params['filter_state'] = 'all';
  $all_instances_list = cloud_get_all_instances_list($filter_params);
  $run_count = _cloud_cluster_get_running_instance_count($cluster_id, '', $all_instances_list);

  $form['submit_buttons'] = array(
    '#type' => 'fieldset',
    '#prefix' => '<span class="clear"></span><div class="container-inline"><div class="buttons">',
    '#suffix' => '</div></div>',
  );

  if (user_access('update cluster')) {
    if ($run_count < 1) {
      $tmpl_active_count = _cloud_server_templates_get_active_template_count_by_cloud();
      if (isset($tmpl_active_count) && count($tmpl_active_count) > 0 ) {
        $form['submit_buttons']['add_server'] = array(
          '#type' => 'submit',
          '#value' => t('Add Server'),
        );
      }
      else {
        $form['submit_buttons']['add_server'] = array(
          '#type' => 'submit',
          '#value' => t('Add Server'),
          '#disabled' => TRUE,
        );
        $template_url = l(t('Design - Template'), 'design/server_templates');
        drupal_set_message(t('There are no Templates'));
      }

      $count_servers = _cloud_cluster_servers_get_count_db($cluster_id);

      if ($count_servers > 0 ) {

        $form['submit_buttons']['start_all_servers']  = array(
          '#type' => 'submit',
          '#value' => t('Start All Servers'),
        );
        // Delete all templates
        $confirm_msg = cloud_get_messagebox('Are you sure you want to delete all the templates?');
        $onclick = $confirm_msg;
        $form['submit_buttons']['delete_all_servers'] = array(
          '#type' => 'submit',
          '#value' => t('Delete All Servers'),
          '#attributes' => array('onclick' => $onclick),
        );
      }
      else {
        $form['submit_buttons']['start_all_servers'] = array(
          '#type' => 'submit',
          '#value' => t('Start All Servers' ),
          '#disabled' => TRUE,
        );
        $form['submit_buttons']['delete_all_servers'] = array(
          '#type' => 'submit',
          '#value' => t('Delete All Servers'),
          '#disabled' => TRUE,
        );
      }
    }
    else {

      $form['submit_buttons']['add_server']         = array(
        '#type' => 'submit',
        '#value' => t('Add Server'),
        '#disabled' => TRUE,
      );
      $form['submit_buttons']['start_all_servers']  = array(
        '#type' => 'submit',
        '#value' => t('Start All Servers' ),
        '#disabled' => TRUE,
      );
      $form['submit_buttons']['delete_all_servers'] = array(
        '#type' => 'submit',
        '#value' => t('Delete All Servers'),
        '#disabled' => TRUE,
      );
    }
  }
  else {
    $form['submit_buttons']                         = array(
      '#type' => 'item',
      '#value' => t(''),
    );
  }

  $query      = _cloud_cluster_templates_get_query();
  $query_args = array(
    ':cluster_id' => $cluster_id,
  );

  $form['server_header'] = array(
    '#prefix' => '<h2>',
    '#markup' => t('Cluster Servers'),
    '#suffix' => '</h2>',
  );

  $form['header'] = array(
    '#type' => 'value',
    '#value' => array(
      array(
        'data' => t('Nickname'       ),
        'field' => 'server_nickname',
        'class' => array('nickname-column'),
      ),
      array(
        'data' => t('Cloud'          ),
        'field' => 'cloud_type',
      ),
      array(
        'data' => t('Template'       ),
        'field' => 'template_nickname',
      ),
      array('data' => t('Servers'        )),
      array('data' => t('Running'        )),
      array('data' => t('Actions'), 'class' => 'action-column'),
    ),
  );

  $cloud_name_list = cloud_get_all_clouds_display_name();
  $server_list = array();

  $order_query =  db_select(CLOUD_CLUSTER_SERVER_TABLE, 't')
                    ->extend('TableSort')
                    ->orderByHeader($form['header']['#value']);
            
                  
  $order_by  = explode('ORDER BY' , $order_query );
  $query .= ' ORDER BY ' . $order_by[1]; // append order

  $result     = db_query( $query, $query_args );


  foreach ($result as $clust) { // Cluster server object
    $server_id =  $clust->server_id;

    $server_list[$server_id]['server_id'         ] = $server_id;
    $server_list[$server_id]['server_name'       ] = $clust->server_nickname;
    $server_list[$server_id]['server_template_id'] = $clust->template_id;
    $server_list[$server_id]['server_template'   ] = $clust->template_nickname;
    $server_list[$server_id]['cloud_context'     ] = $clust->cloud_type;
    $server_list[$server_id]['cloud_context_name'] = isset( $cloud_name_list[$clust->cloud_type] )
                                                   ? $cloud_name_list[$clust->cloud_type]
                                                   : $clust->cloud_type;
    $server_list[$server_id]['servers'           ] = _cloud_cluster_get_servers_count_by_id($cluster_id, $server_id, $all_instances_list = array());
    $server_list[$server_id]['running'           ] = _cloud_cluster_get_running_instance_count($cluster_id, $server_id, $all_instances_list);
  }

  foreach ( $server_list as $cluster_server ) {

    $server_id       = $cluster_server['server_id'    ];
    $cloud_context   = $cluster_server['cloud_context'];

    $form['server_name'    ][$server_id] = array(
        '#prefix' => '<span class=\'anchor\'>',
        '#markup' => t( $cluster_server['server_name'] ),
        '#suffix' => '</span>',
      );
    $form['hdnName'        ][$server_id] = array(
      '#type' => 'hidden',
      '#value' => t( $cluster_server['server_name'] ),
    );
    $form['Cloud_type'     ][$server_id] = array('#markup' => t($cluster_server['cloud_context_name']));
    $form['server_template'][$server_id] = array('#markup' => l($cluster_server['server_template'],
                                                                       'design/server_templates/' . $cloud_context . '/' . $cluster_server['server_template_id'] . '/view',
                                                                        array('query' => array('id' => urlencode($cluster_server['server_template_id'] ))) ));
    $form['server_id'      ][$server_id] = array(
      '#type' => 'hidden',
      '#value' => t( $server_id),
    );
    $form['servers'        ][$server_id] = array('#markup' => t( $cluster_server['servers'] ));
    $form['running'        ][$server_id] = array('#markup' => t( $cluster_server['running'] ));
    $form['cloud_context'  ][$server_id] = array(
      '#type' => 'value',
      '#markup' => $cloud_context,
    );
    $form['server_template_id'][$server_id] = array(
      '#type' => 'value',
      '#markup' => $cluster_server['server_template_id'],
    );
    $template_info = array(
      'server_name' => $cluster_server['server_name'],
      'server_id' => $server_id,
      'run_count' => $cluster_server['running'],
      'cluster_id' => $cluster_id,
      'template_id' => $cluster_server['server_template'],
    );
    $form['Actions'][$server_id] = array('#markup' => cloud_cluster_template_action($cloud_context, $template_info));
  }

  $form['cluster_id'] = array(
    '#type' => 'hidden',
    '#value' => t( $cluster_id),
  );

  return $form;
}


function theme_cloud_cluster_display_templates_list($form) {
  $form = $form['form'];
  $output  = drupal_render($form['server_header']);
  $output .= drupal_render($form['submit_buttons']);

  $cluster_id = $form['cluster_id']['#value'];
  $rows = array();

  $run_count = _cloud_cluster_get_running_instance_count($cluster_id);
  if (isset($form['server_name']) ) {
    foreach (element_children($form['server_name']) as $deplkey) {
      $prop_delete = array();
      $prop_delete['onclick'] = cloud_get_messagebox('Are you sure you want to delete the template "' . $form['hdnName'][$deplkey]['#value'] . '" ?');

      $rows[] = array(
        array(
          'data' => drupal_render($form['server_name'    ][$deplkey]),
          'class' => array('nickname-column'),
        ),
        drupal_render($form['Cloud_type'     ][$deplkey]),
        drupal_render($form['server_template'][$deplkey]),
        drupal_render($form['servers'        ][$deplkey]),
        drupal_render($form['running'        ][$deplkey]),
        array(
          'data' => drupal_render($form['Actions'][$deplkey]),
          'class' => 'action-column',
        ),
      );
    }
  }

  $output .= theme('table', array('header' => $form['header']['#value'], 'rows' => $rows, 'attributes' => array()));
  $output .= drupal_render_children($form);
  return $output;
}

/**
 * Function to render the template actions for a cluster
 */
function cloud_cluster_template_action($cloud_context, $template_info) {
  $action_data = '';
  $server_name = $template_info['server_name'];
  $server_id = $template_info['server_id'];
  $run_count = $template_info['run_count']; 
  $cluster_id = $template_info['cluster_id'];
  $template_id = $template_info['template_id'];

  $prop_delete['onclick'] = cloud_get_messagebox('Are you sure you want to delete the template "' . $server_name . '" ?');

  if (user_access('delete cluster') && $run_count == 0 ) {
    $action_data .= cloud_display_action(
      'images/icon_delete.png',
      t('Delete'),
      CLOUD_CLUSTER_PATH . '/describe/servers/delete_server/' . $cluster_id . '/' . $server_id,
      array('query' => array('server_id' => urlencode($server_id)), 'html' => TRUE),
      $prop_delete['onclick']);
  }
  
  if (user_access('edit server template') && module_exists('cloud_inputs') && $run_count == 0) {
    $destination = array(
      'destination' => 'design/cluster/describe/cluster_templates/' . $cluster_id,
      'template_id' => $template_id,
      'module' => 'cluster',
      'type' => $cloud_context,
      'server_id' => $server_id,
    );
  
    $action_data .= cloud_display_action(
      'images/icon_table_inputs.png', 
      t('Inputs'), 
      'design/cloud_cluster_template/' . $cluster_id . '/inputs', 
      array('query' => $destination, 'html' => TRUE)
    );
  }
  return $action_data;
}

function cloud_cluster_display_templates_list_submit($form_id, &$form_state) {
  $form_values =& $form_state['values'];
  // Check for the operation
  if ($form_values['op'] !== t('Stop All Servers')) {
    // Check if Instances already running then do nothing return
    $run_count = _cloud_cluster_get_running_instance_count($form_values['cluster_id']);
    if ( $run_count > 0 ) {
      drupal_set_message( check_plain( t( 'Cannot perform operation @operation . Instances are running.', array('@operation' => $form_values['op']) ) ), 'error' );
      $form_state['redirect'] = CLOUD_CLUSTER_PATH . '/describe/cluster_templates/' . $form_values['cluster_id'];
    }
  }

  if ($form_values['op'] == t('Add Server')) {
    $enabled_cloud_list = cloud_get_clouds();
    $tmpl_active_count  = _cloud_server_templates_get_active_template_count_by_cloud();
    for ($row_counter = 0; $row_counter < count($enabled_cloud_list); $row_counter++) {
      $cloud_name = $enabled_cloud_list[$row_counter]["cloud_name"];
      if (isset($tmpl_active_count) == FALSE || isset($tmpl_active_count[$cloud_name]) == FALSE ) {
        // No templates repsent
        continue;
      }
      // Goto the first cloud in the list
      $form_state['redirect'] =  CLOUD_CLUSTER_PATH . '/describe/servers/add_server/' . $cloud_name . '/' . $form_values['cluster_id'];
      return;
    }
    // No Cloud found
    drupal_set_message( t( 'There is no enabled cloud. First enable a Cloud and then try adding a template' ), 'warning' );
    $form_state['redirect'] = CLOUD_CLUSTER_PATH . '/describe/servers/' . $form_values['cluster_id'];
  }
  elseif ( $form_values['op'] == t('Start All Servers')) {
    $form_state['redirect'] = CLOUD_CLUSTER_PATH . '/describe/servers/start_all_servers/' . $form_values['cluster_id'];
    return;
  }
  elseif ( $form_values['op'] == t('Delete All Servers')) {
    $run_count = _cloud_cluster_get_running_instance_count($form_values['cluster_id']);

    if ($run_count > 0 ) {
      drupal_set_message(t('Unable to delete. Instances running'), 'error');
      $form_state['redirect'] = CLOUD_CLUSTER_PATH . '/describe/cluster_templates/' . $form_values['cluster_id'];
      return;
    }
    else {
      _cloud_cluster_delete_all_server_db($form_values['cluster_id']);
      drupal_set_message(t('Templates deleted'));
      $form_state['redirect'] = CLOUD_CLUSTER_PATH . '/describe/cluster_templates/' . $form_values['cluster_id'];
      return;
    }
  }
  elseif ( $form_values['op'] == t('Inputs')) {
    $form_state['redirect'] = CLOUD_CLUSTER_PATH . '/' . $form_values['cluster_id'] . '/inputs';
    return;
  }
}

/**
 * Ajax callback
 */
function _cloud_cluster_callback_get_cluster_servers_list() {
  $cluster_id = isset($_REQUEST['cluster_id']) ? $_REQUEST['cluster_id'] : FALSE;

  if (empty($cluster_id)) {

    $output = 'NULL';
    $html = $output;
    print drupal_to_js(array('html' => $html));
    // The exit() call is critical!
    exit();
  }
  
  $form = drupal_get_form('cloud_cluster_display_server_list', $cluster_id);
  $output = theme('cloud_cluster_display_server_list', $form);
  
  $index_start = strrpos( $output, '<tbody>' );
  $index_end   = strrpos( $output, '</tbody>' );
  if ( isset($form['instance_nickname']) === FALSE || sizeof($form['instance_nickname']) == 0 ) { // No element present
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
 * Display List of server's present in a cluster
 *
 * @param $page_submit
 * @param $cluster_id
 *              Cluster ID who's server's are to be displayed
 * @return
 */
function cloud_cluster_display_server_list($form, $form_state = '', $cluster_id = '' ) {

  drupal_add_js(cloud_get_module_base() . 'js/cloud.js');
  drupal_add_js(cloud_get_module_base() . 'js/cloud_cluster_auto_refresh_list_servers.js');

  if (empty($cluster_id) ) {
    drupal_goto( CLOUD_CLUSTER_PATH );
    return;
  }

  $form = array();

  $query      = _cloud_cluster_servers_get_query();
  $query->condition('cluster_id', $cluster_id, '=');
  $result     = $query->execute();

  $filter_params = array();
  $filter_params['filter_state'] = 'all';
  $all_instances_list = cloud_get_all_instances_list($filter_params);

  $form['server_header'] = array(
    '#prefix' => '<h2>',
    '#markup' => t('Cluster Instances'),
    '#suffix' => '</h2>',
  );

  $form['header'] = array(
    '#type' => 'value',
    '#value' => array(
      array(
        'data' => t('Nickname'      ),
        'field' => 'server_name',
        'class' => array('nickname-column'),
      ),
      array(
        'data' => t('Instance name' ),
        'field' => 'instance_nickname',
      ),
      array(
        'data' => t('Cloud'         ),
        'field' => 'cloud_context',
      ),
      array(
        'data' => t('Zone'          ),
        'field' => 'zone',
      ),
      array(
        'data' => t('Hostname'      ),
        'field' => 'host_name',
      ),
      array(
        'data' => t('ID'            ),
        'field' => 'instance_id',
      ),
      array(
        'data' => t('Template'      ),
        'field' => 'server_template',
      ),
      array(
        'data' => t('State'         ),
        'field' => 'state',
      ),
      array(
        'data' => t('Runtime'       ),
        'field' => 'runtime',
      ),
      array(
        'data' => t('Action'        ),
        'class' => 'action-column',
      ),
    ),
  );

  $form['submit_buttons'] = array(
    '#type' => 'fieldset',
    '#prefix' => '<span class="clear"></span><div class="container-inline"><div class="buttons">',
    '#suffix' => '</div></div>',
  );

  if (  _cloud_cluster_stop_all_servers($cluster_id, 'count_running', $all_instances_list) > 0  ) {

    $confirm_msg = cloud_get_messagebox('Are you sure you want to stop all the servers ?');
    $onclick = $confirm_msg;
    $form['submit_buttons']['stop_all_servers'] = array(
      '#type' => 'submit',
      '#value' => t('Stop All Servers'),
      '#attributes' => array('onclick' => $onclick),
    );
  }
  else {

    $form['submit_buttons']['stop_all_servers'] = array(
      '#type' => 'submit',
      '#value' => t('Stop All Servers'),
      '#disabled' => TRUE,
    );
  }

  $destination = drupal_get_destination();
  $count = 0;
  $failover_status = array();
  $cloud_name_list = cloud_get_all_clouds_display_name();
  $server_list     = array();

  $run_count = _cloud_cluster_get_running_instance_count($cluster_id);


  foreach ($result as $clust) { // Cluster server object
    if ($run_count == 0) {

      break; // No instances running
    }

    $inst_count = 0;
    $inst_found = TRUE;
    $instance_ids = array(0); // Dummy array
    $instance_ids_str = $clust->instance_id;
    if ( empty($instance_ids_str) ) {

      $inst_found = FALSE;
    }
    else {

      $instance_ids = explode( ',', $instance_ids_str );
    }

    foreach ($instance_ids as $instance_id ) {

      $inst_count++;
      $server_id = $clust->server_id;
      $index_id  = $clust->server_id . '_' . $instance_id;

      $server_list[$index_id]['index_id'           ] = $index_id;
      $server_list[$index_id]['server_id'          ] = $server_id;
      $server_list[$index_id]['server_name'        ] = $clust->server_nickname;

      if ( $inst_found
       &&    count($instance_ids) > 1 ) {

        $server_list[$index_id]['server_name'        ] .= ' #' . $inst_count; // Append #1, #2...
      }

      $tmpl = cloud_get_server_template_info($clust->template_id);
      $server_list[$index_id]['server_template_id'] = $clust->template_id;
      $server_list[$index_id]['server_template'    ] = $tmpl->template_nickname;
      $server_list[$index_id]['cloud_context'      ] = $tmpl->cloud_type;
      $server_list[$index_id]['cloud_context_name'] = isset( $cloud_name_list[$tmpl->cloud_type] )  ? $cloud_name_list[$tmpl->cloud_type] : $tmpl->cloud_type;


      if ( empty($instance_id) === FALSE
       && isset($all_instances_list[$instance_id]) ) {

        $instance_info = $all_instances_list[$instance_id];

        $server_list[$index_id]['instance_nickname'] = trim($instance_info['name']);
        $server_list[$index_id]['host_name'         ] = $instance_info['hostname'];
        $server_list[$index_id]['instance_id'       ] = $instance_id;
        $server_list[$index_id]['state'             ] = ucwords($instance_info['state']);
        $server_list[$index_id]['zone'              ] = $instance_info['zone'];
        $server_list[$index_id]['runtime'           ] = $instance_info['runtime'];
        $server_list[$index_id]['lock_status'       ] = $instance_info['lock_status'];
      }
      else {

        $server_list[$index_id]['instance_nickname'] = '';
        $server_list[$index_id]['host_name'         ] = '';
        $server_list[$index_id]['instance_id'       ] = '';
        $server_list[$index_id]['state'             ] = '';
        $server_list[$index_id]['zone'              ] = '';
        $server_list[$index_id]['runtime'           ] = '';
        $server_list[$index_id]['lock_status'       ] = '';
      }

      if ( empty($instance_id) === FALSE ) {

        $server_list[$index_id]['instance_id'       ] = $instance_id;
      }

    } // End of foreach ($instance_ids as $instance_id ) {
  } // End of while

  $ts = tablesort_init( $form['header']['#value'] );
  if ($ts['sql']) {

    $field =  $ts['sql'];
    $server_list = _cloud_sort_by_value( $server_list, $field, $ts['sort'] );
  }

  foreach ( $server_list as $cluster_server ) {

    $index_id      = $cluster_server['index_id'          ];
    $server_id     = $cluster_server['server_id'         ];
    $cloud_context = $cluster_server['cloud_context'     ];
    $name          = $cluster_server['instance_nickname'];
    $state         = $cluster_server['state'             ];
    $hostname      = $cluster_server['host_name'         ];
    $zone          = $cluster_server['zone'              ];
    $runtime       = $cluster_server['runtime'           ];
    $lock_status   = $cluster_server['lock_status'       ];
    $instance_id   = $cluster_server['instance_id'       ];

    $cloud_menu_path = cloud_get_menu_path($cloud_context);

    $form['server_name'    ][$index_id] = array(
        '#prefix' => '<span class=\'anchor\'>',
        '#markup' => t( $cluster_server['server_name'] ),
        '#suffix' => '</span>',
      );
    $form['hdnName'        ][$index_id] = array(
      '#type' => 'hidden',
      '#value' => t( $cluster_server['server_name'] ),
    );

    if ( !empty($state)
     && drupal_strtolower($state) == 'running' ) {

      $form['instance_nickname'][$index_id] = array(
        array(
          '#markup' => l($name,
                          $cloud_menu_path . '/instances/describe/' . $instance_id . '/info',
                          array(
            'query' => array('instance_id' => urlencode($instance_id)),
          ))
                                                      . (($lock_status == 'Lock')
                                                      ? '<a class=lockIcon title=Lock>&nbsp;&nbsp;&nbsp;&nbsp;</a>'
                                                      : ''
                        ), // end 'l' function
        ), // end array
      ); // end $form
    }
    else {

      $form['instance_nickname'][$index_id] = array('#markup' => $name);
    }

    $public_dns = substr( $hostname, 0, strpos($hostname,   '.') );
    if ( strlen($public_dns) > 3 ) { // truncate only in case the name is long
      $public_dns = substr($hostname, 0, strpos($hostname,   '.')) . '...';
    }
    else {

      $public_dns = $hostname;
    }

    $form['Cloud_type'     ][$index_id] = array('#markup' => t( $cluster_server['cloud_context_name'] ));
    $form['Zone'           ][$index_id] = array('#markup' => t( $zone           ));
    $form['Public_ip'      ][$index_id] = array('#markup' => l($public_dns,
                                                                  'http://' . $hostname, array(
          'attributes' => array('target' => '_blank'),
          'query' => array(),
        ) ));
    $form['Instanceid'     ][$index_id] = array('#markup' => t( $instance_id    ));
    $form['server_template'][$index_id] = array('#markup' => l( $cluster_server['server_template'],   'design/server_templates/' . $cloud_context . '/' . $cluster_server['server_template_id'] . '/view', array('query' => array('id' => urlencode($cluster_server['server_template_id'] )) )));
    $form['State'          ][$index_id] = array('#markup' => t( ucwords($state) ));
    $form['Runtime'        ][$index_id] = array('#markup' => t( ($runtime)
                                                                 ? cloud_get_runtime($runtime)
                                                                 : ''
                                                                 ));
    $form['server_id'   ][$index_id] = array(
        '#type' => 'hidden',
        '#value' => t( $server_id),
      );
  }

  $form['cluster_id']   = array(
    '#type' => 'hidden',
    '#value' => t( $cluster_id ),
  );

  return $form;
}


/**
 * Theme function for cluster display server list 
 */
function theme_cloud_cluster_display_server_list($form) {
  
  $form = $form['form'];
  $output  = drupal_render($form['server_header'     ]);
  $output .= drupal_render($form['submit_buttons']);

  $cluster_id = $form['cluster_id']['#value'];

  $filter_params = array();
  $filter_params['filter_state'] = 'all';
  $all_instances_list = cloud_get_all_instances_list(array('filter_state' => 'all'));

  $rows = array();
  if (isset($form['server_name']) ) {
    foreach (element_children($form['server_name']) as $deplkey) {

      $row = array(
        array(
          'data' => drupal_render($form['server_name'       ][$deplkey]),
          'class' => array('nickname-column'),
        ),
        drupal_render($form['instance_nickname' ][$deplkey]),
        drupal_render($form['Cloud_type'        ][$deplkey]),
        drupal_render($form['Zone'              ][$deplkey]),
        drupal_render($form['Public_ip'         ][$deplkey]),
        drupal_render($form['Instanceid'        ][$deplkey]),
        drupal_render($form['server_template'   ][$deplkey]),
        drupal_render($form['State'             ][$deplkey]),
        drupal_render($form['Runtime'           ][$deplkey]),
      );

      $action_data = '';
      $server_id   = $form['server_id'][$deplkey]['#value'];
      if (empty($form['Instanceid'][$deplkey]['#markup'])
      ||  drupal_strtolower( $form['State'][$deplkey]['#markup']) == 'terminated'
      ||  drupal_strtolower( $form['State'][$deplkey]['#markup']) == 'stopped'
      ||  empty( $form['State'][$deplkey]['#markup'])) {

        if (user_access('update cluster')) {
          $action_data .= cloud_display_action(
            'images/icon_play.png',
            'launch',
            CLOUD_CLUSTER_PATH . '/describe/servers/launch_server/' . $cluster_id . '/' . $server_id,
            array('query' => array('instance_id' => $form['Instanceid'][$deplkey]['#markup']),
                                   'html'        => TRUE)
          );
        }
      }
      elseif (empty($form['Instanceid'][$deplkey]['#markup']) === FALSE ) {

        $instance_id  = $form['Instanceid'][$deplkey]['#markup'];
        if (isset($all_instances_list[$instance_id]) ) {
          $action_data .= $all_instances_list[$instance_id]['action_data'];
        }
      }

      $row[]  = array(
        'data' => $action_data,
        'class' => 'action-column',
      );
      $rows[] = $row;
    }
  }

  $table_attr = array();
  $table_attr['id'] = 'cluster_servers_list_table';
  $table_attr['autoupdate_url'] = url( CLOUD_CLUSTER_PATH . '/callback_get_cluster_servers_list' );

  $output .= theme('table', array('header' => $form['header']['#value'], 'rows' => $rows, 'attributes' => $table_attr));

  if (count($rows) > 0) {

    $reload_link = l( t( '- Refresh Page -' ), CLOUD_CLUSTER_PATH . '/get_data/' . $cluster_id  ,   array() );
    $ref_link    = array(
      '#type' => 'item',
      '#prefix' => '<div id="link_reload" align="right">',
      '#suffix' => '</div>',
      '#markup' => $reload_link,
    );

    $output .= drupal_render($ref_link);
  }

  $output .= drupal_render($form['cluster_id'] );

  $output .= drupal_render_children($form);

  return $output;
}


function cloud_cluster_display_server_list_submit($form_id, $form_values) {

  $form_values = $form_values['values'];

  if ( $form_values['op'] == t('Stop All Servers')) {

    _cloud_cluster_stop_all_servers($form_values['cluster_id'], 'stop_all' );
    drupal_goto(CLOUD_CLUSTER_PATH . '/describe/servers/' . $form_values['cluster_id'] );
    return;
  }

  return;
}


function _cloud_cluster_get_data($cluster_id) {

  cloud_update_all_cloud_data();
  drupal_goto( CLOUD_CLUSTER_PATH . '/describe/servers/' . $cluster_id  );

  return;
}


function _cloud_cluster_describe() {

  drupal_goto( CLOUD_CLUSTER_PATH );

  return;
}


/**
 * Delete a server from Cluster
 *
 * @param $form_values
 * @param $cluster_id
 *              Cluster from which server is to be deleted
 * @param $server_id
 *              Server to be deleted
 * @return <type>
 */
function _cloud_cluster_delete_server($form, $form_values, $cluster_id = '', $server_id = '') {

  $run_count = _cloud_cluster_get_running_instance_count($cluster_id, $server_id);

  if ($run_count > 0 ) {

    drupal_set_message(t('Cannot delete template. Instances running for this Template'), 'error');
    drupal_goto(CLOUD_CLUSTER_PATH . '/describe/cluster_templates/' . $cluster_id );

    return;
  }

  _cloud_cluster_delete_server_db($server_id);
  drupal_set_message(t('Template deleted from Cluster'));
  drupal_goto(CLOUD_CLUSTER_PATH . '/describe/cluster_templates/' . $cluster_id );

  return;
}


/**
 * Launch a server from Cluster
 *
 * @param $page_submit
 * @param $cluster_id
 *          Cluster from which server is to be launched
 * @param $server_id
 *          Server which is to be launched
 * @return
 */
function _cloud_cluster_launch_server($form, $page_submit = '', $cluster_id = '', $server_id = '' ) {

  $query = _cloud_cluster_get_server_query();
  $query_args = array(
    ':server_id' => $server_id,
  );

  $launch_params = array();
  $result        = db_query( $query, $query_args );

  $cluster_id  = FALSE;
  $server_name = '';
  $template_id = '';

  $server_obj = NULL;

  foreach ($result as $depl) {

    $launch_params['template_id'   ] = $depl->template_id           ;
    $launch_params['nickname'      ] = $depl->server_nickname       ;
    $launch_params['key_name'      ] = $depl->server_sshkey         ;
    $launch_params['security_group'] = $depl->server_security_group;
    $launch_params['placement'     ] = $depl->server_zone           ;


    if ( isset($depl->instance_id)
     &&    empty($depl->instance_id) === FALSE ) {

      $launch_params['count_instances'] = 1;
    }

    $cluster_id  = $depl->cluster_id      ;
    $server_name = $depl->server_nickname;
    $template_id = $depl->template_id     ;

    $server_obj  = $depl                  ;
    $instance_id = $depl->instance_id     ;

  }

  if (!$cluster_id || empty($template_id) ) { // The server configuration not found or the server was deleted
    drupal_set_message(t('Server ID not found'), 'error' );
    drupal_goto(CLOUD_CLUSTER_PATH );

    return;
  }

  $cloud_context = _cloud_server_templates_get_cloud($template_id);

  // Launch the instance
  $result_launch = cloud_perform_action( '', 'launch_using_params', $cloud_context, $launch_params );


  if (count($result_launch)) { // Successfully launched the instance
    //$new_instance_ids = isset( $_REQUEST['new_instance_ids'] ) ? $_REQUEST['new_instance_ids'] : array();

//     if ( isset($new_instance_ids[0]) ) {
//       $new_instance_id = $new_instance_ids[0];
//     }
//     else {
//       $new_instance_id = FALSE;
//     }

//     if (empty($instance_id)) {

//       $instance_id = $new_instance_id;
//     }
//     else {

//       $instance_id = str_replace( $_REQUEST['instance_id'], $new_instance_id, $instance_id);
//     }
    
    $instsance_id = array_shift($result_launch);
    $update_query = _cloud_cluster_update_cluster_server_query($instance_id, $server_id);

    drupal_set_message( check_plain(t('Server launched: @server_name', array('@server_name' => $server_name) ) ) );
    drupal_goto(CLOUD_CLUSTER_PATH . '/describe/servers/' . $cluster_id );
  }
  else {

    // Failed launch instance
    drupal_set_message( check_plain(t('Server launch failed: @server_name', array('@server_name' => $server_name) ) ), 'error' );
    drupal_goto(CLOUD_CLUSTER_PATH . '/describe/servers/' . $cluster_id );
  }

  return;
}


/**
 * Launch all the server's present in a Cluster
 *
 * @param $page_submit
 * @param $cluster_id
 *          Cluster from which server's are to be launched
 * @return
 */
function _cloud_cluster_launch_server_all($form, $page_submit = '', $cluster_id = '' ) {

  if (empty($cluster_id) ) {

    drupal_goto(CLOUD_CLUSTER_PATH);

    return;
  }


  $query      = _cloud_cluster_servers_get_query();
  $query->condition('cluster_id', $cluster_id, '=');
  $result     = $query->execute();

  // Get the list of all instances
  $cloud_list = cloud_get_all_clouds();

  $all_instances_list = array();
  $filter_params = array();
  foreach ($cloud_list as $cloud_context) {

    $filter_params['filter_state'] = 'all';
    $instances_list                = cloud_get_instances($cloud_context, $filter_params );
    $all_instances_list            = array_merge( $all_instances_list, $instances_list );
  }

  $runningserver_cnt = 0;

  foreach ($result as $depl) { // Cluster server object
    $instance_ids_str = $depl->instance_id;
    if (empty($instance_ids_str) ) { // Launch the instance
      $state = '';
    }
    else {

      $instance_ids = explode( ','  , $instance_ids_str );
      foreach ($instance_ids as $instance_id ) {

        if (isset($all_instances_list[$instance_id]) ) {

          $instance_info = $all_instances_list[$instance_id];
          $state         = $instance_info['state'];
          if ( drupal_strtolower($state) === 'running') { // Found a ruuning instance Skip this instance
            $runningserver_cnt += 1;
            continue;
          }
        }
      }
    }

    $server_obj = NULL;

    // Launch instance
    $launch_params = array();
    $launch_params['template_id'   ] =  $depl->template_id           ;
    $launch_params['nickname'      ] =  $depl->server_nickname       ;
    $launch_params['key_name'      ] =  $depl->server_sshkey         ;
    $launch_params['security_group'] =  $depl->server_security_group;
    $launch_params['placement'     ] =  $depl->server_zone           ;

    $server_name                     =  $depl->server_nickname       ;
    $template_id                     =  $depl->template_id           ;
    $server_id                       =  $depl->server_id             ;

    $server_obj                      = $depl;

    $cloud_context = _cloud_server_templates_get_cloud($template_id);

    // Launch the instance
    $result_launch = cloud_perform_action( '', 'launch_using_params', $cloud_context, $launch_params );


    if ($result_launch) { // Successfully launched the instance
      //$new_instance_ids = isset( $_REQUEST['new_instance_ids'] ) ? $_REQUEST['new_instance_ids'] : array();
      $instance_id  = implode(',', $result_launch);
      $update_query = _cloud_cluster_update_cluster_server_query($instance_id, $server_id);

    }
    else {

      // Failed launch instance
      drupal_set_message( check_plain(t('Server launch failed: @server_name', array('@server_name' => $server_name) ) ), 'error' );
      drupal_goto(CLOUD_CLUSTER_PATH . '/describe/cluster_templates/' . $cluster_id );
      return;
    }
  }

  drupal_goto(CLOUD_CLUSTER_PATH . '/describe/servers/' . $cluster_id );
}


/**
 * Delete a Cluster
 * Check whether any Server is running if a server is running do not delete the Cluster.
 *
 * @param $form_values
 * @param $cluster_id
 *              Cluster to be deleted
 * @return
 */
function _cloud_cluster_delete_cluster($form, $form_values, $cluster_id = '') {

  $run_count = _cloud_cluster_get_running_instance_count($cluster_id);

  if ( $run_count > 0 ) {

    drupal_set_message(check_plain('Cannot delete cluster. Instance(s) of cluster are still active. '),   'error');

    if (isset($form_values['cluster_id']) && $form_values['cluster_id'] == $cluster_id ) {

      drupal_goto(CLOUD_CLUSTER_PATH . '/describe/info/' . $cluster_id );
    }
    else {

      drupal_goto(CLOUD_CLUSTER_PATH);
    }
  }
  else {

    _cloud_cluster_delete_by_id($cluster_id)  ;
    drupal_set_message(t('Cluster deleted successfully'));
    drupal_goto(CLOUD_CLUSTER_PATH);
  }
}


/**
 * Add a server to a Cluster
 *
 * @param $cloud_context
 *        Server added from this Sub-cloud
 * @param $cluster_id
 *        Cluster to which server is to be added
 * @return
 */
function _cloud_cluster_add_server( $cloud_context, $cluster_id) {

  if (empty($cluster_id) ) {

    drupal_goto(CLOUD_CLUSTER_PATH);

    return;
  }

  $query      = _cloud_cluster_get_cluster_by_id_query();
  $query_args = array(
    ':cluster_id' => $cluster_id,
  );

  $result = db_query( $query, $query_args );
  $count = 0;

  $cluster_nickname = '';

  foreach ($result as $key) {

    $cluster_nickname = $key->cluster_nickname;
    $count++;
  }

  $params = array();
  $params['cluster_id'      ] = $cluster_id       ;
  $params['cluster_nickname'] = $cluster_nickname;


  $cloud_list = cloud_get_all_clouds_display_name();
  $is_cloud_enabled = cloud_is_settings_done($cloud_context);
  if ($is_cloud_enabled === FALSE ) {
    cloud_set_config_error_message($cloud_list[$cloud_context], $cloud_context);
  }
  else {

    $tmpl_active_count = _cloud_server_templates_get_active_template_count_by_cloud();
    if ( isset($tmpl_active_count[$cloud_context]) == FALSE ||   $tmpl_active_count[$cloud_context] < 1 ) {

      $template_url = filter_xss( l( t('Design - Template'), 'design/server_templates/' . $cloud_context . '/list' ) );
      drupal_set_message(check_plain(t('There are no Templates for @cloud_name > ', array('@cloud_name' => $cloud_list[$cloud_context]))) . $template_url );
    }

  }

  return cloud_server_templates_notify( '', 'cluster_form', $cloud_context, $params );
}
