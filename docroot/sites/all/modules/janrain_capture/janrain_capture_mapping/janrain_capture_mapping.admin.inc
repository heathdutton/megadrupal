<?php

/**
 * @file
 * Mapping admin settings functions
 */

/**
 * Menu callback for the mapping configuration screen.
 */
function janrain_capture_settings_mapping($form, &$form_state) {
  $user_field_options = janrain_capture_mapping_get_user_field_options();
  $capture_field_options = janrain_capture_mapping_get_capture_field_options();
  $map = variable_get('janrain_capture_mapping_map', array());

  // Add an empty row for saving a new mapping.
  $map[] = array();

  foreach ($map as $mid => $mapping) {
    $selected_field = isset($form_state['values'][$mid]['field']) ? $form_state['values'][$mid]['field'] :
      (isset($map[$mid]['field']) ? $map[$mid]['field'] : '');
    $form[$mid] = array(
      'fid' => array(
        '#type' => 'select',
        '#title' => t('Capture field'),
        '#title_display' => 'invisible',
        '#options' => $capture_field_options,
        '#default_value' => isset($map[$mid]['fid']) ? $map[$mid]['fid'] : '',
        '#empty_option' => t('- Select a data field -'),
        '#attributes' => array('class' => array('janrain-capture-field-select', 'mid-' . $mid)),
      ),
      'separator' => array(
        '#markup' => '=>'
      ),
      'field' => array(
        '#type' => 'select',
        '#title' => t('Field'),
        '#title_display' => 'invisible',
        '#options' => $user_field_options,
        '#empty_option' => t('- Select a field -'),
        '#default_value' => isset($map[$mid]['field']) ? $map[$mid]['field'] : '',
        '#attributes' => array('class' => array('field-select', 'mid-' . $mid)),
        '#ajax' => array(
          'callback' => '_janrain_capture_ajax_callback',
          'wrapper' => 'janrain-capture-columns-replace-' . $mid,
        ),
      ),
      'column' => array(
        '#type' => 'select',
        '#title' => t('Column'),
        '#title_display' => 'invisible',
        '#prefix' => '<div id="janrain-capture-columns-replace-' . $mid . '">',
        '#suffix' => '</div>',
        '#options' => _janrain_capture_mapping_get_columns($selected_field),
        '#default_value' => isset($map[$mid]['column']) ? $map[$mid]['column'] : '',
      ),
    );
  }

  if (count($form)) {
    $form['actions'] = array('#type' => 'actions');
    $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Save configuration'));
  }

  $form['#tree'] = TRUE;

  return $form;
}

/**
 * Validation callback for the field mapping form.
 *
 * @see janrain_capture_settings_mapping()
 */
function janrain_capture_settings_mapping_validate($form, &$form_state) {
  $values = $form_state['values'];
  $mapped = array();

  foreach (element_children($values) as $mid) {
    if (is_numeric($mid)) {
      if(!empty($values[$mid]['fid']) && !empty($values[$mid]['field'])) {
        if (!isset($mapped[$values[$mid]['field']])) {
          $mapped[$values[$mid]['field']] = TRUE;
        }
        else {
          form_set_error("{$mid}][field", t('Each user field can only have one capture field mapped to it'));
        }
      }
    }
  }
}

/**
 * Submit handler to update changed Drupal to Capture data mapping.
 *
 * @see janrain_capture_settings_mapping()
 */
function janrain_capture_settings_mapping_submit($form, &$form_state) {
  $values = $form_state['values'];
  $map = array();

  foreach (element_children($values) as $mid) {
    if (is_numeric($mid)) {
      // All field mappings should include a bundle, except for legacy
      // (non-fieldable) entity fields.
      if(!empty($values[$mid]['fid']) && !empty($values[$mid]['field'])) {
        $map[$mid] = array(
          'fid' => $values[$mid]['fid'],
          'field' =>  $values[$mid]['field'],
          'column' => isset($values[$mid]['column']) ? $values[$mid]['column'] : 'value',
        );
      }
    }
  }

  variable_set('janrain_capture_mapping_map', $map);
  drupal_set_message(t('Profile to Capture data mapping has been updated.'));
}

/**
 * Theme Capture field mapping form.
 *
 * @ingroup themeable
 */
