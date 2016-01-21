<?php

/**
 * @file
 * Provides server template functionality for each cloud sub-system.
 * Works with Cloud module.
 *
 * Copyright (c) 2010-2011 DOCOMO Innovations, Inc.
 *
 */

/**
 * Updated by yas   2015/03/25
 * Updated by Jiten 2011/07/12
 * Updated by Jiten 2011/06/20
 * Updated by yas   2011/02/14
 * Updated by yas   2011/02/11
 * Updated by yas   2011/02/10
 * Updated by yas   2011/02/08
 * Updated by yas   2011/02/02
 * Updated by yas   2011/02/01
 * Updated by Jamir 2011/02/01
 */

/**
 * Returns a form with listing of server-templates
 *
 * @param $module, $cloud_context, $params
 * @return return a form
 */
function _cloud_server_templates_list($module, $cloud_context = '', $params = array()) {

  return drupal_get_form('cloud_server_templates_list', $cloud_context, $params);
}


/**
 * Returns a form with listing of server-templates
 * Build a form including table header and table body
 * @param $form_submit
 *        This is the form-submit submitted by list server template form
 * @param $cloud_context
 *        This is the sub-cloud who's template is being displayed
 * @return return a form
 */
function cloud_server_templates_list($form, $form_submit = '', $cloud_context, $params = array()) {

  drupal_add_js(cloud_get_module_base() . 'js/cloud.js');

  $column     = 'template_nickname';
  $cloud_type = $cloud_context;


  $destination          = drupal_get_destination();
  $settings_done        = FALSE;
  $images_found         = FALSE;
  $instances_data_found = FALSE;

  $cloud_list = cloud_get_all_clouds_display_name();
  if ( $cloud_context && empty($params['all']) ) { // Cloud specified
    $is_cloud_enabled = cloud_is_settings_done($cloud_context);
    if ($is_cloud_enabled === FALSE ) {
      cloud_set_config_error_message($cloud_list[$cloud_context], $cloud_context);
    }
    else {
      $settings_done = TRUE;
    }
  }
  else { // Check for all Clouds
    foreach ($cloud_list as $cloud_context_lp => $cloud_display_name ) {

      $is_cloud_enabled = cloud_is_settings_done($cloud_context_lp);
      if ($is_cloud_enabled === FALSE ) {
        cloud_set_config_error_message($cloud_display_name, $cloud_context_lp);
      }
      else {
        $settings_done = TRUE;
      }
    }
  }

  if ($settings_done) {

    if ( $cloud_context && empty($params['all']) ) { // Cloud specified
      $cloud_has_data = cloud_perform_action('', 'check_key_sg_data', $cloud_context);
      if ( $cloud_has_data == FALSE ) {

        $create_ssh_key_url = '';
        $cloud_has_key_data = cloud_perform_action('', 'check_key_data', $cloud_context);
        if ($cloud_has_key_data === FALSE ) {

          if (user_access($cloud_context . ' register key')) {

            $create_ssh_key_url = filter_xss( l( t('Create SSH Key'   ), "clouds/$cloud_context/ssh_keys/create",    array('query' => $destination) ) );
          }

          drupal_set_message(check_plain(t('There is no information of SSH Keys'))
                           . ': '
                           . $create_ssh_key_url,
                             'warning');
        }

        $create_sg_url = '';
        $cloud_has_sg_data = cloud_perform_action('', 'check_sg_data', $cloud_context);
        if ($cloud_has_sg_data === FALSE ) {

          if (user_access($cloud_context . ' register security group')) {

            $create_sg_url = filter_xss( l( t('Create Security Group' ), "clouds/$cloud_context/security_groups/create",    array('query' => $destination) ) );
          }
          drupal_set_message(check_plain(t('There is no information of Security Group'))
                           . ': '
                           . $create_sg_url,
                             'warning');
        }
      }
      else {

        $instances_data_found = TRUE;
      }

      $image_count = cloud_perform_action('', 'get_images_count', $cloud_context);
      if ($image_count == 0) {

        $images_url = filter_xss( l( t('@cloud_name - Update Image Information', array('@cloud_name' => $cloud_list[$cloud_context])), "clouds/$cloud_context/getimagedata", array('query' => $destination) ) );
        drupal_set_message(check_plain(t('There are no image information'))
                         . ': '
                         . $images_url,
                           'warning');
      }
      else {

        $images_found = TRUE;
      }
    }
    else { // Check for all Clouds
      foreach ($cloud_list as $cloud_context_lp => $cloud_display_name ) {

        if ( cloud_is_settings_done($cloud_context_lp) === FALSE ) {
          continue;
        }

        $cloud_has_data = cloud_perform_action('', 'check_key_sg_data', $cloud_context_lp);
        if ( $cloud_has_data == FALSE ) {

          $cloud_has_data = cloud_perform_action('', 'check_key_sg_data', $cloud_context_lp);
          if ( $cloud_has_data == FALSE ) {

            $create_ssh_key_url = '';
            $cloud_has_key_data = cloud_perform_action('', 'check_key_data', $cloud_context_lp);
            if ($cloud_has_key_data === FALSE ) {

              if (user_access($cloud_context_lp . ' register key')) {

                $create_ssh_key_url = filter_xss( l( t('Create SSH Key'   ), "clouds/$cloud_context_lp/ssh_keys/create",    array('query' => $destination) ) );
              }

              drupal_set_message(check_plain(t('@cloud_name - There is no information of SSH Keys',
                                 array('@cloud_name' => $cloud_display_name) ))
                               . ': '
                               . $create_ssh_key_url, 'warning');
            }

            $create_sg_url = '';
            $cloud_has_sg_data = cloud_perform_action('', 'check_sg_data', $cloud_context_lp);
            if ($cloud_has_sg_data === FALSE ) {

              if (user_access($cloud_context_lp . ' register security group')) {

                $create_sg_url = filter_xss( l( t('Create Security Group' ), "clouds/$cloud_context_lp/security_groups/create",    array('query' => $destination) ) );
              }
              drupal_set_message(check_plain(t('@cloud_name - There is no information of Security Group',
                                 array('@cloud_name' => $cloud_display_name) ))
                               . ': '
                               . $create_sg_url, 'warning');
            }
          }
        }
        else {

          $instances_data_found = TRUE;
        }

        $image_count = cloud_perform_action('', 'get_images_count', $cloud_context_lp);
        if ($image_count == 0) {

          $images_url = filter_xss( l( t('@cloud_name - Update Image Information', array('@cloud_name' => $cloud_display_name)), "clouds/$cloud_context_lp/getimagedata", array('query' => $destination) ) );
          drupal_set_message(check_plain(t('There are no image information'))
                           . ': '
                           . $images_url,
                             'warning');
        }
        else {

          $images_found = TRUE;
        }
      }
    }
  }
  else {

    $images_found = FALSE;
    $instances_data_found = FALSE;
  }

  $options = array(
    t('Nickname'     ),
    t('Type'         ),
  );

  $filter     = cloud_get_filter_value($form_submit, 'filter'    );
  $filter     = trim($filter);

  $filter_col = cloud_get_filter_value( $form_submit, 'operation');
  $filter_col = isset($filter_col) && $filter_col ? $filter_col : 0; // default: Template Name

  if ( $filter_col == 0 ) {

    $column  = 'Template Name'      ;
    $sql_col = 'a.template_nickname';
  }
  elseif ($filter_col == 1 ) {

    $column  = 'Instance Type';
    $sql_col = 'instance_type';
  }
  elseif ($filter_col == 1 || isset($cloud_type)) {

    $column  = 'Cloud Type';
    $sql_col = 'cloud_type';
  }
  elseif ($filter_col == 3 ) {

    $column  = 'User Data';
    $sql_col = 'user_data';
  }

  if ( isset($filter) ) {

    $query_args[] = $sql_col;
    $query_args[] = $filter  ;
  }
  else {

    $filter       = ' 1 ';
    $query_args[] = 'a.template_nickname';
    $query_args[] = ' ';
  }

  $form['options'] = array(
    '#prefix' => '<div class="container-inline">',
    '#type' => 'fieldset',
    '#suffix' => '</div>',
  );

  $tmpl_count = _cloud_server_templates_get_server_templatelist_count($cloud_context, $params);
  $filter_disabled = '';
  if ( $tmpl_count < 2 ) {
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

  if (user_access('create server template') ) {
    if ($images_found === FALSE || $instances_data_found === FALSE || $cloud_context == '') {
      $form['options']['server_temaplate'] = array(
        '#type' => 'submit',
        '#value' => t('Create'),
        '#disabled' => TRUE,
      );
    }
    else {
      $form['options']['server_temaplate'] = array(
        '#type' => 'submit',
        '#value' => t('Create'),
      );
    }
  }

  $form['header'] = array(
    '#type' => 'value',
    '#data' => '',
    '#markup' => array(
      array(
        'data' => t('Nickname'    ),
        'field' => 'template_nickname',
        'sort' => 'asc',
        'class' => array('nickname-column'),
      ),
      array(
        'data' => t('Description' ),
        'field' => 'description',
      ),
      array(
        'data' => t('Type'        ),
        'field' => 'instance_type',
      ),
      array(
        'data' => t('Instances'   ),
        'field' => 'count_instances',
      ),
      array(
        'data' => t('Created'     ),
        'field' => 'template_created',
      ),
      array(
        'data' => t('Updated'     ),
        'field' => 'template_updated',
      ),
      array(
        'data' => t('Action'      ),
        'class' => 'action-column',
      ),
    ),
  );

  $query  = _cloud_server_templates_get_server_templatelist($cloud_context, $params);
  $query
      ->condition($query_args[0], '%%' . $query_args[1] . '%%', 'like');

  $result = $query
      ->extend('PagerDefault')
      ->extend('TableSort')
      ->orderByHeader($form['header']['#markup'])
      ->limit(CLOUD_SERVER_TEMPLATES_PAGER_LIMIT)
      ->execute();

  foreach ($result as $template_obj) {

    $date_created = '';
    $date_created = ($template_obj->template_created != '0000-00-00 00:00:00'
                  && $template_obj->template_created != '')
                  ? format_date(strtotime($template_obj->template_created), 'short')
                  : '-';
    $date_updated = '';
    $date_updated = ($template_obj->template_updated != '0000-00-00 00:00:00'
                  && $template_obj->template_updated != '')
                  ? format_date(strtotime($template_obj->template_updated), 'short')
                  : '-';
    $server_template_id = $template_obj->servertemplateid;

    $form['Name'           ][$server_template_id] = array('#markup' => l( $template_obj->template_nickname, CLOUD_SERVER_TEMPLATES_PATH . '/' . $template_obj->cloud_type . '/' . urlencode($server_template_id) . '/view',    array('query' => array('id' => urlencode($server_template_id), 'cloud_context' => $template_obj->cloud_type)))); 
    $form['hdnName'        ][$server_template_id] = array(
      '#type' => 'hidden',
      '#value' => addslashes($template_obj->template_nickname),
    );
    $form['Description'    ][$server_template_id] = array('#markup' => t($template_obj->description));
    $form['cloud_type'     ][$server_template_id] = array('#markup' => t($template_obj->cloud_type));
    $form['instance_type'  ][$server_template_id] = array('#markup' => t($template_obj->instance_type));
    $form['count_instances'][$server_template_id] = array('#markup' => t($template_obj->count_instances));
    $form['Created'        ][$server_template_id] = array('#markup' => $date_created);
    $form['Updated'        ][$server_template_id] = array('#markup' => $date_updated);
    $form['Actions'][$server_template_id] = array('#markup' => cloud_server_template_action(array('cloud_context' => $template_obj->cloud_type, 'template_id' => $server_template_id, 'template_name' => $template_obj->template_nickname, 'count' => $template_obj->count_instances)));
  }
  $server_template_id = !empty($server_template_id) ? $server_template_id : '';

  $form['id'] = array(
    '#type' => 'hidden',
    '#value' => $server_template_id,
  );
  $form['cloud_context'] = array(
    '#type' => 'hidden',
    '#value' => $cloud_context,
  );

  $form['#redirect'] = FALSE;

  return $form;
}


/**
 * Theme function for server template listing page
 * @param type $form
 * @return type
 */
function theme_cloud_server_templates_list($variables) {
  $form = $variables['form'];
  $rows = array();
  if (!empty($form['Name'])) {
    foreach (element_children($form['Name']) as $key) {
      $rows[] = array(
        array(
          'data' => drupal_render($form['Name'][$key]),
          'class' => array('nickname-column'),
        ),
        drupal_render($form['Description'    ][$key]),
        drupal_render($form['instance_type'  ][$key]),
        drupal_render($form['count_instances'][$key]),
        drupal_render($form['Created'        ][$key]),
        drupal_render($form['Updated'        ][$key]),
        array(
          'data' => drupal_render($form['Actions'][$key]),
          'class' => 'action-column',
        ),
      );
      unset($form['cloud_type'][$key]);
    }
  }

  $output  = drupal_render($form['options']);
  $output .= theme('table', array('header' => $form['header']['#markup'], 'rows' => $rows));
  $output .= theme('pager');
  $output .= drupal_render_children($form);

  return $output;
}

/**
 * Function to create template actions 
 */
function cloud_server_template_action($cloud_info) {
  $action_data = array();

  if (user_access('delete server template')) {
    $prop['onclick'] = cloud_get_messagebox('Are you sure you want to delete the template "' . $cloud_info['template_name'] . '" ?');
    $action_data[] = cloud_display_action(
      'images/icon_delete.png', 
      t('Delete'), 
      CLOUD_SERVER_TEMPLATES_PATH . '/' . $cloud_info['cloud_context'] . '/' . urlencode($cloud_info['template_id']) . '/delete', 
      array('html' => TRUE), 
      $prop['onclick']
    );
  }
  
  if (user_access('edit server template')) {
    $action_data[] = cloud_display_action(
      'images/icon_clear.png', 
      t('Edit'), 
      CLOUD_SERVER_TEMPLATES_PATH . '/' . $cloud_info['cloud_context'] . '/' . urlencode($cloud_info['template_id']) . '/edit', 
      array('html' => TRUE)
    );
  }
  
  if (user_access('launch server template') && !strstr($cloud_info['template_name'], 'cp')) {
    $action_data[] = cloud_display_action(
      'images/icon_play.png', 
      t('Launch'), 
      CLOUD_SERVER_TEMPLATES_PATH . '/' . $cloud_info['cloud_context'] . '/' . urlencode($cloud_info['template_id']) . '/launch', 
      array('query' => array('id' => $cloud_info['cloud_context'], 'count' => $cloud_info['count'], 'type' => $cloud_info['cloud_context']), 'html' => TRUE)
    );
  }
  
  if (user_access('copy server template')) {
    $action_data[] = cloud_display_action(
      'images/icon_copy.png', 
      t('Copy'), 
      CLOUD_SERVER_TEMPLATES_PATH . '/' . $cloud_info['cloud_context'] . '/' . urlencode($cloud_info['template_id']) . '/copy', 
      array('query' => array('id' => urlencode($cloud_info['template_id']), 'count' => $cloud_info['count'], 'type' => $cloud_info['cloud_context']), 'html' => TRUE)
    );
  }
  
  if (user_access('edit server template') && module_exists('cloud_inputs')) {
    $action_data[] = cloud_display_action(
      'images/table_gear.png', 
      t('Inputs'), 
      'design/cloud_server_templates/' . urlencode($cloud_info['template_id']) . '/inputs', 
      array('query' => array('template_id' => urlencode($cloud_info['template_id']), 'module' => 'cloud_server_templates', 'type' => $cloud_info['cloud_context']), 'html' => TRUE)
    );
  }
  
  $user_actions = module_invoke_all('cloud_server_templates_action_data', $cloud_info);
  $action_data = array_merge($action_data, $user_actions);
  return implode($action_data);
}

/**
 * Submit function for server template listing
 * @param type $form_id
 * @param type $form_values
 * @return type
 */
function cloud_server_templates_list_submit($form_id, &$form_state) {
  $form_values =& $form_state['values'];
  if ($form_values['op'] == t('Create')) {
    $form_state['redirect'] = CLOUD_SERVER_TEMPLATES_PATH . '/' . $form_values['cloud_context'] . '/create';
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
 * Delete a server-template
 * redirect to listing page
 * @param $templateid
 *        This is template-id
 * @param $cloud_context
 *        This is the sub-cloud who's template is being deleted
 * @return return a form
 */
function cloud_server_templates_delete($templateid = '', $cloud_context) {
  //about to delete server template - allow other modules to do something
  module_invoke_all('cloud_server_template_delete', $templateid, $cloud_context);

  _cloud_server_templates_delete($templateid);

  drupal_set_message(t('Template has been deleted successfully.'));
  drupal_goto(CLOUD_SERVER_TEMPLATES_PATH . '/' . $cloud_context . '/list');
}


/**
 * Copy a server-template
 * redirect to listing page
 * @param $templateid
 *        This is template-id
 * @param $cloud_context
 *        This is the sub-cloud who's template is being copied
 * @return return a form
 */
function cloud_server_templates_copy($template_id = '', $cloud_context) {
  //about to copy server template - allow other modules to do something
  module_invoke_all('cloud_server_template_copy', $template_id, $cloud_context);

  _cloud_server_templates_copy($template_id);

  drupal_set_message(t('Template copied.'));
  drupal_goto(CLOUD_SERVER_TEMPLATES_PATH . '/list/' . $cloud_context);
}


/**
 *
 * @param type $form_id
 * @param type $form_values
 * @return type
 */
function _cloud_server_templates_new_submit($form_id, &$form_state) {

  $form_values = $form_state['values'];

  $cloud_context = $form_values['cloud_type_select'];
  if ( $form_values['op'] == t('Cancel') ) {
    $form_state['redirect'] = array(CLOUD_SERVER_TEMPLATES_PATH . '/' . $cloud_context . '/list');
  }
  elseif ($form_values['op'] == t('Add')) {
    //about to add a new template - allow other modules to do something with the data
    module_invoke_all('cloud_server_template_add', $form_values, $cloud_context);
    _cloud_server_templates_insert_server_template($form_values);
    
    drupal_set_message(t('Template has been saved.'));
    $form_state['redirect'] = array(CLOUD_SERVER_TEMPLATES_PATH . '/' . $cloud_context . '/list');
  }
  elseif ($form_values['op'] == t('Edit')) {
    $template_id = $form_values['templateid'];

    //about to edit a template - allow other modules to do something with the data
    module_invoke_all('cloud_server_template_edit', $template_id, $form_values, $cloud_context);

    _cloud_server_templates_update_server_template($form_values, $template_id);

    drupal_set_message(t('Template has been updated.'));
    $form_state['redirect'] = array(CLOUD_SERVER_TEMPLATES_PATH . '/' . $cloud_context . '/list');
  }
}

/**
 * Theme function for cloud server template add/edit form
 * @param array $form
 * @return type
 */
function theme_cloud_server_templates_new($form) {
  $form = $form['form'];

  $cloud_context     = $form['cloud_type']['cloud_type_select']['#value'];
  $scripting_options = cloud_server_templates_get_scripting_options();

  $rows[] = array(
    drupal_render($form['name']['name_label'] ),
    drupal_render($form['name']['name_text'] ),
  );

  $rows[] = array(
    drupal_render($form['description']['description_label'] ),
    drupal_render($form['description']['description_text'] ),
  );

  $rows[] = array(
    drupal_render($form['cloud_type']['cloud_type_label'       ] ),
    drupal_render($form['cloud_type']['cloud_type_display'     ] ),
  );

  $rows[] = array(
    drupal_render($form['instance_type']['instance_type_label'] ),
    drupal_render($form['instance_type']['instance_type_select'] ),
  );

  if (!strstr($cloud_context, 'cp')) {
    $rows[] = array(
      drupal_render($form['ec2_image_lbl'] ),
      drupal_render($form['ec2_image_id']) . drupal_render($form['ec2_image_id']['ec2_image_id_select'] ),
    );
  }
  else {
    $rows[] = array(
      drupal_render($form['image_id']['image_id_label'] ),
      drupal_render($form['image_id']['image_id_select'] ),
    );
  }

  $rows[] = array(
    drupal_render($form['kernel_image_lbl']                           ),
    drupal_render($form['kernel_image_id']                           )
  . drupal_render($form['kernel_image_id']['kernel_image_id_select'] ),
  );

  $rows[] = array(
    drupal_render($form['ramdisk_image_lbl']),
    drupal_render($form['ramdisk_image_id'])
  . drupal_render($form['ramdisk_image_id']['ramdisk_image_id_select'] ),
  );

  $rows[] = array(
    drupal_render($form['user_name_details']['user_name_label'] ),
    drupal_render($form['user_name_details']['user_name_text'] ),
  );

  $rows[] = array(
    drupal_render($form['ssh_keys_details']['ssh_keys_label'] ),
    drupal_render($form['ssh_keys_details']['ssh_keys_select'] ),
  );

  $rows[] = array(
    drupal_render($form['SG_details']['SG_label'] ),
    drupal_render($form['SG_details']['SG_select'] ),
  );

  $rows[] = array(
    drupal_render($form['count_details']['Count_label'] ),
    drupal_render($form['count_details']['Count_text'] ),
  );

  $rows[] = array(
    drupal_render($form['zone_details']['zone_label'] ),
    drupal_render($form['zone_details']['zone_select'] ),
  );

  foreach ($scripting_options['CLOUD_SCRIPTING_TYPE_OPTIONS'] as $script => $script_val) {
    $rows[] = array(
      drupal_render($form[$script][$script . '_label'  ] ),
      drupal_render($form[$script][$script . '_content'] ),
    );
  }

  $rows[] = array(
    drupal_render($form['failover_scenario_id']['failover_scenario_id_label']   ),
    drupal_render($form['failover_scenario_id']['failover_scenario_id_content'] ),
  );


  $rows[] = array(
    drupal_render($form['alert_id']['alert_id_label']   ),
    drupal_render($form['alert_id']['alert_id_content'] ),
  );

  $rows[] = array(
    drupal_render($form['user_data']['user_data_label'] ),
    drupal_render($form['user_data']['user_data_text'] ),
  );

  $table = theme('table', array('header' => NULL, 'rows' => $rows));
  $form['fieldset_template_info']['#children'] = $table;

  //cf. Waning by Coder module: Potential problem: when FAPI element '#type' is set to 'markup' (default), '#value' only accepts filtered text, be sure to use check_plain(), filter_xss() or similar to ensure your $variable is fully sanitized.
  //$form['fieldset_template_info']['list'] = array('#type' => 'markup', '#value' => $table);

  $output  = drupal_render($form['fieldset_template_info']);
  $output .= drupal_render($form['submit_buttons']);
  $output .= drupal_render($form['type_val']);
  //$output .= drupal_render($form);
  $output .= drupal_render_children($form);

  return $output;
}


/**
 *
 * @param array $form
 * @return type
 */
function theme_cloud_server_templates_view($form) {

  if (isset($form['form'])) {

    $form = $form['form'];
  }

  $cloud_context     = !empty($form['cloud_type']['cloud_type_select']['#value']) ? $form['cloud_type']['cloud_type_select']['#value'] : '';
  $scripting_options = cloud_server_templates_get_scripting_options();
  $rows[] = array(
    drupal_render($form['name']['name_label'] ),
    drupal_render($form['name']['name_text'] ),
  );

  $rows[] = array(
    drupal_render($form['description']['description_label'] ),
    drupal_render($form['description']['description_text'] ),
  );

  $rows[] = array(
    drupal_render($form['cloud_type']['cloud_type_label'] ),
    drupal_render($form['cloud_type']['cloud_type_select'] ),
  );

  $rows[] = array(
    drupal_render($form['instance_type']['instance_type_label'       ] ),
    drupal_render($form['instance_type']['instance_cloud_type_select'] ),
  );

  if (!strstr($cloud_context, 'cp')) {
    $rows[] = array(
      drupal_render($form['ec2_image_lbl'] ),
      drupal_render($form['ec2_image_id'] )
    . drupal_render($form['ec2_image_id']['ec2_image_id_select'] ),
    );
  }
  else {
    $rows[] = array(
      drupal_render($form['image_id']['image_id_label'] ),
      drupal_render($form['image_id']['image_id_select'] ),
    );
  }

  $rows[] = array(
    drupal_render($form['kernel_image_id']['kernel_image_id_label'] ),
    drupal_render($form['kernel_image_id']['kernel_image_id_select'] ),
  );

  $rows[] = array(
    drupal_render($form['ramdisk_image_id']['ramdisk_image_id_label'] ),
    drupal_render($form['ramdisk_image_id']['ramdisk_image_id_select'] ),
  );

  $rows[] = array(
    drupal_render($form['user_name_details']['user_name_label'] ),
    drupal_render($form['user_name_details']['user_name_text'] ),
  );

  $rows[] = array(
    drupal_render($form['ssh_keys_details']['ssh_keys_label'] ),
    drupal_render($form['ssh_keys_details']['ssh_keys_select'] ),
  );

  $rows[] = array(
    drupal_render($form['SG_details']['SG_label'] ),
    drupal_render($form['SG_details']['SG_select'] ),
  );

  $rows[] = array(
    drupal_render($form['count_details']['Count_label'] ),
    drupal_render($form['count_details']['Count_text'] ),
  );

  $rows[] = array(
    drupal_render($form['zone_details']['zone_label'] ),
    drupal_render($form['zone_details']['zone_select'] ),
  );

  foreach ($scripting_options['CLOUD_SCRIPTING_TYPE_OPTIONS'] as $script => $script_val) {
    $rows[] = array(
      drupal_render($form[$script][$script . '_label'  ] ),
      drupal_render($form[$script][$script . '_content'] ),
    );
  }

  $rows[] = array(
    drupal_render($form['failover_scenario_id']['failover_scenario_id_label']   ),
    drupal_render($form['failover_scenario_id']['failover_scenario_id_content'] ),
  );

  $rows[] = array(
    drupal_render($form['alert_id']['alert_id_label'  ] ),
    drupal_render($form['alert_id']['alert_id_content'] ),
  );

  $rows[] = array(
    drupal_render($form['user_data']['user_data_label'] ),
    drupal_render($form['user_data']['user_data_text'] ),
  );

  $table = theme('table', array('header' => NULL, 'rows' => $rows));
  $form['fieldset_template_info']['#children'] = $table;

  //cf. Waning by Coder module: Potential problem: when FAPI element '#type' is set to 'markup' (default), '#value' only accepts filtered text, be sure to use check_plain(), filter_xss() or similar to ensure your $variable is fully sanitized.
  //$form['fieldset_template_info']['list'] = array('#type' => 'markup', '#value' => $table);

  $output  = drupal_render($form['options'               ] );
  $output .= drupal_render($form['submit_buttons'        ] );
  $output .= drupal_render($form['fieldset_template_info'] );
  $output .= drupal_render($form['type_val'              ] );
  $output .= drupal_render($form['cloud_context'         ] );
  $output .= drupal_render_children($form);

  return $output;
}


/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function cloud_server_templates_cloud_action_notify($op, $params) {
  switch ($op) {
    case 'launch':
      if (isset($params['cloud_context']) && isset($params['instance_id']) && isset($params['template_id'])) {
        _cloud_server_templates_insert_instance_info($params['cloud_context'], $params['instance_id'], $params['template_id']);
      }
  }
  return;
}

/**
 * Helper function to get images for a new instance launch 
 */
function _cloud_server_templates_get_images_for_new_launch($image_type_value, $cloud_context = '') {
  $query      = _get_describe_images_query($cloud_context);
  $query->condition('c.cloud_type', $cloud_context, '=');
  $query->condition('image_type', $image_type_value, 'like');
  $result = $query->execute();

  $image_options = array();
  if ($image_type_value == 'ramdisk' || $image_type_value == 'kernel') {
    $image_options[CLOUD_SERVER_TEMPLATES_NONE] = CLOUD_SERVER_TEMPLATES_NONE;
  }
  foreach ($result as $image) {
    $image_options[$image->image_id] = $image->image_nickname;
  }
  asort($image_options);

  return $image_options;
}

/**
 * Get template for a specific cloud context 
 */
function _cloud_server_templates_get_instance_type($cloud_context = '') {
  $instance_type_options = array();
  $query = db_select(CLOUD_SERVER_TEMPLATES_INSTANCE_TYPE_TABLE, 'c');
  $query->condition('c.cloud_type', $cloud_context, '=');
  $query->fields('c');
  $instance_type_result = $query->execute();

  foreach ($instance_type_result as $type) {
    $instance_type_options[$type->instance_type] = $type->instance_type;
  }
  asort($instance_type_options);
  return $instance_type_options;
}

/**
 * Get instance types
 */
function _cloud_server_templates_instance_type($cloud_context = '') {

  if (!module_exists('cloud_pricing')) {
    return NULL;
  }

  $search_where = '';

  if ($cloud_context) {
    $search_where = " and cloud_type='$cloud_context'";
  }

  //Baldwin Note: Why is Pricing table used here? It should be CLOUD_SERVER_TEMPLATES_INSTANCE_TYPE_TABLE
  return $query = 'SELECT * FROM {' . CLOUD_PRICING_INSTANCES_TABLE . '} c WHERE 1 ' . $search_where . ' ';
}

/**
 * Get instance type list
 */
function _cloud_server_templates_get_instance_type_list($cloud_context = '') {

  $query = _cloud_server_templates_instance_type($cloud_context);
  if ($query == NULL) {
    return array();
  }
  $instance_type_options = array();
  $instance_type_result  = db_query( $query );

  foreach ($instance_type_result as $type) {
    $instance_type_options[$type->instance_type] = $type->instance_type;
  }
  asort($instance_type_options);

  return $instance_type_options;
}

/**
 * Get scriptiong options
 */
function cloud_server_templates_get_scripting_options() {

  $_CLOUD_SCRIPTING_TYPE_OPTIONS = array(
    'boot' => t('Boot'       ),
    //'operational' => t('Operational'),
    //'termination' => t('Termination'),
  );

  asort($_CLOUD_SCRIPTING_TYPE_OPTIONS);

  $scripting_options = array('CLOUD_SCRIPTING_TYPE_OPTIONS' => $_CLOUD_SCRIPTING_TYPE_OPTIONS);

  return $scripting_options;
}

