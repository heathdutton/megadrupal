<?php

/**
 * @file
 * All UI functions are here.
 */

/**
 * Individual large graph page.
 *
 * @param $instance_id
 *   A String containing the instance id
 */
function cloud_monitoring_instance_graph_details($instance_id) {
  $output = '';
  $monitor_id = cloud_monitoring_monitor_id_by_instance($instance_id);
  if ($monitor_id) {
    $class = cloud_monitoring_get_class($monitor_id);
    if ($class) {
      $host = cloud_monitoring_dns_by_id($instance_id);
      $params = _cloud_monitoring_get_params();
      $output .= theme('cloud_monitoring_graph_intervals', array('host' => $host, 'params' => $params));
      $output .= theme('cloud_monitoring_graph', array('class' => $class, 'host' => $host, 'seconds' => $params['seconds'], 'params' => $params));

      if ($params['seconds'] <= 3600) {
        $output .= theme('cloud_monitoring_data_table', array('class' => $class, 'host' => $host, 'seconds' => $params['seconds'], 'params' => $params));
      }
      else {
        //show a download link for all datasets above 1 hour
        $output .= theme('cloud_monitoring_data_link', array('class' => $class, 'host' => $host, 'seconds' => $params['seconds'], 'params' => $params));
      }
    }
  }
  return $output;
}

/**
 * Function to generate the dashboard for a single
 * cloud instance
 */
function cloud_monitoring_instance_details($cloud_context, $instance_id) {
  $output = '';
  $monitor_id = cloud_monitoring_monitor_id_by_instance($instance_id);
  $selected_host = cloud_monitoring_dns_by_id($instance_id);

  if ($monitor_id) {
    if (!cloud_monitoring_is_instance_active($monitor_id, $instance_id)) {
      $output .= t('Monitoring has been deactivated. To reactivate it,') . l(t('click here'), "design/cloud_monitoring/{$monitor_id}/start/{$cloud_context}/{$instance_id}", array('query' => array('destination' => "clouds/{$cloud_context}/instances/describe/{$instance_id}/monitor")));
    }
    else {
      $class = cloud_monitoring_get_class($monitor_id);
      if ($class) {
        $seconds = isset($_GET['seconds']) ? $_GET['seconds'] : '86400';
        $output .= drupal_render(drupal_get_form('cloud_monitoring_dashboard_form', '', '', $seconds, FALSE));
        $output .= theme('cloud_monitoring_dashboard', array('class' => $class, 'host' => $selected_host, 'seconds' => $seconds));
      }
    }
  }
  else {
    //return form to map it, also this form can later be used to auto setup the monitor server
    return drupal_get_form('cloud_monitoring_map_server_form', $selected_host, $instance_id, $cloud_context);
  }
  return $output;
}

/**
 * Monitoring Dashboard Page
 */
function cloud_monitoring_dashboard($monitor_id) {
  $output = '';
  if (isset($monitor_id)) {
    $class = cloud_monitoring_get_class($monitor_id);
    if ($class) {
      $hosts = $class->get_hosts();
      if ($hosts) {
        $selected_host = isset($_GET['host']) ? $_GET['host'] : array_shift(array_keys($hosts));
        $seconds = isset($_GET['seconds']) ? $_GET['seconds'] : '86400';

        $output .= drupal_render(drupal_get_form('cloud_monitoring_dashboard_form', $hosts, $selected_host, $seconds));
        $output .= theme('cloud_monitoring_dashboard', array('class' => $class, 'host' => $selected_host, 'seconds' => $seconds));
      }
      else {
        drupal_set_message(t('Collectd and the REST connector are not setup. Cannot generate dashboard. Please make sure the monitoring server is setup properly'), 'error');
      }
    }
  }
  return $output;
}

/**
 * Small form to launch a monitoring server
 */
function cloud_monitoring_launch() {
  $output = '';
  $form_arr = drupal_get_form('cloud_monitoring_launch_form');
  $output .= drupal_render($form_arr) ;
  return $output;

}

/**
 * Theme function to add time interval links at the top
 */
function theme_cloud_monitoring_graph_intervals($variables) {
  $time_links = array();
  $time_interval = array(
    '60' => t('1 Minute'),
    '300' => t('5 Minutes'),
    '3600' => t('1 Hour'),
    '7200' => t('2 Hours'),
    '86400' => t('1 Day'),
    '604800' => t('1 Week'),
    '31449600' => t('1 Year'),
  );

  foreach ($time_interval as $key => $value) {
    $params['seconds'] = $key;
    $time_links[] =  l($value, $_GET['q'], array('query' => $params + $variables['params']));
  }
  return implode(' | ', $time_links);
}

/**
 * Theme function to generate the graph
 */