function theme_janrain_capture_settings_mapping($variables) {
  $form = $variables['form'];

  $rows = array();
  foreach (element_children($form) as $key) {
    // Skip form control elements.
    if (array_key_exists('separator', $form[$key])) {
      $field = &$form[$key];

      // Add the row
      $row = array();
      $row[] = drupal_render($field['fid']);
      $row[] = drupal_render($field['separator']);
      $row[] = drupal_render($field['field']);
      $row[] = drupal_render($field['column']);
      $rows[] = array('data' => $row);
    }
  }
  $header = array(t('Capture Data Field'), '', t('Drupal User Field'), t('Field Column'));

  $output = theme('table', array('header' => $header, 'rows' => $rows));
  $output .= drupal_render_children($form);

  return $output;
}

/**
 * Menu callback: Generate a form to manage Capture profile fields.
 *
 * @ingroup forms
 * @see _janrain_capture_settings_fields_validate()
 * @see _janrain_capture_settings_fields_submit()
 */
function janrain_capture_settings_fields($form, &$form_state) {
  $fields = db_query("SELECT fid, title, path FROM {janrain_capture_mapping_field}");
  while ($field = $fields->fetchObject()) {
    $admin_field_path = 'admin/config/people/janrain_capture/fields/';
    $form[$field->fid] = array (
      'title' => array(
        '#markup' => $field->title,
      ),
      'path' => array(
        '#markup' => $field->path,
      ),
      'edit' => array(
        '#type' => 'link',
        '#title' => t('edit'),
        '#href' => $admin_field_path . 'edit/' . $field->fid,
        '#options' => array('attributes' => array('title' => t('Edit field.'))),
      ),
      'delete' => array(
        '#type' => 'link',
        '#title' => t('delete'),
        '#href' => $admin_field_path . 'delete/' . $field->fid,
        '#options' => array('attributes' => array('title' => t('Delete field.'))),
      ),
    );
  }

  // Additional row: add new field.
  $form['_edit_field'] = array(
    'title' => array(
      '#type' => 'textfield',
      '#title' => t('New field title'),
      '#title_display' => 'invisible',
      '#size' => 15,
      '#attributes' => array('class' => array('janrain-capture-field-title-input')),
      '#description' => t('Title'),
      '#prefix' => '<div class="add-new-placeholder">' . t('Add new field') .'</div>',
    ),
    'path' => array(
      '#type' => 'textfield',
      '#title' => t('New field path'),
      '#title_display' => 'invisible',
      '#size' => 30,
      '#attributes' => array('class' => array('janrain-capture-path-input')),
      '#description' => t('Capture data path'),
      '#prefix' => '<div class="add-new-placeholder">&nbsp;</div>',
    ),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save new field'),
  );
  $form['actions']['reset'] = array(
    '#type' => 'link',
    '#title' => t('Reset to defaults'),
    '#href' => 'admin/config/people/janrain_capture/fields/reset',
  );

  $form['#tree'] = TRUE;
  $form['#validate'][] = '_janrain_capture_settings_fields_validate';
  $form['#submit'][] = '_janrain_capture_settings_fields_submit';

  return $form;
}

/**
 * Validate janrain_capture_settings_fields and janrain_capture_settings_fields_edit submissions.
 */
function _janrain_capture_settings_fields_validate($form, &$form_state) {
  $field = $form_state['values']['_edit_field'];

  // Missing title.
  if (!$field['title']) {
    form_set_error('_edit_field][title', t('You need to provide a title.'));
  }
  // Missing data path.
  if (!$field['path']) {
    form_set_error('_edit_field][path', t('You need to provide a data path.'));
  }

  $query = db_select('janrain_capture_mapping_field');
  $query->fields('janrain_capture_mapping_field', array('fid'));

  if (isset($field['fid'])) {
    $query->condition('fid', $field['fid'], '<>');
  }
  $query_path = clone $query;

  $title = $query
    ->condition('title', $field['title'])
    ->execute()
    ->fetchField();
  if ($title) {
    form_set_error('_edit_field][title', t('The specified title is already in use.'));
  }
  $path = $query_path
    ->condition('path', $field['path'])
    ->execute()
    ->fetchField();
  if ($path) {
    form_set_error('_edit_field][path', t('The specified path is already in use.'));
  }
}

/**
 * Process janrain_capture_settings_fields and janrain_capture_settings_fields_edit submissions.
 */
