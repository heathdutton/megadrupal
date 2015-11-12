<?php


/**
 * @file
 * Provides pricing for hourly rate configuration for Billing module.
 * Works with Cloud and Billing module.
 *
 * Copyright (c) 2010-2011 DOCOMO Innovations, Inc.
 *
 */

/**
 * Updated by yas   2011/02/14
 * Updated by yas   2011/02/02
 */

/**
 * Returns a form with listing of pricing
 * Build a form including table header and table body
 * @param $form_submit
 *        This is the form-submit submitted by list pricing form
 * @param $cloud_context
 *        This is the sub-cloud who's pricing is being displayed
 * @return return a form
 */
function cloud_pricing_list($form, $form_submit, $cloud_context = '') {
  $column  = 'name';
  $form['options'] = array(
    '#type' => 'fieldset',
    '#prefix' => '<div class="container-inline">',
    '#suffix' => '</div>',
  );

  $options = array(
    t('Instance Type'),
  );

  $filter     = cloud_get_filter_value( $form_submit, 'filter'    );
  $filter_col = cloud_get_filter_value( $form_submit, 'operation' );
  $filter     = trim($filter);

  if ($filter_col == 0) {
    $column  = 'Instance Type';
    $sql_col = 'instance_type';
  }

  if (isset($filter)) {

    $query_args[] = $sql_col;
    $query_args[] = $filter  ;
  }
  else {

    $filter       = ' 1 ';
    $query_args[] = ' ';
  }
  asort($options);
  $form['options']['label'    ] = array(
    '#type' => 'item'     ,
    '#title' => t('Filter'),
  );
  $form['options']['operation'] = array(
    '#type' => 'select'   ,
    '#options' => $options,
    '#default_value' => $filter_col,
  );
  $form['options']['filter'   ] = array(
    '#type' => 'textfield',
    '#size' => 40      ,
    '#default_value' => $filter,
  );
  $form['options']['submit'   ] = array(
    '#type' => 'submit'   ,
    '#value' => t('Apply'),
  );

  if (user_access('create pricing')) {
    $form['options']['cloud_pricing'] = array(
      '#type' => 'submit',
      '#value' => t('Create'),
    );
  }

  $form['header'] = array(
    '#type' => 'value',
    '#value' => array(
      array(
        'data' => t('Instance Type'),
        'field' => 'instance_type'        ,
        'sort' => 'asc'                  ,
        'class' => array('nickname-column'),
      ),
      array(
        'data' => t('Linux Usage'  ),
        'field' => 'linux_or_unix_usage',
      ),
      array(
        'data' => t('Windows Usage'),
        'field' => 'windows_usage',
      ),
      array(
        'data' => t('Updated'      ),
        'field' => 'updated',
      ),
      array(
        'data' => t('Action'       ),
      ),
    ),
  );

  $query = _cloud_pricing_get_instances($cloud_context);

  $query
      ->fields('a');

  if ( drupal_strlen($filter) > 0 ) {
    $query->condition($sql_col, '%' . $filter . '%', 'like');
  }

  $query = $query
      ->extend('PagerDefault')
      ->extend('TableSort')
      ->orderByHeader($form['header']['#value'])
      ->limit(CLOUD_PRICING_PAGER_LIMIT);

  $result = $query->execute();
  
  foreach ($result as $pricing_obj) {
    $form['instance_type'      ][$pricing_obj->id] = array('#markup' => t($pricing_obj->instance_type));
    $form['hdnName'            ][$pricing_obj->id] = array(
      '#type' => 'hidden',
      '#value' => addslashes($pricing_obj->instance_type),
    );
    $form['linux_or_unix_usage'][$pricing_obj->id] = array('#markup' => t('$' . number_format($pricing_obj->linux_or_unix_usage, 3     )));
    $form['windows_usage'      ][$pricing_obj->id] = array('#markup' => t('$' . number_format($pricing_obj->windows_usage      , 3     )));
    $form['Updated'            ][$pricing_obj->id] = array('#markup' => format_date(strtotime($pricing_obj->updated), 'short'));
  }

  $form['pager'        ] = array('#value' => theme('pager', array('tags' => NULL, 'element' => 0)));
  $form['cloud_context'] = array(
    '#type' => 'hidden',
    '#value' => ($cloud_context),
  );
  $form['#redirect'    ] = FALSE;
  return $form;
}

/**
 * Theme pricing list page
 */