function theme_cloud_monitoring_graph($variables) {
  $output = '';
  $class = $variables['class'];
  if (count($variables['params'])) {
    $output = $class->get_large_graph($variables['host'], $variables['seconds'], $variables['params']);
  }
  return $output;
}


/**
 * Theme function for generating the graphical dashboard
 * @param $class
 *  Object loaded from the monitoring class
 * @param $monitor
 *  Array containing the monitor's database information
 */
function theme_cloud_monitoring_dashboard($variables) {
  $output = '';
  $class = $variables['class'];
  $host = $variables['host'];
  $seconds = $variables['seconds'];
  if (isset($host)) {
    $plugins = $class->get_supported_plugins();
    foreach ($plugins as $plugin) {
      $plugin_info = $class->get_plugin_details($host, $plugin);
      $img_src = '';
      foreach ($plugin_info as $p) {
        if (is_array($p)) {
          $img_src .= $class->get_small_graph($host, $seconds, $p);
        }
      }

      if (!empty($img_src)) {
        $output .= '<div class="monitoring-graphs">';
        $output .= '<h2>' . $plugin . '</h2>';
        $output .= '<div class="graph">';
        $output .= $img_src;
        $output .= '</div>';
        $output .= '</div>';
      }

    }
  }
  return $output;
}

/**
 * Dashboard selection form
 */
function cloud_monitoring_dashboard_form($form_id, $form_values, $hosts, $selected_host = '', $seconds = '', $show_host = TRUE) {

  $form = array(
    '#method' => 'get',
  );

  if ($show_host == TRUE) {
    $form['host'] = array(
      '#type' => 'select',
      '#title' => t('Select a server'),
      '#options' => $hosts,
      '#default_value' => $selected_host,
    );
  }

  $form['seconds'] = array(
    '#type' => 'select',
    '#title' => t('Timespan'),
    '#options' => array(
      '60' => t('1 Minute'),
      '300' => t('5 Minutes'),
      '3600' => t('1 Hour'),
      '7200' => t('2 Hours'),
      '86400' => t('1 Day'),
      '604800' => t('1 Week'),
      '31449600' => t('1 Year'),
    ),
    '#default_value' => $seconds,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );
  return $form;
}

/**
 * Theme a link for downloading csv data
 */
function theme_cloud_monitoring_data_link($variables) {
  $output = '';
  $class = $variables['class'];
  $host = $variables['host'];
  $seconds = $variables['seconds'];
  $params = $variables['params'];
  
  if (isset($host)) {
    $output .= '<div>';
    $output .= $class->get_csv_data($host, $seconds, $params);
    $output .= '</div>';
  }
  return $output;
}

/**
 * Theme function for a data table
 */
function theme_cloud_monitoring_data_table($variables) {
  $output = '';
  $class = $variables['class'];
  $host = $variables['host'];
  $seconds = $variables['seconds'];
  $params = $variables['params'];
  
  if (isset($host)) {
    $data = $class->get_data($host, $seconds, $params);
    $header = $data['header'];
    $rows = array();
    foreach ($data['row'] as $key => $row) {
      $rows[] = $data['row'][$key];
    }
    $output .= theme('table', array( 'header' => $header, 'rows' => $rows) );
  }
  return $output;
}


/**
 * Main monitor listing page
 */
function cloud_monitoring_list() {
  drupal_add_js(cloud_get_module_base() . 'js/cloud.js');
  $output = '';
  $header = array(
    array(
      'data' => t('Monitor Name'),
      'field' => 'monitor_name',
      'sort' => 'asc',
      'class' => array('nickname-column')
    ),
    array(
      'data' => t('Hostname'),
      'field' => 'hostname',
    ),
    array(
      'data' => t('Type'),
      'field' => 'monitor_type',
    ),
    array(
      'data' => t('Action'),
      'class' => 'action-column',
    ),
  );
  $ts = tablesort_init($header);

  $monitors = cloud_monitoring_get_all_monitors($ts);
  $rows = array();
  foreach ($monitors as $monitor) {
    $action_data = cloud_display_action('images/edit.png', t('Edit'), "design/cloud_monitoring/{$monitor['monitor_id']}/details", array('html' => TRUE));
    $action_data .= cloud_display_action('images/icon_delete.png', t('Delete'), "design/cloud_monitoring/{$monitor['monitor_id']}/delete", array('html' => TRUE));
    $row = array(
      array(
        'data' => l($monitor['monitor_name'], 'design/cloud_monitoring/' . $monitor['monitor_id'] . '/details'),
        'class' => array('nickname-column'),
      ),
      l($monitor['hostname'], "http://{$monitor['hostname']}"),
      $monitor['monitor_type'],
    );
    $row[] = array(
      'data' => $action_data,
      'class' => 'action-column',
    );
    $rows[] = $row;
  }
  
  $form_output = drupal_get_form('cloud_monitoring_add_button');
  $output .= drupal_render($form_output) ; 
  $output .= theme('table', array( 'header' => $header, 'rows' => $rows));
  return $output;
}