function _janrain_capture_settings_fields_submit($form, &$form_state) {
  $field = $form_state['values']['_edit_field'];

  // Remove all elements that are not janrain_capture_mapping_field columns.
  $values = array_intersect_key($field, array_flip(array('title', 'path')));
  if (!isset($field['fid'])) {
    db_insert('janrain_capture_mapping_field')
      ->fields(array('title' => $values['title'], 'path' => $values['path']))
      ->execute();
    drupal_set_message(t('The field has been created.'));
    watchdog('janrain_capture_mapping', 'Capture profile field %field added with path %path.', array('%field' => $field['title'], '%path' => $field['path']), WATCHDOG_NOTICE, l(t('view'), 'admin/config/people/janrain_capture/fields'));
  }
  else {
    db_update('janrain_capture_mapping_field')
      ->fields(array('title' => $values['title'], 'path' => $values['path']))
      ->condition('fid', $field['fid'])
      ->execute();
    drupal_set_message(t('The field has been updated.'));
  }
  cache_clear_all();
  menu_rebuild();

  $form_state['redirect'] = 'admin/config/people/janrain_capture/fields';
  return;
}

/**
 * Returns HTML for the Capture fields overview page.
 *
 * @ingroup themeable
 */
function theme_janrain_capture_settings_fields($variables) {
  $form = $variables['form'];

  $rows = array();
  foreach (element_children($form) as $key) {
    // Skip form control elements.
    if (array_key_exists('path', $form[$key])) {
      $field = &$form[$key];

      // Add the row
      $row = array();
      $row[] = drupal_render($field['title']);
      $row[] = drupal_render($field['path']);
      $row[] = drupal_render($field['edit']);
      $row[] = drupal_render($field['delete']);
      $rows[] = array('data' => $row);
    }
  }

  $header = array(t('Title'), t('Path'));
  $header[] = array('data' => t('Operations'), 'colspan' => 2);

  $output = theme('table', array('header' => $header, 'rows' => $rows));
  $output .= drupal_render_children($form);

  return $output;
}

/**
 * Menu callback: Generate a form to edit an Capture profile field.
 *
 * @ingroup forms
 * @see _janrain_capture_settings_fields_validate()
 * @see _janrain_capture_settings_fields_submit()
 */
