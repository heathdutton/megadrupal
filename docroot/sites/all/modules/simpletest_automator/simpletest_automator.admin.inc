<?php

/**
 * Implementation of hook_help().
 */
function simpletest_automator_help($section, $arg) {
  $output = '';
  switch ($section) {
    case 'admin/build/simpletest_automator':
      $output .= '<p>'. t('The SimpleTest Automator is designed to let you perform manual testing once, and then automate the same actions you performed automatically using the SimpleTest automated testing framework.') .'</p>';
  }
  return $output;
}

/**
 * Menu callback: list of all simpletests.
 */
function simpletest_automator_admin_overview() {
  $tests = db_query('SELECT name, said, description FROM {simpletest_automator}');
  $rows = array();
  while ($test = $tests->fetch()) {
    $rows[] = array(
      check_plain($test->name) .'<div class="description">' . check_plain($test->description) . '</div>',
      l(t('Edit'), 'admin/build/simpletest_automator/' . $test->said),
      l(t('Modules'), 'admin/build/simpletest_automator/' . $test->said . '/modules'),
      l(t('Permissions'), 'admin/build/simpletest_automator/' . $test->said . '/permissions'),
      l(t('Actions'), 'admin/build/simpletest_automator/' . $test->said . '/actions'),
      l(t('Export'), 'admin/build/simpletest_automator/' . $test->said . '/export'),
      l(t('Record'), 'admin/build/simpletest_automator/' . $test->said . '/record'),
      l(t('Delete'), 'admin/build/simpletest_automator/' . $test->said . '/delete'),
    );
  }
  if (empty($rows)) {
    $rows = array(array(array('data' => t('No tests found. You can <a href="!url">add a new one</a>.', array('!url' => url('admin/build/simpletest_automator/add'))), 'colspan' => 8)));
  }
  return theme('table', array('Name', array('data' => t('Operations'), 'colspan' => 7)), $rows);
}


function simpletest_automator_admin_import($form, &$form_state) {
  $form = array();
  $form['import'] = array(
    '#type' => 'textarea',
    '#title' => t('Test'),
    '#description' => t('Enter a previously exported ("For import") SimpleTest automator test, as the serialized output given.'),
    '#size' => 40,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Import'),
  );
  return $form;
}

function simpletest_automator_admin_import_submit($form, &$form_state) {
  eval(trim($form_state['values']['import']));
  $actions = $simpletest_automator->actions;
  $said = simpletest_automator_save($simpletest_automator);
  foreach ($actions as $action) {
    $action->said = $said;
    simpletest_automator_action_save($action);
  }
  drupal_set_message(t('Import successful.'));
  $form_state['redirect'] = 'admin/build/simpletest_automator';
}

/**
 * Menu callback: simpletest editing page.
 */