/**
 * List of instances being monitored by a particular monitoring server
 */
function cloud_monitoring_instance_list($monitor_id) {
  drupal_add_js(cloud_get_module_base() . 'js/cloud.js');
  $output = '';
  $class = cloud_monitoring_get_class($monitor_id);
  if ($class) {
    $server_list = $class->get_server_list();
    $header = array(
        array(
          'data' => t('Name'), 
          'class' => array('nickname-column')
        ), 
        t('Id'), 
        t('Monitoring Status'), 
        array(
          'data' => t('Action'),
          'class' => 'action-column',
      ));
    $output .= theme('table', array( 'header' => $header, 'rows' => $server_list));
  }
  return $output;
}

/**
 * Function to add button to Listing page
 */
function cloud_monitoring_add_button() {
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add Existing Monitor'),
    '#submit' => array('cloud_monitoring_add_monitor'),
  );
  $form['submit_2'] = array(
    '#type' => 'submit',
    '#value' => t('Launch a new Monitor'),
    '#submit' => array('cloud_monitoring_add_monitor'),
  );
  return $form;
}

/**
 * Form submit for cloud_monitoring_add_button()
 */
function cloud_monitoring_add_monitor($form, &$form_state) {
  if ($form_state['values']['op'] == 'Add Existing Monitor') {
    $form_state['redirect'] = array('design/cloud_monitoring/add');
  }
  elseif ($form_state['values']['op'] == 'Launch a new Monitor') {
    $form_state['redirect'] = array('design/cloud_monitoring/launch');
  }
}

/**
 * Autocomplete function.  Call cloud function cloud_get_instances()
 * for autocomplete lookup
 */
function cloud_monitoring_instance_autocomplete($string = '') {
  $matches = array();
  $all_instances_list = array();
  $cloud_list = cloud_get_all_clouds();

  foreach ($cloud_list as $cloud_context) {
    $instances_list = cloud_get_instances($cloud_context, array('column' => 'instance_nickname', 'filter_value' => $string));
    $all_instances_list = array_merge($all_instances_list, $instances_list);
  }
  foreach ($all_instances_list as $instances) {
    $matches["{$instances['instance_id']}:${instances['cloud_context']}"] = $instances['name'];
  }
  print drupal_json_encode($matches);
  exit();
}

/**
 * Add Existing cloud monitor UI
 */
function cloud_monitoring_add() {
  return drupal_get_form('cloud_monitoring_add_form');
}

/**
 * Monitoring system UI details
 */
function cloud_monitoring_details($monitor_id) {
  $output = '';
  if ($monitor_id) {
    $class = cloud_monitoring_get_class($monitor_id);
    if ($class) {
      // warn user if server isn't setup
      if (!$class->is_active()) {
        drupal_set_message(t('This collectd server is not setup. Please ensure Collectd, the REST Connector are setup and active.'), 'error');
      }
      $monitor_data = $class->get_monitor_details();
      $elements = drupal_get_form('cloud_monitoring_detail_form', $monitor_data);
      $output .= drupal_render($elements);
    }
  }
  return $output;
}

/**
 * The monitoring details form
 */
function cloud_monitoring_detail_form($form, $form_state, $edit = array()) {

  $form['monitor_id'] = array(
    '#type' => 'hidden',
    '#value' => $edit['monitor_id'],
  );
  $form['monitor'] = array(
    '#type' => 'fieldset',
    '#title' => t('Monitor Server Details'),
  );

  $form['monitor']['list'] = array(
    '#type' => 'submit',
    '#value' => t('List Monitors'),
    '#submit' => array('cloud_monitor_list_submit'),
  );
  $form['monitor']['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
    '#submit' => array('cloud_monitor_delete_submit'),
  );

  $form['monitor']['info'] = array(
    '#type' => 'fieldset',
    '#title' => t('Monitor Info'),
  );

  $form['monitor']['info']['monitor_name'] = array(
    '#type' => 'item',
    '#title' => t('Cloud Monitor Name'),
    '#markup' => $edit['monitor_name'],
  );

  $form['monitor']['info']['hostname'] = array(
    '#type' => 'item',
    '#title' => 'Hostname',
    '#markup' => $edit['dns_name'],
  );

  $form['monitor']['info']['monitor_type'] = array(
    '#type' => 'item',
    '#title' => 'Monitoring System',
    '#markup' => $edit['monitor_type'],
  );

  $form['monitor']['info']['key_name'] = array(
    '#type' => 'item',
    '#title' => t('SSH Key'),
    '#markup' => $edit['key_name'],
  );

  $form['monitor']['servers'] = array(
    '#type' => 'fieldset',
    '#title' => t('Servers being monitored'),
  );
  $instances = cloud_monitoring_instance_list($edit['monitor_id']);
  $form['monitor']['servers']['instances'] = array(
    '#markup' => $instances,
  );
  return $form;

}


