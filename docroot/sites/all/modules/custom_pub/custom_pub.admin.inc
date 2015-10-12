<?php
/**
 * @file
 *   Admin functions
 */

/**
 * Callback function for Custom Publishing Options menu item
 */
function custom_pub_admin() {
  $table = '';
  //add the js for the admin page
  drupal_add_js(drupal_get_path('module','custom_pub').'/custom_pub.admin.js');
  $types = variable_get('custom_pub_types', array());//get the current custom publishing types
  foreach ($types as $type) {
    // Build table rows for the
    $form_row = array();
    $types_of_nodes = '';
    if (isset($type['node_types']) && is_array($type['node_types'])) {
      $types_of_nodes = implode(', ', $type['node_types']);
    }
    $row = array($type['name'], $type['type'], $types_of_nodes, array('data' => '','class' => array('custom_pub-option-edit-cell')));
    $form_row[] = array(
      'data'=> drupal_get_form('custom_pub_edit_'.$type['type'], $type),
      'colspan'=> 4,
      'class' => array('custom_pub-form-edit'),
    );
    $rows[] = array('data' => $row, 'class' => array('custom_pub-option-row'));
    $rows[] = array('data' => $form_row, 'class' => array('custom_pub-form-edit'));
  }
  if (!empty($rows)) {
    $vars = array(
       'header' => array(
         t('Label'),
         t('Machine Name'),
         t('Node Types'),
         array('data'=>'','class' => array('custom_pub-head-edit')),
       ),
       'rows' => $rows
     );
    $table = theme('table', $vars);
  }

  $output = '';
  $form = drupal_get_form('custom_pub_add');
  $output = render($form);

  return $table . $output;
}

/**
 * Form callback function for add form
 */
function custom_pub_add($form, &$form_state) {
  $node_types = _node_types_build();
  foreach($node_types->types as $node_type => $info) {
    $types[$node_type] = $info->name;
  }
  $form['state_fs'] = array(
    '#type' => 'fieldset',
    '#title' => t('Add A Publishing Option'),
  );
  $form['state_fs']['state'] = array(
    '#title' => t('Publishing label'),
    '#type' => 'textfield',
    '#description' => t('The label for your custom publishing option.This is the text that will be displayed on the node add/edit form'),
    '#maxlength' => 255,
      '#required' => TRUE,
  );
  $form['state_fs']['state_machine'] = array(
    '#type' => 'textfield',
    '#title' => t('Option name'),
    '#description' => t('The machine-readable name of this publishing option. This text will be used for constructing the database table column name. This name must contain only lowercase letters,numbers, and underscores. This name must be unique.'),
      '#required' => TRUE,
  );
  $form['state_fs']['node_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Available on'),
    '#description' => t('The Node Types this option is available on.'),
    '#options' => $types,
  );
  $form['state_fs']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add'),
  );

  return $form;
}

/**
 * Form validation function for add form
 */
function custom_pub_add_validate($form, &$form_state) {
  $types = variable_get('custom_pub_types', array());
  $type = array();
  $type['type'] = trim($form_state['values']['state_machine']);
  $type['name'] = trim($form_state['values']['state']);
  $node = drupal_get_schema('node');

  if (isset($types[$type['type']])) {
    form_set_error('state_machine', t('The machine-readable name %type is already taken.', array('%type' => $type->type)));
  }
  if (!preg_match('!^[a-z0-9_]+$!', $type['type'])) {
    form_set_error('state_machine', t('The machine-readable name must contain only lowercase letters, numbers, and underscores.'));
  }
  // 'theme' conflicts with theme_node_form().
  // '0' is invalid, since elsewhere we check it using empty().
  if (in_array($type['type'], array_keys($node['fields'])) && !isset($types[$type['type']])) {
    form_set_error('state_machine', t("Invalid machine-readable name. That name is already taken by a database column. Please enter a name other than %invalid.", array('%invalid' => $type['type'])));
  }
  foreach ($types as $check) {
    if ($type['name'] == $check['name']) {
      form_set_error('state', t("Invalid Label. That Publishing Label is already taken. Please enter a label other than %invalid.", array('%invalid' => $type['name'])));
    }
  }
}

/**
 * Form submit function for add form
 */
function custom_pub_add_submit($form, &$form_state) {
  $types = variable_get('custom_pub_types', array());
  $node_types = _node_types_build();
  $type = array();
  $type[trim($form_state['values']['state_machine'])]['type'] = trim($form_state['values']['state_machine']);
  $type[trim($form_state['values']['state_machine'])]['name'] = trim($form_state['values']['state']);
  foreach ($form_state['values']['node_types'] as $node_type => $value) {
    if (!empty($value)) {
      $type[trim($form_state['values']['state_machine'])]['node_types'][$node_type] = $node_types->types[$node_type]->name;
    }
  }

  $spec = array(
    'description' => 'Custom publishing option '.t(trim($form_state['values']['state'])),
    'type' => 'int',
    'not null' => TRUE,
    'default' => 0
  );
  try {
    db_add_field('node', trim($form_state['values']['state_machine']), $spec);
    $success = TRUE;
  }
  catch (Exception $e){
    $success = FALSE;
  }
  if ($success){
    cache_clear_all('*', 'cache', TRUE);
    variable_set('custom_pub_types', array_merge($types,$type));
    drupal_set_message(t('Publishing option created.'));
  }
  else {
    drupal_set_message(t('There was an error creating your Publishing option.'),'error');
  }
}

