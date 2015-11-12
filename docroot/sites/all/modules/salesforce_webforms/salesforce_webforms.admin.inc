<?php

/**
 * @file
 * Manages Salesforce webform mapping UI.
 */

/**
 * Form constructor for the mapping list.
 *
 * @param object $node
 *   The node which owns this form.
 *
 * @see salesforce_webforms_maps_form_validate()
 * @see salesforce_webforms_maps_form_submit()
 * @see theme_salesforce_webforms_maps_form()
 *
 * @ingroup forms
 */
function salesforce_webforms_maps_form($form, &$form_state, $node) {
  $maps = salesforce_webforms_get_webform_maps($node);
  $settings = salesforce_webforms_settings_get($node->nid);

  $form = array(
    '#tree' => TRUE,
    '#node' => $node,
    '#theme' => 'salesforce_webforms_maps_form',
    'maps' => array(),
    'nid' => array(
      '#type' => 'value',
      '#value' => $node->nid,
    ),
    'settings' => array(
      '#type' => 'fieldset',
      '#title' => t('Settings'),
      'suppress_errors' => array(
        '#title' => t('Suppress Errors?'),
        '#description' => t('Check this field to suppress display of Salesforce error messages when a user submits this form. Note that this setting has no effect on errors produced when resubmitting results through the admin UI, or on error messages sent to the system log.'),
        '#type' => 'checkboxes',
        '#options' => array(
          '1' => t('Yes'),
        ),
        '#default_value' => $settings['suppress_errors'] ? array('1') : array(),
      ),
    ),
    'helptext' => array(
      '#type' => 'markup',
      '#markup' => t("Maps are used to allow multiple Salesforce objects to be updated / inserted from a single form.  Typically, you will have only one map per Salesforce object, unless the intent is to create/update multiple records of the same object from one form.  For each map that you want to use to update an existing object, you will need to specify the key(s) as well as the fields you want updated."),
    ),
  );

  if (isset($settings['settingid'])) {
    $form['settingid'] = array(
      '#type' => 'value',
      '#value' => $settings['settingid'],
    );
  }

  $objects = salesforce_webforms_get_objects_selection();

  $maxweight = 0;

  foreach ($maps as $map) {
    $mapid = $map['mapid'];
    $form['maps'][$mapid] = array(
      '#salesforce-webforms-extra' => $map,
      'mapname' => array(
        '#markup' => check_plain($map['mapname']),
      ),
      'sf_object' => array(
        '#markup' => check_plain($objects[$map['sf_object']]),
      ),
      'fields' => array(
        '#type' => 'fieldset',
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
        '#title' => t('Fields'),
      ),
      'weight' => array(
        '#type' => 'weight',
        '#title' => t('Weight'),
        '#default_value' => $map['displayorder'],
        '#delta' => 10,
        '#title_display' => 'invisible',
      ),
    );

    $field_info = salesforce_webforms_map_fields_basic($map['sf_object'], $map['fields']);
    // Add each of the fields.
    foreach ($map['fields'] as $fid => $field) {
      $form['maps'][$mapid]['fields'][$fid] = array(
        '#type' => 'textfield',
        '#disabled' => TRUE,
        '#title' => t('Field Name'),
        '#title_display' => 'invisible',
        '#value' => $field_info[$field['sf_fieldname']],
      );
    }

    if ($map['displayorder'] >= $maxweight) {
      $maxweight = $map['displayorder'] + 1;
    }
  }

  // Add the new map section.
  $form['new'] = array(
    'mapname' => array(
      '#type' => 'textfield',
      '#title' => t('Map Name'),
      '#title_display' => 'invisible',
      '#required' => FALSE,
    ),
    'sf_object' => array(
      '#type' => 'select',
      '#title' => t('Object'),
      '#title_display' => 'invisible',
      '#required' => FALSE,
      '#options' => $objects,
    ),
    'weight' => array(
      '#type' => 'weight',
      '#title' => t('Weight'),
      '#default_value' => $maxweight,
      '#delta' => 10,
      '#title_display' => 'invisible',
    ),
  );

  // Add action section.
  $form['actions'] = array(
    '#type' => 'actions',
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Save'),
    ),
  );
  return $form;
}