/**
 * AHAH callback function
 */
function cloud_monitoring_dropdown_callback($form, $form_state) {
  $cloud_type = !empty($form_state['values']['cloud_type']) ? $form_state['values']['cloud_type'] : '';
  return $form['launch']['ssh_wrapper']['key_name'] = array(
    '#type' => 'select',
    '#title' => t('SSH Key'),
    '#description' => t('Select the SSH Key to use'),
    '#options' => !empty($cloud_type) ? _cloud_monitoring_get_second_dropdown($cloud_type) : array(),
    '#default_value' => isset($my_values['key_name']) ? $my_values['key_name'] : '',
  );
}

/**
 * 1. Add support for Existing templates
 * 2. Add scripting support
 */
function cloud_monitoring_launch_form($form, &$form_state) {

  $my_values = array();

  $form = array();
  $form['launch'] = array(
    '#type' => 'fieldset',
    '#description' => 'Launch a new monitor server',
  );

  $form['destination'] = array(
    '#type' => 'hidden',
    '#name' => 'destination',
    '#value' => 'design/cloud_monitoring',
  );

  $form['launch']['monitor_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Cloud Monitor Name'),
    '#description' => t('Monitor Name: e.g. Collectd Monitoring Server'),
    '#required' => TRUE,
  );
  $existing_templates = cloud_monitoring_get_templates();

  if (count($existing_templates)) {
    $form['launch']['existing'] = array(
      '#type' => 'checkbox',
      '#title' => t('Launch with existing server template'),
      '#description' => t('Check this to launch with a server template that is already defined'),
      '#default_value' => TRUE,
    );

    $templates = array('' => '- none -');
    foreach ($existing_templates as $template) {
      $templates[$template['template_id']] = "{$template['cloud_type']} - {$template['template_nickname']}";
    }
    $form['launch']['server_template'] = array(
      '#type' => 'select',
      '#title' => t('Server Template'),
      '#description' => t('Pick a monitoring server template that has already been used before'),
      '#options' => $templates,
    );
  }

  $servers = cloud_server_templates_system_templates();
  $server_options = array('' => '----');
  foreach ($servers as $key => $value) {
    $server_options[$key] = $value['template_nickname'];
  }

  $form['launch']['system_template'] = array(
    '#type' => 'select',
    '#title' => t('System Template'),
    '#description' => t('Pick a system defined server template.  System templates are defined via hook_cloud_server_system_templates().  Once
                        a system template is used, it will be imported into the server templates database tables.  You can then modify them.'),
    '#options' => $server_options,
  );

  $monitor_type = _cloud_monitoring_available_monitors();
  foreach ($monitor_type as $key => $value) {
    if (preg_match('/xcp/', $key) == TRUE) {
      unset($monitor_type[$key]);
    }
  }
  $form['launch']['monitor_type'] = array(
    '#type' => 'select',
    '#title' => t('Monitoring System'),
    '#description' => t('Select the monitoring system this server contains'),
    '#options' => $monitor_type,
  );
  $clouds = cloud_get_clouds();
  $cloud_options = array('' => '----');
  foreach ($clouds as $key => $value) {
    if ($value['cloud_name'] != 'xcp') {
      $cloud_options[$value['cloud_name']] = $value['cloud_display_name'];
    }
  }

  $cloud_type = !empty($form_state['values']['cloud_type']) ? $form_state['values']['cloud_type'] : t('');

  $form['launch']['email'] = array(
    '#type' => 'textfield',
    '#title' => t('Notification Email'),
    '#description' => t('The monitoring server installer will send an email after the script has run.'),
  );
  $form['launch']['cloud_type'] = array(
    '#type' => 'select',
    '#title' => t('Cloud Type'),
    '#description' => t('Select which cloud this monitoring system will live on'),
    '#options' => $cloud_options,
    '#default_value' => $cloud_type,
    '#ajax' => array(
      'callback' => 'cloud_monitoring_dropdown_callback',
      'wrapper' => 'ssh-wrapper',
      'method' => 'replace',
      'effect' => 'fade'
    ),
  );

  $form['launch']['ssh_wrapper'] = array(
    '#tree' => TRUE,
    '#prefix' => '<div id="ssh-wrapper">',
    '#suffix' => '</div>',
  );

  $form['launch']['ssh_wrapper']['key_name'] = array(
    '#type' => 'select',
    '#title' => t('SSH Key'),
    '#description' => t('Select the SSH Key to use'),
    '#options' => _cloud_monitoring_get_second_dropdown($cloud_type),
    '#default_value' => isset($my_values['key_name']) ? $my_values['key_name'] : '',
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Launch'),
  );
  return $form;
}

/**
 * Custom validation function.
 */
