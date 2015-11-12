<?php

/**
 * @file
 * Administration pages for access filters.
 */

/**
 * Form builder to list and manage filters.
 *
 * @ingroup forms
 * @see access_filter_overview_filters_submit()
 * @see theme_access_filter_overview_filters()
 */
function access_filter_overview_filters($form) {
  global $conf, $access_filter_fast_mode_enabled;
  if (!empty($conf['access_filter_disabled'])) {
    drupal_set_message(t('Access filter module is disabled in settings.php.', 'warning'));
    return;
  }

  $process_types = array(
    ACCESS_FILTER_PROCESS_TYPE_DENY => t('Deny from'),
    ACCESS_FILTER_PROCESS_TYPE_ALLOW => t('Allow from'),
  );
  $action_types = array(
    ACCESS_FILTER_DENY_ACTION_403 => t('Error') . ': 403 Forbidden',
    ACCESS_FILTER_DENY_ACTION_404 => t('Error') . ': 404 Not Found',
    ACCESS_FILTER_DENY_ACTION_301 => t('Redirect') . ': 301 Moved Permanently',
    ACCESS_FILTER_DENY_ACTION_302 => t('Redirect') . ': 302 Moved Temporarily',
  );

  $form['#tree'] = TRUE;

  $filters = access_filter_load_all();
  foreach ($filters as $filter) {
    $form[$filter->fid]['#filter'] = $filter;
    $form[$filter->fid]['enabled'] = array('#markup' => check_plain($filter->status >= ACCESS_FILTER_STATUS_ENABLED) ? 'o' : 'x');
    $form[$filter->fid]['name'] = array('#markup' => check_plain($filter->name));

    $paths_markup = '<ul>';
    foreach ($filter->parsed_paths as $parsed) {
      $type_text = $parsed->is_uri ? t('Request uri') : t('Drupal path');
      $modifier_texts = array();
      if ($parsed->is_regex) {
        $modifier_texts[] = t('regex');
      }
      if ($parsed->is_blind) {
        $modifier_texts[] = t('blind');
      }
      if (!empty($modifier_texts)) {
        $type_text .= ' (' . implode(', ', $modifier_texts) . ')';
      }
      $paths_markup .= '<li>' . check_plain($parsed->pattern) . ' <i>#' . check_plain($type_text) . '</i></li>';
    }
    $paths_markup .= '</ul>';
    $form[$filter->fid]['paths'] = array('#markup' => $paths_markup);

    $rules_markup = '<ol>';
    foreach (explode("\n", $filter->rules) as $line) {
      $line = trim($line);
      if (strlen($line)) {
        list($type, $ip) = explode(':', $line, 2);
        $rules_markup .= '<li>' . check_plain($process_types[$type] . ' ' . $ip) . '</li>';
      }
    }
    $rules_markup .= '</ol>';
    $form[$filter->fid]['rules'] = array('#markup' => $rules_markup);

    $deny_action_markup = '<ul>';
    if ($filter->deny_action_settings->type == ACCESS_FILTER_DENY_ACTION_301 || $filter->deny_action_settings->type == ACCESS_FILTER_DENY_ACTION_302) {
      $deny_action_markup .= '<li>' . $action_types[$filter->deny_action_settings->type] . '<br />' . $filter->deny_action_settings->redirect_destination . '</li>';
    }
    else {
      $deny_action_markup .= '<li>' . $action_types[$filter->deny_action_settings->type] . '</li>';
    }
    if ($filter->deny_action_settings->force_logout) {
      $deny_action_markup .= '<li>' . t('Force logout') . '</li>';
    }
    $deny_action_markup .= '</ul>';
    $form[$filter->fid]['action'] = array(
      '#markup' => $deny_action_markup,
    );

    $form[$filter->fid]['weight'] = array(
      '#type' => 'textfield',
      '#title' => t('Weight for @title', array('@title' => $filter->name)),
      '#title_display' => 'invisible',
      '#delta' => 10,
      '#size' => 3,
      '#default_value' => $filter->weight,
    );
    $form[$filter->fid]['edit'] = array(
      '#type' => 'link',
      '#title' => t('edit'),
      '#href' => "admin/config/people/access_filter/$filter->fid/edit",
    );
    $form[$filter->fid]['delete'] = array(
      '#type' => 'link',
      '#title' => t('delete'),
      '#href' => "admin/config/people/access_filter/$filter->fid/delete",
    );
  }

  if (count($filters) > 1) {
    $form['actions'] = array('#type' => 'actions');
    $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
    );
  }
  elseif (isset($filter)) {
    unset($form[$filter->fid]['weight']);
  }

  if ($access_filter_fast_mode_enabled) {
    if ($conf['access_filter_fast'] != access_filter_build_fast($filters)) {
      drupal_set_message('Fast mode is enabled and some filter(s) are changed. Changes does not apply until modify settings.php.', 'warning');
    }
    else {
      drupal_set_message('Fast mode is enabled. Changes does not apply until modify settings.php.', 'warning');
    }
  }

  return $form;
}