/**
 * Form callback function for edit form
 */
function custom_pub_edit($form, &$form_state, $type) {
  $node_types = _node_types_build();
  foreach ($node_types->types as $node_type => $info) {
    $types[$node_type] = $info->name;
  }
  $form['type'] = array(
    '#type' => 'value',
    '#value' => $type,
  );
  $form['state'] = array(
    '#title' => t('Publishing label'),
    '#type' => 'textfield',
    '#maxlength' => 255,
    '#size' => 100,
    '#default_value' => $type['name'],
  );
  $form['node_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Available on'),
    '#description' => t('The Node Types this option is available on.'),
    '#options' => $types,
    '#default_value' => isset($type['node_types']) ? array_keys($type['node_types']) : array(),
  );
  $form['stateh'] = array(
    '#type' => 'hidden',
    '#value' => $type['type'],
  );
  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  if (!isset($type['default'])) {
    $form['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#attributes' => array('id'=>'edit-delete'),
    );
  }
  $form['#theme'][] = 'custom_pub_edit_form';
  $form['#validate'][] = 'custom_pub_edit_validate';
  $form['#submit'][] = 'custom_pub_edit_submit';
  return $form;
}

/**
 * Form validate function for edit form
 */
function custom_pub_edit_validate($form, &$form_state) {
  $types = variable_get('custom_pub_types', array());
  $type = $form_state['values']['type'];
  $name = trim($form_state['values']['state']);
  foreach ($types as $check) {
    if ($type['name'] == $check['name'] && $type['type'] != $check['type']) {
      form_set_error('state', t("Invalid Label. That Publishing Label is already taken. Please enter a label other than %invalid.", array('%invalid' => $type['name'])));
    }
  }
}

/**
 * Form submit function for edit form
 */
function custom_pub_edit_submit($form, &$form_state) {
  $node_types = _node_types_build();
  $type = $form_state['values']['type'];
  $types = variable_get('custom_pub_types', array());

  if ($form_state['values']['op'] == t('Delete')) {
    unset($types[$type['type']]);
    try {
      db_drop_field('node', $type['type']);
      $success = TRUE;
    }
    catch (Exception $e){
      $success = FALSE;
    }
  }
  else{
    unset($types[$type['type']]['node_types']);
    foreach ($form_state['values']['node_types'] as $node_type => $value) {
      if (!empty($value)) {
        $types[$type['type']]['node_types'][$node_type] = $node_types->types[$node_type]->name;
      }
    }
    $type['name'] = trim($form_state['values']['state']);
    $types[$type['type']]['name'] = $type['name'];
    $success = TRUE;
  }
  if ($success) {
    cache_clear_all('*', 'cache', TRUE);
    variable_set('custom_pub_types', $types);
    drupal_set_message(t('!op publishing option successful.',array('!op' => $form_state['values']['op'])));
  }
  else {
    drupal_set_message(t('There was an error trying to !op the publishing option. Please Try Again',array('!op' => $form_state['values']['op'])),'error');
  }
}

/**
 * Menu callback: content administration form.
 * @param $form
 * @param $form_state
 * @return array
 */
function custom_pub_node_admin_content($form, $form_state) {
  custom_pub_node_admin_inc_add();
  if (isset($form_state['values']['operation']) && $form_state['values']['operation'] == 'delete') {
    $del_form = node_multiple_delete_confirm($form, $form_state, array_filter($form_state['values']['nodes']));
    //we have to add a custom validation function that adds the node.admin.inc file ahead of the submit handlers
    array_unshift($del_form['#submit'], 'custom_pub_node_admin_inc_add');
    return $del_form;
  }
  $custom_publish_types = custom_pub_types_list();
  $form['filter'] = custom_pub_node_filter_form();
  $form['#submit'][] = 'custom_pub_node_filter_form_submit';
  $form['admin'] = node_admin_nodes();

  /*
  $changed = $form['admin']['nodes']['#header']['changed'];
  $operations = $form['admin']['nodes']['#header']['operations'];

  unset($form['admin']['nodes']['#header']['changed']);
  unset($form['admin']['nodes']['#header']['operations']);

  $form['admin']['nodes']['#header']['custom_publish_options'] = array('data' => 'Custom Status');
  $form['admin']['nodes']['#header']['changed'] = $changed;
  $form['admin']['nodes']['#header']['operations'] = $operations;

  foreach ($form['admin']['nodes']['#options'] as $nid => $option) {
    $form['admin']['nodes']['#options'][$nid]['custom_publish_options'] = array();

    $node = node_load($nid);
    foreach ($custom_publish_types as $custom_publish_machine_name => $label) {
      if (isset($node->{$custom_publish_machine_name}) && $node->{$custom_publish_machine_name} == 1) {
        $form['admin']['nodes']['#options'][$nid]['custom_publish_options'][] = $label;
      }
    }

    $form['admin']['nodes']['#options'][$nid]['custom_publish_options'] = count($form['admin']['nodes']['#options'][$nid]['custom_publish_options']) ? implode(', ', $form['admin']['nodes']['#options'][$nid]['custom_publish_options']) : t('none');
  }*/

  //same as above
  array_unshift($form['admin']['options']['submit']['#validate'], 'custom_pub_node_admin_inc_add');
  $form['#validate'][] = 'custom_pub_node_admin_inc_add';
  return $form;
}