function cloud_monitoring_launch_form_validate($form, &$form_state) {
  $ssh_key = &$form_state['values']['ssh_wrapper']['key_name'];
  if (isset($form_state['values']['existing']) && $form_state['values']['existing'] == TRUE) {
    //validate that the existing template.
    if (empty($form_state['values']['server_template'])) {
      form_set_error('server_template', t('Server Template field is required.'));
    }
  }
  else {
    if ($ssh_key == '-1') {
      form_set_error('ssh_wrapper][key_name', t('SSH Key must be set'));
    }
    if (empty($form_state['values']['system_template'])) {
      form_set_error('system_template', t('System Template field is required.'));
    }
    if (empty($form_state['values']['cloud_type'])) {
      form_set_error('cloud_type', t('Cloud Type field is required.'));
    }
    if (empty($form_state['values']['monitor_type'])) {
      form_set_error('monitor_type', t('Monitoring System field is required.'));
    }
    if (empty($form_state['values']['email'])) {
      form_set_error('email', t('Email field is required.'));
    }
  }
}

/**
 * Form submit function for Launching a new Monitoring server
 */
function cloud_monitoring_launch_form_submit($form, &$form_state) {
  //handle a new case
  if (isset($form_state['values']['existing']) && $form_state['values']['existing'] == TRUE) {
    //launch from existing template
    _cloud_monitoring_launch_existing_monitor($form_state);
  }
  else {
    _cloud_monitoring_launch_new_monitor($form_state);
  }
}

/**
 * Using an existing server template, launch an cloud instance,
 * and save it as a monitor in the cloud_monitor table
 */
function _cloud_monitoring_launch_existing_monitor($form_state) {
  //get the template id, and then launch it.
  $template_id = &$form_state['values']['server_template'];
  if ($template_id) {
    $cloud_type = _cloud_server_templates_get_cloud($template_id);
    $result = cloud_perform_action('', 'launch_using_params', $cloud_type, array('template_id' => $template_id));
    if (count($result)) {
      $instance_id = (string) array_shift($result);
      //lookup monitor_type
      $lookup = array_shift(cloud_monitoring_get_lookup_record($template_id));
      $monitor_type = !is_null($lookup) ? $lookup['monitor_type'] : '';
      aws_cloud_update_instance_nickname($instance_id, $cloud_type, $form_state['values']['monitor_name']);
      cloud_monitoring_save_monitor_record($instance_id, $form_state['values']['monitor_name'], $monitor_type, $cloud_type);
    }
  }
}

/**
 * Launch a new monitor server from a system template.  The template
 * information, including scripts, and inputs will be saved into the system.
 */
function _cloud_monitoring_launch_new_monitor($form_state) {
  $system_template = _cloud_monitoring_get_system_template($form_state['values']['system_template']);
  if (count($system_template)) {
    //now merge the values collected
    $system_template['cloud_type'] = $form_state['values']['cloud_type'];
    $system_template['key_name'] = $form_state['values']['ssh_wrapper']['key_name'];
    $image_id = _cloud_monitoring_lookup_image_id($system_template['cloud_type'], $system_template['image_nickname']);
    if (!empty($image_id)) {
      $system_template['image_id'] = $image_id;
      //save it to the template table
      $script_ids = _cloud_monitoring_save_scripts($system_template);
      $template_id = _cloud_monitoring_save_template($system_template);
      if ($template_id) {
        //write a record in the lookup table
        cloud_monitoring_save_lookup_record($template_id, $form_state['values']['monitor_type']);

        //save the input record with script_id, and template_id
        $values['module'] = 'cloud_server_templates';
        $values['template_id'] = $template_id;

        //store the user input
        _cloud_monitoring_save_script_input($template_id, $system_template, $form_state['values']['email']);

        //Call cloud launch
        $result = cloud_perform_action('', 'launch_using_params', $system_template['cloud_type'], array('template_id' => $template_id));
        if (count($result)) {
          $instance_id = (string) array_shift($result);
          //update the instance nickname to reflect the monitor server
          aws_cloud_update_instance_nickname($instance_id, $form_state['values']['cloud_type'], $form_state['values']['monitor_name']);
          cloud_monitoring_save_monitor_record($instance_id, $form_state['values']['monitor_name'], $form_state['values']['monitor_type'], $form_state['values']['cloud_type']);
        }
      }
    }
    else {
      drupal_set_message(t('Cannot find image id'), 'error');
    }
  }
  else {
    drupal_set_message(t('Cannot find the system template. Make sure it is declared properly in hook_cloud_server_system_templates()'), 'error');
  }
}

/**
 * Lookup a particular image_id
 */