function theme_cloud_pricing_list($variables) {
  $form = $variables['form'];

  $form['pager']['#value'] = !empty($form['pager']['#value']) ? $form['pager']['#value'] : '';
  $rows = array();

  if ( !empty($form['instance_type']) ) {

    foreach (element_children($form['instance_type']) as $key) {

      $row   = array(
        array(
          'data' => drupal_render($form['instance_type'      ][$key]),
          'class' => array('nickname-column'),
        ),
        drupal_render($form['linux_or_unix_usage'][$key]),
        drupal_render($form['windows_usage'      ][$key]),
        drupal_render($form['Updated'            ][$key]),
      );

      $prop['onclick'] = cloud_get_messagebox('Are you sure you want to delete the Pricing (for "' . $form['hdnName'][$key]['#value'] . '" ?');

      $action_data = '';
      if (user_access('delete pricing')) {

        $action_data .= cloud_display_action('images/icon_delete', t('Delete'), 'admin/config/clouds'
                     . '/' . $form['cloud_context']['#value'] . '/pricing/' . urlencode($key) . '/delete', array('query' => array('id' => urlencode($key)), 'html' => TRUE), $prop['onclick']); //,  'id=' . urlencode($key) , $prop );
      }

      if (user_access('edit pricing')) {
        $action_data .= cloud_display_action('images/icon_clear', t('Edit'), 'admin/config/clouds'
                     . '/' . $form['cloud_context']['#value'] . '/pricing/' . urlencode($key) . '/edit'  , array('query' => array('id' => urlencode($key)), 'html' => TRUE)); //,  'id=' . urlencode($key)  );
      }

      $row[]  = $action_data;
      $rows[] = $row;
    }
  }

  $reload_link = l(t('- Refresh Page -'), 'admin/config/clouds/' . $form['cloud_context']['#value'] . '/pricing/info');
  $ref_link    = array(
    '#type' => 'item',
    '#prefix' => '<div id="link_reload" align="right">',
    '#markup' => $reload_link,
    '#suffix' => '</div>',
  );

  $output  = drupal_render($form['options']);
  $output .= theme('table', array('header' => $form['header']['#value'], 'rows' => $rows));
  $output .= theme('pager', array('tags' => array()));
  $output .= drupal_render($ref_link);
  $output .= drupal_render_children($form    );

  return $output;
}

/**
 * Cloud pricing filter submit function
 */
function cloud_pricing_list_submit($form_id, &$form_state) {

  $form_values = $form_state['values'];
  if ($form_values['op'] == t('Create')) {
    $form_state['redirect'] = 'admin/config/clouds/' . $form_values['cloud_context'] . '/pricing/create';
  }
  elseif ( $form_values['op'] == t('Apply') ) {
    $param_arr = array(
      'filter' => $form_values['filter'],
      'operation' => $form_values['operation'],
    );
    $form_state['redirect']     = array(current_path(), array('query' => $param_arr));
  }
}


/**
 * Delete pricing
 */
function cloud_pricing_delete($pricing_id = '', $cloud_context) {

  _cloud_pricing_delete($pricing_id);

  drupal_set_message(t('Pricing has been deleted successfully.'));
  drupal_goto('admin/config/clouds/' . $cloud_context . '/pricing/info');

  return;
}


/**
 * Form to add a new price
 */
function cloud_pricing_new($form, $form_submit, $pricing_id = '', $cloud_context = '') {

  $action = !($pricing_id) ? t('Add') : t('Edit');

  if ($action == t('Add')) {
    $instance_type_options = _cloud_server_templates_get_instance_type_list();
  }
  else {
    $instance_type_options = _cloud_server_templates_get_instance_type();
  }

  $form['fieldset_pricing_info'] = array('#type' => 'fieldset');
  $instance_type_options[''] = '- Select -';

  $form['instance_type']['instance_type_label'] = array(
    '#type' => 'item',
    '#title' => t('Instance Type'),
    '#required' => TRUE,
  );

  $form['instance_type']['instance_type_select'] = array(
    '#type' => 'textfield',
    '#size' => 50,
  );

  $form['description']['description_label'] = array(
    '#type' => 'item',
    '#title' => t('Description'),
    '#required' => TRUE,
  );
  $form['description']['description_text'] = array('#type' => 'textarea');

  $form['linux_or_unix_usage']['linux_or_unix_usage_label'] = array(
    '#type' => 'item'     ,
    '#title' => t('Linux Usage Charge per Hour ($)'),
    '#required' => TRUE,
  );
  $form['linux_or_unix_usage']['linux_or_unix_usage_text'] = array(
    '#type' => 'textfield',
    '#size' => 100,
  );

  $form['windows_usage']['windows_usage_label'] = array(
    '#type' => 'item',
    '#title' => t('Windows Usage Charge per Hour ($)'),
    '#required' => TRUE,
  );
  $form['windows_usage']['windows_usage_text'] = array(
    '#type' => 'textfield',
    '#size' => 100,
  );

  if ($pricing_id) {
    $query      = _cloud_pricing_get_instances();
    $query->condition('id', $pricing_id);

    $pricing_object    = $query->execute()->fetchObject();
    $action = t('Edit');
    $form['pricing_id'] = array(
      '#type' => 'hidden',
      '#value' => $pricing_id,
    );
    $form['instance_type_old'] = array(
      '#type' => 'hidden',
      '#value' => $pricing_object->instance_type,
    );
    $form['instance_type'      ]['instance_type_select'    ]['#default_value'] = isset_variable($pricing_object->instance_type);
    $form['description'        ]['description_text'        ]['#default_value'] = isset_variable($pricing_object->description);
    $form['linux_or_unix_usage']['linux_or_unix_usage_text']['#default_value'] = isset_variable($pricing_object->linux_or_unix_usage);
    $form['windows_usage'      ]['windows_usage_text'      ]['#default_value'] = isset_variable($pricing_object->windows_usage);
  }

  $form['submit_buttons'] = array(
    '#type' => 'fieldset',
    '#prefix' => '<span class="clear"></span><div class="container-inline"><div class="buttons">',
    '#suffix' => '</div></div>',
  );

  $form['submit_buttons'][$action] = array(
    '#type' => 'submit',
    '#value' => t($action     ),
  );
  $form['submit_buttons']['Cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'    ),
  );
  $form['cloud_context'           ] = array(
    '#type' => 'hidden',
    '#value' => $cloud_context,
  );

  return $form;
}

