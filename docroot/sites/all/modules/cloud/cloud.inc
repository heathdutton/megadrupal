<?php

/**
 * @file
 * Provides common functionalites for cloud.module.
 *
 * Copyright (c) 2010-2011 DOCOMO Innovations, Inc.
 *
 */

/**
 * Updated by yas    2013/12/23
 * Updated by Jamir  2011/04/30
 * Updated by yas    2011/02/17
 */

/* Generate a random UUID
 *
 * @see http://www.ietf.org/rfc/rfc4122.txt
 * @return RFC 4122 UUID
 * @static
 */
function uuid() {

  $node = '127.0.0.1'; //env('SERVER_ADDR');
  $pid  = NULL;

  if (strpos($node, ':') !== FALSE) {

    if (substr_count($node, '::')) {

      $node = str_replace('::', str_repeat(':0000', 8 - substr_count($node, ':')) . ':', $node);
    }
    $node  = explode(':', $node);
    $ipv6  = '';

    foreach ($node as $id) {

      $ipv6 .= str_pad(base_convert($id, 16, 2), 16, 0, STR_PAD_LEFT);
    }

    $node =  base_convert($ipv6,    2,    10);

    if (strlen($node) < 38) {
      $node = NULL;
    }
    else {
      $node = crc32($node);
    }
  }
  elseif (empty($node)) {
    $host = env('HOSTNAME');

    if (empty($host)) {
      $host = env('HOST');
    }

    if (!empty($host)) {
      $ip = gethostbyname($host);

      if ($ip === $host) {
        $node = crc32($host);
      }
      else {
        $node = ip2long($ip);
      }
    }
  }
  elseif ($node !== '127.0.0.1') {
    $node = ip2long($node);
  }
  else {
    $node = NULL;
  }

  if (empty($node)) {
    $node = crc32("The quick brown fox jumped over the lazy dog.");
  }

  if (function_exists('zend_thread_id')) {
    $pid = zend_thread_id();
  }
  else {
    $pid = getmypid();
  }

  if (!$pid || $pid > 65535) {
    $pid = mt_rand(0,    0xfff) | 0x4000;
  }

  list($time_mid, $time_low) = explode(' ',    microtime());
  $uuid = sprintf("%08x-%04x-%04x-%02x%02x-%04x%08x",    (int) $time_low, (int) substr($time_mid, 2) & 0xffff,
  mt_rand(0,    0xfff) | 0x4000, mt_rand(0, 0x3f) | 0x80, mt_rand(0, 0xff), $pid, $node);

  return $uuid;
}

/**
 * Checks if the suppilied variable is correctly set or not
 * @param $variable
 *        This is the sub-cloud who's template is being added
 * @return return a $variable
 */
function isset_variable($variable) {
  return isset($variable) ? $variable : '';
}


/**
 * Returns a query to get snmp readings
 *
 * @param $cloud_context
 *        This is the sub-cloud who's template is being added
 * @param $hostname
 *        This is hostname of server
 * @param $duration_in_sec
 *        This is the numeric value passes as seconds
 * @return return a sql-query
 */
function cloud_snmp_get_readings($cloud_context, $hostname, $duration_in_sec) {

  $monitor_url = cloud_get_info($cloud_context, $cloud_context . '_monitor_url');

  if ( empty($monitor_url) || valid_url($monitor_url, TRUE ) === FALSE ) {
    return;
  }

  $data = array(
    'hName' => $hostname,
    'dSec' => $duration_in_sec,
  );

  cloud_log_to_db($cloud_context . ': instance_monitor_url', $monitor_url);

  $url  = parse_url($monitor_url);
  $host = $url['host'];
  $host = 'http://' . $host . '/';

  //cloud_log_to_db("RRD_HOST", $host);

  $host_uri = $host . CLOUD_RRD_FILE;
  list($header, $content) = cloud_post_request(
    $host_uri,
    'host url',
    $data
  );
  cloud_log_to_db('SNMP reading->' . $hostname, $content);
  return $content;
}


/**
 * This function is used to post a data to external server
 * by preparing the url with compatible parameters
 * @param $url
 *        url of server
 * @param $referer
 *        referer of the url
 * @param $_data
 *        data to be posted
 * @return array of header and content
 */
function cloud_post_request($url, $referer, $_data) {

  // convert variables array to string:
  $data = array();
  while (list($n, $v) = each($_data)) {
    $data[] = "$n=$v";
  }
  $data = implode('&', $data);
  // format --> test1=a&test2=b etc.

  // parse the given URL
  $url = parse_url($url);
  if ($url['scheme'] != 'http') {
    die('Only HTTP request are supported !');
  }

  // extract host and path:
  $host = $url['host'];
  $path = $url['path'];


  $result = '';

  try {

    // open a socket connection on port 80
    $fp = fsockopen($host,    80);
    if ($fp == NULL) {
      cloud_log_to_db('Unable to Coonect to Server', $host);
      return;
    }

    // send the request headers:
    fputs($fp, "POST $path HTTP/1.1\r\n");
    fputs($fp, "Host: $host\r\n");
    fputs($fp, "Referer: $referer\r\n");
    fputs($fp, "Content-type: application/x-www-form-urlencoded\r\n");
    fputs($fp, "Content-length: " . strlen($data) . "\r\n");
    fputs($fp, "Connection: close\r\n\r\n");
    fputs($fp, $data);


    while (!feof($fp)) {
      // receive the results of the request
      $result .= fgets($fp,    128);
    }

    // close the socket connection:
    fclose($fp);
  }
  catch (Exception $ex) {
    cloud_log_to_db("HTTP CONNECTION ERROR", "NOT ABLE TO CONNECT TO ->" . $host);
  }

  // split the result header from the content
  $result = explode("\r\n\r\n", $result,    2);

  $header  = isset($result[0]) ? $result[0] : '';
  $content = isset($result[1]) ? $result[1] : '';

  // return as array:
  return array($header, $content);
}


/**
 *
 * @param unknown_type $field
 * @param unknown_type $value
 * @return unknown_type
 */
function cloud_log_to_db($field, $value) {
  /*
   if (empty($value))
   db_query('INSERT INTO { ' . CLOUD_TEMP_TABLE ." }(LAST_TIME, FIELD, VALUE ) values (CURRENT_TIMESTAMP, '" . $field . "', '" . $field . "')");
   else
   db_query('INSERT INTO { ' . CLOUD_TEMP_TABLE ." }(LAST_TIME, FIELD, VALUE ) values (CURRENT_TIMESTAMP, '" . $field . "', '" . $value . "')");
   */
  return;
}

/**
 * @todo this function is probably not needed now that Drupal7 has fetchAssoc()
 * 
 * This function can be used for fetching data from DB.
 * This will simply create Nested Array having one array corresponding to one row of resultset.
 * So no need to worry about manual adding columns to arrays. This will be taken care automatically.
 *
 * @param $query
 *        This is the query pass to be processsed to return data array
 * @return $return_main_array - data array
 */