function simpletest_automator_admin_add($form, &$form_state) {
  if (!isset($form_state['storage']['simpletest_automator'])) {
    $form_state['storage']['step'] = 0;
  }

  if ($form_state['storage']['step'] == 0) {
    $form = simpletest_automator_admin_edit(array(), NULL);
  }
  elseif ($form_state['storage']['step'] == 1) {
    $form = simpletest_automator_admin_modules_form(array(), NULL, FALSE);
    $form['overview'] = array(
      '#type' => 'item',
      '#title' => t('Please specify which modules will be enabled in your test'),
      '#weight' => -1,
    );
  }
  else {
    $form = simpletest_automator_admin_permissions($form_state, NULL);
    $form['#theme'] = 'simpletest_automator_admin_permissions';
    $form['overview'] = array(
      '#type' => 'item',
      '#title' => t('Please specify which permissions the testing user will have'),
      '#weight' => -1,
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => ($form_state['storage']['step'] != 2 ? t('Next') : t('Finish')),
    '#weight' => 100,
  );

  return $form;
}

function simpletest_automator_admin_modules_form($form_state = array(), $simpletest_automator = NULL, $edit = TRUE) {
  if ($simpletest_automator) {
    $enabled_modules = $simpletest_automator->modules;
  }
  if (!isset($enabled_modules)) {
    $enabled_modules = array();
  }
  // Get current list of modules.
  $files = module_rebuild_cache();
  uasort($files, 'simpletest_automator_admin_modules_by_info_name');
  $dependencies = array();
  $modules = array();
  $form['modules'] = array('#tree' => TRUE);
  // The list of required modules.
  $modules_required = drupal_required_modules();
  // Iterate through each of the modules.
  foreach ($files as $filename => $module) {
    if (isset($module->info['hidden']) && $module->info['hidden'] == TRUE) {
      continue;
    }
    $extra = array();
    // If the module is requried, set it to be so.
    if (in_array($filename, $modules_required)) {
      $extra['required'] = TRUE;
    }
    $extra['enabled'] = in_array($filename, $enabled_modules);
    // If this module has dependencies, add them to the array.
    if (is_array($module->info['dependencies'])) {
      foreach ($module->info['dependencies'] as $dependency) {
        if (!isset($files[$dependency])) {
          $extra['dependencies'][$dependency] = drupal_ucfirst($dependency) . t(' (<span class="admin-missing">missing</span>)');
          $extra['disabled'] = TRUE;
        }
        elseif (!$files[$dependency]->status) {
          $extra['dependencies'][$dependency] = $files[$dependency]->info['name'] . t(' (<span class="admin-disabled">disabled</span>)');
        }
        else {
          $extra['dependencies'][$dependency] = $files[$dependency]->info['name'] . t(' (<span class="admin-enabled">enabled</span>)');
        }
      }
    }
    // Mark dependents disabled so user can not remove modules being depended on.
    $dependents = array();
    foreach ($module->info['dependents'] as $dependent) {
      if ($files[$dependent]->status == 1) {
        $extra['dependents'][] = $files[$dependent]->info['name'] . t(' (<span class="admin-enabled">enabled</span>)');
        $extra['disabled'] = TRUE;
      }
      else {
        $extra['dependents'][] = $files[$dependent]->info['name'] . t(' (<span class="admin-disabled">disabled</span>)');
      }
    }
    $form['modules'][$filename] = _simpletest_automator_admin_modules_build_row($module->info, $extra);
  }
  $form['modules'] += array(
    '#theme' => 'simpletest_automator_admin_modules',
    '#header' => array(
      array('data' => t('Enabled'), 'class' => 'checkbox'),
      t('Name'),
      t('Version'),
      t('Description'),
    ),
  );
  if ($edit) {
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Update'),
    );
  }
  return $form;
}

/**
 * Array sorting callback; sorts modules or themes by their name.
 */
function simpletest_automator_admin_modules_by_info_name($a, $b) {
  return strcasecmp($a->info['name'], $b->info['name']);
}

/**
 * Admin permissions page.
 */