/**
 * Form validation handler for salesforce_webforms_maps_form().
 *
 * @see salesforce_webforms_maps_form()
 */
function salesforce_webforms_maps_form_validate($form, &$form_state) {
  // Make sure that the new map name is unique and formatted properly.
  $name = $form_state['values']['new']['mapname'];
  $nid = $form_state['values']['nid'];

  // If new name is not blank, then validate it.
  if ($name != '') {
    // First make sure that the name is valid.
    if (!preg_match('/^[a-z0-9_]+$/i', $name)) {
      form_set_error('name',
        t('The map name %mapname is invalid. Please include only alphanumeric characters and underscores.',
        array(
          '%mapname' => $name,
        ))
      );

      // No need to process further if the format is invalid.
      return;
    }

    // Now make sure the key isn't duplicated.
    $maps = salesforce_webforms_get_node_maps($nid);
    foreach ($maps as $map) {
      if ($map['mapname'] == $name) {
        form_set_error('name',
          t('The map name %mapname is already in use. Please use a unique name.',
          array(
            '%mapname' => $name,
          ))
        );

        // No need to continue processing.
        return;
      }
    }
  }
}

/**
 * Returns HTML for a salesforce_webforms_maps_form form.
 *
 * @param array $vars
 *   An associative array containing:
 *   - form: The form to be formatted.
 *
 * @see salesforce_webforms_maps_form()
 *
 * @ingroup themeable
 */
function theme_salesforce_webforms_maps_form(array $vars) {
  $form = $vars['form'];

  $header = array(
    t('Map name'),
    t('Salesforce Object'),
    t('Fields (* indicates key field)'),
    t('Weight'),
    array(
      'data' => t('Operations'),
      'colspan' => 2,
    ));

  $rows = array();

  $nid = $form['nid']['#value'];

  foreach (element_children($form['maps']) as $mid) {
    $map = $form['maps'][$mid]['#salesforce-webforms-extra'];

    // Add the required class to the weight for draggable to work.
    $form['maps'][$mid]['weight']['#attributes']['class'] = array('salesforce-webforms-map-weight');

    $rows[] = array(
      'data' => array(
        drupal_render($form['maps'][$mid]['mapname']),
        drupal_render($form['maps'][$mid]['sf_object']),
        drupal_render($form['maps'][$mid]['fields']),
        drupal_render($form['maps'][$mid]['weight']),
        l(t('Edit'), 'node/' . $nid . '/webform/salesforce/edit/' . $map['sf_object'] . '/' . $map['mapid'], array('query' => drupal_get_destination())),
        l(t('Delete'), 'node/' . $nid . '/webform/salesforce/delete/' . $map['mapid'], array('query' => drupal_get_destination())),
      ),
      'class' => array('draggable'),
    );
  }

  // Add the row for a new mapping.
  $form['new']['weight']['#attributes']['class'] = array('salesforce-webforms-map-weight');
  $rows[] = array(
    'data' => array(
      drupal_render($form['new']['mapname']),
      drupal_render($form['new']['sf_object']),
      '',
      drupal_render($form['new']['weight']),
    ),
    'class' => array('draggable'),
  );

  $table_id = "salesforce-webforms-maps-table";
  $output = theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array('id' => $table_id),
  ));

  $output .= drupal_render_children($form);

  drupal_add_tabledrag($table_id, 'order', 'sibling', 'salesforce-webforms-map-weight');

  return $output;
}

/**
 * Form submission handler for salesforce_webforms_maps_form().
 *
 * @see salesforce_webforms_maps_form()
 */