function cloud_get_db_results($query, $params = array()) {

  $result = db_query($query, $params);
  $return_main_array = array();

  foreach ($result as $row) {

    $return_array = array();
    foreach ($row as $key => $val) {
      $return_array[$key] = $val;
    }
    $return_main_array[] = $return_array;
  }
  return $return_main_array;
}


/**
 * To get the last update timediff from SNMP
 * @param $cloud_context
 *        This is the sub-cloud
 * @return diff in mins
 */
function cloud_get_last_update_snmp_timediff($cloud_context) {

  $query = "SELECT TIMESTAMPDIFF(MINUTE, last_update_time, CURRENT_TIMESTAMP ) - host_entries_refresh_time DIFF_IN_MIN from {" . CLOUD_CLOUDS_TABLE . "} where cloud_name = '" . $cloud_context . "'";
  $dns_names  = '';

  $db_result = db_query( $query )->fetchField();
  if ($db_result) {
    return $db_result;
  }
}


/**
 * Returns a query to get list of all enabled clouds
 * This function build a query and search parameters to sql assign during function call.
 *
 * @return return a sql-query
 */
function cloud_get_clouds() {

  return cloud_get_db_results('select * from {' . CLOUD_CLOUDS_TABLE . '} order by cloud_name');
}


/**
 * To format the datetime string from given
 * launchtime string
 * @param $launch_time
 *        This is launctime to analyse
 * @return formatted launhtime
 */
function cloud_get_runtime($launch_time) {

  if (empty($launch_time)) {

    return '';
  }

  $launch_tmp_time = date_parse($launch_time);
  $gm_launch_time  = gmmktime( $launch_tmp_time['hour'], $launch_tmp_time['minute'], $launch_tmp_time['second'], $launch_tmp_time['month'], $launch_tmp_time['day'], $launch_tmp_time['year'] );
  $curr_time       = time();

  $diff =  $curr_time - $gm_launch_time;

  $days = $diff / (86400);
  $hr   = $diff /  (3600);
  $min  = $diff /    (60);
  $sec  = $diff;

  $result = '';
  if ( round( $days) > 0 ) {
    $result .=  round( $days, 1 ) . ' days '  ;
  }
  elseif ( round( $hr )  > 0 ) {
    $result .=  round( $hr   , 1 ) . ' hours ';
  }
  elseif ( round( $min )  > 0 ) {
    $result .=  round( $min  , 1 ) . ' min '   ;
  }
  elseif ( round( $sec ) > 0 ) {
    $result .=  round( $sec  , 1 ) . ' sec '   ;
  }

  return $result;
}


/**
 * Returns a list of all nicknames
 * given by id, nickname and table name
 *
 * @param $id_col
 *        Id column name
 * @param $nickname_col
 *        nickname column name
 * @param $table_name
 *        This is table name of cloud
 * @return return a sql-query
 */
function cloud_get_all_nickname($id_col, $nickname_col, $table_name = NULL) {

  if ( isset($table_name) == FALSE ) {
    return;
  }


  $result = db_query( "SELECT $id_col, $nickname_col FROM {" . $table_name . "} ");

  $result_arr = array();


  foreach ($result as $row_data) {

    $result_arr[$row_data->{$id_col}] = $row_data->{$nickname_col};
  }

  return $result_arr;
}

/**
 * To check whether string is exists in array or not
 * @param $arr
 *        This is $arr to be processed on
 * @param $to_search
 *        This is search string
 * @return whether string is exists in array or not
 */
function cloud_check_in_array($arr, $to_search) {

  if ( empty($arr) ) {
    return NULL;
  }

  foreach ( $arr as $key => $value) {

    if ( $to_search == $key ) {
      return $value;
    }
  }

  return NULL;
}

/**
 * To get the dns-name of cloud
 * by instance-id
 * @param $instance_id
 *        This is instance-id for cloud
 * @param $cloud_context
 *        This is the sub-cloud who's dns-name to be fetched
 * @return array of dns name list
 */
function cloud_get_dns_name($instance_id, $cloud_context) {

  $instances_list = cloud_get_instances($cloud_context  );
  foreach ($instances_list as $id => $instance_data ) {

    if ( $id === $instance_id ) {

      return $instance_data['hostname'];
    }
  }

  return FALSE;
}


/**
 * To get the list of all terminated instances
 * by instance-id
 * @param $cloud_context
 *        This is the sub-cloud who's dns-name to be fetched
 * @param $instance_id
 *        This is instance-id for cloud
 * @return array of instance list
 */
function cloud_terminate_array_instance($cloud_context, $instance_id) {

  cloud_call_function_exec( $cloud_context . '_' . __FUNCTION__, $instance_id );

  return;
}

/**
 * To execute cloud function
 * by method name and its parameters
 * @param $method_name
 *        This is method name which is to be executed
 * @param $params
 *        This is list of parameters
 * @return return
 */
function cloud_call_function_exec($method_name, $params) {

  if ( function_exists($method_name) ) {

    return call_user_func( $method_name, $params );
  }
}


/**
 * Check whether the passed cloud_context is enabled or not
 *
 * @param $cloud_context
 */
function _cloud_is_cloud_enabled($cloud_context) {



  if ( isset($cloud_context) === FALSE ||
          empty($cloud_context) ) { // in case the cloud_context is empty
    return FALSE;
  }

  $enabled_cloud_list = cloud_get_clouds();

  for ($row_counter = 0; $row_counter < count($enabled_cloud_list); $row_counter++) {

    $cloud_name = $enabled_cloud_list[$row_counter]['cloud_name'];
    if ( $cloud_name === $cloud_context ) {

      return TRUE;
    }
  }

  return FALSE;
}


/**
 * This function which will return the value of type key for
 * the sub-cloud.
 * This is a getter function for information related to sub-clouds
 *
 * To get the Key values
 * of key composed of cloud-context and repected keyname
 * @param $cloud_context
 *        This is the sub-cloud who's details to be fetched
 * @param $key
 *        This is keyname for cloud
 * @return
 *      The requested information as per the sub-cloud and key
 *      In case information is not found then it will return empty as default value.
 *      These values are set during the cloud initialisation and sub-cloud settings form.
 */
function cloud_get_info($cloud_context, $key ) {

  if ( isset($cloud_context) === FALSE
    ||     empty($cloud_context)
    ||     empty($key) ) { // in case the cloud_context is empty or the key is empty
    return FALSE;
  }

  if ( _cloud_is_cloud_enabled($cloud_context) === FALSE ) {

    return FALSE;
  }

  //Let the module responsible for this particular $cloud_context
  //do the information fetching.  If empty, try to default
  //to the old behavior before giving up
  $module = cloud_get_module($cloud_context);
  if ($module) {
    $value = module_invoke($module, 'cloud_get_info', $cloud_context, $key);
  }

  //preserve the default variable version, for any
  //backwards compatibility
  if (!isset($value)) {
    $value = variable_get( $cloud_context . '_' . $key,    '' );
    if (empty($value) === FALSE  ) {
      $value = trim($value);
    }
  }
  return $value;
}