/**
 * Return form for node administration filters.
 * The main difference between this function and the base function is the drupal_alter call after node_filters
 */
function custom_pub_node_filter_form() {
  $session = isset($_SESSION['node_overview_filter']) ? $_SESSION['node_overview_filter'] : array();
  $filters = node_filters();
  drupal_alter('node_filters', $filters);

  $i = 0;
  $form['filters'] = array(
    '#type' => 'fieldset',
    '#title' => t('Show only items where'),
    '#theme' => 'exposed_filters__node',
  );
  foreach ($session as $filter) {
    list($type, $value) = $filter;
    if ($type == 'term') {
      // Load term name from DB rather than search and parse options array.
      $value = module_invoke('taxonomy', 'term_load', $value);
      $value = $value->name;
    }
    elseif ($type == 'language') {
      $value = $value == LANGUAGE_NONE ? t('Language neutral') : module_invoke('locale', 'language_name', $value);
    }
    else {
      $val = $filters[$type]['options'][$value];
      unset($filters[$type]['options'][$value]);
      $value = $val;
    }
    $t_args = array('%property' => $filters[$type]['title'], '%value' => $value);
    if ($i++) {
      $form['filters']['current'][] = array('#markup' => t('and where %property is %value', $t_args));
    }
    else {
      $form['filters']['current'][] = array('#markup' => t('where %property is %value', $t_args));
    }
  }

  $form['filters']['status'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('clearfix')),
    '#prefix' => ($i ? '<div class="additional-filters">' . t('and where') . '</div>' : ''),
  );
  $form['filters']['status']['filters'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('filters')),
  );
  foreach ($filters as $key => $filter) {
    $form['filters']['status']['filters'][$key] = array(
      '#type' => 'select',
      '#options' => $filter['options'],
      '#title' => $filter['title'],
      '#default_value' => '[any]',
    );
  }

  $form['filters']['status']['actions'] = array(
    '#type' => 'actions',
    '#attributes' => array('class' => array('container-inline')),
  );
  $form['filters']['status']['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => count($session) ? t('Refine') : t('Filter'),
  );
  if (count($session)) {
    $form['filters']['status']['actions']['undo'] = array('#type' => 'submit', '#value' => t('Undo'));
    $form['filters']['status']['actions']['reset'] = array('#type' => 'submit', '#value' => t('Reset'));
  }

  drupal_add_js('misc/form.js');

  return $form;
}

/**
 * Form submit function for node_admin_content
 */
function custom_pub_node_filter_form_submit($form, $form_state) {
  $types = variable_get('custom_pub_types', array());
  $filters = node_filters();
  switch ($form_state['values']['op']) {
    case t('Filter'):
    case t('Refine'):
      // Apply every filter that has a choice selected other than 'any'.
      foreach ($filters as $filter => $options) {
        $custom = FALSE;
        if (isset($form_state['values'][$filter]) && $form_state['values'][$filter] != '[any]') {
          // Flatten the options array to accommodate hierarchical/nested options.
          $flat_options = form_options_flatten($filters[$filter]['options']);
          // Only accept valid selections offered on the dropdown, block bad input.
          if ($filter == 'status') {
            list($type, $val) = explode('-',$form_state['values'][$filter]);
            $custom = isset($types[$type]);
          }
          if (isset($flat_options[$form_state['values'][$filter]]) || $custom) {
            $_SESSION['node_overview_filter'][] = array($filter, $form_state['values'][$filter]);
          }
        }
      }
      break;
    case t('Undo'):
      array_pop($_SESSION['node_overview_filter']);
      break;
    case t('Reset'):
      $_SESSION['node_overview_filter'] = array();
      break;
  }
}

/**
 * Implements hook_node_filters_alter()
 */
function custom_pub_node_filters_alter(&$filters) {
  $types = variable_get('custom_pub_types', array());
  foreach($types as $type) {
    $filters['status']['options'][$type['type'] . '-1'] = t($type['name']);
    $filters['status']['options'][$type['type'] . '-0'] = t('not ' . $type['name']);
  }
}

/**
 * Adds the node.admin.inc file when called.
 * This is generally called from form validate function or build function to ensure the files are always there.
 */
function custom_pub_node_admin_inc_add() {
  module_load_include('inc','node', 'node.admin');
}