/**
 * Theme function for new pricing entry form
 */
function theme_cloud_pricing_new($variables) {
  $form = $variables['form'];

  $rows = array(
    array(
      drupal_render($form['instance_type']['instance_type_label']),
      drupal_render($form['instance_type']['instance_type_select']),
    ),
    array(
      drupal_render($form['description']['description_label']),
      drupal_render($form['description']['description_text']),
    ),
    array(
      drupal_render($form['linux_or_unix_usage']['linux_or_unix_usage_label']),
      drupal_render($form['linux_or_unix_usage']['linux_or_unix_usage_text']),
    ),
    array(
      drupal_render($form['windows_usage']['windows_usage_label']),
      drupal_render($form['windows_usage']['windows_usage_text']),
    ),
  );

  $table = theme('table', array('header' => NULL, 'rows' => $rows));
  $form['fieldset_pricing_info']['#children'] = $table;

  $output  = drupal_render($form['fieldset_pricing_info']);
  $output .= drupal_render($form['submit_buttons'       ]);
  $output .= drupal_render_children($form);

  return $output;
}


/**
 * Validate function when creating a new entry
 */
function cloud_pricing_new_validate($form_id, $form_values) {
  $form_values = $form_values['values'];
  if ($form_values['op'] == t('Add') || $form_values['op'] == t('Edit')) {
    if (empty($form_values['instance_type_select']) ) {
      form_set_error('instance_type_select',    t('You must enter valid Instance Type'));
    }

    if ( $form_values['op'] == t('Add') ) {
      $pricing_obj = _cloud_pricing_get_by_id( $form_values['cloud_context'], $form_values['instance_type_select'] );
      if (empty($pricing_obj) === FALSE ) {
        form_set_error('instance_type_select',    t('Instance type already exists.'));
      }
    }

    if (empty($form_values['description_text'])) {
      form_set_error('description_text',    t('You must enter description'));
    }
    elseif (!preg_match(CLOUD_PRICING_VALID_NUMBER, $form_values['linux_or_unix_usage_text'])) {
      form_set_error('linux_or_unix_usage_text',    t('You must enter valid usage for Linux'));
    }
    elseif (!preg_match(CLOUD_PRICING_VALID_NUMBER, $form_values['windows_usage_text'])) {
      form_set_error('windows_usage_text',    t('You must enter valid usage for Windows'));
    }
  }
}


/**
 * Submit function for creating a new price
 */
function cloud_pricing_new_submit($form_id, &$form_state) {

  $form_values = $form_state['values'];
  $redirect_to = 'admin/config/clouds/' . $form_values['cloud_context'] . '/pricing/info';

  if ($form_values['op'] == t('Add')) {
    _cloud_pricing_insert($form_values);
    drupal_set_message(t('Pricing has been saved.'));
  }
  elseif ($form_values['op'] == t('Edit')) {
    $pricing_id = $form_values['pricing_id'];
    _cloud_pricing_update($form_values, $pricing_id);
    drupal_set_message(t('Pricing has been saved.'));
    
  }
  $form_state['redirect'] = $redirect_to;
}


/**
 * Submit function to go back to info page
 */
function cloud_pricing_view_submit($form_id, &$form_state) {

  $form_values = $form_state['values'];

  if ($form_values['op'] == t('Back')) {
    $form_state['redirect'] = CLOUD_PRICING_PATH . '/info';
  }
}
