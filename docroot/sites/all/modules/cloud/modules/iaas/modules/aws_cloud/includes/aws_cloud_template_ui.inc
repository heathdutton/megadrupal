<?php
/**
 * @file
 * Contains functions for Server template form generation
 */

/**
 *
 *  Create a new template which will be used for launching instance
 *
 * @param $form_submit
 *          The form
 * @param $cloud_context
 *          Sub-cloud for which the template is to be created
 * @param $params
 *          Parameters to be passed
 * @return
 */
function aws_cloud_server_templates_new($form, $form_submit = '', $cloud_context, $params = array()) {
  //include js file for Scripts - switching n order from left to right
  drupal_add_js(cloud_get_module_base() . 'js/json2.js'          );
  drupal_add_js(cloud_get_module_base() . 'js/selectbox.js'      );
  drupal_add_js(cloud_get_module_base() . 'js/cloud_scripting.js');

  $scripting_options    = cloud_server_templates_get_scripting_options();
  $server_template_id   = !empty($params['templateid']) ? $params['templateid'] : '';
  $selected_scripts_ids = array();
  $selected_scripts     = array();
  
  $cloud_type = !empty($cloud_context) ? $cloud_context : '';
  $action = t('Add');
  $form['header'] = array(
    '#type' => 'value',
    '#value' => array(
      'data' => t('Name'),
      'field' => 'template_nickname',
      'sort' => 'asc',
      'width' => '120',
    ),
  );
  
  $base = cloud_get_module_base();

  $form['fieldset_template_info'] = array('#type' => 'fieldset');

  $form['name']['name_label'] = array(
    '#type' => 'item',
    '#title' => t('Name'),
    '#required' => TRUE,
  );
  $form['name']['name_text'] = array(
    '#type' => 'textfield',
    '#size' => 100,
  );
  $form['description']['description_label'] = array(
    '#type' => 'item',
    '#title' => t('Description'),
  );
  $form['description']['description_text'] = array(
    '#type' => 'textarea',
  );

  $cloud_type_options = cloud_get_clouds();
  $cloud_name_list = cloud_get_all_clouds_display_name();
  $form['cloud_type']['cloud_type_label'  ] = array(
    '#type' => 'item',
    '#title' => t('Cloud Type'),
  );
  $form['cloud_type']['cloud_type_display'] = array(
    '#markup' => $cloud_name_list[$cloud_context],
  );
  
  $form['cloud_type']['cloud_type_select'] = array(
    '#type' => 'hidden',
    '#value' => isset_variable($cloud_context),
  );

  $instance_type_options = _cloud_server_templates_get_instance_type_list($cloud_context);

  $form['instance_type']['instance_type_label'] = array(
    '#type' => 'item',
    '#title' => t('Instance Type'),
  );
  $form['instance_type']['instance_type_select'] = array(
    '#type' => 'select',
    '#options' => $instance_type_options,
    '#default_value' => 'm1.small',
  );

  $form['ec2_image_lbl']['ec2_image_id_label'] = array(
    '#type' => 'item'     ,
    '#title' => t('EC2 Image'),
    '#required' => TRUE       ,
  );
  $form['ec2_image_id']['ec2_image_id_select'] = array(
    '#autocomplete_path' => 'design/server_templates/' . $cloud_context . '/ec2_lib_callback_list_machine_images',
    '#type' => 'textfield',
    '#size' => 100,
  );

  $form['kernel_image_lbl']['kernel_image_id_label'] = array(
    '#type' => 'item',
    '#title' => t('Kernel Image'),
    '#description' => t('Blank is okay.'),
  );
  $form['kernel_image_id']['kernel_image_id_select'] = array(
    '#autocomplete_path' => 'design/server_templates/' . $cloud_context . '/ec2_lib_callback_list_kernel_images',
    '#type' => 'textfield',
    '#size' => 100,
  );

  $form['ramdisk_image_lbl']['ramdisk_image_id_label'] = array(
    '#type' => 'item',
    '#title' => t('Ramdisk Image'),
    '#description' => t('Blank is okay.'),
  );

  $form['ramdisk_image_id']['ramdisk_image_id_select'] = array(
    '#autocomplete_path' => 'design/server_templates/' . $cloud_context . '/ec2_lib_callback_list_ramdisk_images',
    '#type' => 'textfield',
    '#size' => 100,
  );

  $form['user_name_details']['user_name_label'] = array(
    '#type' => 'item',
    '#title' => t('User Name'),
    '#required' => TRUE     ,
  );

  $form['user_name_details']['user_name_text'] = array(
    '#type' => 'textfield',
    '#size' => 64,
    '#default_value' => CLOUD_SSH_USER_NAME,
  );

  $ssh_key_options = _aws_cloud_get_ssh_keys($cloud_context);
  asort($ssh_key_options);
  $form['ssh_keys_details']['ssh_keys_label'] = array(
      '#type' => 'item'   ,
      '#title' => t('SSH Key'),
      '#required' => TRUE     ,
  );
  $form['ssh_keys_details']['ssh_keys_select'] = array(
      '#type' => 'select',
      '#options' => $ssh_key_options,
  );

  $security_group_options = _aws_cloud_get_security_groups($cloud_context);
  asort($security_group_options);
  $form['SG_details']['SG_label'] = array(
    '#type' => 'item',
    '#title' => t('Security Group(s)'),
  );
  $form['SG_details']['SG_select'] = array(
    '#type' => 'select',
    '#options' => $security_group_options,
    '#multiple' => TRUE,
  );

  $form['count_details']['Count_label'] = array(
    '#type' => 'item',
    '#title' => t('Count'),
    '#required' => TRUE,
  );
  $form['count_details']['Count_text'] = array(
    '#type' => 'textfield',
    '#default_value' => 1,
  );

  $zone_options = aws_cloud_get_zones($cloud_context);
  $form['zone_details']['zone_label'] = array(
    '#type' => 'item',
    '#title' => t('Availability Zone'),
  );
  $form['zone_details']['zone_select'] = array(
    '#type' => 'select',
    '#options' => $zone_options,
  );
  $form['user_data']['user_data_label'] = array(
    '#type' => 'item',
    '#title' => t('User Data'),
  );
  $form['user_data']['user_data_text'] = array(
    '#type' => 'textarea',
  );

  if ($server_template_id && is_numeric($server_template_id)) { // edit case
    $query        = _aws_cloud_get_server_template_details_by_id($server_template_id, $cloud_context);
    $result       = db_query($query, array(':cloud_type' => $cloud_context))->fetchObject();
    $template_obj = $result;
    $count  = 0;
    $action = t('Edit');
    $cloud_type = $template_obj->cloud_type;
    $cloud_context = $cloud_type;

    $form['templateid'] = array(
      '#type' => 'hidden',
      '#value' => $server_template_id,
    );
    $form['name'         ]['name_text'           ]['#default_value'] = isset_variable($template_obj->template_nickname);
    $form['description'  ]['description_text'    ]['#default_value'] = isset_variable($template_obj->description);
    $form['cloud_type'   ]['cloud_type_display'  ]['#default_value'] = isset_variable($template_obj->cloud_type);
    $form['cloud_type'   ]['cloud_type_display'  ]['#attributes'] = array('disabled' => TRUE);
    
    $form['type_val'     ] = array(
        '#type' => 'hidden',
        '#value' => isset_variable($template_obj->cloud_type),
    );
    $form['instance_type']['instance_type_select']['#default_value'] = isset_variable($template_obj->instance_type);
    $form['ec2_image_id']['ec2_image_id_select']['#default_value'] = isset_variable($template_obj->image_nickname) . ' (' . isset_variable($template_obj->image_id) . ')';

    if (!empty($template_obj->kernel_id) && $template_obj->kernel_id != AWS_CLOUD_CLOUD_NONE) { // Default value
      $form['kernel_image_id']['kernel_image_id_select']['#default_value'] = $kernel_image_options[$template_obj->kernel_id]  ;
    }
    
    if (!empty($template_obj->ramdisk_id) && $template_obj->ramdisk_id != AWS_CLOUD_CLOUD_NONE ) { // Default value
      $form['ramdisk_image_id']['ramdisk_image_id_select']['#default_value'] = $ramdisk_image_options[$template_obj->ramdisk_id];
    }

    $form['user_name_details']['user_name_text'  ]['#default_value'] = isset_variable($template_obj->user_name       ) ? $template_obj->user_name : CLOUD_SSH_USER_NAME;
    $form['ssh_keys_details']['ssh_keys_select']['#default_value'] = isset_variable($template_obj->key_name        );
    $form['count_details'    ]['Count_text'      ]['#default_value'] = isset_variable($template_obj->count_instances );
    $form['SG_details'       ]['SG_select'       ]['#default_value'] = isset_variable($template_obj->group_id        );
    $form['user_data'        ]['user_data_text'  ]['#default_value'] = isset_variable($template_obj->user_data       );

    $form['zone_details'     ]['zone_select'     ]['#default_value'] = isset_variable($template_obj->placement       );

    if (module_exists('cloud_scripting')) {


      $query      = _cloud_server_templates_get_server_templates_scripts($server_template_id);
      $result1 = db_query( $query);

      $selected_scripts = array();
      foreach ($result1 as $template_obj) {

        $selected_scripts[$template_obj->type][$template_obj->script_id] = $template_obj->script_name;
        $selected_scripts_ids[] = $template_obj->script_id;
      }

      if (module_exists('cloud_scripting')) {
        foreach ($scripting_options['CLOUD_SCRIPTING_TYPE_OPTIONS'] as $script => $script_val) {

          if (isset($form[$script])
              &&    isset($form[$script][$script . '_content'])
              &&    isset($form[$script][$script . '_content'][$script . '_selected'])
              &&    isset($form[$script][$script . '_content'][$script . '_selected']['#options'])) {
            $form[$script][$script . '_content'][$script . '_selected']['#options'] = $selected_scripts[$script];
          }
        }
      }
    }
  }
  else { // Coming from Images Page
    $image_type = isset( $_REQUEST['image_type'] ) ? $_REQUEST['image_type'] : '';

    if ($image_type === 'machine' ) {

      $def_img_val = isset($_REQUEST['ec2_image_id_select']) ? $_REQUEST['ec2_image_id_select'] : '';
      $form['ec2_image_id']['ec2_image_id_select']['#default_value'] = $def_img_val;
    }
    elseif ($image_type === 'kernel' ) {

      $def_img_val = isset($_REQUEST['ec2_image_id_select']) ? $_REQUEST['ec2_image_id_select'] : '';
      $form['kernel_image_id']['kernel_image_id_select']['#default_value'] = $def_img_val;
    }
    elseif ($image_type === 'ramdisk' ) {

      $def_img_val = isset($_REQUEST['ec2_image_id_select']) ? $_REQUEST['ec2_image_id_select'] : '';
      $form['ramdisk_image_id']['ramdisk_image_id_select']['#default_value'] = $def_img_val;
    }

  }

  //on change cloud type case

  if (user_access('set scripts and alerts')) { // permission start{
    // scripting elements
    if (module_exists('cloud_scripting')) {

      foreach ($scripting_options['CLOUD_SCRIPTING_TYPE_OPTIONS'] as $script => $script_val) {

        if ( !empty($_REQUEST[$script . '_selected']) ) {
          $selected_scripts_ids                   = $_REQUEST[$script . '_selected'];
        }
        $script_options = array();
        $script_options = cloud_get_all_scripts($script);
        $script_keys = array_keys($script_options);

        foreach ($selected_scripts_ids as $id) {
          if (in_array($id, $script_keys)) {
            $selected_scripts[$script][$id] = $script_options[$id];
            unset($script_options[$id]);
          }
        }

        $form[$script][$script . '_label'] = array(
            '#type' => 'item',
            '#title' => t( '@script_val  Script', array('@script_val' => $script_val) ),
        );

        $script_elem = array();
        $script_elem = array(
            '#prefix' => '<div class="container-inline">',
            '#type' => 'fieldset',
            '#suffix' => '</div>',
            '#collapsible' => TRUE,
            '#collapsed' => FALSE,
        );
        $script_elem[$script . '_select'  ] = array(
            '#id' => $script . '_select',
            '#type' => 'select',
            '#options' => $script_options,
            '#multiple' => TRUE,
            '#attributes' => array('style' => 'width:150px;margin:5px 0 5px 0;'),
        );
        $script_elem[$script . '_remove'  ] = array(
            '#type' => 'button',
            '#name' => 'removeState',
            '#value' => '<<',
            '#attributes' => array('onclick' => 'javascript:return moveElement(' . $script . '_selected, ' . $script . '_select  )'),
        );
        $script_elem[$script . '_add'     ] = array(
            '#type' => 'button',
            '#name' => 'addState',
            '#value' => '>>',
            '#attributes' => array(
                'style' => 'margin-left:0px',
                'onclick' => 'javascript:return moveElement(' . $script . '_select, ' . $script . '_selected)',
            ),
        );
        $script_elem[$script . '_selected'] = array(
            '#id' => $script . '_selected',
            '#type' => 'select',
            '#options' => array(),
            '#multiple' => TRUE,
            '#attributes' => array('style' => 'width:150px;margin:5px 0 5px 0;'),
        );
        $script_elem[$script . '_up'      ] = array(
            '#type' => 'button',
            '#name' => 'upState'    ,
            '#value' => '^',
            '#attributes' => array(
                'style' => 'margin-left:0px',
                'onclick' => 'javascript:return moveElementUp(' . $script . '_selected)',
            ),
        );
        $script_elem[$script . '_down'    ] = array(
            '#type' => 'button',
            '#name' => 'downState'  ,
            '#value' => 'v',
            '#attributes' => array('onclick' => 'javascript:return moveElementDown(' . $script . '_selected)'),
        );
        $form[$script][$script . '_content'] = $script_elem;
        $form[$script][$script . '_content'][$script . '_selected']['#options'] = isset($selected_scripts[$script])
        ?          $selected_scripts[$script]
        : array();
      }
    }
    /// scripting end

    //failover element
    $selected_failover_scenarios_ids = array();

    if (module_exists('cloud_failover') &&    $server_template_id) {

      $query = _cloud_server_templates_get_failover_scanarios_by_server_templates();
      $query_args = array(
          ':template_id' => '%' . $server_template_id . '%',
      );

      $selected_failover_scenarios = array();

      $result = db_query( $query, $query_args );
      $selected_failover_scenarios_ids = array();

      foreach ($result as $template_obj) {
        $selected_failover_scenarios[$template_obj->fsid] = $template_obj->nickname;
        $selected_failover_scenarios_ids[]                = $template_obj->fsid;
      }
    }

    if (module_exists('cloud_failover')) {
      if ( empty($server_template_id) && ! empty($_REQUEST['failover_scenario_id_selected']) ) {

        $selected_failover_scenarios    = array();
        $selected_failover_scenarios_ids = $_REQUEST['failover_scenario_id_selected'];
      }

      $failover_scenario_options = _cloud_failover_scenario_get_all();
      $failover_scenario_keys    = array_keys($failover_scenario_options);

      if (isset($selected_failover_scenarios_ids) && count($selected_failover_scenarios_ids) > 0) {
        foreach ($selected_failover_scenarios_ids as $id) {
          if (in_array($id, $failover_scenario_keys)) {
            unset($failover_scenario_options[$id]);
          }
        }
      }

      $form['failover_scenario_id']['failover_scenario_id_label'] = array(
          '#type' => 'item',
          '#title' => t('Failover Scenarios'),
      );
      $failover_scenario_elem = array(
          '#prefix' => '<div class="container-inline">',
          '#type' => 'fieldset',
          '#suffix' => '</div>',
          '#collapsible' => TRUE,
          '#collapsed' => FALSE,
      );
      $failover_scenario_elem['failover_scenario_id_select'  ] = array(
          '#id' => 'failover_scenario_id_select',
          '#type' => 'select',
          '#options' => $failover_scenario_options,
          '#multiple' => TRUE,
          '#attributes' => array('style' => 'width:150px;margin:5px 0 5px 0;'),
      );
      $failover_scenario_elem['failover_scenario_id_add'     ] = array(
          '#type' => 'button',
          '#name' => 'addFailoverScanerio',
          '#value' => '>>',
          '#attributes' => array(
              'style' => 'margin-left:0px',
              'onclick' => 'javascript:return moveElement(failover_scenario_id_select, failover_scenario_id_selected)',
          ),
      );
      $failover_scenario_elem['failover_scenario_id_remove'  ] = array(
          '#type' => 'button',
          '#name' => 'removeFailoverScenario',
          '#value' => '<<',
          '#attributes' => array('onclick' => 'javascript:return moveElement(failover_scenario_id_selected, failover_scenario_id_select)'),
      );
      $failover_scenario_elem['failover_scenario_id_selected'] =  array(
          '#id' => 'failover_scenario_id_selected',
          '#type' => 'select',
          '#options' => array(),
          '#multiple' => TRUE,
          '#attributes' => array('style' => 'width:150px;margin:5px 0 5px 0;'),
      );
      $form['failover_scenario_id']['failover_scenario_id_content'] = $failover_scenario_elem;
      $form['failover_scenario_id']['failover_scenario_id_content']['failover_scenario_id_selected']['#options'] = isset($selected_failover_scenarios) ? $selected_failover_scenarios : array();
    }
    //failover end
    $selected_alerts_ids = array();
    //alert element
    if (module_exists('cloud_alerts') &&    $server_template_id) {

      $query = _cloud_server_templates_get_alerts_by_server_templates();

      $query_args = array(
          ':template_id' => '%' . $server_template_id . '%',
      );

      $selected_alerts = array();

      $result1 = db_query( $query, $query_args );

      $selected_alerts_ids = array();

      foreach ($result1 as $template_obj) {
        $selected_alerts[$template_obj->alert_id]  = $template_obj->alert_name;
        $selected_alerts_ids[]                     = $template_obj->alert_id;
      }
    }

    if (module_exists('cloud_alerts')) {

      if ( empty($server_template_id) && ! empty($_REQUEST['alert_id_selected']) ) {
        $selected_alerts = array();
        $selected_alerts_ids                   = $_REQUEST['alert_id_selected'];
      }
      $alert_options = _cloud_alerts_get_all();
      $alert_keys = array_keys($alert_options);
      if (isset($selected_alerts_ids) && count($selected_alerts_ids) > 0) {
        foreach ($selected_alerts_ids as $id) {
          if (in_array($id, $alert_keys)) {
            unset($alert_options[$id]);
          }
        }
      }
      $form['alert_id']['alert_id_label'] = array(
          '#type' => 'item',
          '#title' => t('Alerts'),
      );
      $alert_elem = array(
          '#prefix' => '<div class="container-inline">',
          '#type' => 'fieldset',
          '#suffix' => '</div>',
          '#collapsible' => TRUE,
          '#collapsed' => FALSE,
      );
      $alert_elem['alert_id_select'  ] = array(
          '#id' => 'alert_id_select',
          '#type' => 'select',
          '#options' => $alert_options,
          '#multiple' => TRUE,
          '#attributes' => array('style' => 'width:150px;margin:5px 0 5px 0;'),
      );
      $alert_elem['alert_id_add'     ] = array(
          '#type' => 'button',
          '#name' => 'addAlert',
          '#value' => '>>',
          '#attributes' => array(
              'style' => 'margin-left:0px',
              'onclick' => 'javascript:return moveElement(alert_id_select, alert_id_selected)',
          ),
      );
      $alert_elem['alert_id_remove'  ] = array(
          '#type' => 'button',
          '#name' => 'removeAlert',
          '#value' => '<<',
          '#attributes' => array('onclick' => 'javascript:return moveElement(alert_id_selected, alert_id_select)'),
      );
      $alert_elem['alert_id_selected'] =  array(
          '#id' => 'alert_id_selected',
          '#type' => 'select',
          '#options' => array(),
          //'#value'      => '',
          '#multiple' => TRUE,
          '#attributes' => array('style' => 'width:150px;margin:5px 0 5px 0;'),
      );
      $form['alert_id']['alert_id_content'] = $alert_elem;
      $form['alert_id']['alert_id_content']['alert_id_selected']['#options'] = isset($selected_alerts) ? $selected_alerts : array();
    }
    /// alert end
  }
  //permission end
  $form['submit_buttons'] = array(
      '#type' => 'fieldset',
      '#prefix' => '<span class="clear"></span><div class="container-inline"><div class="buttons">',
      '#suffix' => '</div></div>',
  );

  $form['submit_buttons'][$action] = array(
      '#type' => 'submit',
      '#value' => t($action),
      '#attributes' => array('onclick' => "javascript:selectAllTemplateOptions(); "),
  );
  $form['submit_buttons']['Cancel'] = array(
      '#type' => 'submit',
      '#value' => t('Cancel'),
  );

  return $form;
}