function _cloud_monitoring_lookup_image_id($cloud_type, $image_nickname) {
  $image_id = '';
  $image_ids = aws_cloud_get_all_images_by_str_db($cloud_type, 'machine', $image_nickname);
  if (count($image_ids)) {
    //take the first one and remove blanks
    unset($image_ids[AWS_CLOUD_CLOUD_NONE]);
    $image = array_shift($image_ids);
    //because the db function above already formats
    //the text, do a quick regex to extract only the image_id
    if (preg_match('#\((.*?)\)#', $image, $match)) {
      $image_id = $match[1];
    }
  }
  return $image_id;
}

/**
 * Call cloud_server_templates_system_templates and format
 * the defined system templates.
 */
function _cloud_monitoring_get_system_template($template_key) {
  $template = array();
  $templates = cloud_server_templates_system_templates();
  if (isset($templates[$template_key])) {
    $template = $templates[$template_key];
  }
  return $template;
}

/**
 * Add an existing Monitoring Server
 */
function cloud_monitoring_add_form($form_state) {
  $form['monitor'] = array(
    '#type' => 'fieldset',
    '#description' => 'Use this form to register an existing cloud',
  );
  $form['destination'] = array(
    '#type' => 'hidden',
    '#name' => 'destination',
    '#value' => 'design/cloud_monitoring',
  );
  $form['monitor']['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Create',
  );
  $form['monitor']['monitor_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Cloud Monitor Name'),
    '#description' => t('Monitor Name: e.g. Collectd Monitoring Server'),
    '#required' => TRUE,
  );
  $form['monitor']['instance_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Instance Id'),
    '#description' => t('The Id and cloud_type of the existing server. For example: i980323:amazon_east'),
    '#required' => TRUE,
    '#autocomplete_path' => 'cloud_monitoring/instance_autocomplete',
  );
  $form['monitor']['monitor_type'] = array(
    '#type' => 'select',
    '#title' => t('Monitoring System'),
    '#description' => t('Select the monitoring system this server contains'),
    '#required' => TRUE,
    '#options' => _cloud_monitoring_available_monitors(),
  );
  $form['monitor']['submit_2'] = array(
    '#type' => 'submit',
    '#value' => 'Create',
  );
  return $form;
}

/**
 * Validate function
 */
function cloud_monitoring_add_form_validate($form, &$form_state) {
  $instance = explode(':', $form_state['values']['instance_id']);
  if (count($instance) != 2) {
    form_set_error('instance_id', t('Invalid instance id provided'));
  }

  if (cloud_monitoring_monitor_exists($instance[0]) !== FALSE) {
    form_set_error('instance_id', t('The monitor server @s exists already.', array('@s' => $form_state['values']['instance_id'])));
  }
}

/**
 * Handle submit/edit
 */
function cloud_monitoring_add_form_submit($form, &$form_state) {

  $data = array();
  $instance = explode(':', $form_state['values']['instance_id']);

  $data['monitor_name'] = $form_state['values']['monitor_name'];
  $data['instance_id'] = $instance[0];
  $data['monitor_type'] = $form_state['values']['monitor_type'];
  $data['cloud_type'] = $instance[1];
  $data['created'] = time();
  drupal_write_record('cloud_monitor', $data);
  drupal_set_message(t("Monitor saved"));

}

/**
 * Form to identify a server with a given monitoring server
 */
function cloud_monitoring_map_server_form($form_id, $form_state, $host, $instance_id, $cloud_context) {
  $monitors = cloud_monitoring_get_all_monitors();
  if ($monitors) {
    $form['map'] = array(
      '#type' => 'fieldset',
      '#title' => t('Map server to a monitor'),
      '#description' => t('Map this server to a monitoring server'),
    );

    $select = array();
    foreach ($monitors as $monitor) {
      $select[$monitor['monitor_id']] = $monitor['monitor_name'];
    }
    $form['map']['instance_id'] = array(
      '#type' => 'value',
      '#value' => $instance_id,
    );

    $form['map']['cloud_context'] = array(
      '#type' => 'value',
      '#value' => $cloud_context,
    );

    $form['map']['host'] = array(
      '#type' => 'value',
      '#value' => $host,
    );

    $form['map']['monitor'] = array(
      '#type' => 'select',
      '#title' => t('Select the monitoring server'),
      '#description' => t('Select a monitoring server for this instance.'),
      '#options' => $select,
    );

    //make sure that there are installable scripts defined and supported
    if (cloud_monitoring_get_installable($instance_id, $cloud_context)) {
      $form['map']['install'] = array(
        '#type' => 'checkbox',
        '#title' => t('Check this box to install the monitoring software'),
        '#description' => t('Doing so will automatically install monitoring software on this instance. Please note that if the monitoring system does not support automatic install, this option will be ignored.'),
      );
    }
    else {
      $form['map']['instructions'] = array(
        '#type' => 'item',
        '#title' => 'Instructions',
        '#value' => t('This instance does not support automatic monitoring software installation. To map this cloud instance to the monitoring server, make sure the monitoring software is installed and communicating with the monitoring server.'),
      );
    }

    $form['map']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Submit'),
    );
    return $form;
  }
  else {
    //inform the user
    drupal_set_message(t('No monitoring servers are configured in the system.  Please set one up here:') . l(t('Monitoring Setup'), 'design/cloud_monitoring'));
  }
}