function salesforce_webforms_maps_form_submit($form, &$state) {
  // Save the settings.
  $settings = array(
    'nid' => $state['values']['nid'],
    'suppress_errors' => $state['values']['settings']['suppress_errors'],
  );

  if (isset($state['values']['settingid'])) {
    $settings['settingid'] = $state['values']['settingid'];
  }

  salesforce_webforms_settings_save($settings);

  if (isset($state['values']['maps']) && !empty($state['values']['maps'])) {
    foreach ($state['values']['maps'] as $id => $map) {
      db_update('salesforce_webforms_map')
        ->fields(array(
          'displayorder' => $map['weight'],
        ))
        ->condition('mapid', $id, '=')
        ->execute();
    }
  }

  // Now see if we have a new map to create.
  $new = $state['values']['new'];
  if ($new['mapname'] != '' && $new['sf_object'] != '') {
    // Yes. Create the object and then move to the edit form for it.
    $nid = $state['values']['nid'];
    $obj = $new['sf_object'];
    $newid = salesforce_webforms_map_save(array(
      'action' => 'add',
      'nid' => $nid,
      'object' => $obj,
      'mapname' => $new['mapname'],
    ));

    drupal_goto("node/$nid/webform/salesforce/edit/$obj/$newid", array(
      'query' => drupal_get_destination(),
    ));
  }
}

/**
 * Form constructor for the map delete form.
 *
 * @param array $map
 *   The map to be deleted.
 *
 * @see salesforce_webforms_delete_rule_submit()
 *
 * @ingroup forms
 */
function salesforce_webforms_delete_rule($form, &$form_state, array $map) {
  $objects = salesforce_webforms_get_objects_selection();
  $form = array(
    'mapid' => array(
      '#type' => 'value',
      '#value' => $map['mapid'],
    ),
    'map' => array(
      '#type' => 'fieldset',
      '#collapsed' => FALSE,
      '#collapsible' => FALSE,
      '#title' => t('Map Details'),
      'name' => array(
        '#type' => 'textfield',
        '#title' => t('Map Name'),
        '#value' => $map['mapname'],
        '#disabled' => TRUE,
      ),
      'object' => array(
        '#type' => 'textfield',
        '#title' => t('Salesforce Object'),
        '#value' => $objects[$map['sf_object']],
        '#disabled' => TRUE,
      ),
    ),
  );

  return confirm_form(
    $form,
    t('Are you sure you wish to delete this map?'),
    drupal_get_destination());
}

/**
 * Form submission handler for salesforce_webforms_delete_rule().
 *
 * @see salesforce_webforms_delete_rule()
 */
function salesforce_webforms_delete_rule_submit($form, &$form_state) {
  $mapid = $form_state['values']['mapid'];
  salesforce_webforms_delete_mapping($mapid);
  drupal_set_message(t('Map %mapid has been deleted', array(
    '%mapid' => $mapid,
  )));
}

/**
 * Get the list of maps associated with the webform.
 *
 * @param object $node
 *   The node which owns the webform.
 *
 * @return array
 *   An array of map arrays.
 */
function salesforce_webforms_get_webform_maps($node) {
  if (in_array($node->type, webform_variable_get('webform_node_types'))) {
    $maps = salesforce_webforms_get_node_maps($node->nid);
  }
  return $maps;
}

/**
 * Form constructor for the resubmission form.
 *
 * Allows resubmission of webform data to Salesforce.
 *
 * @param object $node
 *   The node which owns this form.
 * @param object $sub
 *   The submission object.
 *
 * @see salesforce_webforms_resubmit_submit()
 *
 * @ingroup forms
 */