function simpletest_automator_admin_permissions($form_state, $simpletest_automator) {
  $form = array();
  if (!isset($form_state['storage']['permissions'])) {
    global $db_prefix;
    $db_prefix_original = $db_prefix;
    $db_prefix = $simpletest_automator->db_prefix;
    module_list(TRUE, FALSE);

    foreach (module_implements('perm', FALSE, TRUE) as $module) {
      $form_state['storage']['permissions'][$module] = module_invoke($module, 'perm');
    }

    $db_prefix = $db_prefix_original;
    module_list(TRUE, FALSE);
    module_implements(NULL, FALSE, TRUE);
    node_get_types(NULL, NULL, TRUE);
  }
  if ($simpletest_automator) {
    $form_state['storage']['simpletest_automator'] = (array)$simpletest_automator;
  }

  // Render role/permission overview:
  $options = array();
  $hide_descriptions = !system_admin_compact_mode();
  foreach ($form_state['storage']['permissions'] as $module => $permissions) {
    foreach ($permissions as $permission => $permission_data) {
      $form['permissions'][$module][$permission]['name'] = array(
        '#type' => 'item',
        '#markup' => t($permission_data['title']),
        '#description' => $hide_descriptions ? $permission_data['description'] : NULL,
      );
      $form['permissions'][$module][$permission]['enable'] = array(
        '#type' => 'checkbox',
        '#parents' => array('permissions', $permission),
        '#default_value' => (isset($simpletest_automator->permissions) && in_array($permission, $simpletest_automator->permissions)),
      );
    }
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

function simpletest_automator_admin_permissions_submit($form, $form_state) {
  $form_state['storage']['simpletest_automator']['permissions'] = array_keys(array_filter($form_state['values']['permissions']));
  simpletest_automator_save((object) $form_state['storage']['simpletest_automator']);
  $form_state['redirect'] = 'admin/build/simpletest_automator' . $form_state['storage']['simpletest_automator']['said'] . '/permissions';
}

function _simpletest_automator_admin_modules_build_row($info, $extra) {
  // Add in the defaults.
  $extra += array(
    'required' => FALSE,
    'dependencies' => array(),
    'dependents' => array(),
    'disabled' => FALSE,
    'enabled' => FALSE,
    'help' => '',
  );
  $form = array(
    '#tree' => TRUE,
  );
  // Set the basic properties.
  $form['name'] = array(
    '#markup' => t($info['name']),
  );
  $form['description'] = array(
    '#markup' => t($info['description']),
  );
  $form['version'] = array(
    '#markup' => $info['version'],
  );
  $form['#dependencies'] = $extra['dependencies'];
  $form['#dependents'] = $extra['dependents'];

  // Check the compatibilities.
  $compatible = TRUE;
  $status_short = '';
  $status_long = '';

  // Check the core compatibility.
  if (!isset($info['core']) || $info['core'] != DRUPAL_CORE_COMPATIBILITY) {
    $compatible = FALSE;
    $status_short .= t('Incompatible with this version of Drupal core. ');
    $status_long .= t('This version is incompatible with the !core_version version of Drupal core. ', array('!core_version' => VERSION));
  }

  // Ensure this module is compatible with the currently installed version of PHP.
  if (version_compare(phpversion(), $info['php']) < 0) {
    $compatible = FALSE;
    $status_short .= t('Incompatible with this version of PHP');
    if (substr_count($info['php'], '.') < 2) {
      $php_required .= '.*';
    }
    $status_long .= t('This module requires PHP version @php_required and is incompatible with PHP version !php_version.', array('@php_required' => $php_required, '!php_version' => phpversion()));
  }

  // If this module is compatible, present a checkbox indicating
  // this module may be installed. Otherwise, show a big red X.
  if ($compatible) {
    $form['enable'] = array(
      '#type' => 'checkbox',
      '#title' => t('Enable'),
      '#required' => $extra['required'],
      '#default_value' => $extra['required'] || $extra['enabled'],
      '#disabled' => $extra['required'],
    );
  }
  else {
    $form['enable'] = array(
      '#markup' =>  theme('image', 'misc/watchdog-error.png', t('incompatible'), $status_short),
    );
    $form['description']['#value'] .= theme('simpletest_automator_admin_modules_incompatible', $status_long);
  }

  // Show a "more help" link for modules that have them.
  if ($extra['help']) {
    $form['help'] = array(
      '#markup' => $extra['help'],
    );
  }
  return $form;
}

function simpletest_automator_admin_add_submit($form, &$form_state) {
  if ($form_state['storage']['step'] == 0) {
    $form_state['storage']['step'] = 1;
    foreach (array('name', 'description', 'test_group', 'file', 'class', 'method') as $field) {
      $form_state['storage']['simpletest_automator'][$field] = $form_state['values'][$field];
    }
  }
  elseif ($form_state['storage']['step'] == 1) {
    $required_modules = drupal_required_modules();
    foreach ($form_state['values']['modules'] as $module => $enabled) {
      if (!in_array($module, $required_modules) && $enabled['enable']) {
        $form_state['storage']['simpletest_automator']['modules'][] = $module;
      }
    }
    $form_state['storage']['simpletest_automator']['modules'][] = 'simpletest_automator_proctor';
    $form_state['storage']['step'] = 2;

    global $db_prefix, $conf;

    // Store necessary current values before switching to prefixed database.
    $db_prefix_original = $db_prefix;
    $file_directory_path_original = file_directory_path();
    $clean_url_original = variable_get('clean_url', 0);

    // Generate temporary prefixed database to ensure that tests have a clean starting point.
    $db_prefix = Database::getConnection()->prefixTables('{st_automator' . mt_rand(1000, 1000000) . '}');

    include_once DRUPAL_ROOT . '/includes/install.inc';
    drupal_install_system();

    db_query('INSERT INTO {registry} SELECT * FROM ' . $db_prefix_original . 'registry');
    db_query('INSERT INTO {registry_file} SELECT * FROM ' . $db_prefix_original . 'registry_file');

    // Add the specified modules to the list of modules in the default profile.
    $args = func_get_args();
    $modules = array_unique(array_merge(drupal_get_profile_modules('default', 'en'), $form_state['storage']['simpletest_automator']['modules']));;
    drupal_install_modules($modules, TRUE);

    // Because the schema is static cached, we need to flush it between each
    // run. If we don't, then it will contain stale data for the previous run's
    // database prefix and all calls to it will fail.
    drupal_get_schema(NULL, TRUE);

    // Run default profile tasks.
    $task = 'profile';
    default_profile_tasks($task, '');

    // Rebuild caches.
    actions_synchronize();
    _drupal_flush_css_js();
    cache_clear_all('variables', 'cache');
    $conf = variable_init();

    // Restore necessary variables.
    variable_set('install_profile', 'default');
    variable_set('install_task', 'profile-finished');
    variable_set('clean_url', $clean_url_original);
    variable_set('site_mail', 'simpletest@example.com');

    // Use temporary files directory with the same prefix as database.
    variable_set('file_directory_path', $file_directory_path_original . '/' . $db_prefix);
    $directory = file_directory_path();
    // Create the files directory.
    file_check_directory($directory, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS);

    foreach (module_implements('perm', FALSE, TRUE) as $module) {
      $form_state['storage']['permissions'][$module] = module_invoke($module, 'perm');
    }

    $db_prefix = $db_prefix_original;
    module_list(TRUE, FALSE);
    module_implements(NULL, FALSE, TRUE);
  }
  else {
    $form_state['storage']['simpletest_automator']['permissions'] = array_keys(array_filter($form_state['values']['permissions']));
    simpletest_automator_save((object)$form_state['storage']['simpletest_automator']);
    drupal_set_message(t('Your test has been saved.'));
    $form_state['redirect'] = 'admin/build/simpletest_automator';
    unset($form_state['storage']);
  }
}

/**
 * Menu callback: display a confirm form for deleting a SimpleTest.
 */
function simpletest_automator_admin_delete(&$form_state, $simpletest_automator) {
  return confirm_form(array('said' => array('#type' => 'value', '#value' => $simpletest_automator->said)), t('Are you sure you want to delete %test?', array('%test' => $simpletest_automator->name)), 'admin/build/simpletest_automator/' . $simpletest_automator->said, t('Are you sure you want to delete %test? Deleting it will permanently delete all data associated with the test, in a non-recoverable manner.', array('%test' => $simpletest_automator->name)), t('Delete'));
}

function simpletest_automator_admin_delete_submit($form, &$form_state) {
  simpletest_automator_delete($form_state['values']['said']);
  $form_state['redirect'] = 'admin/build/simpletest_automator';
  drupal_set_message(t('Test deleted.'));
}

function simpletest_automator_admin_edit($form_state, $simpletest_automator) {
  $form_state['storage']['simpletest_automator'] = $simpletest_automator;
  $form = array();
  $form['name'] = array(
    '#title' => t('Name'),
    '#description' => t('The human-readable name of the test.'),
    '#attributes' => array('class' => 'simpletest-automator-name'),
  );
  $form['description'] = array(
    '#title' => t('Description'),
    '#description' => t('The human-readable description of the test.'),
  );
  $form['test_group'] = array(
    '#title' => t('Group'),
    '#description' => t('The human-readable group of the test.'),
    // TODO: Add auto-complete and make it hook into simpletest module if enabled.
  );
  $form['file'] = array(
    '#title' => t('File'),
    '#description' => t('The filename of the test.'),
    '#attributes' => array('class' => 'simpletest-automator-file simpletest-automator-php'),
  );
  $form['class'] = array(
    '#title' => t('Class'),
    '#description' => t('The class to use in the test case.'),
    '#attributes' => array('class' => 'simpletest-automator-class simpletest-automator-php'),
  );
  $form['method'] = array(
    '#title' => t('Method'),
    '#description' => t('The method to use in the test case.'),
    '#attributes' => array('class' => 'simpletest-automator-method simpletest-automator-php'),
  );
  foreach ($form as $key => $element) {
    if (isset($simpletest_automator->$key)) {
      $form[$key]['#default_value'] = $simpletest_automator->$key;
    }
    $form[$key]['#required'] = TRUE;
    $form[$key]['#type'] = 'textfield';
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  drupal_add_js(drupal_get_path('module', 'simpletest_automator') . '/simpletest_automator.admin.js');
  return $form;
}

function simpletest_automator_admin_edit_submit($form, &$form_state) {
  simpletest_automator_save((object)($form_state['values'] + (array) $form_state['storage']['simpletest_automator']));
  drupal_set_message(t('SimpleTest saved.'));
  unset($form_state['storage']);
}

/**
 * Menu callback for the actions list page.
 */
function simpletest_automator_admin_action_list(&$form_state, $simpletest_automator) {
  $form['#said'] = $simpletest_automator->said;
  $form['actions'] = array('#tree' => TRUE);
  foreach ($simpletest_automator->actions as $action) {
    $form['actions'][$action->aid] = array();
    $info = _simpletest_automator_action_info($action);
    $form['actions'][$action->aid]['label']['#markup'] = $info['label'];
    $form['actions'][$action->aid]['description']['#markup'] = $info['description'];
    $form['actions'][$action->aid]['edit']['#markup'] = l(t('Edit'), "admin/build/simpletest_automator/$simpletest_automator->said/actions/$action->aid/edit");
    $form['actions'][$action->aid]['delete']['#markup'] = l(t('Delete'), "admin/build/simpletest_automator/$simpletest_automator->said/actions/$action->aid/delete");
    if (count($simpletest_automator->actions) > 1) {
      $form['actions'][$action->aid]['weight'] = array(
        '#type' => 'weight',
        '#delta' => 20,
        '#default_value' => $action->weight,
        '#attributes' => array('class' => 'simpletest-automator-action-weight')
      );
    }
  }
  if (count($simpletest_automator->actions) > 1) {
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
    );
  }
  return $form;
}

/**
 * Submit handler for simpletest_automator_admin_action_list().
 */
function simpletest_automator_admin_action_list_submit($form, &$form_state) {
  $simpletest_automator = simpletest_automator_load($form['#said'], TRUE);
  foreach ($form_state['values']['actions'] as $aid => $data) {
    $simpletest_automator->actions[$aid]->weight = $data['weight'];
  }
  simpletest_automator_save($simpletest_automator);
}

function theme_simpletest_automator_admin_action_list($form) {
  $headers = array(t('Name'), array('data' => t('Operations'), 'colspan' => 2));
  if (count(element_children($form['actions'])) > 1) {
    $headers[] = t('Weight');
    drupal_add_tabledrag('simpletest-automator-actions', 'order', 'sibling', 'simpletest-automator-action-weight');
  }
  $rows = array();
  foreach (element_children($form['actions']) as $key) {
    $row = array();
    $row[] = drupal_render($form['actions'][$key]['label']) .'<br /><span class="description">'. drupal_render($form['actions'][$key]['description']) .'</span>';
    $row[] = drupal_render($form['actions'][$key]['edit']);
    $row[] = drupal_render($form['actions'][$key]['delete']);
    if (count(element_children($form['actions'])) > 1) {
      $row[] = drupal_render($form['actions'][$key]['weight']);
      $rows[] = array('class' => 'draggable', 'data' => $row);
    }
    else {
      $rows[] = $row;
    }
  }
  if (empty($rows)) {
    $rows[] = array(
      array(
        'data' => t('There are no actions yet associated with this SimpleTest automator instance. You can <a href="@url">add a new one</a>.', array('@url' => url('admin/build/simpletest_automator/' . $form['#said'] . '/actions/add'))),
        'colspan' => 4,
      ),
    );
  }
  unset($form['#theme']);
  return theme('table', $headers, $rows, array('id' => 'simpletest-automator-actions')) . drupal_render($form);
}

/**
 * FAPI calblack for the action add/edit page.
 */
function simpletest_automator_admin_action_edit($form_state, $simpletest_automator, $aid) {
  if ($aid) {
    if (!isset($simpletest_automator->actions[$aid])) {
      return drupal_not_found();
    }
    $action = $simpletest_automator->actions[$aid];
  }
  else {
    $action = (object) array('said' => $simpletest_automator->said, 'type' => '', 'parameters' => array(), 'weight' => 0);
  }
  $form = array(
    '#said' => $simpletest_automator->said,
    '#aid' => $aid,
    '#action_weight' => $action->weight,
  );
  // We might need textarea javascript later on. @todo - find better way.
  drupal_add_js('misc/textarea.js');
  $form['type'] = array(
    '#type' => 'select',
    '#title' => t('Type'),
    '#description' => $action->type ? t('This setting cannot be changed after the action has already been added. If you need to add an action of a different type, try adding a new one.') : t('Please choose what type of action you wish to add. Once you choose, more options may become available.'),
    '#options' => array_merge(array('' => t('<none>')), module_invoke_all('simpletest_automator_actions', $action, 'list')),
    '#default_value' => $action->type,
    '#disabled' => (bool)$action->type,
    '#ahah' => array(
      'path' => 'simpletest_automator/admin/js',
      'wrapper' => 'action-parameters',
    ),
    '#suffix' => ($action->type) ? NULL : '<div id="action-parameters">&nbsp;</div>',
    '#required' => TRUE,
  );
  $form['parameters'] = array('#tree' => TRUE);
  if ($action->type) {
    $form['parameters'] += array('#prefix' => '<div id="action-parameters">', '#suffix' => '</div>');
  }
  $form['parameters'] += module_invoke_all('simpletest_automator_actions', $action, 'form');
  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}

/**
 * Submit handler for simpletest_automator_admin_action_edit().
 */
function simpletest_automator_admin_action_edit_submit($form, &$form_state) {
  $action = (object)array(
    'aid' => $form['#aid'],
    'said' => $form['#said'],
    'type' => $form_state['values']['type'],
    'weight' => $form['#action_weight'],
    'parameters' => $form_state['values']['parameters'],
  );
  simpletest_automator_action_save($action);
  $form_state['redirect'] = 'admin/build/simpletest_automator/' . $form['#said'] . '/actions';
}

function simpletest_automator_admin_action_delete($form_state, $simpletest_automator, $aid) {
  if (!isset($simpletest_automator->actions[$aid])) {
    return drupal_not_found();
  }
  $info = _simpletest_automator_action_info($simpletest_automator->actions[$aid]);
  $form = array();
  $form['aid'] = array(
    '#type' => 'value',
    '#value' => $aid,
  );
  $form['said'] = array(
    '#type' => 'value',
    '#value' => $simpletest_automator->said,
  );
  return confirm_form($form, t('Are you sure you want to delete %action', array('%action' => $info['label'])), 'admin/build/simpletest_automator/'. $simpletest_automator->said .'/actions');
}

function simpletest_automator_admin_record($form_state, $simpletest_automator) {
  $form = array();

  $form['said'] = array(
    '#type' => 'value',
    '#value' => $simpletest_automator->said,
  );

  return confirm_form($form, t('Are you sure you want to enter record mode?'), 'admin/build/simpletest_automator', t('Once in record mode, you can exit by clicking the "Stop recording" button at the top of the page.'), t('Record'), t('Cancel'));
}

function simpletest_automator_admin_record_submit($form, &$form_state) {
  global $db_prefix;
  $simpletest_automator = simpletest_automator_load($form_state['values']['said'], TRUE);
  $db_prefix = $simpletest_automator->db_prefix;
  setcookie('simpletest_automator', $simpletest_automator->db_prefix, 0, base_path());
  // Create new role.
  db_query("INSERT INTO {role} (name) VALUES ('%s')", _simpletest_automator_random_name());
  $rid = db_last_insert_id('role', 'rid');
  foreach ($simpletest_automator->permissions as $permission) {
    db_query("INSERT INTO {role_permission} (rid, permission) VALUES (%d, '%s')", $rid, $permission);
  }
  $edit = array();
  $edit['name']   = _simpletest_automator_random_name();
  $edit['mail']   = $edit['name'] . '@example.com';
  $edit['roles']  = array($rid => $rid);
  $edit['pass']   = user_password();
  $edit['status'] = 1;
  $account = user_save('', $edit);
  drupal_function_exists('user_logout');
  $GLOBALS['user'] = $account;

  drupal_goto('<front>');
}

/**
 * Helper function for simpletest_automator_admin_record_submit().
 */
function _simpletest_automator_random_name($number = 4, $prefix = 'simpletest_') {
  $chars = 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_';
  for ($x = 0; $x < $number; $x++) {
    $prefix .= $chars{mt_rand(0, strlen($chars) - 1)};
    if ($x == 0) {
      $chars .= '0123456789';
    }
  }
  return $prefix;
}

function simpletest_automator_admin_export($simpletest_automator, $type = NULL) {
  if (empty($type)) {
    $items = array(
      l(t('File'), 'admin/build/simpletest_automator/' . $simpletest_automator->said . '/export/file') . '<div class="description">' . t('Select this option to export your test to a .test file.') . '</div>',
      l(t('For import'), 'admin/build/simpletest_automator/' . $simpletest_automator->said . '/export/import') . '<div class="description">' . t('Select this option to export your test in a format that can be re-imported in the "import" tab.') . '</div>',
    );
    return theme('item_list', $items);
  }
  else {
    switch ($type) {
      case 'file':
        return '<iframe src="'. url('simpletest_automator/export/' . $simpletest_automator->said) . '" width="1" height="1" style="border:0"></iframe>'. t('Your download should begin immediately. If it doesn\'t, click <a href="!url">here</a>.', array('!url' => url('simpletest_automator/export/' . $simpletest_automator->said)));

      case 'import':
        return drupal_get_form('simpletest_automator_export_php', $simpletest_automator);

      default:
        return drupal_not_found();

    }
  }
  return $form;
}

/**
 * Submission callback for simpletest_automator_admin_action_delete().
 */
function simpletest_automator_admin_action_delete_submit($form, &$form_state) {
  simpletest_automator_action_delete($form_state['values']['aid']);
  drupal_set_message(t('The action has been deleted.'));
  $form_state['redirect'] = 'admin/build/simpletest_automator/' . $form_state['values']['said'] . '/actions';
}

/**
 * AHAH callback upon the change of form type.
 */
function simpletest_automator_admin_actions_js() {
  // Add the new element to the stored form state. Without adding the element
  // to the form, Drupal is not aware of this new elements existence and will
  // not process it. We retreive the cached form, add the element, and resave.
  $form_build_id = $_POST['form_build_id'];
  $type = $_POST['type'];
  $form_state = array('submitted' => FALSE);
  $form = form_get_cache($form_build_id, $form_state);

  if ($form) {
    // Validate the type.
    if (isset($form['type']['#options'][$type])) {
      $action = (object) array('said' => $form['#said'], 'type' => $type, 'parameters' => array());
      $form['parameters'] = array('#tree' => TRUE);
      $form['parameters'] += module_invoke_all('simpletest_automator_actions', $action, 'form');
      form_set_cache($form_build_id, $form, $form_state);
      // Build and render the new elements, then return them in JSON format.
      $form_state = array();
      $form['#post'] = array();
      $form = form_builder($form['form_id']['#value'] , $form, $form_state);
      $output = drupal_render($form['parameters']);
      drupal_json(array('status' => TRUE, 'data' => $output));
    }
    else {
      drupal_json(array('status' => FALSE, 'data' => ''));
    }
  }
  else {
    drupal_json(array('status' => FALSE, 'data' => ''));
  }
}



function theme_simpletest_automator_admin_modules($form) {
  // Individual table rows.
  $rows = array();
  // Iterate through all the modules, which are
  // children of this fieldset.
  foreach (element_children($form) as $key) {
    // Stick it into $module for easier accessing.
    $module = $form[$key];
    $row = array();
    unset($module['enable']['#title']);
    $row[] = array('class' => 'checkbox', 'data' => drupal_render($module['enable']));
    $row[] = '<strong>'. drupal_render($module['name']) .'</strong>';
    $row[] = drupal_render($module['version']);
    $description = '';
    // Add the description, along with any dependencies.
    $description .= drupal_render($module['description']);
    if ($module['#dependencies']) {
     $description .= '<div class="admin-dependencies">' . t('Depends on: ') . implode(', ', $module['#dependencies']) . '</div>';
    }
    if ($module['#dependents']) {
     $description .= '<div class="admin-dependencies">' . t('Required by: ') . implode(', ', $module['#dependents']) . '</div>';
    }
    $row[] = array('data' => $description, 'class' => 'description');
    $rows[] = $row;
  }
  return theme('table', $form['#header'], $rows);
}

/**
 * Themes an incompatible message.
 *
 * @ingroup themeable
 * @param $message
 *   The form array representing the currently disabled modules.
 * @return
 *   An HTML string for the message.
 */
function theme_simpletest_automator_admin_modules_incompatible($message) {
  return '<div class="incompatible">'. $message .'</div>';
}

/**
 * Theme function.
 */
function theme_simpletest_automator_admin_permissions($form) {
  drupal_add_css(drupal_get_path('module', 'user') . '/user.css');
  $submit = $form['submit'];
  $permissions = $form['permissions'];
  unset($form['submit'], $form['permissions'], $form['#theme']);
  $output = drupal_render($form);
  $roles = user_roles();
  foreach (element_children($permissions) as $module) {
    $rows[] = array(array('data' => t('@module module', array('@module' => $module)), 'class' => 'module', 'id' => 'module-'. $module, 'colspan' => 2));
    foreach (element_children($permissions[$module]) as $permission) {
      $rows[] = array(
        array('data' => drupal_render($permissions[$module][$permission]['name']), 'class' => 'permission'),
        array('data' => drupal_render($permissions[$module][$permission]['enable']), 'class' => 'checkbox'),
      );
    }
  }
  $header[] = t('Permission');
  $header[] = array('data' => t('Test user'), 'class' => 'checkbox');
  $output .= theme('table', $header, $rows, array('id' => 'permissions'));
  $output .= drupal_render($submit);
  return $output;
}