/**
 * To get the cloud info
 * by cloud-context
 * @param $cloud_context
 *        This is the sub-cloud who's details to be fetched
 * @return menu path of cloud
 */
function cloud_get_menu_path($cloud_context ) {

  if ( isset($cloud_context) === FALSE ||
          empty($cloud_context) ) { // in case the cloud_context is empty
    return FALSE;
  }

  if ( _cloud_is_cloud_enabled($cloud_context) === FALSE ) {

    return FALSE;
  }

  return variable_get( $cloud_context . '_menu_path', '' );
}

/**
 * To get the cloud base info
 * by cloud-context
 * @param $cloud_context
 *        This is the sub-cloud who's details to be fetched
 * @return base info of cloud
 */
function cloud_get_module_base() {

  return drupal_get_path('module', 'cloud') . CLOUD_PATH_SEPARATOR;
}

/**
 * To get the list of all action styles
 * used in user-actions throught the system
 *
 * @return array of action styles with their respected class name
 */
function cloud_get_action_style() {

  // action icons / images list
  $_action_styles = array(
    'activate' => 'playIcon'        ,
    'console' => 'sshIcon'         ,
    'copy' => 'copyIcon'        ,
    'create snapshot' => 'playIcon'        ,
    'create volume' => 'playIcon'        ,
    'clone' => 'copyIcon'        ,
    'delete' => 'deleteIcon'      ,
    'destroy' => 'deleteIcon'      ,
    'detach' => 'stopIcon'        ,
    'down' => 'downIcon'        ,
    'gray monitor disabled' => 'icon_monitorGray',
    'hard shutdown' => 'shutdownIcon'    ,
    'hard reboot' => 'hardrebootIcon'  ,
    'inputs' => 'tableIcon'       ,
    'launch' => 'playIcon'        ,
    'locked' => 'lockIcon'        ,
    'edit' => 'editIcon'        ,
    'monitor' => 'monitorIcon'     ,
    'monitor enabled' => 'monitorGreenIcon',
    'reboot' => 'rebootIcon'      ,
    'resume' => 'resumeIcon'      ,
    'service down' => 'servicedownIcon',
    'shutdown' => 'stopIcon'        ,
    'start' => 'playIcon'        ,
    'stop' => 'stopIcon'        ,
    'stop failover' => 'stopIcon'        ,
    'stop monitoring' => 'monitorstopIcon',
    'suspend' => 'suspendIcon'     ,
    'terminate' => 'stopIcon'        ,
    'up' => 'failupIcon'      ,
  );

  return $_action_styles;
}

/**
 * To get the list of all action styles
 * used in user-actions throught the system
 *
 * For example:
 *
 * $tmp = cloud_display_action( 'action_stop.gif', 'Terminate', 'clouds/amazon/instances' );
 *
 * @param $params
 *        This is attributes of image tag to render image on page
 * @return ready to use image object to be displayed
 */
function cloud_display_action($src_img_path, $msg, $target, $options, $prop = '') {

  $_action_styles       = cloud_get_action_style();
  $path                 = cloud_get_module_base() . $src_img_path;
  //do not call image_info, since it is a filesystem call.
  //$image_info           = image_get_info( $path );
  $label                = '';
  $attributes['class'] = 'image image-$label ' . (isset($attributes['class'])
                        ? $attributes['class']
                        : '');
  $options['attributes']['title'  ] = $msg;
  $variables = array(
    'path' => $path,
    'alt' => $msg,
    'title' => $msg,
    'attributes' => $options['attributes'],
  );

  $options['attributes']['onclick'] = $prop;
  $out = theme( 'image', $variables );
  $class_name = !empty($_action_styles[drupal_strtolower($msg)])
              ?        $_action_styles[drupal_strtolower($msg)]
              : '';
  if ($class_name) {
    return '<div class="' . $class_name . '">' . l( $out, $target, $options) . '</div>';
  }
  else {

    return l( $out, $target, $options);
  }
}

/**
 * To sort the list by diff. attributes
 * @param $list
 *        list which is to be sorted
 * @param $sort_by
 *        sort by attribute
 * @param $order
 *        order by attribute asc,desc
 * @return sorted list of items
 */
function _cloud_sort_by_value($list, $sort_by, $order ) {

  $result_array = array();

  foreach ( $list as $key => $value ) {

    foreach ( $value as $col_name => $col_value ) {

      if ($col_name === $sort_by ) {

        if ($col_name === 'runtime' ) { // Since the time is in GMK time we need to convert it for sorting.
          $launch_tmp_time = date_parse($col_value);
          $gm_launch_time  = gmmktime( $launch_tmp_time['hour'], $launch_tmp_time['minute'], $launch_tmp_time['second'], $launch_tmp_time['month'], $launch_tmp_time['day'], $launch_tmp_time['year'] );
          $col_value       = $gm_launch_time;
        }
        else {

          $col_value = drupal_strtolower($col_value);
        }

        $key = $col_value . '_' . $key;
        $result_array[$key] = $value;
      }
    }
  }

  if ( empty($result_array) ) {

    $result_array = $list;
  }

  if ($order === 'desc') {
    krsort($result_array);
  }
  else {
    ksort($result_array);
  }

  return $result_array;
}

/**
 * To get the list of all instances on landing page of our system
 * by diff. filtering values
 * @param $form_submit
 *        This is form_values which is posted by filerting values
 * @return form with all integrated instance value and their other info
 */