function salesforce_webforms_resubmit($form, $form_state, $node, $sub) {
  $form = array();

  $sid = $sub->sid;

  $form["markup"] = array('#markup' => "<h1>" . t('Resubmit webform submission !sid', array('!sid' => $sid)) . "</h1>");
  $form["sid"] = array(
    '#type' => 'hidden',
    '#value' => $sid,
  );

  $form["update"] = array(
    '#type' => 'checkboxes',
    '#options' => array(
      '1' => t('Yes'),
    ),
    '#title' => t('Update previously created Salesforce objects?'),
  );

  $form["submit"] = array(
    '#type' => 'submit',
    '#value' => t('Send to Salesforce'),
  );

  $form_state["node"] = $node;
  $form_state["submission"] = $sub;

  return $form;
}

/**
 * Form submission handler for salesforce_webforms_resubmit form.
 *
 * @see salesforce_webforms_resubmit()
 */
function salesforce_webforms_resubmit_submit($form, $form_state) {
  // Load the submission if not already in the state (Webform 3.x).
  if (!isset($form_state['submission'])) {
    $sid = $form_state['values']['sid'];
    $submissions = webform_get_submissions(array('sid' => $sid));
    $form_state['submission'] = $submissions[$sid];
  }
  $sub = $form_state['submission'];

  $update = $form_state['values']['update']['1'];

  // Load the node if not already in the state (Webform 3.x).
  if (!isset($form_state['node'])) {
    $form_state['node'] = node_load($sub->nid);
  }
  $node = $form_state['node'];

  salesforce_webforms_webform_submission_insert($node, $sub, $update, TRUE);

  drupal_set_message(t('Salesforce updated'));
}

/**
 * Form constructor for the manage map form.
 *
 * Allows the form administrator to manage mappings between webform and
 * Salesforce.
 *
 * @param object $node
 *   The node which owns the form.
 * @param string $action
 *   The action to perform. One of:
 *   - add.
 *   - edit.
 * @param string $obj
 *   The name of the Salesforce object being mapped.
 * @param array $map
 *   The current contents of the mapping.
 *
 * @see salesforce_webforms_manage_map_validate()
 * @see salesforce_webforms_manage_map_submit()
 *
 * @ingroup forms
 */
