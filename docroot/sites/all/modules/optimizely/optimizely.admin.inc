<?php
/**
 * @file
 * Admin page callback for the Optimizely module.
 */

/**
 * Builds and returns the Optimizely Add/Update form.
 * 
 * If there are only three arguments in the path it builds the add form and
 * adds a record. Otherwise it builds the update form where the fourth
 * argument is the record ID (oid) in the optimizely table.
 *
 * @parm
 *   $form: Array of form elements
 *
 * @parm
 *   &$form_submit: Array of the stae of the form elements
 *
 * @parm
 *   $target_oid: When an update of a record, $target_oid will have a value
 *
 * @return
 *   $form: An array of the form elements
 */
function optimizely_add_update_form($form, &$form_submit, $target_oid = NULL) {
  
  $form = array();
  $form['#attached'] = array(
      'css' => array(
      'type' => 'file',
      'data' => drupal_get_path('module', 'optimizely') . '/css/optimizely.css',
    ),
  );

  $account_id = variable_get('optimizely_id', 0);

  if ($target_oid == NULL) {

    $form_action = 'Add';

    $intro_message = '';

    $form['optimizely_oid'] = array(
      '#type' => 'value',
      '#value' => NULL,
    );

    // Enable form element defaults - blank, unselected
    $enabled = FALSE;
    $project_code = '';
    $account_code = variable_get('optimizely_id', 0);

  }
  else {

    $form_action = 'Update';

    $query = db_select('optimizely', 'o', array('target' => 'slave'))
      ->fields('o')
      ->condition('o.oid', $target_oid, '=');
    $record = $query->execute()
      ->fetchObject();

    $form['optimizely_oid'] = array(
      '#type' => 'value',
      '#value' => $target_oid,
    );

    $form['optimizely_original_path'] = array(
      '#type' => 'value',
      '#value' => implode("\n", unserialize($record->path)),
    );

    $enabled = $record->enabled;
    $record->project_code == 0 ? $project_code = 'Undefined' : $project_code = $record->project_code;
    $account_code = variable_get('optimizely_id', 0);

  }

  // If we are updating the default record, make the form element inaccessible
  $form['optimizely_project_title'] = array(
    '#type' => 'textfield',
    '#disabled' => $target_oid == 1 ? TRUE : FALSE,
    '#title' => t('Project Title'),
    '#default_value' => $target_oid ? check_plain($record->project_title) : '',
    '#description' => check_plain($target_oid) == 1 ? t('Default project, this field can not be changed.') : t('Descriptive name for the project entry.'),
    '#size' => 60,
    '#maxlength' => 256,
    '#required' => TRUE,
    '#weight' => 10,
  );

  $form['optimizely_project_code'] = array(
    '#type' => 'textfield',
    '#disabled' => $target_oid == 1 ? TRUE : FALSE,
    '#title' => t('Optimizely Project Code'),
    '#default_value' => check_plain($project_code),
    '#description' => ($account_code == 0) ?
      t('The Optimizely account value has not been set in the <a href="/admin/config/system/optimizely/settings">Account Info</a> settings form. The Optimizely account value is used as the project ID for this "default" project entry.') :
      t('The Optimizely javascript file name used in the snippet as provided by the Optimizely website for the project.'),
    '#size' => 30,
    '#maxlength' => 100,
    '#required' => TRUE,
    '#weight' => 20,
  );

  $form['optimizely_path'] = array(
    '#type' => 'textarea',
    '#title' => t('Set Path Where Optimizely Code Snippet Appears'),
    '#default_value' => $target_oid ? implode("\n", unserialize($record->path)) : '',
    '#description' => t('Enter the path where you want to insert the Optimizely Snippet.
      For Example: "/clubs/*" causes the snippet to appear on all pages below "/clubs" in the URL but not
      on the actual "/clubs" page itself.'),
    '#cols' => 100,
    '#rows' => 6,
    '#resizable' => FALSE,
    '#required' => FALSE,
    '#weight' => 40,
  );

  $form['optimizely_enabled'] = array(
    '#type' => 'radios',
    '#title' => t('Enable/Disable Project'),
    '#default_value' => $target_oid ? $record->enabled : 0,
    '#options' => array(
      1 => 'Enable project',
      0 => 'Disable project',
    ),
    '#weight' => 25,
    '#attributes' => $enabled ? array('class' => array('enabled')) : array('class' => array('disabled')),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => $form_action,
    '#weight' => 100,
  );
  
  $form['cancel'] = array(
    '#markup' => l(t('Cancel'), 'admin/config/system/optimizely'),
    '#weight' => 101,
  );

  return $form;

}

/*
 * Pass through function to allow parameters to a form via a hook_menu call
 *
 * @return
 *   Call to drupal_get_form for the optimizely_add_update_form form that
 *   includes the additional parameter $target_project
 */
function optimizely_add_update_update($target_project) {  
  return drupal_get_form('optimizely_add_update_form', $target_project);
}

/**
 * Validate form submissions from optimizely_add_update_form().
 *
 * Check to make sute the project code is unique except for the default
 * entry which uses the account ID but should support an additional entry
 * to allow for custom settings.
 */
function optimizely_add_update_form_validate($form, &$form_state) {
  
  // Watch for "Undefined" value in Project Code, Account ID needed in Settings page
  if ($form_state['values']['optimizely_project_code'] == "Undefined") {
    form_set_error('optimizely_project_code', t('The Optimizely Account ID must be set in the <a href="/admin/config/system/optimizely/settings">Account Info</a> page. The account ID is used as the default Optimizely Project Code.'));
  } // Validate that the project code entered is a number
  elseif (!ctype_digit($form_state['values']['optimizely_project_code'])) {
    form_set_error('optimizely_project_code', t('The project code !code must only contain digits.', array('!code' => $form_state['values']['optimizely_project_code'])));
  }
  elseif ($form_state['values']['op'] == 'Add') {
    
    // Confirm project_code is unique or the entered project code is also the account ID - SELECT the project title in prep for related form error message
    $query = db_query('SELECT project_title FROM {optimizely} WHERE
      project_code = :project_code ORDER BY oid DESC', array(':project_code' => $form_state['values']['optimizely_project_code']));
    $query_count = $query->rowCount();
   
    // Flag submission if existing entry is found with the same project code value AND it's not an SINGLE entry to replace the "default" entry.
    if ($query_count > 0 || ($form_state['values']['optimizely_project_code'] != variable_get('optimizely_id', FALSE) && $query_count >= 2)) {
      
      // Get the title of the project that already had the propject code
      $found_entry_title = $query->fetchField();
      
      // Flag the project code form field
      form_set_error('optimizely_project_code', t('The project code (!project_code) already has an entry in the "!found_entry_title" project.', array('!project_code' => $form_state['values']['optimizely_project_code'], '!found_entry_title' => $found_entry_title)));
      
    }
    
  }
  
  // Skip if disabled entry
  if ($form_state['values']['optimizely_enabled']) {
    
    // Confirm that the project paths point to valid site URLs
    $target_paths = preg_split('/[\r\n]+/', $form_state['values']['optimizely_path'], -1, PREG_SPLIT_NO_EMPTY);
    $valid_path = _optimizely_valid_paths($target_paths);
    if (!is_bool($valid_path)) {
      $valid_path = htmlentities($valid_path);
      form_set_error('optimizely_path', t('The project path "!project_path" is not a valid path. The path or alias could not be resolved as a valid URL that will result in content on the site.', array('!project_path' => $valid_path)));
    }
    
    // There must be only one Optimizely javascript call on a page. Check paths to ensure there are no duplicates  
    // http://support.optimizely.com/customer/portal/questions/893051-multiple-code-snippets-on-same-page-ok-
    list($error_title, $error_path) = _optimizely_unique_paths($target_paths, $form_state['values']['optimizely_oid']);
  
    if (!is_bool($error_title)) {
      form_set_error('optimizely_path', t('The path "!error_path" will result in a duplicate entry based on the other project path settings. Optimizely does not allow more than one project to be run on a page.', array('!error_path' => $error_path)));
    }
  
  }
  
}

/**
 * Process form submissions from optimizely_add_update_form().
 *
 * Either "Add"s or "Update"s a record from the optimizely_add_update_form() form.
 */
function optimizely_add_update_form_submit($form, &$form_state) {

  // Catch form submitted values and prep for processing
  $oid = $form_state['values']['optimizely_oid'];

  $project_title = check_plain($form_state['values']['optimizely_project_title']);
  $project_code = check_plain($form_state['values']['optimizely_project_code']);

  // @totdo - Add support for "<front>" to allow use of check_plain() on ['optimizely_path']
  $path_array = preg_split('/[\r\n]+/', $form_state['values']['optimizely_path'], -1, PREG_SPLIT_NO_EMPTY);

  $enabled = check_plain($form_state['values']['optimizely_enabled']);

  // Process add or edit submission
  // No ID value included in submission - add new entry
  if (!isset($oid))  {

    db_insert('optimizely')
      ->fields(array(
        'project_title' => $project_title,
        'path' => serialize($path_array),
        'project_code' => $project_code,
        'enabled' => $enabled,
      ))
      ->execute();

    drupal_set_message(t('The project entry has been created.'), 'status');

    // Rebuild the provided paths to ensure Optimizely javascript is now included on paths
    if ($enabled) {
      optimizely_refresh_cache($path_array);
    }

  } // $oid is set, update exsisting entry
  else {

    db_update('optimizely')
      ->fields(array(
        'project_title' => $project_title,
        'path' => serialize($path_array),
        'project_code' => $project_code,
        'enabled' => $enabled,
      ))
      ->condition('oid', $oid)
      ->execute();

    drupal_set_message(t('The project entry has been updated.'), 'status');

    // Path originally set for project - to be compaired to the updated value to determine what cache paths needs to be refreshed
    $original_path_array = preg_split('/[\r\n]+/', $form_state['values']['optimizely_original_path'], -1, PREG_SPLIT_NO_EMPTY);

    optimizely_refresh_cache($path_array, $original_path_array);

  }

  // Return to project listing page
  drupal_goto('admin/config/system/optimizely');

}

/**
 * Menu callback. Enter the Optimizely account details.
 */
function optimizely_account_settings_form($form_state) {
  
  $form = array();
  $form['#attached'] = array('css' => array
    (
      'type' => 'file',
      'data' => drupal_get_path('module', 'optimizely') . '/css/optimizely.css',
    ),
  );

  $form['optimizely_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Optimizely ID Number'),
    '#default_value' => variable_get('optimizely_id', ''),
    '#description' => t('Your Optimizely account ID. This is the number after "/js/" in the Optimizely Tracking Code found in your account on the Optimizely website.'),
    '#size' => 60,
    '#maxlength' => 256,
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );

  return $form;

}

/**
 * Validation for optimizely_account_settings_form form
 */
function optimizely_account_settings_form_validate($form, &$form_state) {
  if (!preg_match('/^\d+$/', $form_state['values']['optimizely_id'])) {
    form_set_error('optimizely_id', t('Your Optimizely ID should be numeric.'));
  }
}

/**
 * Process submisison from optimizely_setup_account_settings_form form. This includes saving the
 * entered Optimizely account ID to the varable database table, updating the default optimizely (oid)
 * record with the project ID which is also the account ID.
 */
function optimizely_account_settings_form_submit($form, &$form_state) {

  // Write the variable table
  variable_set('optimizely_id', $form_state['values']['optimizely_id']);

  // Update the default project / experiement entry with the account ID value
  db_update('optimizely')
    ->fields(array(
        'project_code' => $form_state['values']['optimizely_id'],
      ))
    ->condition('oid', '1')
    ->execute();

  // Inform the administrator that the default project / experiement entry is ready to be enabled.
  drupal_set_message(t('The default project entry is now ready to be enabled. This will apply the default Optimizely project tests site wide.'), 'status');

  // Redirect back
  $form_state['redirect'] = 'admin/config/system/optimizely';

}

/**
 * Menu callback. Displays a list of Optimizely projects as records in the Optimizely database table.
 */
function optimizely_project_list_form() {

  $form = array();

  // Load css and js files specific to optimizely admin pages
  $form['#attached']['css'] = array(
    drupal_get_path('module', 'optimizely') . '/css/optimizely.css',
  );

  $form['#attached']['js'] = array(
    drupal_get_path('module', 'optimizely') . '/js/optimizely-admin.js',
  );

  $prefix  = '<ul class="admin-links">';
  $prefix .= '  <li>' . l(t('Add Project Entry'), 'admin/config/system/optimizely/add_update') . '</li>';
  $prefix .= '</ul>';

  $form['projects'] = array(
    '#prefix' => $prefix . '<div id="optimizely-project-listing">',
    '#suffix' => '</div>',
    '#tree' => TRUE,
    '#theme' => 'optimizely_projects_table'
  );

  // Lookup account ID setting to trigger "nag message".
  $account_id = variable_get('optimizely_id', 0);

  // Begin building the query.
  $query = db_select('optimizely', 'o', array('target' => 'slave'))
    ->orderBy('oid')
    ->fields('o');
  $result = $query->execute();

  // Build each row of the table
  foreach ($result as $project_count => $row) {

    // Listing of target paths for the project entry
    $paths = unserialize($row->path);

    $project_paths = '<ul>';
    foreach ($paths as $path) {
      // Deal with "<front>" as one of the paths
      if ($path == '<front>') {
        $front_path = variable_get('site_frontpage');
        $front_path .= ' <-> ' . drupal_lookup_path('alias', $front_path);
        $path = htmlspecialchars('<front>') . ' (' . $front_path . ')';
      }
      $project_paths .= '<li>' . $path . '</li>';
    }
    $project_paths .= '</ul>';

    // Build Edit / Delete links
    if ($row->oid != 1) {
      $edit_link = l(t('Update'), 'admin/config/system/optimizely/add_update/' . $row->oid);
      $delete_link = ' / ' . l(t('Delete'), 'admin/config/system/optimizely/delete/' . $row->oid);
      $default_entry_class = array('');
    }
    else {
      $edit_link = l(t('Update'), 'admin/config/system/optimizely/add_update/' . $row->oid);
      $delete_link = ' / ' . 'Default Entry';
      $default_entry_class = array('default-entry');
    }

    // Build form elements in cluding enable checkbox and data columns
    $form['projects'][$project_count]['enable'] = array(
      '#type' => 'checkbox',
      '#attributes' => array(
        'id' => 'project-enable-' . $row->oid,
        'name' => 'project-' . $row->oid
      ),
      '#default_value' => $row->enabled,
      '#extra_data' => array('field_name' => 'project_enabled'),
      '#suffix' => '<div class="status-container status-' . $row->oid . '"></div>'
    );

    if ($row->enabled) {
      $form['projects'][$project_count]['enable']['#attributes']['checked'] = 'checked';
    }

    $form['projects'][$project_count]['#project_title'] = $row->project_title;
    $form['projects'][$project_count]['#admin_links'] = $edit_link . $delete_link;
    $form['projects'][$project_count]['#paths'] = $project_paths;

    if ($account_id == 0 && $row->oid == 1) {
      $project_code = t('Set Optimizely ID in <strong>') .
        l(t('Account Info'), 'admin/config/system/optimizely/settings') .
        t('</strong> page to enable default project site wide.');
    }
    else {
      $project_code = $row->project_code;
    }
    $form['projects'][$project_count]['#project_code'] = $project_code;
    $form['projects'][$project_count]['#oid'] = $row->oid;

  }

return $form;
  
}


/*
 * Theme function to define the output of the project listing in a table format within a form.
 * The form is need to support the enable column which is a checkbox form element. 
 */
function theme_optimizely_projects_table($vars) {

  $element = $vars['element'];

  $rows = array();
  foreach (element_children($element) as $key) {

    $rows[] = array(
      'class' => array(
        'project-row-' . $element[$key]['#project_code']
      ),
      'id' => array(
        'project-' . $element[$key]['#oid']
      ),
      'data' => array(
        array(
          'class' => $element[$key]['enable']['#value'] ? 'enable-column enabled' : 'enable-column disabled',
          'data' => render($element[$key]['enable']),
        ),
        array(
          'class' => $element[$key]['enable']['#value'] ? 'project-title-column enabled' : 'project-title-column disabled',
          'data' => render($element[$key]['#project_title']),
        ),
        array(
          'class' => $element[$key]['enable']['#value'] ? 'admin-links-column enabled' : 'admin-links-column disabled',
          'data' => render($element[$key]['#admin_links']),
        ),
        array(
          'class' => $element[$key]['enable']['#value'] ? 'paths-column enabled' : 'paths-column disabled',
          'data' => render($element[$key]['#paths']),
        ),
        array(
          'class' => $element[$key]['enable']['#value'] ? 'project-code-column enabled' : 'project-code-column disabled',
          'data' => render($element[$key]['#project_code']),
        ),
      ),
    );

  }

  $header = array(t('Enabled'), t('Project Title'), t('Update / Delete'), t('Paths'), t('Project Code'));

  return theme('table', array('header' => $header, 'rows' => $rows));
}

/**
 * Menu callback. Get confirmation to delete a record from the Optimizely table.
 */
function optimizely_delete_page($form, &$form_state, $vars = NULL) {
  // Prevent deletetion of default project
  if ($form_state['build_info']['args'][0] != 1) {
    return drupal_build_form('optimizely_delete_page_confirm', $form_state);
  }
  else {
    drupal_set_message('Default project can not be deleted.', 'error');
  }
}

/**
 * Build a confirm form for deletion of record in Optimizely table.
 */
function optimizely_delete_page_confirm($form, &$form_state, $vars = NULL) {

  $form['oid'] = array(
    '#type' => 'value',
    '#value' => $vars,
  );

  $heading = t('Delete');
  $cancel_path = array('path' => 'admin/config/system/optimizely');
  $caption  = '<p>' . t('Are you sure you want to delete this configuration?') . '<p>' .
              '<p>' . t('This action cannot be undone.') . '</p>';
  $yes = t('Delete');
  $no = t('Cancel');

  return confirm_form($form, $heading, $cancel_path, $caption, $yes, $no);

}

/**
 * Submit function for the confirm deletion form. Delete the entry in the
 * database and return the user to the Project listing page.
 */
function optimizely_delete_page_confirm_submit($form, &$form_state) {  

  // target $oid
  $oid = $form_state['values']['oid'];

  // Lookup entry details before delete
  $query = db_select('optimizely', 'o', array('target' => 'slave'))
    ->fields('o', array('path', 'enabled'))
    ->condition('o.oid', $oid, '=');
  $record = $query->execute()
    ->fetchObject();

  // Delete entry in database based on the target $oid
  $query = db_delete('optimizely')
    ->condition('oid', $oid);
  $query->execute();

  // Only clear page cache for entries that are active when deleted
  if ($record->enabled) {

    // Always serialized when saved 
    $path_array = unserialize($record->path);
    optimizely_refresh_cache($path_array);

  }

  // Inform the user of the entry being deleted and return them to the
  // listing page.
  drupal_set_message(t('The project entry has been deleted.'), 'status');
  drupal_goto('/admin/config/system/optimizely');

}

/**
 * AJAX callback for click event on project enable checkbox.
 *
 * @return
 *   json response details for AJAX call. 
 */
function optimizely_ajax_enable() {

  // Default
  $unique_path = FALSE;
  $default_project = FALSE;
  $message = '';

  // Retrieve the json POST values
  $target_oid = $_POST['target_oid'];
  $target_enable = $_POST['target_enable'];

  // Only check path values if project is being enabled,
  // Project is currently disabled (FALSE) and is now being enabled (TRUE)
  if ($target_enable == TRUE) {

    // Lookup the current project settings
    $query = db_select('optimizely', 'o', array('target' => 'slave'))
      ->fields('o', array('path', 'project_code'))
      ->condition('o.oid', $target_oid, '=');
    $result = $query->execute()->fetchObject();

    // Prevent the Default project from being enabled when the project code is not set
    if (!($target_oid == 1 && $result->project_code == 0)) {

      // Check that the paths are valid for the newly enabled project
      $target_paths = unserialize($result->path);
      $valid_paths = _optimizely_valid_paths($target_paths);

      // Check to see if the enable project has path entries that will result in
      // duplicates with other enable projects
      if ($valid_paths === TRUE)  {
        list($unique_path, $target_path) = _optimizely_unique_paths($target_paths, $target_oid);
        if ($unique_path !== TRUE) {
         $message =  t('Project was not enabled due to path setting resulting in duplicate path entries between enabled projects.'); 
        }
      }
      else {
        $message = t('Project was not enabled due to path setting: ' . $valid_paths . ' resulting in an invalid path.');
      }

    }
    else {
      $default_project = TRUE;
      $message = t('Default project not enabled. Enter Optimizely ID in Account Info page.');
    }

  }

  // The newly enabled project has unique paths OR the target project is
  // currently enabled (TRUE) and will now be disbaled
  if (($target_enable == FALSE || $unique_path === TRUE) && ($default_project == FALSE))  {

    // Toggle $target_enable
    $target_enable ? $target_enable = 1 : $target_enable = 0;

    // Update database with new enable setting for project entry
    $results = db_update('optimizely')
      ->fields(array(
        'enabled' => (int) $target_enable,
      ))
      ->condition('oid', $target_oid)
      ->execute();
      
    // Refresh cache on project paths, this includes both enable and disabled
    // projects as there will be a need to clear the js calls in both cases
    $query = db_select('optimizely', 'o', array('target' => 'slave'))
      ->fields('o', array('path'))
      ->condition('o.oid', $target_oid, '=');
    $result = $query->execute()->fetchObject();
    $target_paths = unserialize($result->path);
    optimizely_refresh_cache($target_paths);

    // Tell AJAX request of status to trigger jquery
    drupal_json_output(array('status' => 'updated', 'oid' => $target_oid, 'target_enable' => $target_enable, 'message' => $message));

  }
  else {
    drupal_json_output(array('status' => 'rejected', 'oid' => $target_oid, 'issue_path' => $target_path, 'message' => $message));
  }

}

/**
 * optimizely_refresh_cache()
 *
 * @parm
 *   $path_array - An array of the target paths entries that the cache needs to
 *   be cleared. Each entry can also contain wildcards /* or variables "<front>".
 */
function optimizely_refresh_cache($path_array, $original_path_array = NULL) {

  // Determine protocol
  $protocol = (isset($_SERVER['HTTPS']) && $_SERVER['HTTPS'] == 'on') ? 'https' : 'http';
  $cid_base = $protocol . '://' . $_SERVER['HTTP_HOST'] . '/';
  
  // If update of project that includes changes to the path, clear cache on all
  // paths to add/remove Optimizely javascript call
  if (isset($original_path_array)) {
    $path_array = array_merge($path_array, $original_path_array);
  }

  // Loop through every path value
  foreach ($path_array as $path_count => $path) {

    $recursive = NULL;

    // Apply to all paths when there's a '*' path entry (default project entry
    // for example) or it's an exclude path entry (don't even try to figure out
    // the paths, just flush all page cache
    if (strpos($path, '*') !== 0) {

      if (strpos($path, '<front>') === 0) {
        $cid = $cid_base . '/' . variable_get('site_frontpage', 'node');
        $recursive = FALSE;
      }
      elseif (strpos($path, '/*') > 0)  {
        $cid = $cid_base . substr($path, 0, strlen($path) - 2);
        $recursive = TRUE;
      }
      else {
        $cid = $cid_base . $path;
        $recursive = FALSE;
      }

      cache_clear_all($cid, 'cache_page', $recursive);

    }
    else {
      cache_clear_all('*', 'cache_page', TRUE);
      break;
    }

  }
  
  // Varnish
  if (module_exists('varnish')) {
    varnish_expire_cache($path_array);
    drupal_set_message(t('Successfully purged cached page from Varnish.'));
  }

  drupal_set_message(t('Page cache has been cleared based on the project path settings.'), 'status');

}

/*
 * Compare target path against the project paths to conform they're unique
 *
 * @parm
 *   $target_paths - the paths entered for a new project entry OR
 *   the paths of an exsisting project entry that has been enabled.
 * @parm
 *   $target_paths = NULL : the oid of the project entry that has been enabled
 *
 * @return
 *   $target_path: the path that is a duplicate that must be addressed to
 *   enable or create the new project entry or TRUE if unique paths. 
 */
function _optimizely_unique_paths($target_paths, $target_oid = NULL) {

  // Look up alternative paths
  $target_paths = _optimizely_collect_alias($target_paths);

    // Look for duplicate paths in submitted $target_paths
  $duplicate_target_path = _optimizely_duplicate_check($target_paths);

  // Look for duplicate paths within target paths
  if (!$duplicate_target_path) {

    // Collect all of the exsisting project paths that are enabled,
    $query = db_select('optimizely', 'o', array('target' => 'slave'))
      ->fields('o', array('oid', 'project_title', 'path'))
      ->condition('o.enabled', 1, '=');

    // Add target_oid to query when it's an update, $target_oid is will be defined
    if ($target_oid != NULL) {
      $query = $query->condition('o.oid', $target_oid, '<>');
    }

    $projects = $query->execute();

    // No other enabled projects
    if ($query->countQuery()->execute()->fetchField() == 0) {
      return array(TRUE, NULL);
    }

    $all_project_paths = array();

    // Build array of all the project entry paths
    foreach ($projects as $project) {

      // Collect all of the path values and merge into collective array
      $project_paths = unserialize($project->path);
      $all_project_paths = array_merge($all_project_paths, $project_paths);

    }

    // Add any additional aliases to catch all match possiblities
    $all_project_paths = _optimizely_collect_alias($all_project_paths);

    // Convert array into string for drupal_match_path()
    $all_project_paths_string = implode("\n", $all_project_paths);

    // Check all of the paths for all of the active project entries to make sure
    // the paths are unique
    foreach ($target_paths as $target_path) {

      // "*" found in path
      if (strpos($target_path, '*') !== FALSE) {

        // Look for wild card match if not site wide
        if (strpos($target_path, '*') !== 0) {

          $target_path = substr($target_path, 0, -2);

          // Look for duplicate path due to wild card
          foreach ($all_project_paths as $all_project_path) {

            if (strpos($all_project_path, $target_path) === 0 && $all_project_path != $target_path) {
              return array($project->project_title, $target_path);
            }

          }

        } // If site wide wild card then it must be the only enabled path to be unique
        elseif (strpos($target_path, '*') === 0 && (count($target_paths) > 1 || count($all_project_paths) > 0)) {
          return array($project->project_title, $target_path);
        }

        // Look for site wide wild card in target project paths
        if (in_array('*', $all_project_paths)) {
          return array($project->project_title, $target_path);
        }

      } // Parameters found, collect base path for compairson to the other project path entries
      elseif (strpos($target_path, '?') !== FALSE) {
        $target_path = substr($target_path, 0, strpos($target_path, '?'));
      }

      // Look for duplicates 
      if (drupal_match_path($target_path, $all_project_paths_string)) {
          return array($project->project_title, $target_path);      
      }

    }

    return array(TRUE, NULL);

  }
  else {
    return array(NULL, $duplicate_target_path);
  }

}

/*
 * Compare paths within passed array to ensure each item resolves to a unique entry
 *
 * @parm
 *   $paths - a set of paths to be reviewed for uniqueness
 *
 * @return
 *   FALSE if no duplicates found otherwaise the dusplicate path is returned. 
 */
function _optimizely_duplicate_check($paths) {

  $unreviewed_paths = $paths;

  // Check all of the paths 
  foreach ($paths as $path) {

    // Remove path that's being processed from  the front of the list
    array_shift($unreviewed_paths);

    // "*" found in path
    if (strpos($path, '*') !== FALSE) {

      // Look for wild card match that's not site wide (posiiton not zero (0))
      if (strpos($path, '*') !== 0) {

        $path = substr($path, 0, -2);

        foreach ($unreviewed_paths as $unreviewed_path) {
          if (strpos($unreviewed_path, $path) !== FALSE) {
            return $path . '/*';
          }
        }

      } // If site wide wild card then it must be the only path in path set
      elseif (strpos($path, '*') === 0 && count($paths) > 1) {
        return $path;
      }

    }
    elseif (in_array($path, $unreviewed_paths)) {
      return $path;
    }

  }

  return FALSE;

}

/*
 * Lookup all alternatives to the group of paths - alias, <front>
 *
 * @parm
 *   $paths - a set of paths to be reviewed for alternatives
 *
 * @return
 *   $paths - an updated list of paths that include the additional source and alias values. 
 */
function _optimizely_collect_alias($paths) {

  // Add alternative values - alias, source, <front> to ensure matches also check different possiblities
  foreach ($paths as $path_count => $path) {

    // Remove parameters
    if (strpos($path, '?') !== FALSE) {
      $path = substr($path, 0, strpos($path, '?'));
      $paths[$path_count] = $path;
    }

    !drupal_lookup_path('alias', $path) ?: $paths[] = drupal_lookup_path('alias', $path);
    !drupal_lookup_path('source', $path) ?: $paths[] = drupal_lookup_path('source', $path);

    // Collect all the possible values to match <front>
    if ($path == '<front>') {

      $frontpage = variable_get('site_frontpage', FALSE);
      if ($frontpage) {
        $paths[] = variable_get('site_frontpage');
        $paths[] = drupal_lookup_path('alias', $frontpage);
      }

    }

  }

  return $paths;

}

/**
 * _optimizely_valid_paths()
 * 
 * Validate the target paths with drupal_lookup_path.
 *
 * @parm $target_paths
 *   An array of the paths to validate.
 * @parm $include
 *   Boolean, TRUE if the paths are included or FALSE for exclude paths
 *
 * @return
 *   Boolean of TRUE if the paths are valid or a string of the path that failed.
 */
function _optimizely_valid_paths($project_paths) {

   // Validate entered paths to confirm the paths exist on the website
  foreach ($project_paths as $project_path) {

    // Check for site wide wildcard
    if (strpos($project_path, '*') === 0) {

      if (count($project_paths) == 1) {
        return TRUE;
      }
      else {
        return $project_path;
      }

    } // Path wildcards
    elseif (strpos($project_path, '*') !== FALSE) {

      $project_wildpath = substr($project_path, 0, -2);
      if (!drupal_valid_path($project_wildpath, TRUE)) {

        // Look for entries in url_aias
        $query = db_query("SELECT * FROM {url_alias} WHERE
          source LIKE :project_wildpath OR alias LIKE :project_wildpath",
          array(':project_wildpath' => $project_wildpath . '%'));
        $project_wildpath_match = $query->rowCount();

        // No matches found for wildcard path
        if (!$project_wildpath_match) {
          return $project_path;
        }

      }

    } // Parameters
    elseif (strpos($project_path, '?') !== FALSE) {

      // Look for entries in menu_router
      $project_parmpath = substr($project_path, 0, strpos($project_path, '?'));

      // Look for entry in url_alias table
      if (drupal_lookup_path('alias', $project_parmpath) === FALSE &&
          drupal_lookup_path('source', $project_parmpath) === FALSE &&
          drupal_valid_path($project_parmpath, TRUE) === FALSE) {
        return $project_path;
      }

    } // Validation if path valid menu router entry, includes support for <front>
    elseif (drupal_valid_path($project_path, TRUE) === FALSE) {
        
      // Look for entry in url_alias table
      if (drupal_lookup_path('alias', $project_path) === FALSE &&
          drupal_lookup_path('source', $project_path) === FALSE) {
        return $project_path;
      }

    }

  }

  return TRUE;
}