<?php
/**
 * @file
 * Admin forms and functionality for Node Subpages module
 */

/**
 * List the subpages for a node type
 */
function node_subpages_admin_list($form, &$form_state, $type) {
  $type_url_str = str_replace('_', '-', $type->type);

  $subpages = node_subpages_list_for_type($type->type);

  $form['#tree'] = TRUE;
  $form['#theme'] = 'node_subpages_admin_list';

  foreach ($subpages as $path => $details) {
    // @todo: Add a callback to get the Type column content
    if ($details['default_view_page']) {
      $tab_content = 'Default tab';
    }
    elseif($summary_function = ctools_plugin_get_function($details['plugin'], 'config summary')) {
      $tab_content = $summary_function($details['plugin_config'], $details);
    }
    elseif (isset($details['plugin']['title'])) {
      $tab_content = $details['plugin']['title'];
    }
    else {
      $tab_content = 'Unknown';
    }

    $form['subpages'][$path]['id'] = array(
      '#type' => 'value',
      '#value' => $details['id'],
    );
    $form['subpages'][$path]['weight'] = array(
      '#type' => 'weight',
      '#title' => t('Weight for @title', array('@title' => $details['title'])),
      '#title_display' => 'invisible',
      '#default_value' => $details['weight'],
      '#delta' => 10,
    );
    $form['subpages'][$path]['subpath'] = array(
      '#markup' => $path,
    );
    $form['subpages'][$path]['title'] = array(
      '#markup' => $details['title'],
    );
    $form['subpages'][$path]['content'] = array(
      '#markup' => $tab_content,
    );

    if ($details['id'] > 0) {
      $form['subpages'][$path]['edit'] = array(
        '#markup' => l('edit', 'admin/structure/types/manage/' . $type_url_str . '/subpages/edit/' . $details['machine_name'])
      );
      $form['subpages'][$path]['delete'] = array(
        '#markup' => l('delete', 'admin/structure/types/manage/' . $type_url_str . '/subpages/delete/' . $details['machine_name']),
      );
    }
  }

  if (!empty($form['subpages'])) {
    $form['actions'] = array('#type' => 'actions');
    $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Save changes'));
  }

  return $form;
}


/**
 * Save the new subpage order
 */
function node_subpages_admin_list_submit($form, &$form_state) {
  $saved = 0;
  foreach ($form_state['values']['subpages'] as $path => $values) {
    $record = array('id' => $values['id'], 'weight' => $values['weight']);
    drupal_write_record('node_subpages', $record, 'id');
    $saved++;
  }
  if ($saved > 0) {
    drupal_set_message(t('Subpage order saved.'));
  }
  else {
    drupal_set_message(t('No subpages to save.'));
  }
}


/**
 * Theme the admin subpages list
 */
function theme_node_subpages_admin_list($vars) {
  $form = $vars['form'];

  $rows = array();
  if (!empty($form['subpages'])) {
    foreach (element_children($form['subpages']) as $path) {
      $form['subpages'][$path]['weight']['#attributes']['class'] = array('subpage-weight');
      $rows[] = array(
        'data' => array(
          drupal_render($form['subpages'][$path]['subpath']),
          drupal_render($form['subpages'][$path]['title']),
          drupal_render($form['subpages'][$path]['content']),
          drupal_render($form['subpages'][$path]['edit']),
          drupal_render($form['subpages'][$path]['delete']),
          drupal_render($form['subpages'][$path]['weight']),
        ),
        'class' => array('draggable'),
      );
    }
  }

  $headers = array(
    t('Sub-path'),
    t('Title'),
    t('Content'),
    array('data' => t('Operations'), 'colspan' => 2),
  );

  if (empty($rows)) {
    $rows[] = array(
      array(
        'data' => t('No subpages have been added for this content type.'),
        'colspan' => count($headers),
      ),
    );
  }
  else {
    $headers[] = t('Weight');

    // Make the table drag-sortable
    drupal_add_tabledrag('admin_subpage_list', 'order', 'sibling', 'subpage-weight');
  }

  $table_vars = array(
    'header' => $headers,
    'rows' => $rows,
    'attributes' => array('id' => 'admin_subpage_list'),
  );

  // Render the table listing the subpages
  $output = theme('table', $table_vars);

  // Render the submit button
  $output .= drupal_render($form['actions']);

  // Render the last hidden elements and such
  $output .= drupal_render_children($form);
  return $output;
}




/**
 * Form to add a new subpage to the given content type
 */