/**
 * Form Validate for Mapping Server to a monitor
 */
function cloud_monitoring_map_server_form_validate($form, &$form_state) {
  $monitor_id = $form_state['values']['monitor'];
  $class = cloud_monitoring_get_class($monitor_id);
  if ($class) {
    $is_active = $class->is_active();
    if ($is_active == FALSE) {
      form_set_error('monitor', t('The monitoring server is not configured properly'));
    }
    //next make sure this host is available.
    $host = $form_state['values']['host'];
    $info = $class->get_plugin_details($host, NULL, 'false');
    //if an error object comes back, the host isn't configured
    if (!isset($form_state['values']['install']) || $form_state['values']['install'] == FALSE) {
      if (isset($info['error'])) {
        form_set_error('monitor', t('This instance is not setup to talk with the monitoring server'));
      }
    }
  }
  else {
    form_set_error('monitor', t('The monitoring class definition was not found.'));
  }
}

/**
 * Add the server to the database
 */
function cloud_monitoring_map_server_form_submit($form, &$form_state) {
  $cloud_context = $form_state['values']['cloud_context'];
  $instance_id = $form_state['values']['instance_id'];
  $monitor_id = $form_state['values']['monitor'];
  if (isset($form_state['values']['install']) && $form_state['values']['install'] == TRUE) {
    $class = cloud_monitoring_get_class($monitor_id);
    if ($class) {
      //if the class is loaded, then call install_monitor_script
      $params = array(
        'cloud_context' => $cloud_context,
        'instance_id' => $instance_id,
      );
      $return = $class->install_monitor_script($params);
      if ($return != FALSE) {
        //map the record to the monitor server
        cloud_monitoring_save_monitor_instance_record($monitor_id, $instance_id);
        drupal_set_message(t('The install scripts are running. Please refresh this page in 1-2 minutes.'));
      }
      else {
        drupal_set_message(t('The automatic install script cannot run for this instance. Please log into the server and install the monitoring software before mapping the instance.'), 'error');
      }
    }
    else {
      drupal_set_message(t('Unable to load monitoring server plugin.', 'error'));
    }
  }
  else {
    //just map the record
    cloud_monitoring_save_monitor_instance_record($monitor_id, $instance_id);
    drupal_set_message(t('Server is mapped to the monitor.'));
  }

}

/**
 * Redirect Form
 */
function cloud_monitor_list_submit($form, &$form_state) {
  $form_state['redirect'] = array('design/cloud_monitoring');
}

/**
 * Delete confirmation
 */
function cloud_monitor_delete_submit($form, &$form_state) {
  $destination = '';
  if (isset($_REQUEST['destination'])) {
    $destination = drupal_get_destination();
    unset($_REQUEST['destination']);
  }
  $form_state['redirect'] = array('design/cloud_monitoring/' . $form_state['values']['monitor_id'] . '/delete', $destination);
}

/**
 * Confirm cloud_monitor delete
 */
function cloud_monitor_confirm_delete($monitor_id) {

  cloud_monitoring_delete_monitor($monitor_id);
  drupal_goto('design/cloud_monitoring');
  return ;
  
  /*$form['monitor_id'] = array(
    '#type' => 'value',
    '#value' => $monitor_id,
  );
  $monitor = cloud_monitoring_load_monitor($monitor_id);
  return confirm_form($form, t('Are you sure you want to delete the monitor %nickname?', array('%nickname' => $monitor['monitor_name'])),
      'design/cloud_monitoring/' . $monitor_id,
  t('All related monitoring information will be deleted.  This action cannot be undone.'),
  t('Delete'), t('Cancel'));*/
}

/**
 * After confirm delete, do the actual deleting
 */
function cloud_monitor_confirm_delete_submit($form, &$form_state) {
  cloud_monitoring_delete_monitor($form_state['values']['monitor_id']);
  $form_state['redirect'] = 'design/cloud_monitoring';
}

/**
 * Start monitoring on an instance
 */
function cloud_monitoring_instance_start($monitor_id, $cloud_context, $instance_id) {
  $class = cloud_monitoring_get_class($monitor_id);
  if ($class) {
    $class->start_monitor(array('instance_id' => $instance_id, 'monitor_id' => $monitor_id, 'cloud_context' => $cloud_context));
    cloud_monitoring_update_instance_status($monitor_id, $instance_id, TRUE);
    drupal_set_message(t('Monitoring has been enabled'));
    drupal_goto('design/cloud_monitoring/' . $monitor_id);
  }
}

/**
 * Confirmation form for stopping monitoring on an instance
 */