/**
 * Returns HTML for the access filter overview form as a list of filters.
 *
 * @param array $variables
 *   An associative array containing:
 *   - form: A render element representing the form.
 *
 * @see access_filter_overview_filters()
 * @ingroup themeable
 */
function theme_access_filter_overview_filters($variables) {
  $form = $variables['form'];

  $rows = array();

  foreach (element_children($form) as $key) {
    if (isset($form[$key]['name'])) {
      $filter = &$form[$key];

      $row = array();
      $row[] = drupal_render($filter['enabled']);
      $row[] = drupal_render($filter['name']);
      $row[] = drupal_render($filter['paths']);
      $row[] = drupal_render($filter['rules']);
      $row[] = drupal_render($filter['action']);
      if (isset($filter['weight'])) {
        $filter['weight']['#attributes']['class'] = array('access-filter-weight');
        $row[] = drupal_render($filter['weight']);
      }
      $row[] = drupal_render($filter['edit']);
      $row[] = drupal_render($filter['delete']);
      $rows[] = array('data' => $row, 'class' => array('draggable'));
    }
  }

  $header = array(
    t('Enabled'),
    t('Name'),
    t('Paths'),
    t('Rules'),
    t('Actions On deny'),
  );
  if (isset($form['actions'])) {
    $header[] = t('Weight');
    drupal_add_tabledrag('access-filter', 'order', 'sibling', 'access-filter-weight');
  }
  $header[] = array('data' => t('Operations'), 'colspan' => '2');

  $markup = theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'empty' => t(
      'No filters available. <a href="@link">Add filter</a>.',
      array('@link' => url('admin/config/people/access_filter/add'))
    ),
    'attributes' => array('id' => 'access-filter'),
  ));
  return $markup . drupal_render_children($form);
}

/**
 * Form submission handler for access_filter_overview_filters().
 *
 * @see access_filter_overview_filters()
 */
function access_filter_overview_filters_submit($form, &$form_state) {
  foreach ($form_state['values'] as $fid => $filter) {
    if (is_numeric($fid) && $form[$fid]['#filter']->weight != $filter['weight']) {
      $form[$fid]['#filter']->weight = $filter['weight'];
      access_filter_save($form[$fid]['#filter']);
    }
  }
  drupal_set_message(t('The configuration options have been saved.'));
}

/**
 * Form builder for the filter fast settings form.
 */
function access_filter_form_fast($form) {
  $filters = access_filter_load_all();

  $form['dump'] = array(
    '#type' => 'textarea',
    '#value' => "\$conf['access_filter_fast'] = '" . access_filter_build_fast($filters) . "';",
    '#attributes' => array('readonly' => 'readonly'),
    '#prefix' => '<div>' . t('To enable fast mode, add below line(s) into settings.php.') . '</div>',
  );

  return $form;
}

/**
 * Serialize filters for fast mode.
 *
 * @param array $filters
 *   An array of filter object.
 *
 * @return string
 *   A string of serialized filters.
 */
function access_filter_build_fast($filters) {
  foreach ($filters as $i => $filter) {
    if ($filter->status == ACCESS_FILTER_STATUS_ENABLED) {
      // Remove useless properties.
      unset($filter->fid, $filter->name, $filter->status, $filter->weight);
    }
    else {
      unset($filters[$i]);
    }
  }
  return serialize($filters);
}

