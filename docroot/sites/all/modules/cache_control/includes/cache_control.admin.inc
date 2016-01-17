<?php
/**
 * @file
 * Admin functions for cache_control
 *
 * TODO Rewrite the admin interface to follow good Drupal practices
 */

/**
 * Form builder for Cache Control main admin form
 *
 * @see cache_control_admin_submit()
 * @ingroup forms
 */
function cache_control_admin($form, &$form_state) {
  $form = array(
    '#cache' => TRUE,
    '#action' => '',
  );

  $form['fieldset_cache'] = array(
    '#type' => 'fieldset',
    '#title' => t('General settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['fieldset_cache']['cache_control_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Cache Control'),
    '#description' => t('To enable path-specific cache control on the site, check this box. Developers may prefer keeping this disabled while development is on-going.'),
    '#default_value' => variable_get('cache_control_enabled', FALSE),
  );

  // Get information about available storage classes
  $storage_info = module_invoke_all('cache_control_storage_info');
  $storage_classes = array();

  foreach ($storage_info as $storage) {
    if (!empty($storage['storage_class']) && !empty($storage['name'])) {
      $storage_classes[$storage['storage_class']] = $storage['name'];
    }
  }

  if (empty($storage_classes)) {
	   drupal_set_message(t('You need to enable at least one of the storage backend modules before you can use Cache Control.'), 'error');
  }

  $form['fieldset_cache']['cache_control_storage_class'] = array(
    '#type' => 'select',
    '#title' => t('Storage backend'),
    '#description' => t('Storage engine to store information about cacheable paths, page components etc.'),
    '#default_value' => variable_get('cache_control_storage_class', ''),
    '#options' => $storage_classes
  );

  $form['fieldset_cache']['cache_control_expires'] = array(
    '#type' => 'textfield',
    '#title' => t('Cache lifetime'),
    '#default_value' => variable_get('cache_control_expires', 5),
    '#size' => 5,
    '#description' => t('The maximum time clients are allowed to cache content from this site.'),
    '#field_suffix' => t('minute(s)'),
  );

  $form['fieldset_cache_misc'] = array(
    '#type' => 'fieldset',
    '#title' => t('Miscellaneous settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['fieldset_cache_misc']['cache_control_dev_mode'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable development mode'),
    '#description' => t('This makes <i>get_components()</i> wait for one second before returning the content, allowing you to see what your placeholders look like. A debug header is also added notifying why a page load was not cached. Further, all components processed by Cache Control get a noticeable ugly border on the page.'),
    '#default_value' => variable_get('cache_control_dev_mode', FALSE),
  );

  $form['fieldset_cache_misc']['cache_control_no_cache_content_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('No-cache content types'),
    '#description' => t('Disable Cache Control for the checked content types. Takes effect when node/% is being cached.'),
    '#default_value' => variable_get('cache_control_no_cache_content_types', array()),
    '#options' => node_type_get_names()
  );

  return system_settings_form($form);
}

/**
 * Form builder for making paths available for caching
 *
 * @see cache_control_available_paths_submit()
 * @ingroup forms
 */
function cache_control_available_paths() {
  $form = array();
  $paths = cache_control_get_no_cache_paths();

  $form['available_paths'] = array(
    '#type' => 'checkboxes',
    '#options' => $paths,
    '#title' => t('Paths not currently cached'),
    '#description' => t('For these paths, suitable HTTP headers are sent, disallowing clients to cache the reponses.'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Make selected paths cacheable',
  );

  return $form;
}


/**
 * Submit handler
 *
 * @see cache_control_available_paths()
 */
function cache_control_available_paths_submit($form, $form_state) {
  foreach ($form_state['values']['available_paths'] as $path => $add) {
    if ($add) {
      cache_control_set_cacheable_path($path);
    }
  }

  drupal_set_message(t('Changes saved'));
}


/**
 * Form builder for selecting roles for which Cache Control is disabled
 *
 * @see cache_control_roles_submit()
 * @ingroup forms
 */
function cache_control_roles() {
  $form = array();

  $roles = user_roles();
  $role_values = variable_get('cache_control_roles', array());
  $form['roles'] = array(
    '#type' => 'checkboxes',
    '#options' => $roles,
    '#default_value' => $role_values,
    '#title' => t('Disable Cache Control for the following roles'),
    '#description' => t('For these roles, pages are generated normally, not as the anonymous user.'),
  );

  $form['admin'] = array(
    '#type' => 'checkbox',
    '#default_value' => variable_get('cache_control_enable_for_admin_user', FALSE),
    '#title' => t('ENABLE Cache Control for the admin user'),
    '#description' => t('By default, Cache Control is disabled for the Admin user (uid 1). Check this if for some reason you want to enable Cache Control for this user.'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Save',
  );

  return $form;
}

/**
 * Submit handler
 *
 * @see cache_control_roles()
 */
function cache_control_roles_submit($form, $form_state) {
  $roles = array();
  foreach ($form_state['values']['roles'] as $role => $selected) {
    if ($selected) {
      $roles[] = $role;
    }
  }
  variable_set('cache_control_roles', $roles);

  variable_set('cache_control_enable_for_admin_user', $form_state['values']['admin']);

  drupal_set_message(t('Changes saved'));
}


/**
 * Gets menu router paths for which caching is currently allowed
 */
function cache_control_get_cacheable_paths() {
  return cache_control_get_storage()->getPaths();
}

/**
 * Gets menu router paths for which caching is not currently enabled
 */
function cache_control_get_no_cache_paths() {
  $paths = array();

  $not_in = array();
  $i = 0;
  foreach (cache_control_get_cacheable_paths() as $path) {
    $not_in[':path' . $i] = $path->path;
    $i++;
  }

  if (!empty($not_in)) {
    $not_in_sql = "path NOT IN (" . implode(',', array_keys($not_in)) . ") AND";
  }
  else {
    $not_in_sql = '';
  }

  $result = db_query("SELECT path FROM {menu_router} WHERE $not_in_sql path NOT LIKE 'admin%' ORDER BY path", $not_in);

  foreach ($result as $row) {
    $paths[$row->path] = $row->path;
  }

  return $paths;
}

/**
 * Enables caching for a menu router path
 */
function cache_control_set_cacheable_path($path) {
  cache_control_get_storage()->addPath($path, 'router', 0);
}

/**
 * Disables caching from a menu router path
 */
function cache_control_unset_cacheable_path($path) {
  cache_control_get_storage()->deletePath($path);
}

/**
 * Form builder for currently cached paths listing
 *
 * @see cache_control_define_max_age_paths_submit()
 * @ingroup forms
 */
function cache_control_define_max_age_paths() {
  $form = array();
  //get all paths info
  $paths = cache_control_get_cacheable_paths();

  foreach ($paths as $path) {
    $form['cacheable_path_' . base64_encode($path->path)] = array(
      '#type' => 'checkbox',
      '#title' => $path->path,
    );

    $form['cacheable_path_max_age_' . base64_encode($path->path)] = array(
      '#type' => 'textfield',
      '#value' => ($path->ttl ? $path->ttl : 0),
      '#size' => 5,
      '#required' => TRUE,
      '#description' => t('Define max age seconds for this cacheable path. Enter 0 to use default cache life time.'),
    );
  }

  $form['submit_remove'] = array(
    '#type' => 'submit',
    '#value' => t('Remove selected paths'),
  );
  $form['submit_save'] = array(
    '#type' => 'submit',
    '#value' => t('Save max age'),
  );

  return $form;
}

/**
 * Submit handler
 *
 * @see cache_control_define_max_age_paths()
 *
 */
function cache_control_define_max_age_paths_submit($form, $form_state) {
  $post_vals = $form_state['input'];
  if ($post_vals['op'] == 'Save max age') {
    $paths = cache_control_get_cacheable_paths();
    foreach ($paths as $path) {
      $max_age = intval($post_vals['cacheable_path_max_age_' . base64_encode($path->path)]);

       cache_control_get_storage()->setPathTTL($path->path, $max_age);
    }
    drupal_set_message(t('Changes saved'));
  }
  elseif ($post_vals['op'] == 'Remove selected paths') {
    $paths = cache_control_get_cacheable_paths(TRUE);
    foreach ($paths as $path) {
      $selected = $post_vals['cacheable_path_' . base64_encode($path->path)];
      if ($selected && $selected == 1) {
        cache_control_unset_cacheable_path($path->path);
      }
    }
    drupal_set_message(t('Changes saved'));
  }
}