function cloud_monitoring_confirm_monitoring_stop(&$form_state, $monitor_id, $cloud_context, $instance_id) {
  $form['monitor_id'] = array(
    '#type' => 'value',
    '#value' => $monitor_id,
  );
  $form['instance_id'] = array(
    '#type' => 'value',
    '#value' => $instance_id,
  );
  $form['cloud_context'] = array(
    '#type' => 'value',
    '#value' => $cloud_context,
  );
  return confirm_form($form, t('Are you sure you want to stop monitoring?'),
    'design/cloud_monitoring/' . $monitor_id,
    t('After stopping the monitoring software, you can enable it again at a later time.'),
    t('Stop'), t('Cancel'));
}

/**
 * Stop monitoring
 */
function cloud_monitoring_confirm_monitoring_stop_submit($form, &$form_state) {
  //delete it using the class implementation
  $monitor_id = $form_state['values']['monitor_id'];
  $class = cloud_monitoring_get_class($monitor_id);
  $instance_id = $form_state['values']['instance_id'];
  if ($class) {
    $class->stop_monitor(array('instance_id' => $instance_id, 'monitor_id' => $monitor_id, 'cloud_context' => $form_state['values']['cloud_context']));
    //update the status
    cloud_monitoring_update_instance_status($monitor_id, $instance_id, FALSE);
    drupal_set_message(t('Monitoring has been disabled for instance %instance_id', array('%instance_id' => $instance_id)));
  }
  $form_state['redirect'] = 'design/cloud_monitoring/' . $monitor_id;
}

/**
 * Delete monitoring from an instance
 */
function cloud_monitoring_instance_confirm_delete($form_id, $form_state, $monitor_id, $cloud_context, $instance_id) {
  $form['monitor_id'] = array(
    '#type' => 'value',
    '#value' => $monitor_id,
  );
  $form['instance_id'] = array(
    '#type' => 'value',
    '#value' => $instance_id,
  );
  $form['cloud_context'] = array(
    '#type' => 'value',
    '#value' => $cloud_context,
  );
  return confirm_form($form, t('Are you sure you want to delete monitoring on this instance?'),
      'design/cloud_monitoring/' . $monitor_id,
  t('After deleting, the monitoring server will not be collecting any data on this instance'),
  t('Delete'), t('Cancel'));
}

/**
 * Uninstall monitoring and unmap it in the cloud_monitor_instance table
 */
function cloud_monitoring_instance_confirm_delete_submit($form, &$form_state) {
  $monitor_id = $form_state['values']['monitor_id'];
  $class = cloud_monitoring_get_class($monitor_id);
  $instance_id = $form_state['values']['instance_id'];
  if ($class) {
    $class->delete_monitor(array('instance_id' => $instance_id, 'monitor_id' => $monitor_id, 'cloud_context' => $form_state['values']['cloud_context']));
    cloud_monitoring_delete_instance($monitor_id, $instance_id);
    drupal_set_message(t('Monitoring software delete from instance'));
  }
  $form_state['redirect'] = 'design/cloud_monitoring/' . $monitor_id;
}

/**
 * Function that gets all the available monitors
 * and builds an array.
 */
function _cloud_monitoring_available_monitors() {
  $monitors = cloud_monitoring_get_monitor_definitions();
  $data = array('' => '- none -');
  foreach ($monitors as $key => $value) {
    $data[$key] = $value['display_name'];
  }
  return $data;
}


/**
 * Helper function to get the parameters
 * needed for graphs and data
 */
function _cloud_monitoring_get_params() {
  $host = isset($_GET['host']) ? $_GET['host'] : NULL;
  $plugin = isset($_GET['plugin']) ? $_GET['plugin'] : NULL;
  $plugin_instance = isset($_GET['plugin_instance']) ? $_GET['plugin_instance'] : NULL;
  $type = isset($_GET['type']) ? $_GET['type'] : NULL;
  $type_instance = isset($_GET['type_instance']) ? $_GET['type_instance'] : NULL;
  $image_preset = isset($_GET['image_preset']) ? $_GET['image_preset'] : 'medium';
  $seconds = isset($_GET['seconds']) && is_numeric($_GET['seconds']) ? $_GET['seconds'] : '86400';

  $params = array(
    'host' => $host,
    'plugin' => $plugin,
    'plugin_instance' => $plugin_instance,
    'type' => $type,
    'type_instance' => $type_instance,
    'seconds' => $seconds,
  );
  return $params;
}

/**
 * Helper function that gets a list of ssh keys given
 * a cloud type.  The AHAH callback for launching a
 * monitor uses this function to load the apporopriate
 * set of SSH keys
 */
function _cloud_monitoring_get_second_dropdown($cloud_type) {
  $keys = _aws_cloud_get_ssh_keys($cloud_type);
  asort($keys);
  return $keys;
}
