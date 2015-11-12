<?php

/**
 * @file
 * Provides input parameter user interface for Server Templates and Cluster module.
 * Works with Cloud module.
 *
 * Copyright (c) 2010-2011 DOCOMO Innovations, Inc.
 *
 */

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function cloud_inputs_edit($form, $form_submit = '', $module = '', $id = '') {
  $inputs_options = _cloud_inputs_get_options();

  drupal_add_js(cloud_get_module_base() . 'js/cloud_inputs.js');

  $template_id = '';
  $template_ids = '';
  $cluster_id = '';
  $instance_options = array();

  if ($module == 'cloud_server_templates') {

    $template_id = $id;
    $search_arg = 'server_template_id';
    $search_val = $id;
  }
  else {

    $cluster_id = $id;
    $search_arg = 'cluster_id';
    $search_val = $cluster_id;

    $query      = _cloud_cluster_servers_get_query();
    $query->condition('cluster_id', $cluster_id, '=');

    $result     = $query->execute();
    foreach ($result as $cluster_obj) {

      $server_id = !empty($cluster_obj->serverid) ? $cluster_obj->serverid : '';
      $instance_options[$server_id] = $cluster_obj->server_nickname;
    }
  }

  $ssh_key_options = array();

  $action = t('Add');
  $filter = ' 1 ';
  $query_args[] = $search_arg;
  $query_args[] = $search_val;

  if ($module == 'cloud_server_templates') {

    $query = _cloud_inputs_list_get($id, $search_arg, $search_val);
    $result = db_query( $query);
  }
  else {
    $server_id = -1;
    //cloud_cluster_template is passed when coming from
    //cluster detail form
    if ($module == 'cloud_cluster_template') {
      $server_id = isset($_GET['server_id']) ? $_GET['server_id'] : -1;
    }

    $query = _cloud_inputs_get_cluster($cluster_id, $server_id);
    $result = db_query( $query, $query_args );
  }
  $t_rows = 0;
  foreach ($result as $input_obj) {


    $input_param          = $input_obj->input_parameter;
    $param_id             = (int) $input_obj->param_id;
    $param_type           = $input_obj->param_type;
    $value_of_instance_id = $input_obj->value_of_instance_id;

    $input_value = ($module == 'cloud_server_templates')
                 ? $input_obj->template_value
                 : $input_obj->cluster_value;

    $form['cloud_inputs'][$param_id][$param_id . '_label'            ] = array(
      '#type' => 'item',
      '#title' => t($input_param),
    );
    $form['cloud_inputs'][$param_id][$param_id . '_input_type_select'] = array(
      '#id' => $param_id . '_inputType',
      '#type' => 'select',
      '#options' => $inputs_options['CLOUD_INPUTS_TYPE_OPTIONS'],
    );
    $form['cloud_inputs'][$param_id][$param_id . '_input_type_select']['#attributes'] = array('onChange' => 'javaScript:switchOptions(' . $param_id . ')');
    $form['cloud_inputs'][$param_id][$param_id . '_text'             ] = array(
      '#id' => $param_id . '_inputText',
      '#type' => 'textfield',
      '#size' => 50,
    );
    $form['cloud_inputs'][$param_id][$param_id . '_key'              ] = array(
      '#id' => $param_id . '_inputKey',
      '#type' => 'select',
      '#options' => $ssh_key_options,
    );
    $form['cloud_inputs'][$param_id][$param_id . '_env'              ] = array(
      '#id' => $param_id . '_inputEnv',
      '#type' => 'select',
      '#options' => $inputs_options['ENV_VARIABLES_OPTIONS'],
    );
    $form['cloud_inputs'][$param_id][$param_id . '_instance_select'  ] = array(
      '#id' => $param_id . '_instance_select',
      '#type' => 'select',
      '#options' => $instance_options,
    );

    $form['cloud_inputs'][$param_id][$param_id . '_input_type_select']['#default_value'] = $param_type;
    if ($param_type == 'key') {
      $form['cloud_inputs'][$param_id][$param_id . '_key']['#attributes'   ] = array('style' => '');
      $form['cloud_inputs'][$param_id][$param_id . '_key']['#default_value'] = $input_value;
      $form['cloud_inputs'][$param_id][$param_id . '_text']['#attributes'  ] = array('style' => 'display:none');
      $form['cloud_inputs'][$param_id][$param_id . '_env']['#attributes'   ] = array('style' => 'display:none');
      $form['cloud_inputs'][$param_id][$param_id . '_instance_select'      ]['#attributes'] = array('style' => 'display:none');
    }
    if ($param_type == 'text') {
      $form['cloud_inputs'][$param_id][$param_id . '_text']['#attributes'   ] = array('style' => '');
      $form['cloud_inputs'][$param_id][$param_id . '_text']['#default_value'] = $input_value;
      $form['cloud_inputs'][$param_id][$param_id . '_key']['#attributes'    ] = array('style' => 'display:none');
      $form['cloud_inputs'][$param_id][$param_id . '_env']['#attributes'    ] = array('style' => 'display:none');
      $form['cloud_inputs'][$param_id][$param_id . '_instance_select']['#attributes'] = array('style' => 'display:none');
    }

    if ($param_type == 'env' && $module != 'cloud_server_templates') {
      $form['cloud_inputs'][$param_id][$param_id . '_env']['#default_value'] = $input_value;
      $form['cloud_inputs'][$param_id][$param_id . '_instance_select'      ]['#default_value'] = $value_of_instance_id;
      $form['cloud_inputs'][$param_id][$param_id . '_key'                  ]['#attributes'   ] = array('style' => 'display:none');
      $form['cloud_inputs'][$param_id][$param_id . '_text'                 ]['#attributes'   ] = array('style' => 'display:none');
    }
    $t_rows++;
  }
  if ($t_rows == 0) {
    drupal_set_message(t('- No Inputs -'));
  }

  //adding server_id
  if ($module == 'cloud_cluster_template') {
    $form['server_id'] = array(
      '#type' => 'hidden',
      '#value' => $server_id,
    );
  }
  $form['template_id'] = array(
    '#type' => 'hidden',
    '#value' => $template_id,
  );
  $form['template_ids'] = array(
    '#type' => 'hidden',
    '#value' => $template_ids,
  );
  $form['cluster_id'  ] = array(
    '#type' => 'hidden',
    '#value' => $cluster_id,
  );
  $form['module'      ] = array(
    '#type' => 'hidden',
    '#value' => $module,
  );
  $form['submit_buttons'] = array(
    '#type' => 'fieldset',
    '#prefix' => '<span class="clear"></span><div class="container-inline"><div class="buttons">',
    '#suffix' => '</div></div>',
  );

  if ($t_rows > 0) {
    $form['submit_buttons']['Save'  ] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
    );
  }
  return $form;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function theme_cloud_inputs_edit($variables) {
  $form = $variables['form'];

  $rows = array();
  if ( !empty($form['cloud_inputs']) ) {
    foreach (element_children($form['cloud_inputs']) as $key) {

      $row = array(
        drupal_render($form['cloud_inputs'][$key][$key . '_label'            ] ),
        drupal_render($form['cloud_inputs'][$key][$key . '_input_type_select'] ),
        drupal_render($form['cloud_inputs'][$key][$key . '_text'             ] ),
        drupal_render($form['cloud_inputs'][$key][$key . '_key'              ] ),
        drupal_render($form['cloud_inputs'][$key][$key . '_env'              ] ),
        drupal_render($form['cloud_inputs'][$key][$key . '_instance_select'  ] ),
        drupal_render($form['cloud_inputs'][$key][$key . '_server_id']),
      );
      $rows[] = $row;
    }
  }
  $output  = theme('table', array('header' => NULL, 'rows' => $rows));

  $output .= drupal_render($form['submit_buttons']);
  $output .= drupal_render($form['template_id'       ]);
  $output .= drupal_render($form['template_ids'      ]);
  $output .= drupal_render($form['cluster_id'        ]);
  $output .= drupal_render($form['module'            ]);
  $output .= drupal_render_children($form);

  return $output;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function cloud_inputs_edit_submit($form_id, $form_values) {

  $form_values = $form_values['values'];
  $module = &$form_values['module'];
  if ( $form_values['op'] == t('Cancel') ) {

    drupal_goto( CLOUD_INPUTS_PATH . '/list');
    return;

  }
  elseif ($form_values['op'] == t('Save')) {
    //NOT NEEDED
//    $template_id = ($form_values['template_id'])
//                 ? $form_values['template_id']
//                 : '';
//    $cluster_id  = ($form_values['cluster_id'])
//                 ? $form_values['cluster_id']
//                 : '';
//    $module      = ($form_values['module'])
//                 ? $form_values['module']
//                 : '';
//    
//    if ($module == 'cloud_server_templates') {
//      $query_str = '&template_id=' . $template_id . '&module=' . $module ;
//    }
//    else {
//      $query_str = '&cluster_id='  . $cluster_id  . '&module=' . $module ;
//    }

    _cloud_inputs_update($form_values);
    drupal_set_message(t('Inputs have been saved.'));
    if ($module == 'cloud_server_templates') {
      drupal_goto('design/server_templates/list');
    }
    else {
      drupal_goto('design/cluster');
    }
  }
}