/**
 * Form builder for the filter editing form.
 *
 * @ingroup forms
 * @see access_filter_form_filter_submit()
 * @see access_filter_form_filter_validate()
 */
function access_filter_form_filter($form, &$form_state, $filter = NULL) {
  if (!isset($form_state['term'])) {
    $defaults = array(
      'status' => ACCESS_FILTER_STATUS_ENABLED,
      'name' => '',
      'is_strict' => TRUE,
      'paths' => '',
      'rules' => '',
      'deny_action_settings' => (object) array(
        'type' => ACCESS_FILTER_DENY_ACTION_403,
        'error_message' => array('value' => '', 'format' => NULL),
        'redirect_destination' => '',
        'force_logout' => FALSE,
      ),
      'weight' => 0,
    );
    if (!$filter) {
      $filter = new stdClass();
    }
    foreach ($defaults as $key => $value) {
      if (!isset($filter->$key)) {
        $filter->$key = $value;
      }
      elseif (is_object($value)) {
        foreach ((array) $value as $sub_key => $sub_value) {
          if (!isset($filter->$key->$sub_key)) {
            $filter->$key->$sub_key = $sub_value;
          }
        }
      }
    }
    $form_state['filter'] = $filter;
  }
  else {
    $filter = $form_state['filter'];
  }

  if (isset($filter->fid)) {
    $form['vid'] = array('#type' => 'value', '#value' => $filter->fid);
  }

  $form['basics'] = array(
    '#type' => 'fieldset',
    '#title' => t('Basics'),
  );
  $form['basics']['status'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enabled'),
    '#default_value' => $filter->status,
  );
  $form['basics']['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#default_value' => $filter->name,
  );

  $form['path'] = array(
    '#type' => 'fieldset',
    '#title' => 'Paths',
  );
  $form['path']['paths'] = array(
    '#type' => 'textarea',
    '#default_value' => $filter->paths,
  );
  $form['path']['paths']['#description'] = '<div>' . t('Enter one path per line.') . '</div>';
  $form['path']['paths']['#description'] .= '<div>' . t('Format: %format', array('%format' => t('Type[+Modifiers,...]:Pattern'))) . '</div>';
  $form['path']['paths']['#description'] .= '<div>' . t('Types:') . '</div>';
  $form['path']['paths']['#description'] .= '<ul>';
  $form['path']['paths']['#description'] .= '  <li>' . ACCESS_FILTER_PATH_TYPE_DRUPAL . ' = ' . t('Drupal path') . '</li>';
  $form['path']['paths']['#description'] .= '  <li>' . ACCESS_FILTER_PATH_TYPE_URI . ' = ' . t('Request uri (contains parameters)') . '</li>';
  $form['path']['paths']['#description'] .= '</ul>';
  $form['path']['paths']['#description'] .= '<div>' . t('Modifiers:') . '</div>';
  $form['path']['paths']['#description'] .= '<ul>';
  $form['path']['paths']['#description'] .= '  <li>' . ACCESS_FILTER_PATH_MODIFIER_REGEX . ' = ' . t('Use regex') . '</li>';
  $form['path']['paths']['#description'] .= '  <li>' . ACCESS_FILTER_PATH_MODIFIER_BLIND . ' = ' . t('Blind mode (disable looking up aliases)') . '</li>';
  $form['path']['paths']['#description'] .= '</ul>';
  $form['path']['paths']['#description'] .= '<div>' . t('Exapmles:') . '</div>';
  $form['path']['paths']['#description'] .= '<ul>';
  $form['path']['paths']['#description'] .= '  <li>' . ACCESS_FILTER_PATH_TYPE_DRUPAL . ':&lt;front&gt; <i>' . t('# Front page') . '</i></li>';
  $form['path']['paths']['#description'] .= '  <li>' . ACCESS_FILTER_PATH_TYPE_DRUPAL . ':&lt;admin&gt; <i>' . t('# Admin paths') . '</i></li>';
  $form['path']['paths']['#description'] .= '  <li>' . ACCESS_FILTER_PATH_TYPE_DRUPAL . ':admin/config/people/access_filter/*</li>';
  $form['path']['paths']['#description'] .= '  <li>' . ACCESS_FILTER_PATH_TYPE_DRUPAL . '+' . ACCESS_FILTER_PATH_MODIFIER_BLIND . ':node/3</li>';
  $form['path']['paths']['#description'] .= '  <li>' . ACCESS_FILTER_PATH_TYPE_DRUPAL . '+' . ACCESS_FILTER_PATH_MODIFIER_REGEX . ACCESS_FILTER_PATH_MODIFIER_BLIND . ':^node/[123]$</li>';
  $form['path']['paths']['#description'] .= '  <li>' . ACCESS_FILTER_PATH_TYPE_DRUPAL . '+' . ACCESS_FILTER_PATH_MODIFIER_REGEX . ':^admin\\/config\\/people\\/access_filter\\/[0-9]+\\/edit$</li>';
  $form['path']['paths']['#description'] .= '  <li>' . ACCESS_FILTER_PATH_TYPE_URI . ':admin/config/people/access_filter</li>';
  $form['path']['paths']['#description'] .= '  <li>' . ACCESS_FILTER_PATH_TYPE_URI . ':admin/config/people/access_filter?mode=sample&type=1</li>';
  $form['path']['paths']['#description'] .= '  <li>' . ACCESS_FILTER_PATH_TYPE_URI . '+' . ACCESS_FILTER_PATH_MODIFIER_REGEX . ':^admin\\/config\\/people\\/access_filter\\?mode=(ex1|ex2)</li>';
  $form['path']['paths']['#description'] .= '</ul>';

  $form['rule'] = array(
    '#type' => 'fieldset',
    '#title' => t('Rules'),
  );
  $form['rule']['rules'] = array(
    '#type' => 'textarea',
    '#default_value' => $filter->rules,
  );
  $form['rule']['rules']['#description'] = '<div>' . t('Enter one rule per line.') . '</div>';
  $form['rule']['rules']['#description'] .= '<div>' . t('Format: %format', array('%format' => t('Process type:IP address'))) . '</div>';
  $form['rule']['rules']['#description'] .= '<div>' . t('Process types:') . '</div>';
  $form['rule']['rules']['#description'] .= '<ul>';
  $form['rule']['rules']['#description'] .= '  <li>' . ACCESS_FILTER_PROCESS_TYPE_DENY . ' = ' . t('Deny') . '</li>';
  $form['rule']['rules']['#description'] .= '  <li>' . ACCESS_FILTER_PROCESS_TYPE_ALLOW . ' = ' . t('Allow') . '</li>';
  $form['rule']['rules']['#description'] .= '</ul>';
  $form['rule']['rules']['#description'] .= '<div>' . t("'*' is matches any IP addresses.") . '</div>';
  $form['rule']['rules']['#description'] .= '<div>' . t('Exapmles:') . '</div>';
  $form['rule']['rules']['#description'] .= '<div>' . t('Allow from only @allowed except @denied.', array('@allowed' => '192.168.0.0/24', '@denied' => '192.168.0.10-192.168.0.20')) . '</div>';
  $form['rule']['rules']['#description'] .= '<ul>';
  $form['rule']['rules']['#description'] .= '  <li>' . ACCESS_FILTER_PROCESS_TYPE_DENY . ':*</li>';
  $form['rule']['rules']['#description'] .= '  <li>' . ACCESS_FILTER_PROCESS_TYPE_ALLOW . ':192.168.0.0/24</li>';
  $form['rule']['rules']['#description'] .= '  <li>' . ACCESS_FILTER_PROCESS_TYPE_DENY . ':192.168.0.10-192.168.0.20</li>';
  $form['rule']['rules']['#description'] .= '</ul>';

  $form['deny_action_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('On deny'),
    '#tree' => TRUE,
  );
  $form['deny_action_settings']['type'] = array(
    '#type' => 'select',
    '#title' => t('Response'),
    '#options' => array(
      ACCESS_FILTER_DENY_ACTION_403 => t('Error') . ': 403 Access Forbidden',
      ACCESS_FILTER_DENY_ACTION_404 => t('Error') . ': 404 Not Found',
      ACCESS_FILTER_DENY_ACTION_301 => t('Redirect') . ': 301 Moved Permanently',
      ACCESS_FILTER_DENY_ACTION_302 => t('Redirect') . ': 302 Moved Temporarily',
    ),
    '#default_value' => $filter->deny_action_settings->type,
  );
  $form['deny_action_settings']['error_message'] = array(
    '#type' => 'text_format',
    '#title' => t('Error message'),
    '#default_value' => $filter->deny_action_settings->error_message['value'],
    '#format' =>  $filter->deny_action_settings->error_message['format'],
  );
  $form['deny_action_settings']['redirect_destination'] = array(
    '#type' => 'textfield',
    '#title' => t('Redirect URL'),
    '#default_value' => $filter->deny_action_settings->redirect_destination,
  );
  $form['deny_action_settings']['force_logout'] = array(
    '#type' => 'checkbox',
    '#title' => t('Force logout'),
    '#default_value' => $filter->deny_action_settings->force_logout,
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Save'));
  $form['actions']['back'] = array('#type' => 'submit', '#value' => t('Back'));
  $form['#validate'][] = 'access_filter_form_filter_validate';

  $form['test'] = array(
    '#type' => 'fieldset',
    '#title' => t('Test'),
    '#weight' => 1000,
  );
  $form['test']['test_result'] = array(
    '#type' => 'markup',
    '#markup' => '<div id="test-result"></div>',
  );
  $form['test']['test_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path/URL'),
    '#field_prefix' => 'http://' . $_SERVER['HTTP_HOST'] . '/',
    '#default_value' => current_path(),
  );
  $form['test']['test_ip'] = array(
    '#type' => 'textfield',
    '#title' => t('IP address'),
    '#size' => 15,
    '#default_value' => ip_address(),
  );
  $form['test']['submit'] = array(
    '#type' => 'button',
    '#value' => t('Test'),
    '#ajax' => array(
      'callback' => 'access_filter_form_filter_callback_test',
      'wrapper' => 'test-result',
    ),
  );

  $form['#attached']['js'] = array(
    drupal_get_path('module', 'access_filter') . '/access_filter.js',
  );

  return $form;
}