/**
 * Display the template
 *
 * @param $form_submit
 *              Form to be displayed
 * @param $cloud_context
 *              Template is of this Sub-cloud
 * @param $params
 *              Params to be passed
 * @return
 */
function aws_cloud_server_templates_view($form, $form_submit = '', $cloud_context = '', $params = array()) {
  $server_template_id = !empty($params['templateid']) ? $params['templateid'] : '';

  // if templateid not passed, error out
  if (!isset($server_template_id)) {
    drupal_set_message(t('No template id passed into the aws_cloud_server_templates_views function()'), 'error');
    return;
  }

  $server_template_id   = !empty($params['templateid']) ? $params['templateid'] : '';
  $scripting_options    = cloud_server_templates_get_scripting_options();
  $selected_scripts_ids = array();
  $selected_scripts     = array();

  // get the template details
  $query         = _aws_cloud_get_server_template_details_by_id($server_template_id, $cloud_context);
  $template_obj  = db_query( $query, array(':cloud_type' => $cloud_context) )->fetchObject();
  $cloud_type    = $template_obj->cloud_type;
  $template_name   = $template_obj->template_nickname;
  $t_instances     = $template_obj->count_instances;
  $cloud_name_list = cloud_get_all_clouds_display_name();

  // get extra image,kernel and image information
  $image_options = aws_cloud_get_all_images_db($cloud_context, 'machine');
  $kernel_image_options = aws_cloud_get_all_images_db($cloud_context, 'kernel');
  $ramdisk_image_options = aws_cloud_get_all_images_db($cloud_context, 'ramdisk');

  $form['fieldset_template_info'] = array(
      '#type' => 'fieldset',
      '#title' => t('Details'),
  );

  $form['templateid'] = array(
      '#type' => 'value',
      '#value' => $server_template_id,
  );


  $form['name']['name_label'] = array('#type' => 'item', '#title' => t('Name'));
  $form['name']['name_text'] = array('#markup' => check_plain(isset_variable($template_obj->template_nickname)));

  $form['description']['description_label'] = array('#type' => 'item', '#title' => t('Description'));
  $form['description']['description_text'] = array('#markup' => check_plain( isset_variable($template_obj->description) ));

  $form['cloud_type']['cloud_type_label'] = array('#type' => 'item', '#title' => t('Cloud Type'));
  $form['cloud_type']['cloud_type_select'] = array('#markup' => t(isset_variable($cloud_name_list[$template_obj->cloud_type])));
  $form['cloud_type']['cloud_type_value'] = array('#type' => 'value', '#value' => $template_obj->cloud_type);

  $form['instance_type']['instance_type_label'] = array('#type' => 'item', '#title' => t('Instance Type'));
  $form['instance_type']['instance_cloud_type_select'] = array('#markup' => t( isset_variable($template_obj->instance_type)));

  $form['ec2_image_lbl']['ec2_image_id_label'] = array('#type' => 'item', '#title' => t('EC2 Image'));
  $form['ec2_image_id']['ec2_image_id_select'] = array('#markup' => t(isset_variable($image_options[$template_obj->image_id])));

  $form['kernel_image_id']['kernel_image_id_label'] = array('#type' => 'item', '#title' => t('Kernel Image'));
  $form['kernel_image_id']['kernel_image_id_select'] = array('#markup' => t(isset_variable($kernel_image_options[$template_obj->kernel_id])));

  $form['ramdisk_image_id']['ramdisk_image_id_label'] = array('#type' => 'item', '#title' => t('Ramdisk Image'));
  $form['ramdisk_image_id']['ramdisk_image_id_select'] = array('#markup' => t(isset_variable($ramdisk_image_options[$template_obj->ramdisk_id])));


  $form['user_name_details']['user_name_label'] = array('#type' => 'item', '#title' => t('User Name'));
  $form['user_name_details']['user_name_text'] = array('#markup' => t(isset_variable(($template_obj->user_name)
      ? $template_obj->user_name
      : CLOUD_SSH_USER_NAME) ));

  $form['ssh_keys_details']['ssh_keys_label'] = array('#type' => 'item', '#title' => t('SSH Key'));
  $form['ssh_keys_details']['ssh_keys_select'] = array('#markup' => t(isset_variable(($template_obj->key_name != '-1')
      ? $template_obj->key_name
      : AWS_CLOUD_CLOUD_NONE)));

  $form['count_details']['Count_label'] = array('#type' => 'item', '#title' => t('Count'));
  $form['count_details']['Count_text'] = array('#markup' => t(isset_variable($template_obj->count_instances)));
  $form['count_details']['count_text_value'] = array('#type' => 'value', '#value' => $template_obj->count_instances);

  $form['SG_details']['SG_label'] = array('#type' => 'item', '#title' => t('Security Group(s)'));
  $form['SG_details']['SG_select'] = array('#markup' => t( isset_variable($template_obj->group_id)));

  $form['user_data']['user_data_label'] = array('#type' => 'item', '#title' => t('User Data'));
  $form['user_data']['user_data_text'] = array('#markup' => check_plain(isset_variable($template_obj->user_data) ));

  $form['zone_details']['zone_label'] = array('#type' => 'item', '#title' => t('Availability Zone'));
  $form['zone_details']['zone_select'] = array('#markup' => t(isset_variable($template_obj->placement) ));

  if (module_exists('cloud_scripting')) {
    $query      = _cloud_server_templates_get_server_templates_scripts($server_template_id);
    $result1 = db_query( $query);

    $selected_scripts = array();

    foreach ($result1 as $template_obj) {
      $selected_scripts[$template_obj->type][$template_obj->script_id] = $template_obj->script_name;
      $selected_scripts_ids[] = $template_obj->script_id;
    }

    foreach ($scripting_options['CLOUD_SCRIPTING_TYPE_OPTIONS'] as $script => $script_val) {

      $form[$script][$script . '_label'] = array(
          '#type' => 'item',
          '#title' => t( '@script_val Script', array('@script_val' => $script_val) ),
      );
      $script_elem = array(
          '#type' => 'fieldset',
          '#prefix' => '<div class="container-inline">',
          '#suffix' => '</div>',
          '#collapsible' => TRUE,
          '#collapsed' => FALSE,
      );
      $script_elem[$script . '_selected'] =  array('#markup' => t(isset_variable(@implode('<br />', $selected_scripts[$script]))));
      $form[$script][$script . '_content'] = $script_elem;
    }
  }

  if (module_exists('cloud_failover')) {
    $query = _cloud_server_templates_get_failover_scanarios_by_server_templates();
    $query_args = array(
        ':template_id' => '%' . $server_template_id . '%',
    );
    $selected_failover_scenarios     = array();
    $results = db_query( $query, $query_args );

    foreach ($results as $template_obj) {
      $selected_failover_scenarios[$template_obj->fsid] = $template_obj->nickname;
    }

    $form['failover_scenario_id']['failover_scenario_id_label'] = array(
        '#type' => 'item',
        '#title' => t('Failover Scenarios'),
    );
    $failover_scenario_elem = array(
        '#prefix' => '<div class="container-inline">',
        '#type' => 'fieldset',
        '#suffix' => '</div>',
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
    );
    $failover_scenario_elem['failover_scenario_id_selected'] =  array('#markup' => t( isset_variable(implode(',', $selected_failover_scenarios)) ));
    $form['failover_scenario_id']['failover_scenario_id_content'] = $failover_scenario_elem;
  }


  if (module_exists('cloud_alerts')) {
    $query = _cloud_server_templates_get_alerts_by_server_templates();
    $query_args = array(
        ':template_id' => '%' . $server_template_id . '%',
    );

    $selected_alerts     = array();
    $selected_alerts_ids = array();
    $result1 = db_query( $query, $query_args );

    foreach ($result1 as $template_obj) {
      $selected_alerts[$template_obj->alert_id] = $template_obj->alert_name;
      $selected_alerts_ids[]                    = $template_obj->alert_id;
    }

    $form['alert_id']['alert_id_label'] = array(
        '#type' => 'item',
        '#title' => t('Alerts'),
    );
    $alert_elem = array(
        '#prefix' => '<div class="container-inline">',
        '#type' => 'fieldset',
        '#suffix' => '</div>',
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
    );
    $alert_elem['alert_id_selected'] =  array('#markup' => t( isset_variable(implode(',', $selected_alerts)) ));
    $form['alert_id']['alert_id_content'] = $alert_elem;
  }


  $form['submit_buttons'] = array(
      '#type' => 'fieldset',
      '#prefix' => '<span class="clear"></span><div class="container-inline"><div class="buttons">',
      '#suffix' => '</div></div>',
  );

  if (user_access('launch server template')) {
    $form['submit_buttons']['Launch'] = array(
        '#type' => 'submit',
        '#value' => t('Launch'),
    );
  }

  if (user_access('edit server template')) {
    $form['submit_buttons']['Copy'] = array(
        '#type' => 'submit',
        '#value' => t('Copy'),
    );

    $form['submit_buttons']['Edit'] = array(
        '#type' => 'submit',
        '#value' => t('Edit'),
    );

    if (module_exists('cloud_inputs') && user_access('edit inputs')) {
      $form['submit_buttons']['Inputs'] = array(
          '#type' => 'submit',
          '#value' => t('Inputs'),
      );
    }
  }

  if (user_access('delete server template')) {
    $confirm_msg = cloud_get_messagebox(t('Are you sure you want to delete the template "@template_name" ?', array('@template_name' => $template_name)));
    $form['submit_buttons']['Delete'] = array(
        '#type' => 'submit',
        '#value' => t('Delete'),
        '#attributes' => array('onclick' => $confirm_msg),
    );
  }

  $form['submit_buttons']['List Templates'] = array(
      '#type' => 'submit',
      '#value' => t('List Templates'),
  );

  $form['cloud_context'] = array(
      '#type' => 'hidden',
      '#value' => $cloud_context,
  );
  return $form;
}