function janrain_capture_settings_fields_edit($form, &$form_state, $arg = NULL) {
  if (is_numeric($arg)) {
    $fid = $arg;
    $field = db_query('SELECT fid, title, path FROM {janrain_capture_mapping_field} WHERE fid = :fid', array('fid' => $fid))->fetchAssoc();

    if (!$field) {
      drupal_not_found();
      drupal_exit();
    }
    drupal_set_title(t('Edit %title Capture field', array('%title' => $field['title'])), PASS_THROUGH);
    $form['_edit_field']['fid'] = array(
      '#type' => 'value',
      '#value' => $fid,
    );
  }
  else {
    drupal_not_found();
    drupal_exit();
  }

  $form['_edit_field']['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => $field['title'],
    '#attributes' => array('class' => array('janrain-capture-field-title-input')),
    '#description' => t('The title of the field. The title is shown in the mapping form next to the data path. An example title is "Email". '),
  );
  $form['_edit_field']['path'] = array(
    '#type' => 'textfield',
    '#title' => t('Capture data path'),
    '#default_value' => $field['path'],
    '#attributes' => array('class' => array('janrain-capture-path-input')),
    '#description' => t("The path to the data within the Capture schema, delimited by dot-notation.
An example path is <code>gender</code> or <code>primaryAddress.city</code>."),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save field'),
  );

  $form['#tree'] = TRUE;
  $form['#validate'][] = '_janrain_capture_settings_fields_validate';
  $form['#submit'][] = '_janrain_capture_settings_fields_submit';

  return $form;
}

/**
 * Menu callback; deletes a Capture field from the Drupal to Capture mapping
 * configuration.
 */
function janrain_capture_settings_fields_delete($form, &$form_state, $fid = NULL) {
  $field = db_query("SELECT title FROM {janrain_capture_mapping_field} WHERE fid = :fid", array(':fid' => $fid))->fetchObject();
  if (!$field) {
    drupal_not_found();
    drupal_exit();
  }
  $form['fid'] = array('#type' => 'value', '#value' => $fid);
  $form['title'] = array('#type' => 'value', '#value' => $field->title);

  return confirm_form($form,
    t('Are you sure you want to delete the Capture field %field?', array('%field' => $field->title)), 'admin/config/people/janrain_capture/fields',
    t('This action cannot be undone. The Drupal to Capture data mappings using this field will be deleted as well.'),
    t('Delete'), t('Cancel'));
}

/**
 * Process an Capture field delete form submission.
 */
function janrain_capture_settings_fields_delete_submit($form, &$form_state) {
  db_delete('janrain_capture_mapping_field')
    ->condition('fid', $form_state['values']['fid'])
    ->execute();

  cache_clear_all();

  drupal_set_message(t('The Capture field %field has been deleted.', array('%field' => $form_state['values']['title'])));
  watchdog('janrain_capture_mapping', 'Capture field %field deleted.', array('%field' => $form_state['values']['title']), WATCHDOG_NOTICE, l(t('view'), 'admin/config/people/janrain_capture/fields'));

  $form_state['redirect'] = 'admin/config/people/janrain_capture/fields';
  return;
}

/**
 * Menu callback; confirmation form.
 */
function janrain_capture_settings_fields_reset($form, &$form_state) {
  return confirm_form($form, t('Are you sure you want to revert to the default Janrain Capture profile fields?'), 'admin/config/people/janrain_capture/fields/', t('Any new fields that you have created will be lost. Any existing mappings will be edelted as well. This action cannot be undone.'), t('Reset'));
}

/**
 * Process fields reset form submissions.
 *
 * Reset Capture fields by deleting all existing fields and creating default ones
 * (those that are created at module installation).
 */
function janrain_capture_settings_fields_reset_submit($form, &$form_state) {
  db_query("DELETE FROM {janrain_capture_mapping_field}");

  _janrain_capture_mapping_insert_defaults();

  drupal_set_message(t('The Janrain Capture profile fields have been reverted to the defaults.'));
  $form_state['redirect'] = 'admin/config/people/janrain_capture/fields/';
}

/********************************* Helper Functions ************************************************/

/**
 * Construct an array of options for the user field dropdown.
 *
 * @see janrain_capture_settings_mapping()
 */
function janrain_capture_mapping_get_user_field_options() {
  $fields = array();

  // Add the fields defined by the User entity.
  foreach (field_info_instances('user', 'user') as $field_name => $instance) {
    // Don't allow any mapping to the uuid field as the base module maps the
    // uuid to this field automatically.
    if ($field_name == 'field_janrain_capture_uuid') {
      continue;
    }
    $fields['field'][$field_name] = $instance['label'];
  }

  // Add properties
  $wrapper = entity_metadata_wrapper('user');
  foreach ($wrapper->getPropertyInfo() as $name => $info) {
    // Don't allow any mapping to the uid or email field.
    if (in_array($name, array('uid', 'mail'))) {
      continue;
    }

    // Exclude properties that are arrays, e.g. "User roles".
    if (isset($info['type']) && strpos($info['type'], 'list') === 0) {
      continue;
    }
    // Fields have been dealt with above so exclude them here.
    if (!isset($info['field'])) {
      $fields['property'][$name] = $info['label'];
    }
  }
  return $fields;
}

/**
 * Construct an array of options for the capture field dropdown.
 *
 * @see janrain_capture_settings_mapping()
 */
function janrain_capture_mapping_get_capture_field_options() {
  $options = array('' => '');
  $fields = db_query("SELECT fid, title FROM {janrain_capture_mapping_field}");
  while ($field = $fields->fetchObject()) {
    $options[$field->fid] = $field->title;
  }
  return $options;
}

/**
 * Returns the columns for a particular field.
 */
function _janrain_capture_mapping_get_columns($selected_field) {
  if (!empty($selected_field)) {
    $field = field_info_field($selected_field);
    if (isset($field['columns']) && !empty($field['columns'])) {
      return drupal_map_assoc(array_keys($field['columns']));
    }
  }
  return array('value' => 'value');
}

/**
 * Ajax callback for the column dropdown.
 */
function _janrain_capture_ajax_callback($form, $form_state) {
  $name = $form_state['triggering_element']['#name'];
  $matches = array();
  if (preg_match("/(\d+)\[field\]/", $name, $matches)) {
    $mid = $matches[1];
  }
  return $form[$mid]['column'];
}