function cloud_display_dashboard($form_submit = '') {

  // fetch sub-clouds by calling $cloud_list = cloud_get_clouds()?
  // according to $cloud_list
  // call hook_cloud_get_instance_all() for each $cloud_list
  // hook_get_instance_all will return the $array = array() values: Nickname ID Hostname State Zone UserR untime Action
  // Merge $array1, $array2 for each cloud's instance list
  // Make list table
  // asort ($array);

  global $user;

  drupal_add_js(cloud_get_module_base() . 'js/cloud.js'                                );
  drupal_add_js(cloud_get_module_base() . 'js/cloud_auto_refresh_list_all_instances.js');

  $column      = 'instance_nickname';

  $form['options'] = array(
    '#type' => 'fieldset',
    '#prefix' => '<div class="container-inline">',
    '#suffix' => '</div>',
  );

  $options = array(
    t('Nickname' ),
    t('ID'       ),
    t('Hostname' ),
  );

  $filter     = isset($_REQUEST['filter'   ]) ? $_REQUEST['filter'   ] : '';
  $filter_col = isset($_REQUEST['operation']) ? $_REQUEST['operation'] : 0  ; // default: Nickname
  $filter     = trim($filter);

  if ( $filter_col == 0 ) {
    $column  = 'Nickname'           ;
    $sql_col = 'instance_nickname'  ;
  }
  elseif ($filter_col == 1 ) {
    $column  = 'ID'                 ;
    $sql_col = 'instance_id'        ;
  }
  elseif ($filter_col == 2 ) {
    $column  = 'Public-DNS-Name'    ;
    $sql_col = 'dns_name'           ;
  }
  if ( isset($filter) ) {

    $query_args[] = $sql_col;
    $query_args[] = $filter  ;
  }
  else {
    $filter       = ' 1 ';
    $query_args[] = ' ';
  }

  $all_instances_count = cloud_get_all_instances_list_count();
  $filter_disabled     = '';
  if ( $all_instances_count < 2 ) {

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
    '#value' => t('Apply')                                 ,
    '#disabled' => $filter_disabled,
  );

  $form['header'] = array(
    '#type' => 'value',
    '#value' => array(
      array(
        'data' => t('Nickname'),
        'field' => 'instance_nickname',
        'class' => array('nickname-column'),
      ),
      array(
        'data' => t('ID'      ),
        'field' => 'instance_id',
      ),
      array(
        'data' => t('Hostname'),
        'field' => 'dns_name',
      ),
      array(
        'data' => t('State'   ),
        'field' => 'instance_status',
      ),
      array(
        'data' => t('Zone'    ),
        'field' => 'placement',
        'sort' => 'asc',
      ),
      array(
        'data' => t('User'    ),
        'field' => 'key_name',
      ),
      array(
        'data' => t('Runtime' ),
        'field' => 'launch_time',
      ),
      array(
        'data' => t('Action'  ),
        'class' => 'action-column',
      ),
    ),
  );

  $cloud_list = cloud_get_all_clouds();

  $all_instances_list = array();
  $filter_params = array(
    'column' => $sql_col,
    'filter_value' => $filter,
  );
  foreach ($cloud_list as $cloud_context) {

    if (user_access($cloud_context . ' ' . 'list instances') === FALSE) continue;

    $instances_list = cloud_get_instances($cloud_context, $filter_params );
    if (empty($instances_list) === FALSE) {
      $all_instances_list = array_merge( $all_instances_list, $instances_list );
    }
  }


  $destination = drupal_get_destination();
  // Fetch Console information

  $ts = tablesort_init( $form['header']['#value'] );
  if ($ts['sql']) {

    if ($ts['sql'] === 'instance_id' ) {

      $field = 'instance_id';
    }
    elseif ($ts['sql'] === 'instance_nickname' ) {

      $field = 'name';
    }
    elseif ($ts['sql'] === 'dns_name' ) {

      $field = 'hostname';
    }
    elseif ($ts['sql'] === 'instance_status' ) {

      $field = 'state';
    }
    elseif ($ts['sql'] === 'placement' ) {

      $field = 'zone';
    }
    elseif ($ts['sql'] === 'key_name' ) {

      $field = 'user';
    }
    elseif ($ts['sql'] === 'launch_time' ) {

      $field = 'runtime';
    }
    else {

      $field = 'id';
    }

    $sort_by_key = array();

    foreach ($all_instances_list as $key => $row) {
      $sort_by_key[$key]  = drupal_strtolower($row[$field]);
      $zone[$key]         = drupal_strtolower($row['zone']);
      $nickname[$key]     = drupal_strtolower($row['name']);

      $launch_tmp_time = date_parse($row['runtime']);

      // We need to parse $row['runtime'] because the date format is different depending on the sub-clouds
      // Amazon EC2 Date Format: "2011-02-25T07:20:58.000Z"
      // OpenStack  Date Format: "2011-03-02T02:06:05Z"
      // XCP        Date Format: "20110128T00:23:12Z"
      $runtime[$key]  = gmmktime( $launch_tmp_time['hour'], $launch_tmp_time['minute'], $launch_tmp_time['second'],
                                  $launch_tmp_time['month'], $launch_tmp_time['day'   ], $launch_tmp_time['year'  ] );
    }

    if (empty($zone) === FALSE ) {
      // This is the default sort
      if ($field === 'zone' && $ts['sort'] === 'asc') {
        array_multisort($zone, SORT_ASC, $nickname, SORT_ASC, $runtime, SORT_DESC, $all_instances_list);
      }
      else {
        if ($field === 'runtime') {
          $sort_by_key = $runtime;
        }

        $sort_order = SORT_ASC;
        if ($ts['sort'] === 'desc') {
          $sort_order = SORT_DESC;
        }
        array_multisort($sort_by_key, $sort_order, $all_instances_list);
      }
    }
  }

  foreach ( $all_instances_list as $instance ) {

    $cloud_context  = $instance['cloud_context'];
    $key            = $instance['instance_id'  ];
    $name           = trim($instance['name'    ]);
    $state          = $instance['state'        ];
    $user_key       = $instance['user'         ];
    $hostname       = $instance['hostname'     ];
    $zone           = $instance['zone'         ];
    $runtime        = $instance['runtime'      ];
    $lock_status    = isset( $instance['lock_status'  ] ) ? $instance['lock_status'  ] : FALSE;

    $cloud_menu_path = cloud_get_menu_path($cloud_context );
    $form['Nickname'][$key] =  array(
      '#markup' => l( $name,
                       $cloud_menu_path . '/instances/describe/' . $key . '/info',
                       array('query' => array('instance_id' => urlencode($key))))
                  . ( ($lock_status == 'Lock')
                      ? '<a class=lockIcon title=Lock>&nbsp;&nbsp;&nbsp;&nbsp;</a>'
                      : ''
                    ),
    ); // end of array

    $instance_idFormatted = (strlen($key) > 10)
                          ? substr($key, 0, 8) . '...'
                          : $key;
    $form['hdnName'   ][$key] = array(
      '#type' => 'hidden',
      '#value' => addslashes($name),
    );
    $form['keyUser'   ][$key] = array('#markup' => t( $user_key ));
    $form['ID'        ][$key] = array('#markup' => t( $instance_idFormatted        ));
    $form['State'     ][$key] = array(
      '#type' => 'hidden',
      '#value' => t( $state ),
    );
    $form['Status'    ][$key] = array('#markup' => t( ucwords($state)              ));

    $public_dns = substr( $hostname, 0, strpos($hostname,    '.') );
    if ( strlen($public_dns) > 3 ) { // truncate only in case the name is long
      $public_dns = substr($hostname, 0, strpos($hostname,    '.')) . '...';
    }
    else {

      $public_dns = $hostname;
    }

    $form['Public_DNS'][$key] = array('#markup' => l($public_dns, 'http://' . $hostname, array('attributes' => array('target' => '_blank'), 'query' => array()) ));
    $form['Zone'      ][$key] = array('#markup' => t( $zone));
    $form['Runtime'   ][$key] = array('#markup' => t( ($runtime)
                                                       ? cloud_get_runtime($runtime)
                                                       : '1 day' ));
    $form['Cloud'     ][$key] = array(
      '#type' => 'hidden',
      '#value' => $cloud_context,
    );
    $form['KeyName'   ][$key] = array(
      '#type' => 'hidden',
      '#value' => ($user_key)
                                                                      ?  $user_key
                                                                      :  '-',
    );
  }

  $form['pager'    ] = array('#value' => theme('pager', array('tags' => NULL, 'element' => 0)));
  $form['#redirect'] = FALSE;

  return $form;
}