/**
 * Handle view button submissions
 */
function aws_cloud_server_templates_view_submit($form, &$form_state) {
  $cloud_context = $form_state['values']['cloud_context'];
  $template_id = $form_state['values']['templateid'];
  $count = $form_state['values']['count_text_value'];

  switch ($form_state['values']['op']) {
    case 'Launch':
      $form_state['redirect'] = array(CLOUD_SERVER_TEMPLATES_PATH . '/' . $cloud_context . '/' . $template_id . '/launch', array('query' => array('id' => $template_id, 'count' => $count, 'type' => $cloud_context)));
      break;
    case 'Copy':
      $form_state['redirect'] = array(CLOUD_SERVER_TEMPLATES_PATH . '/' . $cloud_context . '/' . $template_id . '/copy', array('query' => array('id' => $template_id, 'count' => $count, 'type' => $cloud_context)));
      break;
    case 'Edit':
      $form_state['redirect'] = array(CLOUD_SERVER_TEMPLATES_PATH . '/' . $cloud_context . '/' . $template_id . '/edit');
      break;
    case 'Inputs':
      $form_state['redirect'] = array('design/cloud_server_templates' . '/' . $template_id . '/inputs', array('query' => array('template_id' => $template_id, 'module' => 'cloud_server_templates', 'type' => $cloud_context)));
      break;
    case 'Delete':
      $form_state['redirect'] = array(CLOUD_SERVER_TEMPLATES_PATH . '/' . $cloud_context . '/' . $template_id . '/delete');
      break;
    default:
      // default catch all redirect
      $form_state['redirect'] = array(CLOUD_SERVER_TEMPLATES_PATH . '/' . $cloud_context);
      break;
  }
}