function node_subpages_add_subpage($form, &$form_state, $type, $machine_name = NULL) {
  // Make sure CTools is installed correctly
  if (!node_subpages_check_ctools_version()) {
    $form = array();
    $form['error'] = array(
      '#markup' => 'CTools must be installed',
      '#prefix' => '<div>',
      '#suffix' => '</div>',
    );
    return $form;
  }

  $form = array();
  $defaults = array(
    'subpath' => '',
    'title' => '',
    'plugin' => array('name' => ''),
    'plugin_config' => array(),
    'machine_name' => '',
  );

  // If editing an existing subpage, load the details for use as default
  // values in the form
  if ($machine_name) {
    $defaults = node_subpages_load_specific($machine_name);
    if (isset($defaults['id'])) {
      $form['#id'] = $defaults['id'];
    }
  }

  $form['#node_type'] = $type;

  $form['subpath'] = array(
    '#type' => 'textfield',
    '#title' => t('Subpath'),
    '#description' => t('Subpath that will be used in the URL for this subpage on the node.'),
    '#size' => 40,
    '#maxlength' => 255,
    '#required' => TRUE,
    '#default_value' => $defaults['subpath'],
  );
  $form['machine_name'] = array(
    '#type' => 'machine_name',
    '#default_value' => $defaults['machine_name'],
    '#maxlength' => 128,
    '#machine_name' => array(
      'exists' => 'node_subpages_machine_name_exists',
      'source' => array('subpath'),
    ),
  );
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Tab Title'),
    '#description' => t('Displayed as the text of the tab link for this subpage.'),
    '#size' => 40,
    '#maxlength' => 255,
    '#required' => TRUE,
    '#default_value' => $defaults['title'],
  );

  $form['data_source'] = array(
    '#type' => 'fieldset',
    '#title' => t('Data Source'),
    '#description' => t('Select the source for the content of this subpage. Only choose one.'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );


  $content_plugins = _node_subpages_get_content_plugins();
  foreach ($content_plugins as $plugin_key => $plugin) {
    // Check if the plugin has module dependencies
    if (!empty($plugin['dependencies']) && !is_array($plugin['dependencies'])) {
      $plugin['dependencies'] = array($plugin['dependencies']);
    }
    if (is_array($plugin['dependencies'])) {
      foreach ($plugin['dependencies'] as $module_name) {
        if (!module_exists($module_name)) {
          continue 2;
        }
      }
    }

    $form['data_source'][$plugin_key]['enable'] = array(
      '#type' => 'radio',
      '#title' => $plugin['title'],
      '#description' => $plugin['description'],
      '#return_value' => $plugin_key,
      '#default_value' => ($plugin_key == $defaults['plugin']['name']),
    );

    // If this plugin offers more options, add the form elements for those
    if($config_form_function = ctools_plugin_get_function($plugin, 'config form')) {
      // Create a fieldset for the config options and make it dependent on the
      // radio button for the content plugin
      $form['data_source'][$plugin_key]['options'] = array(
        '#type' => 'fieldset',
        '#title' => $plugin['title'] . ' ' . t('Options'),
        '#collapsible' => FALSE,
        '#states' => array(
          'visible' => array(
            ':input[name="enable"]' => array('value' => $plugin_key),
          ),
        ),
      );

      // Get the config form fields for this plugin
      $config_form = $config_form_function($type, $defaults['plugin_config']);
      // ...and merge them into the fieldset
      $form['data_source'][$plugin_key]['options'] = array_merge($form['data_source'][$plugin_key]['options'], $config_form);
    }
  }

  if ($machine_name) {
    $form['update'] = array(
      '#type' => 'submit',
      '#value' => t('Update'),
    );
  }
  else {
    $form['add'] = array(
      '#type' => 'submit',
      '#value' => t('Add'),
    );
  }
  return $form;
}

/**
 * Determine if a machine name for a subpage path already exists.
 */
function node_subpages_machine_name_exists($value) {
  $name_exists = db_query_range("SELECT 1 FROM {node_subpages} WHERE machine_name = :name", 0, 1, array(':name' => $value))->fetchField();
  return $name_exists;
}

/**
 * Submit the subpage add form, to create a new subpage type
 */
function node_subpages_add_subpage_submit($form, &$form_state) {
  // Make sure CTools is installed correctly
  if (!node_subpages_check_ctools_version()) {
    return FALSE;
  }

  $values = $form_state['values'];
  $node_type = $form['#node_type'];

  $record = array(
    'node_type' => $node_type->type,
    'subpath' => $values['subpath'],
    'title' => $values['title'],
    'machine_name' => $values['machine_name'],
  );

  $plugin_key = $record['source_type'] = $values['enable'];
  $plugin = _node_subpages_get_content_plugin($plugin_key);
  // Prep the data for save
  if($save_prep_function = ctools_plugin_get_function($plugin, 'config save prep')) {
    $record['source_data'] = $save_prep_function($values);
  }

  if ($form['#id'] > 0) {
    $record['id'] = $form['#id'];
    $saved = drupal_write_record('node_subpages', $record, 'id');
  }
  else {
    $saved = drupal_write_record('node_subpages', $record);
  }

  if ($saved) {
    if ($saved == SAVED_NEW) {
      drupal_set_message(t('New subpage saved.'));
    }
    else {
      drupal_set_message(t('Subpage updated.'));
    }

    $type_url_str = str_replace('_', '-', $node_type->type);

    // Rebuild the menu cache so that the path is updated
    menu_rebuild();

    drupal_goto('admin/structure/types/manage/' . $type_url_str . '/subpages');
  }
  else {
    drupal_set_message(t('Error saving subpage. Please try again.'));
    return FALSE;
  }
}


/**
 * Form to delete subpage: confirm before deleting
 */
function node_subpages_delete_subpage($form, &$form_state, $node_type, $machine_name) {
  $type_url_str = str_replace('_', '-', $node_type->type);
  $cancel_path = 'admin/structure/types/manage/' . $type_url_str . '/subpages';
  $details = node_subpages_load_specific($machine_name);
  $question = t('Are you sure you want to delete the %subpage subpage from %content_type?', array(
    '%subpage' => $details['title'],
    '%content_type' => $node_type->name,
  ));

  $form = array();
  $form['#id'] = $details['id'];
  $form['#node_type'] = $node_type;
  return confirm_form($form, $question, $cancel_path, NULL, t('Delete'));
}

/**
 * Submit the subpage deletion confirmation form
 */
function node_subpages_delete_subpage_submit($form, &$form_state) {
  db_delete('node_subpages')
  ->condition('id', $form['#id'])
  ->execute();
  drupal_set_message(t('Subpage deleted.'));

  $type_url_str = str_replace('_', '-', $form['#node_type']->type);
  $redirect_path = 'admin/structure/types/manage/' . $type_url_str . '/subpages';
  drupal_goto($redirect_path);
}