/**
 * To handle the submit event of cloud_display_dashboard function
 * by diff. form values
 * @param $form_id
 *        This is $form_id which is posted by page
 * @param $form_values
 *        This is form_values which is posted by filerting values
 * @return form_values with key-values
 */
function cloud_display_dashboard_submit($form_id, &$form_state) {
  $form_values = $form_state['values'];
  $form_state['redirect'] = url('clouds', array('query' => array('filter' => $form_values['filter'], 'operation' => $form_values['operation']), 'absolute' => TRUE));
}

/**
 * To handle the theme of cloud_display_dashboard function
 * by diff. form values
 * @param $form
 *        This is $form which is posted by filerting values
 * @return form object with its theme settings
 */
function theme_cloud_display_dashboard($variables) {
  $form = $variables['form'];

  $output = drupal_render($form['options']);

  if ( isset($form['options']['filter']['#default_value']) ) {

    $filter_col = $form['options']['operation']['#default_value'];
    if ( $filter_col == 0 ) {
      $sql_col = 'instance_nickname';
    }
    elseif ($filter_col == 1 ) {
      $sql_col = 'instance_id';
    }
    elseif ($filter_col == 2 ) {
      $sql_col = 'dns_name';
    }

    $filter = $form['options']['filter']['#default_value'];
    $filter_params = array(
      'column' => $sql_col,
      'filter_value' => $filter,
    );
  }
  else {

    $filter_params = array();
  }

  $all_instances_list = cloud_get_all_instances_list($filter_params);


  $form['Nickname'] = !empty($form['Nickname']) ? $form['Nickname'] : array();
  $form['pager']['#value'] = !empty($form['pager']['#value']) ? $form['pager']['#value'] : '';
  $rows = array();
  foreach (element_children($form['Nickname']) as $key) {

    $cloud_name = $form['Cloud'][$key]['#value'];

    $row = array();
    $lock_image = '';

    $row[] = array(
      'data' => drupal_render($form['Nickname'][$key]) . $lock_image,
      'class' => array('nickname-column'),
    );
    $row[] = drupal_render($form['ID'        ][$key]);
    $row[] = drupal_render($form['Public_DNS'][$key]);
    $row[] = drupal_render($form['Status'    ][$key]);
    $row[] = drupal_render($form['Zone'      ][$key]);
    $row[] = drupal_render($form['keyUser'   ][$key]);
    $row[] = drupal_render($form['Runtime'   ][$key]);

    $action_data = $all_instances_list[$key]['action_data'];
    $row[] =  array(
      'data' => $action_data,
      'class' => 'action-column',
    );
    $rows[] = $row;
  }

  $table_attr = array();
  $table_attr['id'] = 'all_instances_list_table';
  $table_attr['autoupdate_url'] = url( 'clouds/callback_get_all_instances_list' );

  $output .= theme('table', array('header' => $form['header']['#value'], 'rows' => $rows, 'attributes' => $table_attr));


  if ($form['pager']['#value']) {
    $output .= drupal_render($form['pager']);
  }

  $reload_link = l(t('- Refresh Page -'), 'clouds/getdata',    array('query' => array('src' => 'clouds')) ); //,  NULL ,  "src=clouds" ) ;
  $ref_link = array(
    '#type' => 'item',
    '#prefix' => '<div id="link_reload" align="right">',
    '#suffix' => '</div>',
    '#markup' => $reload_link,
  );
  $output .= drupal_render($ref_link);
  $output .= drupal_render_children($form);

  return $output;
}


/**
 * AJAX callback for main cloud listing page
 */