/**
 * Form validation handler for access_filter_edit_filter().
 *
 * @see access_filter_form_filter()
 * @see access_filter_form_filter_submit()
 */
function access_filter_form_filter_validate($form, &$form_state) {
  if ($form_state['triggering_element']['#value'] != t('Save')) {
    return;
  }

  if (strlen($form_state['values']['name']) > 255) {
    form_set_error('name', t(
      '%field field value must be shorter than %length characters.',
      array('%field' => $form['basics']['name']['#title'], '%length' => 255)
    ));
  }

  $type = $form_state['values']['deny_action_settings']['type'];
  $is_redirect = ($type == ACCESS_FILTER_DENY_ACTION_301 || $type == ACCESS_FILTER_DENY_ACTION_302);
  if ($is_redirect && !strlen($form_state['values']['deny_action_settings']['redirect_destination'])) {
    form_set_error('deny_action_settings][redirect_destination', t(
      '%field field is required.',
      array('%field' => $form['deny_action_settings']['redirect_destination']['#title'])
    ));
  }
}

/**
 * Form submission handler for access_filter_form_filter().
 *
 * @see access_filter_form_filter()
 * @see access_filter_form_filter_validate()
 */
function access_filter_form_filter_submit($form, &$form_state) {
  if ($form_state['triggering_element']['#value'] == t('Back')) {
    $form_state['redirect'] = 'admin/config/people/access_filter';
    return;
  }
  elseif ($form_state['triggering_element']['#value'] != t('Save')) {
    return;
  }

  $filter = access_filter_form_submit_build_filter($form_state);
  switch (access_filter_save($filter)) {
    case SAVED_NEW:
      drupal_set_message(t('Created new access filter %filter.', array('%filter' => $filter->name)));
      watchdog('access_filter', 'Created new access filter %filter.', array('%filter' => $filter->name), WATCHDOG_NOTICE, l(t('edit'), 'admin/config/people/access_filter/' . $filter->fid . '/edit'));

      break;

    case SAVED_UPDATED:
      drupal_set_message(t('Updated access filter %filter.', array('%filter' => $filter->name)));
      watchdog('access_filter', 'Updated access filter %filter.', array('%filter' => $filter->name), WATCHDOG_NOTICE, l(t('edit'), 'admin/config/people/access_filter/' . $filter->fid . '/edit'));

      break;
  }

  $form_state['redirect'] = 'admin/config/people/access_filter';
}