function salesforce_webforms_manage_map($form, $form_state, $node, $action, $obj, array $map = NULL) {
  $form = array();
  $obj_info = salesforce_webforms_get_object_fields($obj);

  $schema_version = salesforce_webforms_schema_version();
  $include_filter = ($schema_version >= 7004);

  if ($include_filter) {
    $filters = salesforce_webforms_filters();
  }
  else {
    drupal_set_message(
      t('There are database updates pending. Please ask your system admiistrator to run update.php'),
      'warning'
    );
  }

  $form['map'] = array(
    '#type' => 'fieldset',
    '#title' => ($action == 'edit') ? t('Edit mapping') : t('Add mapping'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );

  $form['map']['object'] = array(
    '#type' => 'hidden',
    '#value' => $obj,
  );

  $form['map']['action'] = array(
    '#type' => 'hidden',
    '#value' => $action,
  );

  if ($action == 'edit' && $map) {
    $form['map']['mapid'] = array(
      '#type' => 'hidden',
      '#value' => $map['mapid'],
    );
  }
  $form['map']['nid'] = array(
    '#type' => 'hidden',
    '#value' => $node->nid,
  );

  $form['map']['mapname'] = array(
    '#type' => 'textfield',
    '#title' => t('Map name'),
    '#default_value' => (isset($map['mapname'])) ? $map['mapname'] : '',
    '#description' => t("The name of this map. Must use only letters, numbers, and '_'. Used to identify this mapping in other maps."),
    '#required' => TRUE,
    '#size' => 60,
    '#maxlength' => 64,
    '#weight' => 1,
  );

  $form['map']['tokens'] = array(
    '#markup' => theme('webform_token_help', array(
      'groups' => array('node', 'submission'),
      )
    ),
    '#weight' => 3,
  );

  // Do we need to add extra info for old webform version?
  if (salesforce_webforms_webform_version() < 4) {
    $fieldset = array(
      '#title' => t('Salesforce tokens'),
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#children' => '<div>' . t('@token - The unique ID of the Salesforce object associated with a webform mapping. Replace the "?" with the name of the mapping', array('@token' => '[submission:salesforce:?]')) . '</div>',
      '#attributes' => array('class' => array('collapsible', 'collapsed')),
    );

    $form['map']['tokens']['#markup'] .= theme('fieldset', array('element' => $fieldset));
  }

  $form['map']['map_fields'] = array(
    '#type' => 'fieldset',
    '#title' => t('Fields'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#weight' => 4,
    '#tree' => TRUE,
  );

  foreach ($obj_info as $fname => $fdata) {
    // Skip if not writable.
    if ($fdata['createable'] == FALSE || $fdata['updateable'] == FALSE) {
      continue;
    }

    $form['map']['map_fields'][$fname] = array(
      '#type' => 'fieldset',
      '#title' => $fdata['label'],
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#tree' => TRUE,
    );

    $tmp = array(
      '#title' => t('Source data'),
      '#required' => FALSE,
      '#description' => t('Select Token or Constant to insert/update or to use as the key value for this field.  Typically this will be the token [submission:values:?] where ? is replaced with the field key.  See more details under Webform submission values by clicking on Browse available tokens.'),
    );

    if ($fdata['type'] == 'textarea') {
      $tmp['#type'] = 'textarea';
    }
    else {
      $tmp['#maxlength'] = 32767;
      $tmp['#type'] = 'textfield';
    }
    $form['map']['map_fields'][$fname]['target'] = $tmp;

    // Special handling extra select field for some types.
    if ($fdata['type'] == 'picklist') {
      $form['map']['map_fields'][$fname]['picklist'] = array(
        '#type' => 'select',
        '#title' => t('Or select a predefined value'),
        '#options' => salesforce_webforms_picklist_select($fdata['picklistValues']),
      );
    }
    elseif ($fdata['type'] == 'multipicklist') {
      $form['map']['map_fields'][$fname]['picklist'] = array(
        '#type' => 'select',
        '#multiple' => TRUE,
        '#title' => t('Or select predefined value(s)'),
        '#options' => salesforce_webforms_picklist_select($fdata['picklistValues'], FALSE),
      );
    }
    elseif ($fdata['type'] == 'boolean') {
      $form['map']['map_fields'][$fname]['picklist'] = array(
        '#type' => 'select',
        '#title' => t('Or select a predefined value'),
        '#options' => array(
          '' => t('- Select -'),
          'false' => t('FALSE'),
          'true' => t('TRUE'),
        ),
      );
    }
    elseif ($fdata['type'] == 'reference') {
      $options = array('' => t('- Select -'));
      // Get a list of all records of the appropriate type(s).
      foreach ($fdata['referenceTo'] as $ref) {
        $options[$ref] = salesforce_webforms_get_object_records($ref);
      }
      $form['map']['map_fields'][$fname]['picklist']
        = array(
          '#type' => 'select',
          '#title' => t('Or select a predefined value'),
          '#options' => $options,
        );
    }

    // Did we add a picklist?
    if (isset($form['map']['map_fields'][$fname]['picklist'])) {
      $field = $form['map']['map_fields'][$fname]['picklist'];
      $multiple = isset($field['#multiple']) ? $field['#multiple'] : FALSE;
      $form['map']['map_fields'][$fname]['target']['#states'] = array(
        'enabled' => array(
          ':input[name^="map_fields[' . $fname . '][picklist]"]' => $multiple ? array('select_not_empty' => FALSE) : array('value' => ''),
        ),
        'disabled' => array(
          ':input[name^="map_fields[' . $fname . '][picklist]"]' => $multiple ? array('select_not_empty' => TRUE) : array('!value' => ''),
        ),
      );
    }

    // Do we have any valid filters for this data type?
    if ($include_filter) {
      $filter_options = salesforce_webforms_filter_options($filters, $fdata['type']);
      if (count($filter_options) > 0) {
        $form['map']['map_fields'][$fname]['filter'] = array(
          '#type' => 'select',
          '#title' => t('Apply a filter to the data'),
          '#options' => array('' => t('--None--')) + $filter_options,
          '#description' => t('Any selected filter will be applied after all tokens have been replaced.'),
        );
      }
    }
    $form['map']['map_fields'][$fname]['keyfield'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Key field?'),
      '#options' => array('1' => t('Yes')),
    );
  }

  // Set default values if available.
  if ($map) {
    foreach ($map['fields'] as $field) {
      $form['map']['map_fields'][$field['sf_fieldname']]['#collapsed'] = FALSE;
      $form['map']['map_fields'][$field['sf_fieldname']]['target']['#default_value'] = $field['source_data'];
      if (isset($form['map']['map_fields'][$field['sf_fieldname']]['picklist'])) {
        $form['map']['map_fields'][$field['sf_fieldname']]['picklist']['#default_value'] = $multiple ? explode(';', $field['source_data']) : array($field['source_data']);
      }
      $form['map']['map_fields'][$field['sf_fieldname']]['keyfield']['#default_value'] = $field['keyfield'] ? array('1') : array();
      if ($include_filter) {
        $form['map']['map_fields'][$field['sf_fieldname']]['filter']['#default_value'] = isset($field['filter']) ? array($field['filter']) : array();
      }
    }
  }

  $form['map']['submit'] = array(
    '#type' => 'submit',
    '#value' => (isset($map['mapid'])) ? t('Save map') : t('Add map'),
    '#weight' => 25,
  );

  $form['#attached']['js'][] = drupal_get_path('module', 'salesforce_webforms') . '/js/salesforce-admin.js';

  return $form;
}

/**
 * Form validation handler for salesforce_webforms_manage_map().
 *
 * @see salesforce_webforms_manage_map()
 */
function salesforce_webforms_manage_map_validate($form, &$form_state) {
  // Make sure that the map name is unique and formatted properly.
  $name = $form_state['values']['mapname'];
  $mapid = $form_state['values']['mapid'];
  $nid = $form_state['values']['nid'];

  // First make sure that the name is valid.
  if (!preg_match('/^[a-z0-9_]+$/i', $name)) {
    form_set_error('name',
      t('The map name %mapname is invalid. Please include only alphanumeric characters and underscores.',
      array(
        '%mapname' => $name,
      ))
    );

    // No need to process further if the format is invalid.
    return;
  }

  // Now make sure the key isn't duplicated.
  $maps = salesforce_webforms_get_node_maps($nid);
  foreach ($maps as $map) {
    if ($map['mapname'] == $name && $map['mapid'] != $mapid) {
      form_set_error('name',
        t('The map name %mapname is already in use. Please use a unique name.',
        array(
          '%mapname' => $name,
        ))
      );

      // No need to continue processing.
      return;
    }
  }
}

/**
 * Form submission handler for salesforce_webforms_manage_map().
 *
 * @see salesforce_webforms_manage_map()
 */
function salesforce_webforms_manage_map_submit($form, &$form_state) {
  $values = $form_state['values'];
  salesforce_webforms_map_save($values);
}

/**
 * Returns an array of filter options valid for the given data type.
 *
 * @param array $filters
 *   An array of all defined filters.
 * @param string $datatype
 *   The current Salesforce data type.
 *
 * @return array
 *   An array, suitable for use as an '#options' element in a form.
 */
function salesforce_webforms_filter_options($filters, $datatype) {
  $options = array();

  foreach ($filters as $key => $filter) {
    if (!isset($filter['field types']) || in_array($datatype, $filter['field types'])) {
      $options[$key] = $filter['label'];
    }
  }

  return $options;
}

/**
 * Form constructor for the configuration form.
 *
 * Allows the site administrator to manage the frequency of cache refreshes.
 *
 * @see salesforce_webforms_config_submit()
 *
 * @ingroup forms
 */
function salesforce_webforms_config($form, $form_state) {
  $form = array(
    'salesforce_webforms_cron_min' => array(
      '#type' => 'textfield',
      '#title' => t('Minimum cache refresh time'),
      '#description' => t('The minimum amount of time before cron will force a cache clear.'),
      '#element_validate' => array('element_validate_integer'),
      '#required' => TRUE,
      '#size' => 5,
      '#default_value' => variable_get('salesforce_webforms_cron_min', 60),
      '#field_suffix' => t('Minutes'),
    ),
    'salesforce_webforms_all_picklists' => array(
      '#type' => 'radios',
      '#title' => t('Filter available picklists'),
      '#options' => array(
        SALESFORCE_WEBFORMS_INCLUDE_ALL => t('Allow all picklists'),
        SALESFORCE_WEBFORMS_INCLUDE_FILTERED => t('Limit the available picklists'),
      ),
      '#default_value' => variable_get('salesforce_webforms_all_picklists', 1),
    ),
    'salesforce_webforms_picklists' => array(
      '#type' => 'select',
      '#title' => t('Available picklists'),
      '#description' => t('Select all picklists which should be available to form administrators'),
      '#multiple' => TRUE,
      '#default_value' => variable_get('salesforce_webforms_picklists', array()),
      '#states' => array(
        'visible' => array(
          ':input[name="salesforce_webforms_all_picklists"]' => array('value' => '2'),
        ),
        'hidden' => array(
          ':input[name="salesforce_webforms_all_picklists"]' => array('value' => '1'),
        ),
      ),
      '#options' => salesforce_webforms_get_picklist_options(FALSE),
    ),
    'salesforce_webforms_all_objects' => array(
      '#type' => 'radios',
      '#title' => t('Filter available objects'),
      '#options' => array(
        SALESFORCE_WEBFORMS_INCLUDE_ALL => t('Allow all objects'),
        SALESFORCE_WEBFORMS_INCLUDE_FILTERED => t('Limit the available objects'),
      ),
      '#default_value' => variable_get('salesforce_webforms_all_objects', 1),
    ),
    'salesforce_webforms_objects' => array(
      '#type' => 'select',
      '#title' => t('Available objects'),
      '#description' => t('Select all objects which should be available to form administrators'),
      '#multiple' => TRUE,
      '#options' => salesforce_webforms_get_objects_selection(),
      '#default_value' => variable_get('salesforce_webforms_objects', array()),
      '#states' => array(
        'visible' => array(
          ':input[name="salesforce_webforms_all_objects"]' => array('value' => '2'),
        ),
        'hidden' => array(
          ':input[name="salesforce_webforms_all_objects"]' => array('value' => '1'),
        ),
      ),
    ),
  );

  return system_settings_form($form);
}

/**
 * Helper function to convert a Saleforce picklist into an options array.
 *
 * @param array $picklist
 *   An array of data from Salesforce defining the picklist.
 *
 * @return array
 *   An array suitable for use as an 'options' element in a form.
 */
function salesforce_webforms_picklist_select(array $picklist, $include_select = TRUE) {
  if ($include_select) {
    $options = array('' => t('- Select -'));
  }
  else {
    $options = array();
  }

  foreach ($picklist as $pick) {
    if ($pick['active']) {
      $options[$pick['value']] = $pick['label'];
    }
  }

  return $options;
}

/**
 * Gets all webform components on a node as an array keyed by cid.
 *
 * @param object $node
 *   The node which owns the form.
 *
 * @return array
 *   An associative array mapping the component ID to the name.
 */
function salesforce_webforms_get_webform_components($node) {
  $ret = array();
  $components = $node->webform['components'];
  foreach ($components as $cid => $component) {
    $ret[$cid] = $component['name'];
  }

  return $ret;
}