function _cloud_callback_get_all_instances_list() {
  // retrieve the table from the cloud_display_dashboard form
  $form = drupal_get_form('cloud_display_dashboard');
  $output = drupal_render($form);
  // Send only the body do not send the headers
  $index_start = strrpos($output, '<tbody>');
  $index_end   = strrpos($output, '</tbody>');

  if ( isset($form['Nickname']) === FALSE) { // No element present
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
 * To get the list of all users of our system
 * with sorting by user's name
 *
 * @return array of users list
 */
function cloud_get_users() {

  $user_list = array();
  $query = "SELECT name FROM {users} ";

  $result = db_query( $query );
  $user_list[CLOUD_NONE] = CLOUD_NONE;


  foreach ($result as $user) {
    if (empty($user->name)) {
      continue;
    }

    $user_list[$user->name] = $user->name;
  }

  //asort($user_list);
  return $user_list;
}

/**
 * To get the cloud type of server-template
 * by template-id
 * @param $server_template_id
 *        This is template-id is passed to get the cloud type info
 * @return cloud-type info object
 */
function cloud_server_templates_get_cloud_type($server_template_id = '') {

  if ( module_exists('cloud_server_templates')) {

    return _cloud_server_templates_get_cloud($server_template_id);
  }
}

/**
 * To get the server-template info details
 * by template-id
 * @param $template_id
 *        This is template-id is passed to get its info
 * @return server-template info object
 */
function cloud_get_server_template_info($template_id) {

  return _cloud_server_templates_get_info($template_id);
}

/**
 * Implements hook_cloud_save().
 */
function cloud_save(&$cloud) {
  $op = ''; //op is passed to all modules
  if ($cloud->is_new == TRUE) {
    $op = 'create';
    drupal_write_record(CLOUD_CLOUDS_TABLE, $cloud);
  }
  else {
    $op = 'edit';
    drupal_write_record(CLOUD_CLOUDS_TABLE, $cloud, 'cloud_name');
  }
  //allow other modules to interact on the data
  module_invoke_all('cloud_save', $op, $cloud);
}

/**
 * Basic cloud load function
 * Stubbed out for future use
 */
function cloud_load($cloud_names) {
  // ERROR: return statement not found in hook_load

}

/**
 * Basic cloud delete function
 */
function cloud_delete($cloud_name) {
  $num_deleted = db_delete(CLOUD_CLOUDS_TABLE)
      ->condition('cloud_name', $cloud_name)
      ->execute();
  module_invoke_all('cloud_delete', $cloud_name);
}

/**
 * Set information of a sub-cloud
 * This function does the necessary initialisation for a sub-cloud
 * This is the mode of communication between the Cloud base module and the sub-cloud
 *
 * @param $op
 *          Operation that was performed on the sub-cloud
 *          enable: When a sub-cloud is enabled
 *                      Do the necessary initialisation for a sub-cloud
 *          disable: When a sub-cloud is disabled
 *                      Do the cleanup required when a sub-cloud is disabled.
 *
 * @param $module
 *             The sub-cloud which was enabled/disabled.
 * @return
 */
function _cloud_set_info($op, $module) {

  $params = module_invoke($module, 'cloud_set_info');
  // if someone doesn't implement cloud_onfo
  if (empty($params)) {
    return;
  }

  switch ($op) {
    case 'enable':

      // for cloud sub system

      $insert_query = db_insert(CLOUD_CLOUDS_TABLE)
        ->fields(
          array(
        'cloud_name' => $module,
        'cloud_display_name' => empty($params['cloud_display_name']) ? $module : $params['cloud_display_name'],
        'last_update_time' => REQUEST_TIME,
        'host_entries_refresh_time' => empty($params['host_entries_refresh_time']) ? CLOUD_HOST_ENTRIES_REFRESH_TIME : $params['host_entries_refresh_time'],
        'module' => $params['module'],
        'base_cloud' => $params['base_cloud'],
      ))
          ->execute();

      // If path is specified then use that path else default path is clouds/module_name

      $menu_path = isset($params['menu_path'])
                 ? $params['menu_path']
                 : 'clouds/' . $module;
      variable_set( $module . '_menu_path', $menu_path );

      //default admin menu path
      variable_set($module . '_admin_menu_path', 'admin/config/' . $module);
      cloud_server_templates_notify( '', 'enable', $module );

      $instance_types = empty($params['instance_types'] )
                      ? array()
                      : $params['instance_types'];

      if ( empty( $instance_types) === FALSE ) { // If Instance Types array is passed
        // Intialize the Instance type table for this cloud_context

        $insert_query = db_insert(CLOUD_INSTANCE_TYPE_TABLE)
          ->fields(
            array(
          'instance_type',
          'description',
          'display_order',
          'cloud_type',
        ));

        $i = 0;
        foreach ($instance_types as $key => $val) {

          $insert_query
            ->values(
              array(
            'instance_type' => $key,
            'description' => $val,
            'display_order' => $i,
            'cloud_type' => $module,
          )
            );


          if ($i < count($instance_types) - 1 ) {

          }
          $i++;
        }

        $insert_query->execute();
      }

      // Pricing module is enabled. Populate the Pricing data
      if ( module_exists('cloud_pricing') ) {

        $pricing_data = empty($params['cloud_pricing_data'] )
                      ? array()
                      : $params['cloud_pricing_data'];
        foreach ($pricing_data as $key => $data) {

          $data_values = array(
            'instance_type_select' => $data['instance_type'     ]  ,
            'description_text' => $data['description'       ]  ,
            'linux_or_unix_usage_text' => $data['linux_or_unix_cost']  ,
            'windows_usage_text' => $data['windows_cost'      ]  ,
            'cloud_context' => $module  ,
          );

          // Insert the data in the Pricing table
          _cloud_pricing_insert($data_values);
        }
      }

      break;

    case 'disable':

      $query_args   = array();


      $query_args = array(':cloud_name' => $module);
      $num_deleted = db_delete(CLOUD_CLOUDS_TABLE)
        ->condition('cloud_name', $module)
        ->execute();
      $query_args = array(':cloud_type' => $module);
      $num_deleted = db_delete(CLOUD_INSTANCE_TYPE_TABLE)
        ->condition('cloud_type', $module)
        ->execute();

      cloud_server_templates_notify( '', 'disable', $module );

      if ( module_exists('cloud_pricing') ) {

        cloud_pricing_delete_data($module);
      }

      // Delete cloud_context_menu_path
      variable_del( $module . '_menu_path');

      break;
  }
}



/**
 *  Get deploy info
 *
 * @param $params
 * @return
 */
function cloud_get_deploy_info($params) {

  return module_exists('cloud_resource_allocator')
  ? module_invoke($resource_allocator, 'get_deploy_info', $params)
  : FALSE;
}


/**
 * Thisi is a helper function used by sub-clouds for admin settings
 *
 * @param $form
 *          Form to be displayed by sub-cloud
 * @param $cloud_context
 *          Sub-cloud for which this form is to be filled
 * @return
 *          settings  form for sub-cloud
 */
function cloud_settings_form(&$form, $cloud_context) {

  $cloud_found        = FALSE;
  $enabled_cloud_list = cloud_get_clouds();

  for ($row_counter = 0; $row_counter < count($enabled_cloud_list); $row_counter++) {

    $cloud_name = $enabled_cloud_list[$row_counter]['cloud_name'];
    if ( $cloud_name == $cloud_context ) {

      $cloud_found = TRUE;
      break;
    }
  }

  if ( $cloud_found === FALSE  ) {

    drupal_set_message(t('Cloud not present in the enabled Cloud list'), 'error' );
    drupal_goto('/clouds');
    return;
  }

  $base_form = array();
  foreach ( $form as $key => $value ) {

    $base_form[$cloud_context . '_' . $key] = $value;
    $default_value = !empty($value['#default_value']) ? $value['#default_value'] : '';
    $base_form[$cloud_context . '_' . $key]['#default_value'] = @variable_get( $cloud_context . '_' . $key, $default_value );
  }

  return system_settings_form($base_form);
}


/**
 * Return the type of instance
 * It should be small, micro etc.
 *
 * @param  $cloud_context
 * @return
 */
function cloud_get_instance_types($cloud_context) {

  // Move instance_type table in pricing to cloud.install
  // call $array() = hook_get_instance_types() followed by $cloud_coontext
  // hook_get_instance_types will return an array of instance types

  return $instance_types;
}

/**
 *
 * @param type $cloud_context
 * @param type $cloud
 * @param type $filter_params
 * @return type
 */
// function cloud_launch_instance($cloud_context, $cloud, $filter_params = array() ) {

//   $result_launch = module_invoke($cloud_context, 'cloud_launch_instance_using_params', $cloud, $filter_params);
//   return $result_launch;
// }


/**
 *
 * @param type $cloud_context
 * @param type $cloud
 * @param type $filter_params
 * @return type
 */
// function cloud_terminate_instance($cloud_context, $cloud,    $filter_params = array() ) {

//   $result_launch = module_invoke($cloud_context, 'cloud_terminate_instance_using_params', $cloud, $filter_params );
//   return $result_launch;
// }


/**
 *
 * @param type $cloud_context
 * @param type $cloud
 * @param type $params
 * @return type
 */
function cloud_update_instance_staticip_mapping($cloud_context, $cloud, $params = array() ) {

  $result = module_invoke($cloud_context, 'cloud_update_elastic_ip_mapping', $cloud, $params );
  return $result;
}

/**
 *
 * @param type $cloud_context
 * @param type $cloud
 * @param type $params
 * @return type
 */
function cloud_associate_instance_staticip_mapping($cloud_context, $cloud, $params = array() ) {

  $result_associate = module_invoke($cloud_context, 'cloud_associate_elastic_ip', $cloud, $params );
  return $result_associate;
}

/**
 *
 * @param type $cloud_context
 * @param type $cloud
 * @param type $params
 * @return type
 */
function cloud_get_instance_pub_ip($cloud_context, $cloud, $params = array() ) {

  $pub_ip = module_invoke($cloud_context, 'cloud_get_instance_pub_ip', $cloud, $params );

  return $pub_ip;
}


/**
 * Hook hook_cloud_get_all_instances().
 * This hook returns the list of instances present in a sub-cloud
 * This will iterate through all the enabled sub-clouds
 * and call the hook to get the instances information.
 * All the instances are merged in one list
 * If $return is passed, drupal_goto will not be called.  the drupal_goto
 * is not really needed, but kept here in case a module uses it
 */
function cloud_get_instances($cloud_context, $filter_params = array(), $return = FALSE) {
  $cloud_found = FALSE;
  $enabled_cloud_list = cloud_get_clouds();
  $enabled_cloud = array();

  foreach ($enabled_cloud_list as $key => $value) {
    $cloud_name = $enabled_cloud_list[$key]['cloud_name'];
    if ( $cloud_name == $cloud_context ) {
      $cloud_found = TRUE;
      $enabled_cloud = $enabled_cloud_list[$key];
      break;
    }
  }

  if ( $cloud_found === FALSE  ) {
    drupal_set_message(t('Cloud not present in the enabled Cloud list'), 'error' );
    if ($return == FALSE) {
      drupal_goto('/clouds');
    }
    return array();
  }

  $instances_list = module_invoke($enabled_cloud['module'], 'cloud_get_all_instances', $cloud_context, $filter_params );
  return $instances_list;
}


/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function cloud_get_module_name($cloud_context) {

  $enabled_cloud_list = cloud_get_clouds();
  $enabled_cloud = array();

  foreach ($enabled_cloud_list as $key => $value) {
    $cloud_name = $enabled_cloud_list[$key]['cloud_name'];
    if ( $cloud_name == $cloud_context ) {
      return $enabled_cloud_list[$key]['module'];
    }
  }
  return '';
}


/**
 * Hook hook_cloud_get_instance_volumes().
 * This hook returns the list of volumes present in a instance
 * This will iterate through all the instances of a cloud
 * and call the hook to get the volume information.
 * All the volumes are merged in one list
 */
function cloud_get_instance_volumes($cloud_context, $instance_id ) {
  $cloud_found = FALSE;
  $enabled_cloud_list = cloud_get_clouds();
  $enabled_cloud = array();

  foreach ($enabled_cloud_list as $key => $value) {
    $cloud_name = $enabled_cloud_list[$key]['cloud_name'];
    if ( $cloud_name == $cloud_context ) {
      $cloud_found = TRUE;
      $enabled_cloud = $enabled_cloud_list[$key];
      break;
    }
  }

  if ( $cloud_found === FALSE  ) {
    drupal_set_message(t('Cloud not present in the enabled Cloud list'), 'error' );
    drupal_goto('/clouds');
    return;
  }

  $volumes_list = module_invoke($enabled_cloud['module'], 'cloud_get_all_instance_volumes', $cloud_context, $instance_id );
  return $volumes_list;
}



/**
 * Update a particular entry on the Monitoring server
 *
 * @param $op
 * @param $dns
 * @return unknown_type
 */
function cloud_snmp_update_entry($cloud_context, $op, $dns) {

  $content = '';

  if ($op == 'add' || $op == 'remove' || $op == 'all') {
    $content .= _cloud_snmp_update_host_entries($cloud_context, $op, $dns);
  }
  cloud_log_to_db("SNMP Updation Result", $content);
  return;
}

/**
 * Update SNMP entries
 * This is used for monitoring.
 *
 * @param $op
 *          Operation is to add/delete/all
 * @param $dns
 *          DNS entry to be used for updation
 * @return
 *          The response
 */
function _cloud_snmp_update_host_entries($cloud_context, $op, $dns) {

  $monitor_url = cloud_get_info( $cloud_context, 'monitor_url' );

  cloud_log_to_db('INSTANCE_MONITOR_URL', $monitor_url);
  $data = array(
    'op' => $op,
    'dns' => $dns,
  );

  if (  empty($monitor_url) || valid_url($monitor_url, TRUE ) === FALSE  ) {

    cloud_log_to_db($cloud_context . ': instance_monitor_url is empty.', $monitor_url);
    return;
  }

  $url  = parse_url($monitor_url);
  $host = 'http://' . $url['host'] . '/';

  list($header, $content) = cloud_post_request(
    $host . CLOUD_REMOTE_SNMP_MANAGER,
    'drupal server',
    $data
  );

  cloud_log_to_db('RRD_SERVER_CONTENT', $content);

  return $content;
}


/**
 * Audit the user activity
 * This is maintained on the server
 *
 * @param  $arr_audit
 *              array of the information
 * @return
 */
function cloud_audit_user_activity($arr_audit = array()) {

  if (module_exists('cloud_activity_audit')) {
    cloud_activity_audit_log($arr_audit);
  }

  return;
}

/**
 * This is a wrapper created for a Javascript message
 *
 * @param $msg
 * @return
 */
function cloud_get_messagebox($msg, $action_of_location = '') {

  return "javascript: if ( confirm('$msg') )  { $action_of_location   return TRUE ; }  else return false ; ";
}

/**
 *
 *  Update the billing information for a particular sub-cloud
 *
 * @param $cloud_context
 *          Sub-cloud of which information is to be updated
 * @param $instance_id
 *          instance-id who's information is to be updated
 * @param $instance_state
 *          state of the instance whether it's running halted etc.
 * @param $launch_time
 *          The launch time of instance
 * @param $instance_type
 *          Whether it's a small micro etc.
 * @return
 */
function cloud_billing_update($cloud_context, $instance_id = '', $instance_state = '', $launch_time = '', $instance_type = '') {

  if (module_exists('cloud_billing')) {
    _cloud_billing_update($cloud_context,
                   $instance_id   ,
                   $instance_state,
                   $launch_time   ,
                   $instance_type);
  }

  return;
}


/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function cloud_failover_get_failover_flag($cloud_context, $instance_id) {
  if (module_exists('cloud_failover')) {
    return _cloud_failover_get_instance_failover_flag($cloud_context, $instance_id);
  }
}

/**
 * Get the billing usage estimate for a particular cloud
 *
 * @param $cloud_context
 *          sub-cloud which is to be queried for billing information
 * @return
 */
function cloud_billing_usage_estimate($cloud_context) {

  if (module_exists('cloud_billing')) {
    _cloud_billing_usage_estimate($cloud_context);
  }

  return;
}



/**
 * Get the sub-clouds information
 *
 * @return
 *      array of all enabled sub-clouds
 */
function cloud_get_all_clouds() {

  $enabled_cloud_list = cloud_get_clouds();
  $cloud_list = array();

  for ($row_counter = 0; $row_counter < count($enabled_cloud_list); $row_counter++) {
    $cloud_name = $enabled_cloud_list[$row_counter]['cloud_name'];
    $cloud_list[$cloud_name] = $cloud_name;
  }

  return $cloud_list;
}


/**
 * Get the display names of all the enabled sub-clouds
 *
 * @return
 *      array of cloud names
 */
function cloud_get_all_clouds_display_name() {

  $enabled_cloud_list = cloud_get_clouds();
  $cloud_list         = array();

  for ($row_counter = 0; $row_counter < count($enabled_cloud_list); $row_counter++) {
    $cloud_name = $enabled_cloud_list[$row_counter]['cloud_name'];
    $cloud_list[$cloud_name] = $enabled_cloud_list[$row_counter]['cloud_display_name']  ;
  }

  return $cloud_list;
}

/**
 * Helper function to generate admin url for a
 * particular sub-cloud. The admin url is stored during cloud
 * creation
 */
function cloud_get_admin_url($cloud_display_name, $cloud_context) {
  $admin_url = variable_get($cloud_context . '_admin_menu_path', '');
  return filter_xss(l(t('@cloud_name Settings', array('@cloud_name' => $cloud_display_name)), $admin_url));
}

/**
 * Helper function to set the a drupal message that configuration
 * isn't setup.
 */
function cloud_set_config_error_message($cloud_display_name, $cloud_context) {
  $admin_url = '';
  if (user_access($cloud_context . ' administer cloud')) {
    $admin_url = cloud_get_admin_url($cloud_display_name, $cloud_context);
  }
  else {
    $admin_url = $cloud_display_list[$cloud_context];
  }
  drupal_set_message(t('The variables are not correctly configured:') . $admin_url, 'error', FALSE);
}


function _cloud_instance_templates($cloud_context) {

  return drupal_goto( 'design/server_templates/' . $cloud_context . '/list')  ;
}


/**
 * Get templates of a particular sub-cloud
 * @param $cloud_context
 * @return
 */
function cloud_get_server_templates($cloud_context = '') {

  return cloud_server_templates_get_server_templates_by_cloud_db($cloud_context);
}


/**
 * Get a instance information using the instance_id from list of instances
 *
 * @param $cloud_context
 *          sub-cloud to be searched for instance
 * @param $instance_id
 *          instance-id which is to be searched
 * @return
 */
function cloud_get_instance($cloud_context = '', $instance_id = '' ) {

  $filter = array();
  $filter['column'] = 'instance_id';
  $filter['filter_value'] = $instance_id;

  $module = cloud_get_module($cloud_context);

  $instance_list = module_invoke($module, 'cloud_get_instance', $cloud_context, $filter);
  if ( empty($instance_list) || isset($instance_list[$instance_id] ) === FALSE  ) {

    return FALSE;
  }

  return $instance_list[$instance_id];
}


/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function cloud_get_ssh_key($cloud_context = '', $key_name = '') {

  $params['key_name'] = $key_name;
  $params['cloud_context'] = $cloud_context;
  $module = cloud_get_module($cloud_context);
  return module_invoke($module, 'cloud_get_ssh_key', $params);
}


/**
 *  This function is called for notification of operations
 *
 */
function cloud_callback_cluster_operation($op, $cloud_context = '', $params = array() ) {

  switch ($op) {

    case 'cluster_form_submit':
      if ( module_exists('cloud_cluster')) {
        module_invoke( 'cloud_cluster', 'add_server_submit', $cloud_context, $params);
      }

      return;
  }
}


/**
 *  This function is called for notification of Script operations
 *
 */
function cloud_script_operation($cloud_context = ''  , $op, $params = array() ) {

  if ( module_exists('cloud_scripting')) {

    switch ($op) {

      case 'get_master_script_id':
        return _cloud_scripting_get_id_from_master($params['script_type'] );

      case 'save_new_execute_script':
        return _cloud_scripting_register_script_save($instance_id, $script_id,    '0', $cloud_context, $template_id );
        //return _cloud_scripting_register_script_save($instance_id, $script_id, $status , $cloud , $template_id);


    }
  }
  return;
}

function _cloud_activity_audit_get_cloud_module_list() {

  $module_list = array(
    'cloud_alerts' => 'cloud_alerts'          ,
    'cloud_activity_audit' => 'cloud_activity_audit'  ,
    'cloud_cluster' => 'cloud_cluster'         ,
    'cloud_failover' => 'cloud_failover'        ,
    'cloud_inputs' => 'cloud_inputs'          ,
    'cloud_scripting' => 'cloud_scripting'       ,
    'cloud_server_templates' => 'cloud_server_templates',
  );

  asort( $module_list);
  return $module_list;
}



/**
 * Fetch the filter from the submitted form
 *
 * @param $form_values
 *          array with the form elements
 * @param $value_type
 *          whether it is operation or the filter value
 * @param $default_value
 *           in case the form doesn't have a value then this is the default value
 * @return
 *          The value present in the form
 */
function cloud_get_filter_value($form_values, $value_type, $default_value = FALSE) {

  /*if ( isset($form_values) === FALSE
   ||    isset($form_values['post']) === FALSE  ) {

   return $default_value;
   }

   $form_values = $form_values['post'];*/


  if ( isset($form_values) === FALSE
    ||     isset($form_values['input']) === FALSE  ) {

    return $default_value;
  }

  $form_values = $form_values['input'];

  if (isset($form_values) === FALSE ) {

    return $default_value;
  }

  if (isset($form_values[$value_type] ) ) {

    return $form_values[$value_type];
  }

  if (isset($_REQUEST[$value_type] ) ) {

    return $_REQUEST[$value_type];
  }

  return $default_value;
}



/**
 * Get the instances list of all sub-clouds
 * This will query all the enabled sub-clouds for the instances.
 *
 * @param $filter_params
 *              This filter will be applied by the sub-cloud.
 *              Only those instances which pass this filter will be returned
 * @return
 *              List of instances from all the enabled sub-clouds
 */
function cloud_get_all_instances_list($filter_params = array() ) {

  // Get the list of all instances
  $cloud_list         = cloud_get_all_clouds();
  $all_instances_list = array();


  foreach ($cloud_list as $cloud_context) {

    $instances_list     = cloud_get_instances($cloud_context, $filter_params  );
    if (empty($instances_list) === FALSE) {
      $all_instances_list = array_merge($all_instances_list   , $instances_list );
    }
  }

  return $all_instances_list;
}


/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function cloud_get_all_instances_list_count() {

  return count(cloud_get_all_instances_list());
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function cloud_is_settings_done($cloud_context) {

  $is_cloud_enabled = TRUE;

  $host_uri       = trim( cloud_get_info( $cloud_context, 'host_uri'       ) );
  $api_version    = trim( cloud_get_info( $cloud_context, 'api_version'    ) );
  $access_key     = trim( cloud_get_info( $cloud_context, 'aws_access_key' ) );
  $secret_key     = trim( cloud_get_info( $cloud_context, 'aws_secret_key' ) );
  $cloud_user_id  = trim( cloud_get_info( $cloud_context, 'user_id'        ) );

  if ( empty($host_uri       )
      ||    empty($api_version   )
      ||    empty($access_key    )
      ||    empty($secret_key    )
      ||    empty($cloud_user_id )
    ) {

    $is_cloud_enabled = FALSE;
  }

  return $is_cloud_enabled;
}

/**
 * Helper function that reads the cron semaphore.
 * If it is set, that means cron is running and is
 * updating the sub cloud tables.
 */
function _cloud_is_update_allowed() {
  $semaphore = variable_get('cron_semaphore', FALSE);
  $allowed = TRUE;
  if ($semaphore) {
    drupal_set_message(t("Cron is performing the cloud data refresh.  Please wait and try again."));
    $allowed = FALSE;
  }
  return $allowed;
}