/**
 * Ajax callback function for access_filter_form_filter().
 * Test filter and show result.
 */
function access_filter_form_filter_callback_test($form, $form_state) {
  // Enables filter during test.
  $filter = access_filter_form_submit_build_filter($form_state);
  $filter->status = ACCESS_FILTER_STATUS_ENABLED;
  $filter->testing = TRUE;
  access_filter_parse_filter($filter);

  $allowed = access_filter_check_access($filter, $form_state['values']['test_path'], $form_state['values']['test_ip']);
  if ($allowed) {
    $message = t('Access to %path from %ip will be allowed.', array(
      '%path' => $form_state['values']['test_path'],
      '%ip' => $form_state['values']['test_ip'],
    ));
    $message_type = 'status';
  }
  else {
    $message = t('Access to %path from %ip will be denied.', array(
      '%path' => $form_state['values']['test_path'],
      '%ip' => $form_state['values']['test_ip'],
    ));
    $message_type = 'error';
  }
  return '<div id="test-result" class="messages ' . $message_type . '">' . $message . '</div>';
}

/**
 * Updates the form state's filter by processing this submission's values.
 *
 * @param array $form_state
 *   An array of form state.
 *
 * @return object
 *   An object of filter.
 */
function access_filter_form_submit_build_filter(&$form_state) {
  $filter = $form_state['filter'];
  if (!$filter) {
    $filter = new stdClass();
  }
  $filter->status = $form_state['values']['status'];
  $filter->name = $form_state['values']['name'];
  $filter->paths = access_filter_clean_list($form_state['values']['paths']);
  $filter->rules = access_filter_clean_list($form_state['values']['rules']);
  $filter->deny_action_settings = (object) $form_state['values']['deny_action_settings'];
  return $filter;
}