/**
 * Validate the template form.
 */
function aws_cloud_server_templates_new_validate($form_id, $form_values) {

  $form_values = $form_values['values'];

  // Cancel Button pressed
  if ($form_values['op'] == t('Cancel'))
    return $form_values['redirect'] = array(CLOUD_SERVER_TEMPLATES_PATH . '/' . $cloud_context);

  if (!preg_match(CLOUD_SERVER_TEMPLATES_VALID_NOT_EMPTY, $form_values['name_text']) ) {
    form_set_error('name_text', t('You must enter a valid Template Name'));
  }
  else {
    $query = _cloud_server_templates_get_server_template();
    $query->condition('template_nickname', $form_values['name_text'], '=');
    // if templateid is set, then it's an edit
    if (isset($form_values['templateid'])) {
      $query->condition('template_id', $form_values['templateid'], '!=');
    }
    $db_obj = $query->execute()->fetchObject();

    if ($db_obj) {
      form_set_error('name_text', t('Name is already used by a ServerTemplate.'));
    }
  }
  if (empty($form_values['ssh_keys_select']) || $form_values['ssh_keys_select'] == -1 ) {
    form_set_error('ssh_keys_select', t('You must enter valid SSH Key'));
  }

  // Check EC2 Image
  $img_found_flag = FALSE;
  $image_id = _cloud_server_templates_get_first_element_image($form_values['ec2_image_id_select']);
  $query_img_srch = _aws_cloud_get_image_query($form_values['cloud_type_select']);
  $result_img_srch = db_query($query_img_srch, array(':image_id' => $image_id));

  foreach ($result_img_srch as $image) {
    $img_found_flag = TRUE;
  }

  if ($img_found_flag == FALSE) {
    form_set_error('ec2_image_id_select', t('You must select EC2 Image'));
  }
  elseif (!preg_match(CLOUD_VALID_DIGIT, $form_values['Count_text']) || $form_values['Count_text'] == 0) {
    $error = TRUE;
    form_set_error('Count_text', t('Please enter valid count value. The number must be more than one (1).'));
  }

  // Validate Kernel image
  $ker_img_srch = _aws_cloud_validate_image($form_values['cloud_type_select'], $form_values['kernel_image_id_select'] );
  if ($ker_img_srch == FALSE) {
    form_set_error('kernel_image_id_select', t('You must select correct kernel image'));
  }
  else {
    $ker_img_srch = _aws_cloud_validate_image($form_values['cloud_type_select'], $form_values['ramdisk_image_id_select'] );
    if ($ker_img_srch == FALSE) {
      form_set_error('ramdisk_image_id_select', t('You must select correct ramdisk image'));
    }
  }

  if (empty($form_values['user_name_text']) ) {
    form_set_error('user_name_text', t('You must enter valid User Name'));
  }
}

/**
 * Submit handler for new template.  Calls _cloud_server_templates_new_submit()
 */
function aws_cloud_server_templates_new_submit($form_id, &$form_state) {

  return _cloud_server_templates_new_submit($form_id, $form_state);
}


/**
 * Validate the ec2 image
 */
function _aws_cloud_validate_image($cloud_context, $image_name) {
  $img_found_flag = FALSE;
  $image_id = _cloud_server_templates_get_first_element_image($image_name);
  if ( CLOUD_SERVER_TEMPLATES_NONE === $image_id ) {
    return TRUE;
  }

  $image_id = trim($image_id);
  if (empty($image_id)) {
    return FALSE;
  }

  $query_img_srch = _aws_cloud_get_image_query($cloud_context);
  $query_img_srch_args = array(
    ':image_id' => $image_id,
  );

  $result_img_srch = db_query($query_img_srch, $query_img_srch_args);

  foreach ($result_img_srch as $image) {
    $img_found_flag = TRUE;
  }

  return $img_found_flag;
}
