<?php

/**
 * @file
 * Instance UI file
 *
 * Copyright (c) 2010-2011 DOCOMO Innovations, Inc.
 *
 */

/**
 * Updated by yas     2015/05/06
 * Updated by yas     2015/03/25
 * Updated by Abhijit 2011/09/08
 * Updated by Abhijit 2011/09/07
 * Updated by Jiten   2011/07/06
 * Updated by Jiten   2011/06/24
 * Updated by Jamir   2011/04/30
 * Updated by yas     2011/03/24
 * Updated by yas     2011/03/14
 * Updated by Jamir   2011/03/14
 * Updated by yas     2011/02/17
 * Updated by yas     2011/02/14
 * Updated by yas     2011/02/11
 * Updated by yas     2011/02/10
 * Updated by yas     2011/02/08
 * Updated by yas     2011/02/03
 * Updated by yas     2011/02/02
 */

/**
 * Function lists all instances in a particular cloud_context
 * @param     $form_submit
 * @param     $cloud_context
 * @return
 */
function aws_cloud_display_instances_list($form, $form_state = '', $cloud_context) {

  $column = 'instance_nickname';

  drupal_add_js(cloud_get_module_base() . 'js/cloud.js');
  drupal_add_js(drupal_get_path('module', 'aws_cloud') . CLOUD_PATH_SEPARATOR . 'js/aws_cloud_auto_refresh_list_instances.js');

  $options = array(
    t('Nickname'),
    t('ID'      ),
    t('Zone'    ),
    t('State'   ),
    t('User'    ),
    t('Hostname'),
  );

  $filter     = isset($_REQUEST['filter'   ]) ? $_REQUEST['filter'   ] : '' ;
  $filter_col = isset($_REQUEST['operation']) ? $_REQUEST['operation'] : 0  ; // default: Nickname
  $filter     = trim($filter);

  if ( $filter_col == 0 ) {
    $column  = 'Nickname'           ;
    $sql_col = 'instance_nickname'  ;
  }
  elseif ($filter_col == 1 ) {
    $column  = 'AWS-ID'             ;
    $sql_col = 'c.instance_id'      ;
  }
  elseif ($filter_col == 2 ) {
    $column  = 'Availability Zone'  ;
    $sql_col = 'placement'          ;
  }
  elseif ($filter_col == 3 ) {
    $column  = 'State'              ;
    $sql_col = 'instance_state_name';
  }
  elseif ($filter_col == 4 ) {
    $column  = 'User'               ;
    $sql_col = 'key_name'           ;
  }
  elseif ($filter_col == 5 ) {
    $column  = 'Public-DNS-Name'    ;
    $sql_col = 'dns_name'           ;
  }
  elseif ($filter_col == 6 ) {
    $column  = 'Private-DNS-Name'   ;
    $sql_col = 'private_dns_name'   ;
  }

  if ( isset($filter) ) {
    $query_args[] = $sql_col;
    $query_args[] = $filter  ;
    $query_args[] = $cloud_context;
  }
  else {
    $filter       = ' 1 ';
    $query_args[] = ' '   ;
    $query_args[] = $cloud_context;
  }

  $form['options'] = array(
    '#prefix' => '<div class="container-inline">',
    '#type' => 'fieldset',
    '#suffix' => '</div>',
  );

  $instance_count = _aws_cloud_get_instances_list_count($cloud_context);
  $filter_disabled = '';
  if ( $instance_count < 2 ) {
    $filter_disabled = TRUE;
  }

  $form['options']['label'    ] = array(
    '#type'  => 'item'     ,
    '#title' => t('Filter'),
  );
  $form['options']['operation'] = array(
    '#type'          => 'select'        ,
    '#options'       => $options        ,
    '#default_value' => $filter_col     ,
    '#disabled'      => $filter_disabled,
  );
  $form['options']['filter'   ] = array(
    '#type'          => 'textfield'     ,
    '#size'          => 40              ,
    '#default_value' => $filter         ,
    '#disabled'      => $filter_disabled,
  );
  $form['options']['submit'   ] = array(
    '#type'          => 'submit'        ,
    '#value'         => t('Apply')      ,
    '#disabled'      => $filter_disabled,
  );

  $form['header'] = array(
    '#type' => 'value',
    '#value' => array(
      array('data' => t('Nickname'), 'field' => 'instance_nickname'  , 'class' => array('nickname-column')),
      array('data' => t('ID'      ), 'field' => 'instance_id'                                             ),
      array('data' => t('Hostname'), 'field' => 'dns_name'                                                ),
      array('data' => t('State'   ), 'field' => 'instance_state_name'                                     ),
      array('data' => t('Zone'    ), 'field' => 'placement'          , 'sort' => 'asc'                    ),
      array('data' => t('User'    ), 'field' => 'key_name'                                                ),
      array('data' => t('Runtime' ), 'field' => 'launch_time'                                             ),
      array('data' => t('Action'  ),                                   'class' => array('action-column')  ),
    ),
  );

  if (empty($_REQUEST['sort']) && !$filter) {

    $query = _aws_cloud_get_instances_list_query($cloud_context);
    $query->condition('c.cloud_type', $cloud_context, '=');
    $result = $query->execute();

  }
  else {

    $query  = _aws_cloud_get_instances_list_query($cloud_context);
    $query->condition('c.cloud_type', $cloud_context, '=');

    if ( isset($filter) ) {
      $query->condition($sql_col, '%' . $filter . '%', 'like');
    }

    tablesort_init($form['header']);

    $query
      ->extend('TableSort')
      ->orderByHeader($form['header']['#value']);

    $result = $query->execute();
  }

  $destination = drupal_get_destination();
  $lock_status = _aws_cloud_get_instances_lock_status($cloud_context);

  $cloud_menu_path = cloud_get_menu_path($cloud_context );

  foreach ($result as $instance) {

    $form['Nickname'][$instance->instance_id] = array(
      array('#markup' => l( $instance->instance_nickname,
            $cloud_menu_path . '/instances/describe/' . $instance->instance_id . '/info',
            array('query' => array('instance_id' => $instance->instance_id)) )
                                            . (( !empty($lock_status[$instance->instance_id])
                                            &&          $lock_status[$instance->instance_id] == 'Lock' )
                                            ? '<a class=lockIcon title=Lock>&nbsp;&nbsp;&nbsp;&nbsp;</a>'
                                            : '' )),
    );
    
    $form['keyUser'][$instance->instance_id] = array(
      array('#markup' => l( $instance->key_name,  
            $cloud_menu_path . '/ssh_keys/describe',  
            array('query' => array('key_name' => $instance->key_name )))),
    );

    $form['hdnName'][$instance->instance_id] = array(
      '#type' => 'hidden',
      '#value' => addslashes($instance->instance_nickname),
    );
    $form['ID'      ][$instance->instance_id] = array(
      array('#markup' => t( substr($instance->instance_id, 0, 10) )),
    );

    if (isset($instance->image_nickname) == FALSE
    ||  empty($instance->image_nickname) ) {
      $instance->image_nickname = $instance->image_id;
    }

    $status     = $instance->instance_state_name;
    $status     = _aws_cloud_get_instance_status($instance->instance_id, $status, $cloud_context );

    $public_dns = substr( $instance->dns_name, 0, strpos($instance->dns_name,    '.') );
    if ( strlen($public_dns) > 3 ) { // truncate only in case the name is long
      $public_dns = substr($instance->dns_name, 0, strpos($instance->dns_name,    '.')) . '...';
    }
    else {
      $public_dns = $instance->dns_name;
    }
    
    $form['State'     ][$instance->instance_id] = array('#type'   => 'hidden', '#value' => t( $instance->instance_state_name ));
    $form['Status'    ][$instance->instance_id] = array('#markup' => t(ucwords($instance->instance_state_name)));
    $form['Public_DNS'][$instance->instance_id] = array('#markup' => l($public_dns, 'http://' . $instance->dns_name, array('attributes' => array('target' => '_blank'), 'query' => array())));
    $form['Zone'      ][$instance->instance_id] = array('#markup' => t($instance->placement));
    $form['Runtime'   ][$instance->instance_id] = array('#markup' => t(cloud_get_runtime($instance->launch_time)));
    $form['KeyName'   ][$instance->instance_id] = array('#type'   => 'hidden', '#value' => $instance->key_name);
    
    $instance_info = array(
      'instance_id' => $instance->instance_id        ,
      'name'        => $instance->instance_nickname  ,
      'state'       => $instance->instance_state_name,
      'user'        => $instance->key_name           ,
    );
    // icons are rendered in its own reusable function
    $form['ListAction'][$instance->instance_id] = array('#type' => 'markup', '#markup' => aws_cloud_get_instances_action($cloud_context, $instance_info));
  }

  $form['filter_hdn'   ] = array(
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
  $form['pager'        ] = array('#value' => theme('pager', array('tags' => NULL, 'element' => 0)));
  $form['#redirect'    ] = FALSE;

  return $form;
}

/**
 * Theme function for display instance list
 */
function theme_aws_cloud_display_instances_list($form) {

  if (isset($form['form'])) { // drupal 7 passed form as an array
    $form = $form['form'];
  }

  $cloud_context = $form['cloud_context']['#value'];
  $lock_status   = _aws_cloud_get_instances_lock_status($cloud_context);
  $user_keys_all =  aws_cloud_get_all_keys($cloud_context);

  $output                  = drupal_render($form['options']);
  $cloud_menu_path         = cloud_get_menu_path($cloud_context);
  $form['Nickname']        = !empty($form['Nickname']       ) ? $form['Nickname']        : array();
  $form['pager']['#value'] = !empty($form['pager']['#value']) ? $form['pager']['#value'] : '';

  $rows = array();

  foreach (element_children($form['Nickname']) as $key) {

    $lock_image = '';

    if ( !empty($lock_status[$key]) && $lock_status[$key] == 'Lock') {

      $src_img_path = 'images/lock';
      $path         = cloud_get_module_base() . $src_img_path;
      $image_info   = image_get_info( $path );

      $msg = '';

      $attributes = array('class'  => 'image image-$label ' . (isset($attributes['class']) ? $attributes['class'] : ''),
                          'width'  => $image_info['width' ],
                          'height' => $image_info['height'],
                    );
/*
      $out = theme( 'image', array(
          'path'       => $path,
          'alt'        => $msg,
          'title'      => $msg,
          'width'      => $image_info['width'],
          'height'     => $image_info['height'],
          'attributes' => array('class' => $attributes['class']),
      ));
*/
    }

    $rows[] = array(
      array('data' => drupal_render($form['Nickname'  ][$key]), 'class' => array('nickname-column')),
                      drupal_render($form['ID'        ][$key]                                      ),
                      drupal_render($form['Public_DNS'][$key]                                      ),
                      drupal_render($form['Status'    ][$key]                                      ),
                      drupal_render($form['Zone'      ][$key]                                      ),
                      drupal_render($form['keyUser'   ][$key]                                      ),
                      drupal_render($form['Runtime'   ][$key]                                      ),
      array('data' => drupal_render($form['ListAction'][$key]), 'class' => array('action-column'  )),
    );
  }

  $table_attr = array();
  $table_attr['id'] = 'instances_list_table';
  $table_attr['autoupdate_url'] = url($cloud_menu_path . '/callback_get_instances_list' );

  $output .= theme('table', array('header' => $form['header']['#value'], 'rows' => $rows, 'attributes' => $table_attr));

  $output .= drupal_render($form['cloud_context']);
  $output .= drupal_render($form['filter_hdn'   ]);
  $output .= drupal_render($form['operation_hdn']);

  if ($form['pager']['#value']) {
    $output .= drupal_render($form['pager']);
  }

  $destination = drupal_get_destination();
  $reload_link = l( t('- Refresh Page -'), $cloud_menu_path . '/get_instances_data',    array('query' => $destination) );
  $ref_link    = array(
    '#type' => 'item',
    '#prefix' => '<div id="link_reload" align="right">',
    '#markup' => $reload_link,
    '#suffix' => '</div>',
  );
  $output .= drupal_render($ref_link);
  $output .= drupal_render_children($form);

  return $output;
}

/**
 * Page that replaces console.inc.  This gives drupal
 * control of the variables.  The dom does not have to be passed
 * using awkward javascript document.write() anymore. 
 */
function aws_cloud_console_popup($cloud_context, $instance_id) {
  $output = '';
  $result = aws_cloud_get_key_material_by_id($cloud_context, $instance_id);
  $key_file_name = bin2hex($result[0]);
  $instance_ip   = $result[2];
  $key_material  = $result[1];
  $ssh_user_name = aws_cloud_get_ssh_user($cloud_context, $instance_id);
  $mind_term_jar = base_path() . AWS_CLOUD_SSH_CLIENT_JAR_FILE;
  $cloud_key_jar = base_path() . drupal_get_path('module', 'cloud') . '/CloudSSHKeyManager.jar';
  
  if (empty($key_material)) {
    $output = <<<EOF
    <!DOCTYPE html PUBLIC "-//W3C//DTD XHTML+RDFa 1.0//EN" "http://www.w3.org/MarkUp/DTD/xhtml-rdfa-1.dtd">
    <html>
      <body>
        Key not found for the user <b>' . $ssh_user_name . '. </b> <br>
        Please set the key first.
      <body>
    </html>
    
EOF;
    
  }
  else {
    $file_url = url("clouds/$cloud_context/ssh_keys/key_material/$key_name", array('absolute' => TRUE));
    $browser_cookie = session_name() . '=' . session_id();
    
    $output = <<<EOF
    <!DOCTYPE html PUBLIC "-//W3C//DTD XHTML+RDFa 1.0//EN" "http://www.w3.org/MarkUp/DTD/xhtml-rdfa-1.dtd">
    <html>
    <body>
      <h1>Use the Console window</h1>
      <input type="button" value="Close Window" onClick="javascript:window.close();">
      <applet archive="$cloud_key_jar" code="com.clanavi.CloudSSHKeyManager.class" width="0" height="0">
        <param name="file_url" value="$file_url">
        <param name=private-key value="$private_key">
        <param name=browser_cookie value="$browser_cookie">
      </applet>
      <applet archive="$mind_term_jar" code=com.mindbright.application.MindTerm.class width="0" height="0">
        <param name="server" value="$instance_ip">
        <param name="username" value="$ssh_user_name">
        <param name="port" value="22">
        <param name="sepframe" value="true">
        <param name="private-key" value="$key_file_name">
        <param name="auth-method" value="publickey">
        <param name="quiet" value="true">
        <param name="verbose" value="true">
        <param name="debug" value="true">
        <param name="autoprops" value="both">
        <param name="geometry" value="100x35">
      </applet>
    </body>
    </html>
    
EOF;
    
  }
  print $output;
  exit;
}

/**
 * Helper function to generate icons for instances. 
 * Operations such as reboot, terminate, console and edit
 * @param unknown_type $cloud_context
 * @param unknown_type $instance_info_arr
 */
function aws_cloud_get_instances_action($cloud_context, $instance_info) {
  $action_data = array();
  
  $cloud_menu_path = cloud_get_menu_path( $cloud_context ) ;
  $user_keys_all = aws_cloud_get_all_keys($cloud_context);
  $instance_id = $instance_info['instance_id'];
  $instance_name = trim($instance_info['name']) ;
  $state = $instance_info['state'] ;
  $key_name = $instance_info['user'] ;

  $action_data[] = cloud_display_action( 'images/icon_clear', t('Edit'), $cloud_menu_path . '/instances/describe/' . $instance_id . '/info', array('query' => array('instance_id' => $instance_id), 'html' => TRUE) );
  
  if ($state != 'terminated' && $state != 'stopped') {
    
    $console_url = url($cloud_menu_path . '/instances/describe/' . $instance_id . '/console_popup', array('absolute' => TRUE));
    $prop_reboot['onclick'] = cloud_get_messagebox('Are you sure you want to reboot the instance "' . $instance_name . '" ?');
    $prop['onclick'] = cloud_get_messagebox('Are you sure you want to terminate the instance "' . $instance_name . '" ?');
    $prop_monitor['onclick'] = cloud_get_messagebox('Are you sure you want to monitor the instance "' . $instance_name . '" ?');
    $prop_not_monitor['onclick'] = cloud_get_messagebox('Are you sure you want turn off monitoring the instance "' . $instance_name . '" ?');
    $prop_console['onclick'] = "popWin = open('$console_url', 'Console', 'width=300, height=300');  popWin.focus(); return false";
    
    if (user_access($cloud_context . ' access all console')) {
      $action_data[] = cloud_display_action( 'images/icon_ssh', 'Console', $console_url, array('html' => TRUE), $prop_console['onclick']); 
    }
    elseif (user_access($cloud_context . ' access own console')) {
      if (in_array($key_name, $user_keys_all)) {
        $action_data[] = cloud_display_action( 'images/icon_ssh', 'Console', $console_url, array('html' => TRUE), $prop_console['onclick']);
      }
    }

    if (isset($lock_status[$instance_id]) == FALSE || $lock_status[$instance_id] == 'Unlock') {
      if (user_access($cloud_context . ' terminate all instances') ) {
        $action_data[] = cloud_display_action( 'images/icon_reboot', t('Reboot'   ), $cloud_menu_path . '/instances/reboot', array('query' => array('instance_id' => $instance_id, 'name' => $instance_name), 'html' => TRUE), $prop_reboot['onclick']);
        $action_data[] = cloud_display_action( 'images/action_stop', t('Terminate'), $cloud_menu_path . '/instances/terminate', array('query' => array('instance_id' => $instance_id, 'name' => $instance_name), 'html' => TRUE), $prop['onclick']); 
      }
      elseif (user_access($cloud_context . ' terminate own instance')) {
        if (in_array($key_name, $user_keys_all)) {
          $action_data[] = cloud_display_action('images/icon_reboot', t('Reboot'   ), $cloud_menu_path . '/instances/reboot', array('query' => array('instance_id' => $instance_id, 'name' => $instance_name), 'html' => TRUE), $prop_reboot['onclick']);
          $action_data[] = cloud_display_action('images/action_stop', t('Terminate'), $cloud_menu_path . '/instances/terminate', array('query' => array('instance_id' => $instance_id, 'name' => $instance_name), 'html' => TRUE), $prop['onclick']);
        }
      }
    }
  }

  $user_actions = module_invoke_all('aws_cloud_instances_action_data', $instance_info);
  $action_data = array_merge($action_data, $user_actions);
  return implode($action_data);
}

/**
 *
 * @param     $form_id
 * @param     $form_values
 * @return
 */
function aws_cloud_display_instances_list_submit($form_id, &$form_state) {

  $form_values   = $form_state['values'       ];
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
 *
 * @param     $cloud_context
 * @param     $key_name
 * @param     $key_material
 * @param     $instance_ip
 * @return
 */
function _aws_cloud_create_key_file( $cloud_context, $key_name, $key_material, $instance_ip) {

  if (empty($key_material)) {
    return '';
  }

  $output_dir = _aws_cloud_prepare_directory();
  if ($output_dir != FALSE) {
    // Clean up temporary files
    if ($handle = opendir($output_dir)) {
      while (false !== ($file = readdir($handle))) {
        if ($file != '.'
          &&    $file != '..') {
          $part = split('_', $file); // get timestamp = $part[0]
          if ($part[0] < (REQUEST_TIME - 600) && !is_dir( "{$output_dir}/{$file}")) { // file created before 10 min (600 sec)
            @unlink("$output_dir/$file"); // delete
          }
        }
      }
    }
    closedir($handle);
  
    $secret_key = cloud_get_info( $cloud_context, 'aws_secret_key' )  ;
  
    $filename = $output_dir . '/'
              . REQUEST_TIME . '_' . sha1( $_SERVER['HTTP_HOST']
                             . REQUEST_TIME
                             . $key_name
                             . $instance_ip
                             . AWS_CLOUD_SEED
                             . $secret_key
                             );
  
    $fh = fopen($filename, 'w');
    if ( $fh == FALSE) {
      return '';
    }
  
    fwrite($fh, $key_material );
    fclose($fh);
  
    return $filename;
  }
}

/**
 * AJAX Callback for Instance List 
 */
function _aws_cloud_callback_get_instances_list($cloud_context) {
  $form = drupal_get_form('aws_cloud_display_instances_list', $cloud_context);
  $output = theme('aws_cloud_display_instances_list', $form);
  // Send only the body do not send the headers
  // @todo: Render the form properly, without stripping the rendered output
  $index_start = strrpos( $output, '<tbody>'  );
  $index_end   = strrpos( $output, '</tbody>' );
   if ( isset($form['Nickname']) === FALSE || sizeof($form['Nickname']) == 0 ) { // No element present
     $output = 'NULL';
   }
   else {
    $output = substr( $output, $index_start, $index_end - $index_start );
    $output .= '</tbody>';
  }
  
  print drupal_json_encode(array('html' => $output));
  exit();
}

/**
 * Reboot an instance given the instance_id
 * @param     $cloud_context
 * @return
 */
function aws_cloud_reboot_instance($cloud_context, $params = array()) {

  if (empty($params)) {
    $instance_id = isset( $_REQUEST['instance_id']) ? $_REQUEST['instance_id'] : FALSE;
  }
  else {
    $instance_id = $params[0];
  }
  $instance_name = isset($_REQUEST['name']) ? $_REQUEST['name'] : '';

  // if instance_name is empty, try to load it from database
  if (empty($instance_name)) {
    $instance_info = _aws_cloud_get_instance_details_by_id($cloud_context, $instance_id);
    if (empty($instance_info) === FALSE) {
      $instance_name = $instance_info['name'];
    }
  }
  // reboot instance and print results
  $result = _aws_cloud_reboot_instance($cloud_context, array($instance_id));
  if ($result) {
    drupal_set_message(t('Instance rebooting: @instance_name. This will take some time.', array('@instance_name' => $instance_name)));
  }
  else {
    drupal_set_message(t('There was a problem while trying to reboot the instance: @instance_name', array('@instance_name' => $instance_name) ), 'error');
  }

  if (empty($params)) { // Came from instances page
    $cloud_menu_path = cloud_get_menu_path($cloud_context);
    drupal_goto($cloud_menu_path . '/instances');
  }
  return;
}


/**
 * Terminate a instance
 * @param     $cloud_context
 * @return
 */
function aws_cloud_terminate_instance($cloud_context, $params = array()) {
  $cloud_menu_path = cloud_get_menu_path($cloud_context);
  
  if (empty($params)) {
    $instance_id = isset($_REQUEST['instance_id']) ? $_REQUEST['instance_id'] : FALSE;
  }
  else {
    $instance_id = $params[0];
  }

  $instance_name = isset($_REQUEST['name']) ? $_REQUEST['name'] : '';
  
  if (empty($instance_name)) {
    $instance_info = _aws_cloud_get_instance_details_by_id($cloud_context, $instance_id);
    if (empty($instance_info) === FALSE) {
      $instance_name = $instance_info['name'];
    }
  }
  // @todo: add a pre terminate hook and move back into cloud_failover module
  if (module_exists('cloud_failover')) {
    $fail_over_flag = cloud_failover_get_failover_flag($cloud_context, $instance_id);
    if ($fail_over_flag != CLOUD_FAILOVER_FLAG_NO_FAILOVER) {
      drupal_set_message( t('You cannot terminate a server while failover enabled: @instance_name', array('@instance_name' => $instance_name) ),    'error' );
      drupal_goto($cloud_menu_path . '/instances');
    }
  }

  // call terminate function
  $result = _aws_cloud_terminate_instance($cloud_context, $instance_id);
  if ($result == TRUE) {
    drupal_set_message(t('Instance terminating: @instance_name', array('@instance_name' => $instance_name)));
  }
  else {
    drupal_set_message(t('There was a problem while trying to terminate the instance: @instance_name', array('@instance_name' => $instance_name)), 'error');
  }

  $cloud_menu_path = cloud_get_menu_path($cloud_context);

  if (empty($params)) { // Came from instances page
    drupal_goto($cloud_menu_path . '/instances');
  }

  return;
}


/**
 *
 * @param     $cloud_context
 * @return
 */
function aws_cloud_monitor_instance($cloud_context) {

  $instance_id   = isset( $_REQUEST['instance_id'] ) ? $_REQUEST['instance_id'] : FALSE;
  $instance_name = isset( $_REQUEST['name'       ] ) ? $_REQUEST['name'       ] : ''    ;
  $action        = isset( $_REQUEST['action'     ] ) ? $_REQUEST['action'     ] : ''    ;

  if ($action == 'Monitor') {

    $script_id = cloud_script_operation($cloud_context, 'get_master_script_id', array('script_type' => 'MONITOR_SCRIPT') )  ;

    if ( empty($script_id) || $script_id == '' ) {

      drupal_set_message( t('Master script not set fo Monitor. Pelase set it first') );
      $cloud_menu_path = cloud_get_menu_path($cloud_context );
      drupal_goto($cloud_menu_path . '/instances');
      return;
    }

    $check_record_query      = _cloud_scripting_get_by_script_id_instance_id();
    $check_record_query_args = array(
      ':instance_id' => $instance_id,
      ':script_id'   => $script_id,
    );

    $result_check_record = db_query( $check_record_query, $check_record_query_args );

    $found = FALSE;

    foreach ($result_check_record as $result_data) {

      $found = TRUE;
    }

    // If the record is not found then insert it in the database so that script will be executed
    if ($found == FALSE ) {

      _cloud_scripting_register_script_save($instance_id, $script_id,    '0', $cloud_context,    '' );
    }

    aws_cloud_instance_monitoring_status_update($cloud_context, $instance_id,    1);

    drupal_set_message(  check_plain(t('Monitoring turned on for instance $instance_name.  This will require some time.') ) );

  }
  else {

    drupal_set_message(  check_plain( t('Monitoring turned OFF for instance $instance_name. This will require some time.') ) );
    aws_cloud_instance_monitoring_status_update($cloud_context, $instance_id,    0);
  }

  $cloud_menu_path = cloud_get_menu_path($cloud_context );
  drupal_goto($cloud_menu_path . '/instances');

  return;
}

/**
 * Fine available ip addresses.
 * @param     $cloud_context
 * @param     $instance_id
 * @return
 */
function aws_cloud_get_elastic_ips($cloud_context, $instance_id) {
  $resp = array();
  // Get the available Elastic IP values
  $elastic_ip_options = array(
    AWS_CLOUD_CLOUD_NONE => AWS_CLOUD_CLOUD_NONE,
  );
  $default_rec = AWS_CLOUD_CLOUD_NONE;
  $default_ip  = '';

  $query  = _aws_cloud_get_describe_elastic_ips_query($cloud_context);
  $result = $query->execute();

  foreach ($result as $elastic_ip) {
    if (empty($instance_id) == FALSE && $elastic_ip->instance_id == $instance_id ) {
      $default_rec =  'org_value';
      if ($elastic_ip->public_ip_name == '- changeme -') {
        $elastic_ip_options['org_value'] = $elastic_ip->public_ip;
      }
      elseif ($elastic_ip->public_ip_name === $elastic_ip->public_ip) {
        $elastic_ip_options['org_value'] = $elastic_ip->public_ip;
      }
      else {
        $elastic_ip_options['org_value'] = $elastic_ip->public_ip_name . ' (' . $elastic_ip->public_ip . ')';
      }
      $default_ip = $elastic_ip->public_ip;
    }
    elseif (strlen($elastic_ip->instance_id) == 0 || $elastic_ip->instance_id == 'nobody') {
      if ($elastic_ip->public_ip_name == '- changeme -') {
        $elastic_ip_options[$elastic_ip->public_ip] = $elastic_ip->public_ip;
      }
      elseif ($elastic_ip->public_ip_name === $elastic_ip->public_ip ) {
        $elastic_ip_options[$elastic_ip->public_ip] = $elastic_ip->public_ip;
      }
      else {
        $elastic_ip_options[$elastic_ip->public_ip] = $elastic_ip->public_ip_name . ' (' . $elastic_ip->public_ip . ')';
      }
    }
  }
  $resp[0] = $elastic_ip_options;
  $resp[1] = $default_rec       ;
  $resp[2] = $default_ip        ;

  return $resp;
}

/**
 *
 * @param     $cloud_context
 * @param     $instance_id
 * @return
 */
function aws_cloud_get_lock_status($cloud_context, $instance_id) {

  $lock_query   = _aws_cloud_get_lock_status_query($cloud_context);
  $query_args = array(
    ':instance_id' => $instance_id  ,
    ':cloud_type'  => $cloud_context,
  );

  $lock_status = 'Unlock';

  $result      = db_query( $lock_query, $query_args );
  $count       = 0;

  foreach ($result as $key) {
    $lock_status = $key->is_locked;
  }

  return $lock_status;
}


/**
 * Display instance information.
 * This will display the details of an instance
 *
 * @param     $form_submit
 * @param     $cloud_context
 * @return
 */
function aws_cloud_display_instance_info($form_submit = '', $form_state, $cloud_context, $instance_id) {

  $cloud_menu_path = cloud_get_menu_path($cloud_context );
  if (empty($instance_id)) {
    drupal_set_message(t('No Instance Id Passed.'), 'error');
    drupal_goto( $cloud_menu_path . '/instances');
  }
  
  // query to get instance information
  $query = _aws_cloud_get_instance_query($cloud_context);
  $key = db_query($query, array(':instance_id' => $instance_id, ':cloud_type' => $cloud_context))->fetchObject();
  
  if (!$key) { 
    drupal_set_message(t('No Instance found'), 'error');
    drupal_goto( $cloud_menu_path . '/instances');
  }
  
  if ($key) {
    $instance_state_name = $key->instance_state_name;
    $instance_nickname   = cloud_get_filter_value( $form_submit, 'Name_text', $key->instance_nickname );
    $key_name            = $key->key_name;
    $lock_status = aws_cloud_get_lock_status($cloud_context, $key->instance_id);
    $elastic_ip_options = aws_cloud_get_elastic_ips($cloud_context, $key->instance_id);
    
    $form['instance_info_header'] = array(
        '#prefix' => '<h2>',
        '#markup' => t('Instance Info'),
        '#suffix' => '</h2>',
    );
    $form['fieldset_instance_info'] = array(
        '#type'  => 'fieldset',
        '#title' => t('Details'),
    );
    $form['Instance_Name']['Name_label'] = array(
      '#type'  => 'item',
      '#title' => t('Nickname'),
    );
    $form['Instance_Name_details']['Name_text'] =  array(
      '#type' => 'textfield',
      '#default_value' => t($instance_nickname),
    );
    if (!user_access($cloud_context . ' update instance details') || $lock_status == 'Lock') {
      $form['Instance_Name_details']['Name_text']['#disabled'] = TRUE;
    }
    $form['Instance_State'  ]['State_label'  ] = array(
      '#type'  => 'item',
      '#title' => t('State'),
    );
    $form['Instance_State'  ]['State_text'   ] = array('#markup' => ucwords($key->instance_state_name ));

    $form['Instance_pub_dns']['pub_dns_label'] = array(
      '#type'  => 'item',
      '#title' => t('Public DNS Name'),
    );
    $form['Instance_pub_dns']['pub_dns_text'] = array('#markup' => l(t( $key->dns_name),    'http://' . $key->dns_name,    array('attributes' => array('target' => '_blank'), 'query' => array()) ));

    $form['Instance_pvt_dns']['pvt_dns_label'] = array(
      '#type'  => 'item',
      '#title' => t('Private DNS Name'),
    );
    $form['Instance_pvt_dns']['pvt_dns_text'] = array('#markup' => t( $key->private_dns_name));

    $form['Instance_elastic_ip']['elastic_ip_label'] = array(
      '#type'  => 'item',
      '#title' => t('Current Elastic IP'),
    );

    $form['Instance_elastic_ip_details']['elastic_ip_text'] = array(
        '#type'          => 'select',
        '#options'       => $elastic_ip_options[0],
        '#default_value' => $elastic_ip_options[1],
    );
    
    if (!user_access($cloud_context . ' assign IP') || $lock_status == 'Lock' ) {
      $form['Instance_elastic_ip_details']['elastic_ip_text']['#disabled'] = TRUE;
    }

    $form['Instance_elastic_ip']['elastic_ip_text_default_val'] = array(
      '#type'  => 'hidden',
      '#value' => $elastic_ip_options[1],
    );
    $form['Instance_elastic_ip']['elastic_ip_text_default_ip'] = array(
      '#type'  => 'hidden',
      '#value' => $elastic_ip_options[2],
    );

    $form['Instance_lock']['lock_label'] = array(
      '#type'  => 'item',
      '#title' => t('Safety Status'),
    );

    $form['Instance_lock_details']['lock_text'] = array(
      '#type'    => 'select',
      '#options' => array(
        'Lock'   => t('Lock'),
        'Unlock' => t('Unlock'),
       ),
      '#default_value' => $lock_status,
    );
    
    if (!user_access($cloud_context . ' update instance details') ) {
      $form['Instance_lock_details']['lock_text']['#disabled'] = TRUE;
    }

    $form['Instance_aws']['aws_label'] = array(
      '#type' => 'item',
      '#title' => t('ID'),
    );
    $form['Instance_aws']['aws_text'] = array('#markup' => t( $key->instance_id ));

    $form['user_name_details']['user_name_label'] = array(
      '#type' => 'item'      ,
      '#title' => t('User Name'),
    );
    $user_name = aws_cloud_get_ssh_user($cloud_context, $key->instance_id);
    $user_name = cloud_get_filter_value( $form_submit, 'user_name_text', $user_name );
    
    $form['user_name_details']['user_name_text']  = array(
      '#type'          => 'textfield',
      '#size'          => 32,
      '#default_value' => $user_name,
    );
    
    if (!user_access($cloud_context . ' update instance details') || $lock_status == 'Lock' ) {
      $form['user_name_details']['user_name_text']['#disabled'] = TRUE;
    }

    $form['Instance_ssh']['ssh_label'    ] = array(
      '#type'  => 'item',
      '#title' => t('SSH Key'),
    );
    $form['Instance_ssh']['ssh_text'     ] =  array('#markup' => l($key->key_name, $cloud_menu_path . '/ssh_keys/describe', array('query' => array('key_name' => $key->key_name))));

    $form['Instance_sg']['sg_label'      ] = array(
      '#type'  => 'item',
      '#title' => t('Security group(s)'),
    );

    $sg_arr = explode( ',', $key->group_id );
    $sg_link_arr = '';

    foreach ($sg_arr as $sg_item) {
      //find the name mapping
      $group_name = _aws_cloud_get_security_group_name_by_id($cloud_context, $sg_item);
      if ($group_name != FALSE) {
        $sg_link_arr .= l( $group_name, $cloud_menu_path . '/security_groups/describe', array('query' => array('group_name' => $group_name)) ) . ', ';
      }
    }
    $sg_link_arr = substr( $sg_link_arr, 0, -1 );
    $sg_link_arr = rtrim($sg_link_arr, ',');
    
    $form['Instance_sg']['sg_text'       ] = array('#markup' => $sg_link_arr);

    $form['Instance_zone']['zone_label'  ] = array(
      '#type' => 'item',
      '#title' => t('Availability Zone'),
    );
    $form['Instance_zone']['zone_text'   ] = array('#markup' => t( $key->placement ));

    $form['Instance_type']['type_label'  ] = array(
      '#type' => 'item',
      '#title' => t('Instance type'),
    );
    $form['Instance_type']['type_text'   ] = array('#markup' => t( $key->instance_type ));

    $form['Instance_image']['image_label'] = array(
      '#type' => 'item',
      '#title' => t('Image'),
    );
    $form['Instance_image']['image_text'] = array('#markup' => t('@image_nickname (@image_id)', array(
          '@image_nickname' => $key->image_nickname,
          '@image_id' => $key->image_id,
        ) ));
    $form['Instance_owner']['owner_label'] = array(
      '#type' => 'item',
      '#title' => t('Owner'),
    );
    $form['Instance_owner']['owner_text'] = array('#markup' => t( $key->owner_id ));

    $form['Instance_Id'   ]['instance_id'] = array(
      '#type' => 'hidden',
      '#value' => $key->instance_id,
    );

    $launch_time = '';
    $runtime     = '';
    if (!empty($key->launch_time)) {
      $runtime     = cloud_get_runtime($key->launch_time);
      $launch_time = date_parse($key->launch_time);
      $launch_time = gmmktime( $launch_time['hour' ], $launch_time['minute'], $launch_time['second'],  
                               $launch_time['month'], $launch_time['day'   ], $launch_time['year'  ]);
      $launch_time = gmdate('Y-m-d H:i', $launch_time);
    }

    $form['instance_launch_time']['launch_time_label'] = array(
      '#type' => 'item',
      '#title' => t('Launch Time'),
    );
    $form['instance_launch_time']['launch_time_text']  = array('#markup' => t($launch_time));

    $form['instance_runtime']['runtime_label']         = array(
      '#type' => 'item',
      '#title' => t('Runtime'),
    );
    $form['instance_runtime']['runtime_text']          = array('#markup' => t($runtime));

    $form['save_button'] = array(
      '#type' => 'submit',
      '#value' => t('Update Details'),
      '#disabled' => user_access($cloud_context . ' update instance details') ? FALSE: TRUE,
    );
  

    $form['submit_buttons'] = array(
      '#type' => 'fieldset',
      '#prefix' => '<span class="clear"></span><div class="container-inline"><div class="buttons">',
      '#suffix' => '</div></div>',
    );
  
    $form['submit_buttons']['save_button'] = array(
      '#type' => 'submit',
      '#value' => t('Update Details'),
      '#disabled' => user_access($cloud_context . ' update instance details') ? FALSE: TRUE,
    );

    $user_keys_all         =  aws_cloud_get_all_keys($cloud_context);
    $confirm_reboot_msg    = cloud_get_messagebox(t('Are you sure you want to reboot the instance "@instance_nickname" ?'             , array('@instance_nickname' => $instance_nickname)));
    $confirm_terminate_msg = cloud_get_messagebox(t('Are you sure you want to terminate the instance "@instance_nickname" ?'          , array('@instance_nickname' => $instance_nickname)));

    if ($instance_state_name != 'terminated' && $instance_state_name != 'stopped' && $instance_state_name != 'shutting-down') {
      if ($lock_status == 'Unlock') {
        if (user_access($cloud_context . ' terminate all instances') || (user_access($cloud_context . ' terminate own instance') && in_array($key_name, $user_keys_all))) {
          $form['submit_buttons']['Reboot'] = array(
            '#type' => 'submit',
            '#value' => t('Reboot'),
            '#attributes' => array(
              'onclick' => $confirm_reboot_msg,
            ),
          );
  
          $form['submit_buttons']['Terminate'] = array(
            '#type' => 'submit',
            '#value' => t('Terminate'),
            '#attributes' => array(
              'onclick' => $confirm_terminate_msg,
            ),
          );
        }
        
        $root_device_type = _aws_cloud_get_root_device_type($key->image_id);
        $key = cloud_get_ssh_key($cloud_context, $key_name);
        $disabled = empty($key) ? TRUE : FALSE;
        
        
        if (user_access($cloud_context . ' bundle instance') ) {
          if ($root_device_type != FALSE) {
            if ($root_device_type == 'instance-store') {
              // bundle - This is S3
              $form['submit_buttons']['Bundle'] = array(
                '#type' => 'submit',
                '#value' => t('Bundle Instance'),
                '#disabled' => $disabled,
              );
            }
            elseif ($root_device_type == 'ebs') {
              // create image
              $form['submit_buttons']['Bundle'] = array(
                '#type' => 'submit',
                '#value' => t('Backup Instance'),
                '#disabled' => $disabled,
              );
            }
          }
        }
        if (module_exists('cloud_failover')) {
          $fail_over_flag = cloud_failover_get_failover_flag($cloud_context, $instance_id);
            if (isset($fail_over_flag) && $fail_over_flag != CLOUD_FAILOVER_FLAG_NO_FAILOVER ) {
            $form['submit_buttons']['Failover'] = array(
              '#type' => 'submit',
              '#value' => t('Disable Failover'),
            );
            $form['submit_buttons']['Failover']['#disabled'] = user_access($cloud_context . ' disable failover') ? $disabled : TRUE;
          }
          else {
            $form['submit_buttons']['Failover'] = array(
              '#type' => 'submit',
              '#value' => t('Enable Failover'),
            );
            $form['submit_buttons']['Failover']['#disabled'] = user_access($cloud_context . ' enable failover') ? $disabled : TRUE;
          }
        }
  
      }
    }
  
    $form['submit_buttons']['List Instances'] = array(
      '#type' => 'submit',
      '#value' => t('List Instances'),
    );
    $form['cloud_context'] = array(
      '#type' => 'hidden',
      '#value' => $cloud_context,
    );
  }
  return $form;
}

/**
 * Theme Instance Detail Page
 * @param $form
 */
function theme_aws_cloud_display_instance_info($form) {
  $form = $form['form'];
  $rows = array(
    array(
      drupal_render($form['Instance_Name']['Name_label'       ]),
      drupal_render($form['Instance_Name_details']['Name_text']),
    ),
    array(
      drupal_render($form['Instance_State']['State_label']),
      drupal_render($form['Instance_State']['State_text' ]),
    ),
    array(
      drupal_render($form['Instance_lock']['lock_label'       ]),
      drupal_render($form['Instance_lock_details']['lock_text']),
    ),
    array(
      drupal_render($form['Instance_pub_dns']['pub_dns_label']),
      drupal_render($form['Instance_pub_dns']['pub_dns_text' ]),
    ),
    array(
      drupal_render($form['Instance_pvt_dns']['pvt_dns_label']),
      drupal_render($form['Instance_pvt_dns']['pvt_dns_text' ]),
    ),
    array(
      drupal_render($form['Instance_elastic_ip']['elastic_ip_label'        ]),
      drupal_render($form['Instance_elastic_ip_details']['elastic_ip_text' ]),
    ),
    array(
      drupal_render($form['user_name_details']['user_name_label']),
      drupal_render($form['user_name_details']['user_name_text' ]),
    ),
    array(
      drupal_render($form['Instance_ssh']['ssh_label']),
      drupal_render($form['Instance_ssh']['ssh_text' ]),
    ),
    array(
      drupal_render($form['Instance_sg']['sg_label']),
      drupal_render($form['Instance_sg']['sg_text' ]),
    ),
    array(
      drupal_render($form['Instance_zone']['zone_label']),
      drupal_render($form['Instance_zone']['zone_text' ]),
    ),
    array(
      drupal_render($form['Instance_type']['type_label']),
      drupal_render($form['Instance_type']['type_text' ]),
    ),
    array(
      drupal_render($form['Instance_aws']['aws_label']),
      drupal_render($form['Instance_aws']['aws_text' ]),
    ),
    array(
      drupal_render($form['Instance_image']['image_label']),
      drupal_render($form['Instance_image']['image_text' ]),
    ),
    array(
      drupal_render($form['Instance_owner']['owner_label']),
      drupal_render($form['Instance_owner']['owner_text' ]),
    ),
    array(
      drupal_render($form['instance_launch_time']['launch_time_label']),
      drupal_render($form['instance_launch_time']['launch_time_text' ]),
    ),
    array(
      drupal_render($form['instance_runtime']['runtime_label']),
      drupal_render($form['instance_runtime']['runtime_text'] ),
    ),
    array(
      drupal_render($form['save_button']),
      '',
    ),
  );

  $table = theme('table', array('header' => NULL, 'rows' => $rows));
  $form['fieldset_instance_info']['#children'] = $table;
  $output  = drupal_render($form['instance_info_header'  ]);
  $output .= drupal_render($form['submit_buttons'        ]);
  $output .= drupal_render($form['fieldset_instance_info']);
  $output .= drupal_render_children($form);

  return $output;
}


/**
 * Helper function to return the Instance ID
 * @todo: turn this into a more generic function
 * @param     $cloud_context
 * @return
 */
function _aws_cloud_get_instance_id($cloud_context) {
  $instance_id = isset( $_REQUEST['instance_id']) ? $_REQUEST['instance_id'] : FALSE;
  return $instance_id;
}


/**
 * Form submit for instance details page
 * @param type $form_id
 * @param type $form_values
 */
function aws_cloud_display_instance_info_submit($form_id, &$form_state) {

  $form_values     = $form_state['values'];
  $cloud_context   = $form_values['cloud_context'];
  $cloud_menu_path = cloud_get_menu_path($cloud_context );
  $instance_id     = _aws_cloud_get_instance_id($cloud_context);

  switch ($form_values['op']) {
    case t('List Instances'):
      $form_state['redirect'] = $cloud_menu_path . '/instances';
      break;
    case t('Bundle Instance'):
      // not supported yet.
      $form_state['redirect'] = url($cloud_menu_path . '/instances/bundle_instance', array('query' => array('instance_id' => $instance_id), 'absolute' => TRUE));
      break;
    case t('Backup Instance'):
      $form_state['redirect'] = url($cloud_menu_path . '/instances/backup/' . $instance_id, array('absolute' => TRUE));
      break;
    case t('Reboot'):
      $form_state['redirect'] = url($cloud_menu_path . '/instances/reboot', array('query' => array('instance_id' => $instance_id, 'name' => $form_values['Name_text'], 'destination' => $cloud_menu_path . '/instances'), 'absolute' => TRUE));
      break;
    case t('Terminate'):
      $form_state['redirect'] = url($cloud_menu_path . '/instances/terminate', array('query' => array('instance_id' => $instance_id, 'name' => $form_values['Name_text'], 'destination' => $cloud_menu_path . '/instances'), 'absolute' => TRUE));
      break;
    case t('Update Details'):
      // update the nickname
      _aws_cloud_update_instance_query($cloud_context, $instance_id, $form_values['Name_text']);
      //Update the cloud billing instances details
      if (module_exists('cloud_billing')) {
        _cloud_billing_update_instance_query($cloud_context, $instance_id, $form_values['Name_text']);
      }
      // Assigning Elastic IP
      _aws_cloud_update_elastic_ip($cloud_context, $instance_id, $form_values['elastic_ip_text_default_val'], $form_values['elastic_ip_text'], $form_values['elastic_ip_text_default_ip']);
      // Update lock status
      $lock_record_exists = _aws_cloud_set_lock_status($cloud_context, $instance_id, $form_values['lock_text']);
      // Updating user
      $ssh_user_name  = $form_values['user_name_text'];

      if ($lock_record_exists == FALSE) {

        _aws_cloud_set_ssh_user_query($cloud_context, $instance_id, 'Unlock', $ssh_user_name);
      }
      else {

        _aws_cloud_update_ssh_user_query($cloud_context, $instance_id, $ssh_user_name);
      }

      drupal_set_message(t('Instance information updated: @instance_id', array('@instance_id' => $instance_id)));

      cloud_audit_user_activity(
        array(
          'type' => 'user_activity',
          'message' => t('Instance information updated: @instance_id', array('@instance_id' => $instance_id)),
          'link' => '',
        )
      );

      if (empty($instance_id) === FALSE) {

        $form_state['redirect'] = $cloud_menu_path . '/instances/describe/' . $instance_id . '/info';
      }
      else {

        $form_state['redirect'] = $cloud_menu_path . '/instances';
      }
      break;

    case t('Enable Failover'):

      $form_state['redirect'] = url($cloud_menu_path . '/instances/enable_failover',
                                array('query' => array('instance_id' => $instance_id),
                                                       'absolute' => TRUE));
      break;

    case t('Disable Failover'):

      $form_state['redirect'] = url($cloud_menu_path . '/instances/disable_failover',
                                array('query' => array('instance_id' => $instance_id),
                                                       'absolute' => TRUE));
      break;

    default:

      $form_state['redirect' ] =    $cloud_menu_path . '/instances';
      break;
  }
}


/**
 * Volume Instance Form
 * @param     $form_submit
 * @param     $cloud_context
 * @return
 */
function aws_cloud_display_instance_volume_info($form_submit = '', $form_state, $cloud_context, $instance_id) {

  drupal_add_js(cloud_get_module_base() . 'js/cloud.js');

  if (user_access($cloud_context . ' list volume') === FALSE ) {

    $form['cloud_context'] = array(
      '#type'  => 'hidden',
      '#value' => $cloud_context,
    );
    $form['instance_id'] = array(
      '#type'  => 'hidden',
      '#value' => $instance_id,
    );
    drupal_set_message(t('You are not authorized to access this page.'));
    return $form;
  }

  $form['instance_volume_info_header'] = array(
    '#prefix' => '<h2>',
    '#markup' => t('Volume Info'),
    '#suffix' => '</h2>',
  );


  $form['fieldset_volumes_info'] = array(
    '#type' => 'fieldset',
    '#title' => t('Currently Attached Volumes'),
  );

  $form['cloud_context'] = array(
    '#type' => 'hidden',
    '#value' => $cloud_context,
  );

  $cloud_menu_path = cloud_get_menu_path($cloud_context );
  // $instance_id     = _aws_cloud_get_instance_id($cloud_context) ;

  if (empty($instance_id)
    || strlen($instance_id) == 0  ) {
    drupal_goto( $cloud_menu_path . '/instances');
    return;
  }

  if (user_access($cloud_context . ' attach volume') ) {

    $attach_vol_disabled = '';
    $lock_status = _aws_cloud_get_lock_status_by_id_query($cloud_context, $instance_id);
    if (empty($lock_status) === FALSE && $lock_status === 'Lock' ) {

      $attach_vol_disabled = TRUE;
    }

    $form['submit_buttons'] = array(
      '#type' => 'fieldset',
      '#prefix' => '<span class="clear"></span><div class="container-inline"><div class="buttons">',
      '#suffix' => '</div></div>',
    );

    $form['submit_buttons']['attach_volume'] = array(
      '#type' => 'submit',
      '#value' => t('Attach Volume'),
      '#disabled' => $attach_vol_disabled,
    );
  }
  else {
    $form['submit_buttons']['attach_volume'] = array('#value' => '');
  }

  $query = _aws_cloud_get_volumes_by_instance_query($cloud_context);
  $query_args = array(
    ':instance_id' => $instance_id,
    ':cloud_type' => $cloud_context,
  );

  $form['instance_id'] = array(
    '#type' => 'hidden',
    '#value' => $instance_id,
  );

  $result = db_query( $query, $query_args );

  $form['header'] = array(
    '#type'  => 'value',
    '#value' => array(
      array(
        'data'  => t('Nickname'),
        'class' => array('nickname-column'),
      ),
      array('data' => t('ID'    )),
      array('data' => t('Size'  )),
      array('data' => t('Device')),
      array('data' => t('Status')),
      array(
        'data'  => t('Actions' ),
        'class' => array('action-column'),
      ),
    ),
  );

  $destination = drupal_get_destination();

  foreach ($result as $vol) {

    $form['Nickname'][$vol->volume_id] = array('#markup' => l( $vol->nickname, $cloud_menu_path . '/ebs_volumes/describe', array('query' => array('vol_id' => $vol->volume_id)) ));
    $form['ID'      ][$vol->volume_id] = array('#markup' => t( $vol->volume_id));
    $form['Size'    ][$vol->volume_id] = array('#markup' => t( $vol->vol_size ));
    $form['Device'  ][$vol->volume_id] = array('#markup' => t( $vol->device   ));
    $form['Status'  ][$vol->volume_id] = array('#markup' => t( $vol->status   ));
  }

  return $form;
}


/**
 * Theme volume instance form
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function theme_aws_cloud_display_instance_volume_info($form) {


  if (isset($form['form'])) { // drupal 7 passed form as an array 
    $form = $form['form'];
  }

  $form['Nickname'] = !empty($form['Nickname']) ? $form['Nickname'] : array();
  $rows = array();

  $cloud_context = $form['cloud_context']['#value'];

  if (user_access($cloud_context . ' list volume') === FALSE ) {

    $output = drupal_render($form['cloud_context']);
    $output .= drupal_render($form['instance_id' ]);
    return $output;
  }


  foreach (element_children($form['Nickname']) as $key) {

    $row = array(
      array(
        'data' => drupal_render($form['Nickname'][$key]),
        'class' => array('nickname-column'),
      ),
      drupal_render($form['ID'    ][$key]),
      drupal_render($form['Size'  ][$key]),
      drupal_render($form['Device'][$key]),
      drupal_render($form['Status'][$key]),
    );

    $cloud_menu_path = cloud_get_menu_path($cloud_context );

    $action_data = '';
    if (user_access($cloud_context . ' create snapshot') ) {
      $action_data .= cloud_display_action( 'images/icon_play.png',  
                                            'Create Snapshot',  
                                            $cloud_menu_path . '/ebs_volumes/create_snapshot',  
                                            array('query' => array('vol_id' => $key), 'html' => TRUE));
    }

    $lock_status = _aws_cloud_get_lock_status_by_id_query($cloud_context, $form['instance_id']['#value']);
    if (user_access($cloud_context . ' detach volume') && (empty($lock_status) || $lock_status === 'Unlock' ) ) {
      if (substr_count( $form['Status'][$key][0]['#markup'], 'attached') > 0) {
        $action_data .= cloud_display_action('images/icon_clear.png', 'Detach', $cloud_menu_path . '/instances/describe/' . $form['instance_id']['#value'] . '/volume/detach_volume', array('query' => array('vol_id' => urlencode($key)), 'html' => TRUE));
      } //,  'vol_id=' . urlencode($key) );
    }

    $row[]  = array(
      'data'  => $action_data,
      'class' => array('action-column'),
    );
    $rows[] = $row;
  }

  $table = theme('table', array('header' => $form['header']['#value'], 'rows' => $rows));
  $form['fieldset_volumes_info']['#children'] = $table;

  //cf. Waning by Coder module: Potential problem: when FAPI element '#type' is set to 'markup' (default), '#value' only accepts filtered text, be sure to use check_plain(), filter_xss() or similar to ensure your $variable is fully sanitized.
  //$form['fieldset_volumes_info']['list'] = array('#type' => 'markup', '#value' => $table);

  $output  = drupal_render($form['instance_volume_info_header']);
  $output .= drupal_render($form['submit_buttons'             ]);
  $output .= drupal_render($form['fieldset_volumes_info'      ]);
  $output .= drupal_render($form['cloud_context'              ]);
  $output .= drupal_render($form['instance_id'                ]);
  //$output .= drupal_render($form);
  $output .= drupal_render_children($form);

  return $output;
}


/**
 *
 * @param     $form_id
 * @param     $form_values
 * @return
 */
function aws_cloud_display_instance_volume_info_submit($form_id, $form_values) {

  $form_values     = $form_values['values'       ];
  $cloud_context   = $form_values['cloud_context'];
  $cloud_menu_path = cloud_get_menu_path($cloud_context );

  if ( $form_values['op'] == t('Attach Volume') ) {

   drupal_goto( $cloud_menu_path . '/instances/describe/' . $form_values['instance_id'] . '/volume/attach_volume' );
  }
}


/**
 * Attach Volume to instance
 *
 * @param     $form_submit
 * @param     $cloud_context
 * @return
 */
function aws_cloud_attach_instance_volume($form, $form_submit = '', $cloud_context, $instance_id) {

  $form['fieldset_volume_info'] = array('#type' => 'fieldset' /* '#title' => 'Volume Info' */);
  $form['cloud_context'] = array(
    '#type' => 'hidden',
    '#value' => $cloud_context,
  );
  $form['instance_id'  ] = array(
    '#type' => 'hidden',
    '#value' => $instance_id,
  );


  $form['attach_header']  = array(
    '#prefix' => '<h2>',
    '#markup' => t( 'Volume to Server Attachment'),
    '#suffix' => '</h2>',
  );

  $query = _aws_cloud_get_describe_volumes_query( $cloud_context, $instance_id, 'available');

  $result = db_query( $query);
  $volume_options = array();
  $volume_options[''] = '- Select Volume -';

  foreach ($result as $vol) {

    $volume_options[$vol->volume_id] = $vol->nickname;
  }

  $query       = _aws_cloud_get_volumes_by_instance_query($cloud_context);
  $instance_id = _aws_cloud_get_instance_id($cloud_context);
  $query_args = array(
    ':instance_id' => $instance_id,
    ':cloud_type' => $cloud_context,
  );


  $result = db_query( $query, $query_args );

  foreach ($result as $vol) {

    $server_name = $vol->instance_nickname;
  }

  $instance_id = _aws_cloud_get_instance_id($cloud_context);
  $server_name = $instance_id;

  $volume_device_options = array(
    '/dev/sdj' => '/dev/sdj',
    '/dev/sdk' => '/dev/sdk',
    '/dev/sdl' => '/dev/sdl',
    '/dev/sdm' => '/dev/sdm',
    '/dev/xvdb' => '/dev/xvdb',
    '/dev/xvdc' => '/dev/xvdc',
    '/dev/xvdd' => '/dev/xvdd',
    '/dev/xvde' => '/dev/xvde',
    '/dev/xvdf' => '/dev/xvdf',
    '/dev/xvdj' => '/dev/xvdj',
    '/dev/xvdk' => '/dev/xvdk',
    '/dev/xvdl' => '/dev/xvdl',
    '/dev/xvdm' => '/dev/xvdm',
  );

  //asort($volume_device_options); 

  $form['Volume_details']['volume_label'    ] = array(
    '#type' => 'item'  ,
    '#title' => t('Volume'),
  );
  $form['Volume_details']['volume_select'   ] = array(
    '#type' => 'select',
    '#options' => $volume_options,
  );

  $form['Volume_details']['server_label'    ] = array(
    '#type' => 'item'  ,
    '#title' => t('Server'),
  );
  $form['Volume_details']['server_text'     ] = array(
    '#markup' => t( $server_name ));

  $form['Volume_details']['device_label'    ] = array(
    '#type' => 'item'  ,
    '#title' => t('Device'),
  );
  $form['Volume_details']['device_select'   ] = array(
    '#type' => 'select',
    '#options' => $volume_device_options,
  );


  $form['Volume_details']['attach_opt_label'] = array(
    '#type' => 'item',
    '#title' => t('Attachment Options'),
  );
  $form['Volume_details']['attach_opt_text'] = array('#markup' => t( 'Attach now' ));

  $form['submit_buttons'] = array(
    '#type' => 'fieldset',
    '#prefix' => '<span class="clear"></span><div class="container-inline"><div class="buttons">',
    '#suffix' => '</div></div>',
  );

  $form['submit_buttons']['OK'    ] = array(
    '#type' => 'submit',
    '#value' => t('OK'    ),
  );
  $form['submit_buttons']['Cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
  );

  return $form;
}


/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function theme_aws_cloud_attach_instance_volume($form) {

  if (isset($form['form'])) { // drupal 7 passed form as an array  
    $form = $form['form'];
  }

  $rows = array(
    array(
      drupal_render($form['Volume_details']['volume_label'    ]),
      drupal_render($form['Volume_details']['volume_select'   ]),
    ),
    array(
      drupal_render($form['Volume_details']['server_label'    ]),
      drupal_render($form['Volume_details']['server_text'     ]),
    ),
    array(
      drupal_render($form['Volume_details']['device_label'    ]),
      drupal_render($form['Volume_details']['device_select'   ]),
    ),
    array(
      drupal_render($form['Volume_details']['attach_opt_label']),
      drupal_render($form['Volume_details']['attach_opt_text' ]),
    ),
  );

  $table = theme('table', array('header' => NULL, 'rows' => $rows));
  $form['fieldset_volume_info']['#children'] = $table;

  //cf. Waning by Coder module: Coder module: Potential problem: when FAPI element '#type' is set to 'markup' (default), '#value' only accepts filtered text, be sure to use check_plain(), filter_xss() or similar to ensure your $variable is fully sanitized.
  //$form['fieldset_volume_info']['list'] = array('#type' => 'markup', '#value' => $table);

  $output  = drupal_render($form['attach_header'       ]);
  $output .= drupal_render($form['fieldset_volume_info']);
  $output .= drupal_render($form['submit_buttons'      ]);
  $output .= drupal_render($form['cloud_context'       ]);
  $output .= drupal_render($form['instance_id'         ]);
  $output .= drupal_render_children($form);  

  return $output;
}


function aws_cloud_attach_instance_volume_validate($form_id, $form_values) {
  $form_values = $form_values['values'];
  if ( $form_values['op'] == t('OK') ) {

    if ( empty( $form_values['volume_select']) == TRUE) {
      form_set_error('', t('Please select volume'));
      return;
    }
    elseif ( empty($form_values['device_select']) ) {
      form_set_error('', t('Please select device'));
      return;
    }
  }
}

/**
 * Attach a volume to an instance 
 */
function aws_cloud_attach_instance_volume_submit($form_id, $form_values) {

  $form_values     = $form_values['values'];
  $cloud_context   = $form_values['cloud_context'];
  $cloud_menu_path = cloud_get_menu_path($cloud_context );

  if ( $form_values['op'] == t('Cancel') ) {
    $instance_id = _aws_cloud_get_instance_id($cloud_context);
    drupal_goto( $cloud_menu_path . '/instances/describe/' . $instance_id . '/volume' );
  }
  elseif ( $form_values['op'] == t('OK') ) {
    $instance_id = _aws_cloud_get_instance_id($cloud_context);
    $result = aws_cloud_attach_volume($cloud_context, $form_values['volume_select'], $instance_id, $form_values['device_select']);
    if ($result) {
      drupal_set_message( check_plain(t('Attached Volume successfully to the instance: @instance_id', array('@instance_id' => $instance_id) ) ) );
      drupal_goto( $cloud_menu_path . '/instances/describe/' . $instance_id . '/volume' );
    }
    else {
      drupal_set_message(t('There was a problem while attaching the volume'),    'error');
      drupal_goto( $cloud_menu_path . '/instances/describe/' . $instance_id . '/volume' );
    }
  }
}


/**
 * Detach a volume from a cloud instance
 * @param     $cloud_context
 * @return
 */
function aws_cloud_detach_instance_volume($cloud_context, $instance_id) {
  $vol_id          = isset( $_REQUEST['vol_id'] ) ? $_REQUEST['vol_id'] : FALSE;
  $cloud_menu_path = cloud_get_menu_path($cloud_context );
  
  if ($vol_id) {
    $result = aws_cloud_detach_volume($cloud_context, $vol_id);
    if ($result) {
      drupal_set_message(t('Detaching Volume: @vol_id. This may take some time.', array('@vol_id' => $vol_id)));
    }
    else {
      drupal_set_message(t('There was a problem while detaching the volume: @vol_id', array('@vol_id' => $vol_id)), 'error');
    }
  }
  else {
    drupal_set_message(t('No volume id.  Cannot detach volume'));
  }
  drupal_goto($cloud_menu_path . '/instances/describe/' . $instance_id . '/volume');
}


/**
 * Theme monitor info page
 */
function theme_aws_cloud_display_instance_monitor_info() {
  $output  = drupal_render($form['monitor_details']);
  $output .= drupal_render($form['monitor_display_img']);
  $output .= drupal_render($form);
  return $output;
}


function aws_cloud_display_instance_monitor_info_submit($form_id, $form_values) {
  $form_values = $form_values['values'];
}



function aws_cloud_get_key_material_by_id($cloud_context, $instance_id = '') {

  $query = _aws_cloud_get_instance_query($cloud_context);
  $query_args = array(
    ':instance_id' => $instance_id,
    ':cloud_type' => $cloud_context,
  );

  $result = db_query( $query, $query_args );
  $key_name    = '';
  $instance_ip = '';

  foreach ($result as $key) {
    $key_name  = $key->key_name;
    $instance_ip  = $key->dns_name;
  }

  $query      = aws_cloud_get_ssh_keys_query($cloud_context);
  $query_args   = array(
    ':key_name' => $key_name,
    ':cloud_type' => $cloud_context,
  );

  $result = db_query( $query, $query_args );
  $key_material  =  '';

  foreach ($result as $key) {
    $key_material  =  $key->key_material;
  }

  $key_material = str_replace( "\r\n",    "\n", $key_material );
  $resp = array();
  $resp[0] = $key_name;
  $resp[1] = $key_material;
  $resp[2] = $instance_ip;
  return $resp;
}


/**
 * Display Console of the instance
 *
 * @param     $page_submit
 * @param     $cloud_context
 * @return
 */
function aws_cloud_display_instance_console($page_submit = '', $form_state, $cloud_context, $instance_id) {
  if (!user_access($cloud_context . ' access all console') ) {
    if (user_access($cloud_context . ' access own console')) {
      $user_keys_all = aws_cloud_get_all_keys($cloud_context);
      if (!in_array($form['KeyName'][$key]['#value'], $user_keys_all)) {
        drupal_set_message(t('You are not authorized to access this page.'));
        return;
      }
    }
  }
  else {
    $console_url = url(cloud_get_menu_path($cloud_context) . '/instances/describe/' . $instance_id . '/console_popup', array('absolute' => TRUE));
    $arr['onclick'] = "popWin = open('$console_url', 'Console', 'width=300, height=300'); popWin.focus();";
    $form['onclick'] = array(
      '#type' => 'item',
      '#prefix' => "<script lang='javascript'>
                      {$arr['onclick']} ; 
                    </script>",
      '#suffix' => '',
    );
    return $form;
  }
}

/**
 * Form to display console output
 */
function aws_cloud_display_instance_console_output($form_submit = '', $form_state, $cloud_context, $instance_id) {

  if (!user_access($cloud_context . ' access all console') && !user_access($cloud_context . ' access own console')) {
    drupal_set_message(t('You are not authorized to access this page.'));
  }
  else {
    $cloud_menu_path = cloud_get_menu_path($cloud_context );
    if (empty($instance_id)) {
      drupal_goto( $cloud_menu_path . '/instances');
      return;
    }

    $obj_response = aws_cloud_get_console_output($cloud_context, $instance_id);
    if ($obj_response == FALSE) {
      return FALSE;
    }

    $form['fieldset_output_info'] = array('#type' => 'fieldset');
    $form['timestamp']['timestamp_label'] = array(
      '#type' => 'item',
      '#title' => t('Last output update'),
    );
    $form['timestamp']['timestamp_text'] = array('#markup' => ( ($obj_response->timestamp) ));

    $form['output']['output_label'] = array(
      '#type' => 'item',
      '#title' => t('Output'),
    );
    $form['output']['output_text'] = array(
      '#type' => 'textarea',
      '#readonly' => 'readonly',
      '#rows' => 25,
      '#align' => 'left',
      '#default_value' => ($obj_response->output) ? $obj_response->output : t('No output'),
    );

    $form['cloud_context'] = array(
      '#type' => 'hidden',
      '#value' => $cloud_context,
    );
    $form['instance_id'] = array(
      '#type' => 'hidden',
      '#value' => $instance_id,
    );
    return $form;
  }
}

/**
 * Theme the console output form
 */
function theme_aws_cloud_display_instance_console_output($form) {
  $form = $form['form'];

  $output = '';
  $rows = array(
    array(
      drupal_render($form['timestamp']['timestamp_label'] ),
      drupal_render($form['timestamp']['timestamp_text'] ),
    ),
    array(
      drupal_render($form['output']['output_label'] ),
      drupal_render($form['output']['output_text'] ),
    ),
  );

  $table = theme('table', array('header' => NULL, 'rows' => $rows));
  $form['fieldset_output_info']['#children'] = $table;

  $output .= drupal_render($form['fieldset_output_info']);
  $output .= drupal_render_children($form);
  return $output;
}

/**
 * Form for Instance Bundling
 *
 * @param   $form_submit
 *   Form Object
 * @param   $cloud_context
 *   Sub-Cloud
 * @return
 */
function aws_cloud_bundle_instance($form_submit = '', $form_state, $cloud_context) {

  $form['bundl_image_details']['instance_label'] = array(
    '#type' => 'item'     ,
    '#title' => t('Instance'),
  );
  $instance_id     = _aws_cloud_get_instance_id($cloud_context);
  $nickname   = '';
  $bucketname = '';
  $foldername = '';
  $cloud_menu_path  = cloud_get_menu_path($cloud_context );
  
  $bundle_disabled = FALSE;
  if (empty($instance_id) === FALSE) {
    $bundle_status = aws_cloud_db_bundle_instance_get_status($instance_id);
    if ($bundle_status == 'initiated' || $bundle_status == 'processed') {
      // show message saying that bundling can't happen 
      // until the last bundle is successful
      drupal_set_message(t('A bundling request has been initiated.  Please wait until current request has completed before bundling again.'), 'error');
      $bundle_disabled = TRUE;
    }
    elseif ($bundle_status == 'error') {
      drupal_set_message(t('An error was encountered during bundling.  Please check the log file /tmp/clanavi/bundle_image-test.log on the instance for more information.'), 'error');
    }
    
    $instance_info    = _aws_cloud_get_instance_details_by_id($cloud_context, $instance_id);
    $form['bundl_image_details']['instance_name'] = array('#markup' => l( $instance_info['name'], $cloud_menu_path . '/instances/describe/' . $instance_id . '/info', array('query' => array('instance_id' => urlencode($instance_id)))));
    $nickname   = $instance_info['name'] . '_' . date( "Y_n_j G:i:s T");
    $nickname   = str_replace( array(' ', ':', '-', '_'), '', $nickname );
    $nickname   = strtolower($nickname);
    $nickname   = empty($_REQUEST['nickname_text'  ]) ? $nickname : $_REQUEST['nickname_text'  ];
    $bucketname = empty($_REQUEST['bucketname_text']) ? ''        : $_REQUEST['bucketname_text'];
    $foldername = empty($_REQUEST['foldername_text']) ? ''        : $_REQUEST['foldername_text'];
  }
  else {
    drupal_goto( $cloud_menu_path . '/instances');
    return;
  }

  $vol_size = isset($_REQUEST['vol_size_text']) ? $_REQUEST['vol_size_text'   ] : '10240';

  $form['bundl_image_details']['nickname_label'   ] = array(
    '#type' => 'item'     ,
    '#title' => t('Image Nickname'),
  );
  $form['bundl_image_details']['nickname_text'    ] = array(
    '#type' => 'textfield',
    '#value' => $nickname,
    '#description' => 'No spaces, no special characters, no numbers allowed',
    '#required' => TRUE,
  );

  $form['bundl_image_details']['bucketname_label'] = array(
    '#type' => 'item'     ,
    '#title' => t('Bucket Name'),
  );
  $form['bundl_image_details']['bucketname_text'  ] = array(
    '#type' => 'textfield',
    '#value' => $bucketname,
    '#description' => 'No spaces, no special characters, no numbers allowed',
  );

  $form['bundl_image_details']['foldername_label'] = array(
    '#type' => 'item'     ,
    '#title' => t('Folder Name' ),
  );
  $form['bundl_image_details']['foldername_text'  ] = array(
    '#type' => 'textfield',
    '#value' => $foldername,
  );

  $form['bundl_image_details']['vol_size_label'   ] = array(
    '#type' => 'item'     ,
    '#title' => t('Root Volume Size(in MB)'),
  );
  $form['bundl_image_details']['vol_size_text'    ] = array(
    '#type' => 'textfield',
    '#value' => $vol_size  ,
    '#required' => TRUE,
  );

  $form['bundl_image_details']['cloud_text'       ] = array(
    '#type' => 'hidden'   ,
    '#value' => $cloud_context,
  );

  $form['cloud_context'] =  array(
    '#type' => 'hidden',
    '#value' => $cloud_context,
  );
  $form['instance_id']   =  array(
    '#type' => 'hidden',
    '#value' => $instance_id,
  );

  $form['submit'] = array(
    '#type' => 'submit'   ,
    '#value' => t('Bundle'),
    '#disabled' => $bundle_disabled,
  );

  return $form;
}


/**
 *
 * @param   $form
 * @return
 */
function theme_aws_cloud_bundle_instance($form) {
  $form = $form['form'];
  $rows = array(
    array(
      drupal_render($form['bundl_image_details']['instance_label'] ),
      drupal_render($form['bundl_image_details']['instance_name']),
    ),
    array(
      drupal_render($form['bundl_image_details']['nickname_label']),
      drupal_render($form['bundl_image_details']['nickname_text'] ),
    ),
    array(
      drupal_render($form['bundl_image_details']['bucketname_label']),
      drupal_render($form['bundl_image_details']['bucketname_text'] ),
    ),
    array(
      drupal_render($form['bundl_image_details']['foldername_label']),
      drupal_render($form['bundl_image_details']['foldername_text'] ),
    ),
    array(
      drupal_render($form['bundl_image_details']['vol_size_label']),
      drupal_render($form['bundl_image_details']['vol_size_text'] ),
    ),
  );

  $output  = theme('table', array('header' => NULL, 'rows' => $rows));
  $output .= drupal_render($form['submit']);
  $output .= drupal_render_children($form);
  return $output;
}

/**
 * Form validation for instance bundling
 * @param unknown_type $form_id
 * @param unknown_type $form_values
 */
function aws_cloud_bundle_instance_validate($form_id, &$form_values) {

  $values = $form_values['values'];
  $op     = $values['op'];
  switch ($op) {
    case 'Bundle':
      $result         = preg_match('/[^a-z0-9]/', $values['nickname_text'])   ;
      $result_bucket  = preg_match('/[^a-z]/', $values['bucketname_text'])   ;
      if ( $result  == TRUE ) {
        form_set_error('nickname_text', t('Please enter only small letters and numbers for Nickname. Spaces and special characters not allowed.'));
      }
      elseif ( empty($values['bucketname_text']) || $result_bucket == TRUE) {
        form_set_error('bucketname_text', t('Please enter only small letters for Bucket Name. Spaces, numbers and special characters not allowed. Bucket name is required and cannot be empty.'));
      }

  }
}


/**
 * Form submit for instance bundling
 * @param $form_id
 * @param $form_values
 */
function aws_cloud_bundle_instance_submit($form_id, &$form_state) {

  $values = $form_state['values'];
  $op     = $values['op'];

  switch ($op) {

    case 'Bundle':
      aws_cloud_db_bundle_instance( 
        $values['cloud_context'  ],
        $values['instance_id'    ],
        $values['nickname_text'  ],
        $values['bucketname_text'],
        $values['foldername_text'],
        $values['vol_size_text'  ],
        'initiated' );
      drupal_set_message(t('Added the instance for Bundling'));

      return;
  }

  return;
}

/**
 * Enable failover
 */
function aws_cloud_enable_failover($cloud_context) {
  $instance_id = _aws_cloud_get_instance_id($cloud_context);
  cloud_action_notify('enable_failover', array('cloud_context' => $cloud_context, 'instance_id' => $instance_id));
  drupal_set_message(check_plain(t('Failover turned on for instance.  This will require some time.')));
  $cloud_menu_path  = cloud_get_menu_path($cloud_context );
  drupal_goto( $cloud_menu_path . '/instances/describe/' . $instance_id . '/info');
}

/**
 * Disable failover
 */
function aws_cloud_disable_failover($cloud_context) {
  $instance_id     = _aws_cloud_get_instance_id($cloud_context);
  cloud_action_notify('disable_failover', array('cloud_context' => $cloud_context, 'instance_id' => $instance_id));
  drupal_set_message(check_plain(t('Failover turned off for instance.  This will require some time.')));
  $cloud_menu_path  = cloud_get_menu_path($cloud_context );
  drupal_goto( $cloud_menu_path . '/instances/describe/' . $instance_id . '/info');
}

/**
 * Helper function to update the elastic ip
 * @param unknown_type $ip_org
 * @param unknown_type $ip_new
 * @param unknown_type $ip_old
 */
function _aws_cloud_update_elastic_ip($cloud_context, $instance_id, $ip_org, $ip_new, $ip_old) {
  if ($ip_org == AWS_CLOUD_CLOUD_NONE && $ip_new != AWS_CLOUD_CLOUD_NONE) {
    $result = aws_cloud_associate_ip($cloud_context, $instance_id, $ip_new);
    if ($result) {
      drupal_set_message(t('Assigned the IP (@ip) to the Instance (@instance_id) ( This may take some time)', array('@ip' => $ip_new, '@instance_id' => $instance_id)));
      if (module_exists('cloud_failover')) {
        cloud_action_notify('update_eip_info', array('cloud_context' => $cloud_context, 'instance_id' => $instance_id, 'ip' => $ip_new));
      }
    }
    else {
      drupal_set_message(t('There was a problem while assigning the IP (@ip) to the instance (@instance_id)', array('@ip' => $ip_new, '@instance_id' => $instance_id)), 'error');
    }
  }
  elseif ($ip_org != AWS_CLOUD_CLOUD_NONE && $ip_new == AWS_CLOUD_CLOUD_NONE) {

    //$ip_old = $form_values['elastic_ip_text_default_ip'];
    $result = aws_cloud_disassociate_ip($cloud_context, $ip_old);
    if ($result) {
      drupal_set_message(t('The IP has been released: @ip ( This may take some time)', array('@ip' => $ip_old)));

      if (module_exists('cloud_failover')) {
        cloud_action_notify('update_eip_info', array('cloud_context' => $cloud_context, 'instance_id' => $instance_id));
      }
    }
    else {
      drupal_set_message(t('There was a problem while releasing the IP: @ip', array('@ip' => $ip_old)), 'error');
    }
  }
}

/**
 * Helper function to set lock status
 */
function _aws_cloud_set_lock_status($cloud_context, $instance_id, $status_lock) {
  $lock_record_exists = FALSE;

  $lock_query   = _aws_cloud_get_lock_status_query($cloud_context);
  $query_args = array(
    ':instance_id' => $instance_id,
    ':cloud_type' => $cloud_context,
  );
  $result = db_query($lock_query, $query_args);

  if ($result->rowCount()) {
    $lock_record_exists = TRUE;
  }

  if (!$lock_record_exists) {
    $insert_query = _aws_cloud_set_lock_status_query($cloud_context);
    $insert_query
      ->values(
        array(
          'instance_id' => $instance_id,
          'cloud_type' => $cloud_context,
          'is_locked' => $status_lock,
          'ssh_user_name' => CLOUD_SSH_USER_NAME,
        )
      )
      ->execute();
    $lock_record_exists = TRUE;
  }
  else {
    $update_query = _aws_cloud_update_lock_status_query($cloud_context);
    $update_query
      ->fields(
        array(
          'is_locked' => $status_lock,
      ))
      ->condition('instance_id', $instance_id, '=')
      ->condition('cloud_type', $cloud_context, '=')
      ->execute();
  }
  
  return $lock_record_exists; 
}