/**
 * Clean up list values to save.
 *
 * @param string $list
 *   A string of list values.
 *
 * @return string
 *   A string of cleaned list values.
 */
function access_filter_clean_list($list) {
  $list = str_replace(array("\r\n", "\r", "\n"), "\n", $list);
  $list_array = array();
  foreach (explode("\n", $list) as $item) {
    $item = preg_replace('/^[\s\t　]+/u', '', $item);
    $item = preg_replace('/[\s\t　]+$/u', '', $item);

    if (strlen($item) > 0) {
      $list_array[] = $item;
    }
  }
  return implode("\n", $list_array);
}

/**
 * Form builder to confirmation of delete filter.
 *
 * @ingroup forms
 * @see access_filter_form_confirm_delete()
 */
function access_filter_form_confirm_delete($form, &$form_state, $filter = NULL) {
  $form_state['filter'] = $filter;
  $form['#submit'] = array('access_filter_form_confirm_delete_submit');

  return confirm_form($form,
    t('Are you sure you want to delete the filter %title?',
    array('%title' => $filter->name)),
    'admin/config/people/access_filter',
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Form submission handler for access_filter_form_confirm_delete().
 *
 * @see access_filter_form_confirm_delete()
 */
function access_filter_form_confirm_delete_submit($form, &$form_state) {
  $filter = $form_state['filter'];
  db_delete('access_filter')->condition('fid', $filter->fid)->execute();

  drupal_set_message(t('Deleted access filter %filter.', array('%filter' => $filter->name)));
  watchdog('access_filter', 'Deleted access filter %filter.', array('%filter' => $filter->name), WATCHDOG_NOTICE, l(t('delete'), 'admin/config/people/access_filter/' . $filter->fid . '/delete'));

  $form_state['redirect'] = 'admin/config/people/access_filter';
}
