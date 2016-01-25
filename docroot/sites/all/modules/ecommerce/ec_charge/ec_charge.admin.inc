<?php

/**
 * @file
 * Provide administration functions for ec_charge module.
 */

/**
 *
 */
function ec_charge_admin_list($form, &$form_state) {
  ctools_include('export');
  $form = array();

  $charges = ctools_export_load_object('ec_charge');
  uasort($charges, 'ec_sort');

  $delta = empty($charges) ? 10 : max(array(
    abs(min(array_map('ec_common_get_weights', $charges))),
    abs(max(array_map('ec_common_get_weights', $charges))),
  ));
  $delta = ($delta < 10) ? 10 : $delta;
  $delta = (round(count($charges) / 2) > $delta) ? round(count($charges) / 2) : $delta;

  $form['chg'] = array(
    '#tree' => TRUE,
    '#theme' => 'ec_charge_admin_list_table',
  );
  foreach ($charges as $name => $chg) {
    $form['chg'][$name]['chgid'] = array(
      '#type' => 'value',
      '#value' => $chg->chgid,
    );
    $form['chg'][$name]['name'] = array(
      '#value' => $chg->name,
    );
    $form['chg'][$name]['description'] = array(
      '#value' => $chg->description,
    );
    $form['chg'][$name]['chg_type'] = array(
      '#value' => $chg->chg_type ? t('Product') : t('Transaction'),
    );
    $form['chg'][$name]['enabled'] = array(
      '#value' => $chg->enabled ? t('Yes') : t('No'),
    );
    $form['chg'][$name]['weight'] = array(
      '#type' => 'weight',
      '#default_value' => $chg->weight,
      '#delta' => $delta,
    );
    $form['chg'][$name]['operations'] = array(
      '#value' => l(t('edit'), 'admin/config/store/charge/' . $chg->name . '/edit') . ' ' . l(t('delete'), 'admin/config/store/charge/' . $chg->name . '/delete'),
    );
    $form['chg'][$name]['operations']['#value'] .= ' ' . l(t('export'), 'admin/config/store/charge/' . $chg->name . '/export');
    if ($chg->enabled) {
      $form['chg'][$name]['operations']['#value'] .= ' ' . l(t('disable'), 'admin/config/store/charge/' . $chg->name . '/disable');
    }
    else {
      $form['chg'][$name]['operations']['#value'] .= ' ' . l(t('enable'), 'admin/config/store/charge/' . $chg->name . '/enable');
    }
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
  );

  return $form;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ec_charge_admin_list_submit(&$form, &$form_state) {
  foreach ($form_state['values']['chg'] as $record) {
    drupal_write_record('ec_charge', $record, 'chgid');
  }

  drupal_set_message(t('The configuration options have been saved.'));
}

/**
 * Import charge from another system
 */
function ec_charge_admin_import_form($form, &$form_state) {
  $form = array();

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
  );

  $form['import'] = array(
    '#type' => 'textarea',
    '#title' => t('Import'),
    '#rows' => 20,
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Import'),
    '#submit' => array('ec_charge_admin_import_form_submit'),
  );

  return $form;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ec_charge_admin_import_form_validate(&$form, &$form_state) {
  $import = $form_state['values']['import'];

  if (empty($import)) {
    return;
  }

  ob_start();
  eval($import);
  ob_end_clean();

  if (preg_replace('/[a-z0-9_]/i', '', $form_state['values']['name'])) {
    form_set_error('name', t('The name field can only contain alphanumeric characters and underscores'));
  }

  if (is_object($chg)) {
    if (!empty($form_state['values']['name'])) {
      $chg->name = $form_state['values']['name'];
    }

    if (ec_charge_load($chg->name)) {
      form_set_error('name', t('The name of the new charge already exists, please use a different name.'));
    }

    $errorno = 0;
    foreach (array('filters', 'variables', 'calculations', 'modifications') as $types) {
      foreach ($chg->{$types} as $info) {
        $type = drupal_substr($types, 0, -1);
        if (!ec_charge_get_info($type, 'type', $info['component'])) {
          form_set_error('componet_' . $errorno++, t('@type %name cannot be found. Make sure the correct modules are installed.', array('@type' => drupal_ucfirst($type), '%name' => $info['component'])));
        }
      }
    }

    form_set_value($form['import'], $chg, $form_state);
  }
  else {
    form_set_error('import', t('There has been a problem importing the charge.'));
  }
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ec_charge_admin_import_form_submit(&$form, &$form_state) {
  $chg = & $form_state['values']['import'];

  ec_charge_save($chg);

  $form_state['redirect'] = 'admin/config/store/charge';
}

/**
 * Default main form for adding of charges.
 */
function ec_charge_admin_edit_form($form, &$form_state, $chg = NULL) {
  ctools_include('ajax');
  ctools_include('modal');
  ctools_modal_add_js();
  drupal_add_js('misc/collapse.js');
  drupal_add_css(drupal_get_path('module', 'ec_charge') . '/css/admin.css');

  $form_state['name'] = $chg ? $chg->name : 'new';

  $cache = ec_charge_admin_edit_get_page_cache($form_state['name']);
  if (!$cache) {
    if (!$chg) {
      $chg = (object) array(
        'name' => '',
        'description' => '',
        'chg_type' => 0,
        'chg_group' => '',
        'variables' => array(),
        'filters' => array(),
        'calculations' => array(),
        'modifications' => array(),
        'enabled' => 0,
      );
    }

    foreach (array('variables', 'filters', 'calculations', 'modifications') as $component) {
      $chg->{$component} = isset($chg->{$component}) ? $chg->{$component} : array();
    }

    ec_charge_admin_edit_set_page_cache($form_state['name'], $chg);
  }
  else {
    $chg = & $cache;
  }

  $form_state['chg'] = & $chg;

  $form = array();

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#default_value' => $chg->name,
    '#description' => t('Enter a unique id which contains only letters, numbers and underscores "_", this name can be used to to reference the charge as a variable'),
    '#required' => TRUE,
    '#size' => 32,
    '#max_length' => 32,
  );

  $form['description'] = array(
    '#type' => 'textfield',
    '#title' => t('Description'),
    '#default_value' => $chg->description,
    '#description' => t('Enter a description which will be displayed on the invoice'),
    '#required' => TRUE,
  );

  $form['chg_type'] = array(
    '#type' => 'select',
    '#title' => t('Charge type'),
    '#default_value' => $chg->chg_type,
    '#options' => array(
      0 => t('Transaction'),
      1 => t('Product'),
    ),
    '#description' => t('Choose where this charge is going to be applied to the transaction. If you choose transaction the charge will be added as an additional line item on the invoice, or a product is where the charge will be included in the total price of the product.'),
  );

  $form['chg_group'] = array(
    '#type' => 'textfield',
    '#title' => theme('advanced_help_topic', array('module' => 'ec_charge', 'topic' => 'charge-group')) . t('Transaction group'),
    '#default_value' => $chg->chg_group,
  );

  $form['subform'] = array(
    '#value' => '<div id="sub-form"></div>',
  );

  $form['right'] = array(
    '#prefix' => '<div id="right-container">',
    '#suffix' => '</div>',
  );

  $form['left'] = array(
    '#prefix' => '<div id="left-container">',
    '#suffix' => '</div><span class="clear" />',
  );

  ec_charge_add_filter_form($form, $form['left'], $form_state, $chg);
  ec_charge_add_variable_form($form, $form['left'], $form_state, $chg);
  ec_charge_add_calc_form($form, $form['right'], $form_state, $chg);
  ec_charge_add_modification_form($form, $form['right'], $form_state, $chg);

  if (empty($chg->chgid)) {
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Create'),
      '#submit' => array('ec_charge_admin_edit_form_submit'),
    );
  }
  else {
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Update'),
      '#submit' => array('ec_charge_admin_edit_form_submit'),
    );
    $form['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#submit' => array('ec_charge_admin_edit_form_delete'),
    );
  }
  $form['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
    '#submit' => array('ec_charge_admin_edit_form_cancel'),
  );

  return $form;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ec_charge_admin_edit_form_validate(&$form, &$form_state) {
  if (preg_replace('/[a-z0-9_]/i', '', $form_state['values']['name'])) {
    form_set_error('name', t('The name field can only contain alphanumeric characters and underscores'));
  }
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ec_charge_admin_edit_form_submit(&$form, &$form_state) {
  $chg = ec_charge_admin_edit_get_page_cache($form_state['name']);

  foreach ($form_state['values'] as $key => $value) {
    if (in_array($key, array('filters', 'variables', 'calculations', 'modifications'))) {
      if (!empty($form_state['values'][$key])) {
        foreach ($form_state['values'][$key] as $delta => $component) {
          $chg->{$key}[$delta] = array_merge($chg->{$key}[$delta], $component);
        }
      }
    }
    else {
      $chg->{$key} = $value;
    }
  }

  foreach (array('filters', 'variables', 'calculations', 'modifications') as $component) {
    if (isset($chg->{$component}) && is_array($chg->{$component})) {
      uasort($chg->{$component}, 'ec_sort');
    }
  }

  ec_charge_save($chg);

  ec_charge_admin_edit_clear_page_cache($form_state['name']);
  $form_state['redirect'] = 'admin/config/store/charge';
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ec_charge_admin_edit_form_cancel(&$form, &$form_state) {
  ec_charge_admin_edit_clear_page_cache($form_state['name']);
  $form_state['redirect'] = 'admin/config/store/charge';
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ec_charge_admin_edit_form_delete(&$form, &$form_state) {
  $chg = & $form_state['chg'];
  ec_charge_admin_edit_clear_page_cache($form_state['name']);
  $form_state['redirect'] = 'admin/config/store/charge/' . $chg->name . '/delete';
}

/**
 * Build the form to create filters
 */
function ec_charge_add_filter_form(&$form, &$form_location, $form_state, $chg) {
  $form_location['filters'] = array(
    '#tree' => TRUE,
  );
  ec_charge_add_form($form, $form_location['filters'], $form_state, 'filter', $chg->filters, t('Filters are used to determine if a charge is going to be added to transaction. Multiple filters can be added and in such a case when this is done the conditions need to be meet by all the filters to be a valid charge on this transaction.'));
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ec_charge_add_variable_form(&$form, &$form_location, &$form_state, $chg) {
  $form_location['variables'] = array(
    '#tree' => TRUE,
  );
  ec_charge_add_form($form, $form_location['variables'], $form_state, 'variable', $chg->variables, t('Variables are used to calculate initial figures for use with calculations. Variables can be made up of different cross sections of the transaction or any other basic parameters in the system.'));
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ec_charge_add_calc_form(&$form, &$form_location, &$form_state, $chg) {
  $form_location['calculations'] = array(
    '#tree' => TRUE,
  );
  ec_charge_add_form($form, $form_location['calculations'], $form_state,     'calculation', $chg->calculations, t('Calculations do all the heavy lifting in the creating of charges. These can as simple of complex as needed and the value of an individual transaction will flow onto the next.'));
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ec_charge_add_modification_form(&$form, &$form_location, &$form_state, $chg) {
  $form_location['modifications'] = array(
    '#tree' => TRUE,
  );
  ec_charge_add_form($form, $form_location['modifications'], $form_state, 'modification', $chg->modifications, t('Modifications are used to make changes to the charges that are calculated, to modify how they are going to be displayed on the transaction.'));
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ec_charge_add_form(&$form, &$form_location, &$form_state, $type, $params, $description) {
  ctools_include('ajax');
  ctools_include('modal');

  if (empty($params)) {
    $params = array();
  }

  ec_charge_add_form_component_list($form, $form_location, $form_state, $type, $params);

  $form_location['new'] = array(
    '#markup' => ctools_modal_text_button(t('Add !type', array('!type' => $type)), 'store/nojs/charge-component-add/' . $form_state['name'] . '/' . $type, t('Add !type', array('!type' => $type))),
  );

  $form_location['description'] = array(
    '#prefix' => '<div class="description">',
    '#suffix' => '</div>',
    '#markup' => $description,
  );

  ec_charge_load_js($form_location);
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ec_charge_add_form_component_list(&$form, &$form_location, &$form_state, $type, $params) {
  $parents = & drupal_static(__FUNCTION__ . '_parents', array());

  ctools_include('ajax');
  ctools_include('modal');

  if (!isset($parents[$type])) {
    $parents[$type] = ec_charge_component_get_parents($type, $params);
  }

  $form_location['#theme'] = 'ec_charge_component_form';
  $form_location['#ctype'] = $type;
  $form_location['#prefix'] = '<h3>' . drupal_ucfirst($type . 's') . '</h3>';

  foreach ($params as $key => $param) {
    if (!isset($param['delete']) || !$param['delete']) {
      $info = ec_charge_get_info($type, 'type', $param['component']);
      $form_location[$key]['#parent'] = isset($info['parent']) ? $info['parent'] : FALSE;
      $form_location[$key]['name'] = array(
        '#markup' => (isset($param['parent']) && $param['parent'] ? theme('indentation') : '') . $param['name'],
      );
      if ($type != 'variable') {
        if ($parents[$type]) {
          $form_location[$key]['id'] = array(
            '#type' => 'hidden',
            '#value' => $key,
            '#attributes' => array(
              'class' => array($type . '-id'),
            ),
          );
          $form_location[$key]['parent'] = array(
            '#type' => 'hidden',
            '#default_value' => isset($param['parent']) ? $param['parent'] : '',
            '#attributes' => array(
              'class' => array($type . '-parent'),
            ),
          );
        }

        $form_location[$key]['weight'] = array(
          '#type' => 'weight',
          '#default_value' => isset($param['weight']) ? $param['weight'] : 0,
        );
      }
      if (!isset($info['no form']) || !$info['no form']) {
        $form_location[$key]['operations']['edit'] = array(
          '#markup' => ctools_modal_text_button(t('edit'), 'store/nojs/charge-component/' . $form_state['name'] . '/' . $type . '/' . $key . '/edit', t('edit')),
        );
      }
      $form_location[$key]['operations']['delete'] = array(
        '#markup' => ctools_ajax_text_button(t('delete'), 'store/nojs/charge-component/' . $form_state['name'] . '/' . $type . '/' . $key . '/delete', t('delete')),
      );
    }
  }
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ec_charge_component_form_render_row($type, &$form) {
  $row = array();
  $row[] = drupal_render($form['name']);
  if ($type != 'variable') {
    $row[] = drupal_render($form['weight']) . (isset($form['id']) ? drupal_render($form['id']) : '') . (isset($form['parent']) ? drupal_render($form['parent']) : '');
  }
  $row[] = drupal_render($form['operations']);

  return $row;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ec_charge_load_js(&$form) {
  $loaded = & drupal_static(__FUNCTION__ . '_loaded', FALSE);
  if (empty($form['#ctype'])) {
    return;
  }

  $type = $form['#ctype'];
  $keys = _ec_charge_get_table_ids($form);
  $weight = ($type == 'variable' ? 0 : 1);
  $parent = NULL;

  $form['new']['new_' . $type]['#attributes']['class'][] = $type . '-select';
  $form['new']['new_' . $type . '_submit']['#attributes']['class'][] = $type . '-add';

  if (!empty($keys)) {
    foreach ($keys as $key) {
      $form[$key]['weight']['#attributes']['class'][] = $type . '-weight';
      $parent = isset($parent) ? $parent : (isset($form[$key]['parent']) ? TRUE : FALSE);
    }
  }

  if ($weight) {
    drupal_add_tabledrag($type . '-table', 'order', 'sibling', $type . '-weight');
    if ($parent) {
      drupal_add_tabledrag($type . '-table', 'match', 'parent', $type . '-parent', $type . '-parent', $type . '-id');
    }
  }
}

function _ec_charge_get_table_ids($form) {
  $ids = element_children($form);
  return array_filter($ids, '_ec_charge_table_filter');
}

function _ec_charge_table_filter($a) {
  return ($a !== 'new' && $a !== 'description') ? TRUE : FALSE;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ec_charge_admin_edit_component($op, $name, $type, $id = NULL, $ajax = FALSE) {
  $GLOBALS['devel_shutdown'] = FALSE;
  ctools_include('ajax');
  ctools_include('modal');

  $chg = ec_charge_admin_edit_get_page_cache($name);
  $component = $op == 'add' ? $id : $chg->{$type . 's'}[$id]['component'];
  $info = ec_charge_get_info($type, 'type', $component);

  if (isset($info['no form']) && $info['no form']) {
    if ($op == 'add') {
      $chg->{$type . 's'}[] = array(
        'name' => $info['name'],
        'type' => $type,
        'component' => $id,
      );
    }
    ec_charge_admin_edit_set_page_cache($name, $chg);

    if ($ajax) {
      $output = array();
      $output[] = ctools_modal_command_dismiss();
      $form = array($type . 's' => array('#tree' => TRUE), 'values' => array());
      $form_state = array('name' => $name);
      ec_charge_add_form_component_list($form, $form[$type . 's'], $form_state, $type, $chg->{$type . 's'});
      unset($form[$type . 's']['#prefix']);

      $form = form_builder('', $form, $form_state);

      $table = drupal_render($form[$type . 's']);
      $output[] = ajax_command_replace('#' . $type . '-wrapper', $table);

      echo ajax_render($output);
    }
    else {
      if (isset($chg->chgid)) {
        drupal_goto('admin/config/store/charge/' . $chg->name . '/edit');
      }
      else {
        drupal_goto('admin/config/store/charge/add');
      }
    }
  }
  else {
    $form_state = array();
    $form_state['op'] = $op;
    $form_state['type'] = $type;
    $form_state['name'] = $name;
    $form_state['component'] = $component;
    $form_state['id'] = $id;
    $form_state['ajax'] = $ajax;
    $form_state['chg'] = $chg;

    $output = ctools_modal_form_wrapper('ec_charge_admin_edit_form_component', $form_state);

    if (!empty($form_state['executed']) && $ajax) {
      $output = array();
      $output[] = ctools_modal_command_dismiss();

      $form = array($type . 's' => array('#tree' => TRUE));
      $form_state = array('name' => $name, 'values' => array());
      ec_charge_add_form_component_list($form, $form[$type . 's'], $form_state, $type, $chg->{$type . 's'});
      unset($form[$type . 's']['#prefix']);

      $form = form_builder('', $form, $form_state);

      $table = drupal_render($form[$type . 's']);
      $output[] = ajax_command_replace('#' . $type . '-wrapper', $table);
    }

    if (!$ajax && isset($form_state['redirect'])) {
      drupal_goto($form_state['redirect']);
    }

    if ($ajax) {
      echo ajax_render($output);
    }
    else {
      return $output;
    }
  }
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ec_charge_admin_edit_form_component($form, &$form_state) {
  $info = ec_charge_get_info($form_state['type'], 'type', $form_state['component']);
  $type = $form_state['type'];
  $chg = & $form_state['chg'];

  if ($form_state['op'] == 'add') {
    $chg->{$type . 's'}[] = array(
      'name' => $info['name'],
      'type' => $type,
      'component' => $form_state['id'],
    );

    $id = end(array_keys($form_state['chg']->{$type . 's'}));
  }
  else {
    $id = $form_state['id'];
  }
  $settings = & $form_state['chg']->{$type . 's'}[$id];
  $form_state['delta'] = $id;

  $form = array();

  $form_state['title'] = t('@operation @name @charge_type', array('@charge_type' => $form_state['type'], '@operation' => ($form_state['op'] == 'add' ? t('Add') : t('Edit')), '@name' => $info['name']));
  drupal_set_title($form_state['title']);

  if ($function = ec_charge_get_callback_function($form_state['type'], $form_state['component'], 'form')) {
    $form = $function($form_state, $settings);
  }

  $default = array();
  if (isset($settings['id'])) {
    $default['id'] = array(
      '#type' => 'value',
      '#value' => $settings['id'],
    );
  }
  $default['component'] = array(
    '#type' => 'value',
    '#value' => $settings['component'],
  );
  $default['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#default_value' => isset($settings['name']) ? $settings['name'] : $info['name'],
    '#weight' => -10,
  );
  $default['weight'] = array(
    '#type' => 'value',
    '#value' => isset($settings['weight']) ? $settings['weight'] : ec_charge_get_component_weight($form_state['chg'], $form_state['type']),
  );
  if ($form_state['type'] == 'filter') {
    $default['reverse'] = array(
      '#type' => 'checkbox',
      '#title' => t('Reverse the outcome of this filter.'),
      '#default_value' => isset($settings['reverse']) ? $settings['reverse'] : 0,
    );
  }
  $default['submit'] = array(
    '#type' => 'submit',
    '#value' => $form_state['op'] === 'add' ? t('Add') : t('Update'),
    '#submit' => array('ec_charge_admin_edit_component_submit'),
    '#name' => 'edit-update',
  );
  $default['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
    '#submit' => array('ec_charge_admin_edit_component_cancel'),
    '#name' => 'edit-cancel',
  );
  $default['#validate'] = array('ec_charge_admin_edit_component_validate');

  $form += $default;

  return $form;
}

function ec_charge_admin_edit_component_validate(&$form, &$form_state) {
  $chg = & $form_state['chg'];
  if ($function = ec_charge_get_callback_function($form_state['type'], $form_state['component'], 'validate')) {
    $function($form, $form_state);
  }
}

function ec_charge_admin_edit_component_submit(&$form, &$form_state) {
  $chg = & $form_state['chg'];
  if ($function = ec_charge_get_callback_function($form_state['type'], $form_state['component'], 'submit')) {
    $function($form, $form_state);
  }

  $defaults = array('id', 'name', 'component', 'weight', 'reverse');
  $settings = array();
  if ($function = ec_charge_get_callback_function($form_state['type'], $form_state['component'], 'settings')) {
    $settings = $function();
    if (!is_array($settings)) {
      $settings = array();
    }
  }
  $settings = array_merge($settings, $defaults);

  foreach ($settings as $field) {
    if (isset($form_state['values'][$field])) {
      $chg->{$form_state['type'] . 's'}[$form_state['delta']][$field] = $form_state['values'][$field];
    }
  }

  ec_charge_admin_edit_set_page_cache($form_state['name'], $chg);
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ec_charge_admin_edit_component_cancel(&$form, &$form_state) {
  $chg = & $form_state['chg'];

  if ($form_state['ajax']) {
    $form_state['commands'] = ctools_modal_command_dismiss();
  }
  elseif (isset($chg->chgid)) {
    $form_state['redirect'] = 'admin/config/store/charge/' . $chg->name . '/edit';
  }
  else {
    $form_state['redirect'] = 'admin/config/store/charge/add';
  }
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ec_charge_admin_delete_component($name, $type, $delta, $ajax) {
  ctools_include('ajax');
  $chg = ec_charge_admin_edit_get_page_cache($name);

  if (isset($chg->{$type . 's'}[$delta])) {
    unset($chg->{$type . 's'}[$delta]);
    ec_charge_admin_edit_set_page_cache($name, $chg);
  }

  if ($ajax) {
    $form = array($type . 's' => array('#tree' => TRUE));
    $form_state = array('name' => $name, 'values' => array());
    ec_charge_add_form_component_list($form, $form[$type . 's'], $form_state, $type, $chg->{$type . 's'});
    unset($form[$type . 's']['#prefix']);

    $form = form_builder('', $form, $form_state);

    $table = drupal_render($form[$type . 's']);

    $output = array();
    $output[] = ajax_command_replace('#' . $type . '-wrapper', $table);

    echo ajax_render($output);
  }
  elseif (isset($chg->chgid)) {
    drupal_goto('admin/config/store/charge/' . $chg->name . '/edit');
  }
  else {
    drupal_goto('admin/config/store/charge/add');
  }
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ec_charge_get_component_weight($chg, $type) {
  $weights = array_map('_ec_charge_get_component_weight', $chg->{$type . 's'});
  return empty($weights) ? 0 : max($weights) + 1;
}

function _ec_charge_get_component_weight($a) {
  return isset($a['weight']) ? $a['weight'] : 0;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ec_charge_admin_delete_confirm($form, &$form_state, $chg) {
  $form = array();
  $form['chg'] = array(
    '#type' => 'value',
    '#value' => $chg,
  );

  return confirm_form(
    $form,
    t('Are you sure you want to delete charge %name', array('%name' => $chg->name)),
    'admin/config/store/charge',
    t('This charge will be deleted and cannot be undone'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ec_charge_admin_delete_confirm_submit(&$form, &$form_state) {
  $chg = & $form_state['values']['chg'];

  ec_charge_delete($chg->chgid);
  drupal_set_message(t('Charge %name has been deleted.', array('%name' => $chg->name)));
  $form_state['redirect'] = 'admin/config/store/charge';
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ec_charge_admin_enable_confirm($form, &$form_state, $chg) {
  $form = array();
  $form['chg'] = array(
    '#type' => 'value',
    '#value' => $chg,
  );

  return confirm_form(
    $form,
    t('Are you sure you want to enable charge %name', array('%name' => $chg->name)),
    'admin/config/store/charge',
    t('This charge will be applied to all transactions from now onwards'),
    t('Enable'),
    t('Cancel')
  );
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ec_charge_admin_enable_confirm_submit(&$form, &$form_state) {
  $chg = & $form_state['values']['chg'];
  $chg->enabled = 1;
  ec_charge_save($chg);
  drupal_set_message(t('Charge %name has been enabled', array('%name' => $chg->name)));

  $form_state['redirect'] = 'admin/config/store/charge';
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ec_charge_admin_disable_confirm($form, &$form_state, $chg) {
  $form = array();
  $form['chg'] = array(
    '#type' => 'value',
    '#value' => $chg,
  );

  return confirm_form(
    $form,
    t('Are you sure you want to disable charge %name', array('%name' => $chg->name)),
    'admin/config/store/charge',
    t('This charge will not be applied to all transactions from now onwards'),
    t('Disable'),
    t('Cancel')
  );
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ec_charge_admin_disable_confirm_submit(&$form, &$form_state) {
  $chg = & $form_state['values']['chg'];
  $chg->enabled = 0;
  ec_charge_save($chg);
  drupal_set_message(t('Charge %name has been disabled', array('%name' => $chg->name)));

  $form_state['redirect'] = 'admin/config/store/charge';
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ec_charge_admin_export($form, $form_state, $chg) {
  ctools_include('export');
  $form = array();

  unset($chg->chgid);

  $form['export'] = array(
    '#type' => 'textarea',
    '#value' => ctools_export_object('ec_charge', $chg, '', 'chg'),
    '#rows' => 20,
  );

  return $form;
}

/**
 * get the page cache for the edit
 */
function ec_charge_admin_edit_get_page_cache($name) {
  ctools_include('object-cache');

  return ctools_object_cache_get('ec_charge_edit', $name);
}

/**
 * Store changes to a change in the object cache.
 */
function ec_charge_admin_edit_set_page_cache($name, $chg) {
  ctools_include('object-cache');
  $cache = ctools_object_cache_set('ec_charge_edit', $name, $chg);
}

/**
 * Remove an item from the object cache.
 */
function ec_charge_admin_edit_clear_page_cache($name) {
  ctools_include('object-cache');
  ctools_object_cache_clear('ec_charge_edit', $name);
}

/**
 * Provide a list of components which can be used to define a charge.
 */
function ec_charge_admin_edit_add_component($name, $type, $ajax = FALSE, $category = NULL) {
  ctools_include('ajax');
  ctools_include('modal');

  $output = theme('ec_charge_add_component', array('name' => $name, 'type' => $type, 'category' => $category));

  if ($ajax) {
    ctools_modal_render(t('Add new !type', array('!type' => $type)), $output);
  }
  else {
    return $output;
  }
}

