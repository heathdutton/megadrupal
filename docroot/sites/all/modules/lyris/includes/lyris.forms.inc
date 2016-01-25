<?php
/**
 * @file
 * Content management forms for handling entities, bundles and fields.
 */
/**
 * List Default Settings.
 * FORM
 */
function lyris_form_defaults($form, &$form_state, $struct_name, $obj_type, $parent_entity = NULL) {
  $form = array();
  $entity_type = "lyris_{$obj_type}";

  // Get the parent entity's eid.
  if (is_object($parent_entity)) {
    drupal_set_title(t('Default !name Values for !title', array('!name' => ucwords($obj_type), '!title' => $parent_entity->title)));
    $parent_eid = $parent_entity->eid;
  }
  else {
    drupal_set_title(t('Global Lyris !name Default Values', array('!name' => ucwords($obj_type))));
    $parent_eid = 0;
  }

  // Create an instance of the entity we're defaulting.
  $entity = lyris_entity_create($entity_type, $parent_eid);

  // Store the list so we don't have to build it again in the submit handler.
  $form_state['entity']      = $entity;
  $form_state['struct_name'] = $struct_name;

  // Get the list from the struct data.
  $struct = lyris_struct($struct_name, 'defaults_form', lyris_api_version(), $entity);

  // Set field permissions for all roles
  $entity->populatePerms(array(), $parent_eid);

  foreach (element_children($struct) as $child) {
    _lyris_form_defaults_process_field($struct[$child], $child, $entity);
  }

  $form += $struct;
  $form['#attached']['css'][] = LYRIS_MOD_PATH . '/lyris.css';
  $form['#attached']['js'][]  = LYRIS_MOD_PATH . '/js/lyris.js';

  $form['parent_eid'] = array(
    '#type' => 'value',
    '#value' => $parent_eid,
  );

  // Add a token tree at the bottom of each defaults form.
  $form['lyris_token_ref'] = array(
    '#type' => 'lyris_token_fieldset',
    'token_tree' => array(
      '#theme' => 'token_tree',
      '#token_types' => array('lyris_list', 'lyris_content'),
    ),
  );


  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save !type Defaults', array('!type' => ucwords($obj_type))),
  );

  return $form;
}

/**
 * Helper function to add role permssions to each field.
 */
function _lyris_form_defaults_process_field(&$field, $name, $entity) {
  // Handle the vertical tabs
  if ($field['#type'] == 'vertical_tabs') {
    foreach (element_children($field) as $child) {
      _lyris_form_defaults_process_field($field[$child], $child, $entity);
    }
  }
  // Run through each vertical tab fieldset.
  elseif ($field['#type'] == 'group' || ($field['#type'] == 'fieldset' && $field['#tab'])) {
    $field['#theme'] = array('lyris_form_default_perms_table');
    foreach (element_children($field) as $child) {
      _lyris_form_defaults_process_field($field[$child], $child, $entity);
    }
  }
  else {
    // Remove requirements from all default fields.  They only need to apply to
    // the true data entry forms.
    if ($field['#required']) {
      $field['#required'] = FALSE;
      $field['#description'] = '<span class="form-required">(Required by Lyris)</span> ' . $field['#description'];
    }

    // Alter the type and value for fields marked as not defaultable.
    if (!$field['#defaultable']) {
      $field['#type'] = 'item';
      $field['#markup'] = 'Not Defaultable';
      $field['#element_validate'] = array();
      unset($field['#field_prefix']);
    }

    // Separate the struct field and add perms fields for each role.
    $new_field = array();
    $new_field['#tree'] = TRUE;
    $new_field['#weight'] = $field['#weight'];
    $new_field['site_default'] = $field;
    $new_field['perms'] = array();

    // Build the $ops array for field permissions per role.
    $ops = array();
    if ($field['#permission']['hide']) {
      $ops[0] = t('Hidden');
    }
    if ($field['#permission']['read']) {
      $ops[1] = t('View');
    }
    if ($field['#permission']['write']) {
      $ops[2] = t('Edit');
    }

    foreach (user_roles() as $rid => $role) {
      // Set the default value to the current setting.
      if (array_key_exists($name, $entity->field_perms) && array_key_exists($rid, $entity->field_perms[$name])) {
        $default = $entity->field_perms[$name][$rid]['status'];
      }
      // If there is no current setting and only one option available, set that option.
      elseif (count($ops) == 1 || !array_key_exists(LYRIS_FIELD_PERM_DEFAULT, $ops)) {
        $_ops = array_keys($ops);
        $default = $_ops[0];
      }
      // Otherwise, use a global default value.
      else {
        $default = LYRIS_FIELD_PERM_DEFAULT;
      }

      $new_field['perms'][$rid] = array(
        '#type' => 'radios',
        '#options' => $ops,
        '#default_value' => $default,
      );
    }

    $field = $new_field;
  }
}

/**
 * Theme tables into each fieldset/tab.
 */
function theme_lyris_form_default_perms_table(&$vars) {
  $form = &$vars['form'];

  $rows = array();
  foreach (element_children($form) as $child) {
    $row = array();
    $row[] = drupal_render($form[$child]['site_default']);
    foreach (element_children($form[$child]['perms']) as $perm) {
      $def = $form[$child]['perms'][$perm]['#default_value'];
      $row[] = array('class' => 'status-' . $def, 'data' => drupal_render($form[$child]['perms'][$perm]));
    }
    $rows[] = $row;
  }

  $headers = array(t('Default Settings'));
  $headers += user_roles();

  return theme('table', array('rows' => $rows, 'header' => $headers, 'attributes' => array('class' => array('lyris-field-perms'))));
}

/**
 * List Default Settings.
 * SUBMIT
 *
 * With about 100 ListStruct fields multiplied by the number of roles on a
 * given system, we have the potential of a completely unreasonable number of
 * database calls to update these defaults and permissions.  Rather than
 * insert or update all values, we'll compare the submitted value to the
 * previously loaded value and only insert new records and update changed
 * records.
 */
function lyris_form_defaults_submit($form, &$form_state) {
  $vals   = $form_state['values'];
  $entity = $form_state['entity'];
  $struct = $form_state['struct_name'];

  // Cycle through all valid struct fields.
  foreach (lyris_struct($struct, 'local', $entity->api_version) as $field => $value) {
    // If the valid field exists in the submitted values.
    if (array_key_exists($field, $vals) && is_array($vals[$field])) {
      // Compare the default values.
      if (array_key_exists('site_default', $vals[$field]) && $entity->localVal($field) != $vals[$field]['site_default']) {
        $record = array(
          'struct'        => $struct,
          'name'          => $field,
          'parent_eid'    => $vals['parent_eid'],
          'default_value' => serialize($vals[$field]['site_default']),
        );

        // Update the existing default value.
        if (array_key_exists($field, $entity->field_defaults) && $record['parent_eid'] == $entity->field_defaults[$field]['parent_eid']) {
          $record['sfid'] = $entity->field_defaults[$field]['sfid'];

          // If the new value is the same as the last inheritted value, delete
          // this record instead of creating a new default value that is
          // overriding the exact same value.
          if ($vals[$field]['site_default'] == $entity->field_defaults[$field]['previous']) {
            $query = db_delete(LYRIS_DEFAULTS_TABLE)->condition('sfid', $record['sfid'])->execute();
          }
          else {
            drupal_write_record(LYRIS_DEFAULTS_TABLE, $record, 'sfid');
          }
        }
        // Insert a new default value.
        else {
          drupal_write_record(LYRIS_DEFAULTS_TABLE, $record);
        }
      }

      // Cycle through the permissions for each field-role combination.
      foreach ($vals[$field]['perms'] as $rid => $status) {
        $perm = array(
          'struct'     => $struct,
          'name'       => $field,
          'parent_eid' => $vals['parent_eid'],
          'rid'        => $rid,
          'status'     => $status,
        );

        // If the permission hasn't been stored and is not the default, insert
        // the record.
        if ((!array_key_exists($field, $entity->field_perms) || !array_key_exists($rid, $entity->field_perms[$field]) || $entity->field_perms[$field][$rid]['parent_eid'] != $perm['parent_eid']) && $status != LYRIS_FIELD_PERM_DEFAULT) {
          drupal_write_record(LYRIS_PERM_TABLE, $perm);
        }
        // If the permission has been set and has changed, update the record.
        elseif (array_key_exists($field, $entity->field_perms) && array_key_exists($rid, $entity->field_perms[$field]) && $entity->field_perms[$field][$rid]['parent_eid'] == $perm['parent_eid'] && ($status !== $entity->field_perms[$field][$rid]['status'])) {

          // If the new value is the same as the last inheritted value, delete
          // this record instead of creating a new default value that is
          // overriding the exact same value.
          if ($status === $entity->field_perms[$field][$rid]['previous']) {
            $query = db_delete(LYRIS_PERM_TABLE)
                   ->condition('struct', $struct)
                   ->condition('name', $field)
                   ->condition('parent_eid', $vals['parent_eid'])
                   ->condition('rid', $rid)
                   ->execute();
          }
          else {
            drupal_write_record(LYRIS_PERM_TABLE, $perm, array('struct', 'name', 'parent_eid', 'rid'));
          }
        }
      }
    }
  }

  drupal_set_message(t('Field defaults have been set.'));
}

/**
 * LIST CRUD: Create/Edit (new).
 * FORM
 * Create a new Lyris List.
 */
function lyris_form_list($form, &$form_state, $list = NULL) {
  $form['#list_prefix'] = '';

  // @TODO: detect whether the active user has permission to create/update lists
  // in lyris.

  if (!$list || !$list->localVal('ListName')) {
    // If this is a new list, get list defaults.
    $list = lyris_list();
    $list->setLocalVal('DateCreated', time());
    $form['#list_prefix'] = lyris_list_prefix();
    drupal_set_title(t('Create a New List'));
  }
  else {
    drupal_set_title(t('Edit @name', array('@name' => $list->title)));
  }

  $list->old_name = $list->localVal('ListName');

  $form['#redirect'] = 'admin/structure/lyris/list';
  $form['#list'] = $list;

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#attached' => array(
      'js'  => array(LYRIS_MOD_PATH . '/js/list-name-validate.js'),
      'css' => array(LYRIS_MOD_PATH . '/js/lyris.css'),
    ),
    '#default_value' => $list->title,
    '#attributes' => array('class' => array('lyris-validate-name')),
    '#suffix' => '<div id="list-name-validate"></div>',
    '#weight' => -99,
    '#required' => TRUE,
  );

  // If running in offline mode, add a field to determine which server this
  // list will be synced with.
  if (variable_get('lyris_mode') == LYRIS_MODE_OFFLINE) {
    $ops = _lyris_options('lyris_mode');
    unset($ops[LYRIS_MODE_OFFLINE]);

    $form['lyris_mode'] = array(
      '#type' => 'radios',
      '#title' => t('Lyris Mode'),
      '#description' => t('Which Lyris server should this list be synced with then Lyris becomes available?'),
      '#options' => $ops,
      '#required' => TRUE,
      '#default_value' => $list->server,
    );
  }

  // List Settings
  // Controls the Lyris Struct data.
  $struct = lyris_struct('ListStruct', 'entity_form', lyris_api_version(), $list);

  // Make sure the ListName field is editable so the machine name field will
  // work.
  if ($list->is_new) {
    $struct[llf('ListName')]['#type'] = 'machine_name';
    $struct[llf('ListName')]['#field_prefix'] = lyris_list_prefix();
    unset($struct[llf('ListName')]['#value']);
  }

  // Add the appropriate struct fields to the form.
  $form += $struct;

  // Add a field to hold template markup that will be prepoulated into the
  // content body field for each new content.
  $form['template'] = array(
    '#type' => 'text_format',
    '#title' => t('Content Template'),
    '#description' => t('Create a template to prepopulate each new content.'),
    '#default_value' => $list->template,
    '#format' => $list->template_format,
  );

  // Add tokens
  $form['lyris_token_ref'] = array(
    '#type' => 'lyris_token_fieldset',
    '#weight' => 98,
    'token_tree' => array(
      '#theme' => 'token_tree',
      '#token_types' => array('lyris', 'lyris_mailing'),
    ),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save List'),
    '#weight' => 99,
  );

  if (isset($list->eid) && $list->eid != 0) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete List'),
      '#weight' => 45,
    );
  }

  return $form;
}

/**
 * LIST CRUD: Create/Edit (new).
 * ELEMENT VALIDATE
 *
 * Validate that a lyris list name is not already taken.
 * We use this over the default machine_name 'exists' function to customize
 * the response to the user.
 */
function lyris_list_name_unique_validate($element) {
  if (isset($element['#value'])) {
    if (($element['#default_value'] != $element['#value']) && lyris_list_name_exists($element['#field_prefix'] . $element['#value'])) {
      form_error($element, t('The list name %name is already in use.  Please choose another name or try !link.', array('%name' => $element['#field_prefix'] . $element['#value'], '!link' => l('importing the existing list', 'admin/structure/lyris/import'))));
    }
  }
}

/**
 * LIST CRUD: Create/Edit (new).
 * VALIDATE
 * Create a new Lyris List.
 */
function lyris_form_list_validate($form, &$form_state) {
  // Format the listname with the prefix.
  if (isset($form_state['values'][llf('ListName')])) {
    $form_state['values'][llf('ListName')] = $form['#list_prefix'] . $form_state['values'][llf('ListName')];
  }
}

/**
 * LIST CRUD: Create (new).
 * SUBMIT
 * Create a new Lyris List.
 */
function lyris_form_list_submit($form, &$form_state) {
  $vals = $form_state['values'];
  $op = isset($vals['op']) ? $vals['op'] : '';

  // Get the list object from the initial form.  It is either an empty shell of
  // a list of a pre-populated list from the list-management form.
  $list = $form['#list'];

  // Process the delete request.
  if ($op == t('Delete List')) {
    $form_state['redirect'] = 'admin/structure/lyris/manage/' . $list->eid . '/delete';
    return;
  }

  // Connect to the proper Lyris server for this list.
  $lyris = lyris(NULL, $list->server);

  // Set non-lyris values from the form fields.
  $list->server = (isset($vals['lyris_mode']) ? $vals['lyris_mode'] : $lyris->mode);
  $list->title = $vals['title'];
  $list->template = $vals['template']['value'];
  $list->template_format = $vals['template']['format'];

  // Prepare a full struct with default data.
  $list->populateStruct();

  // Merge the submitted Lyris form fields.
  $list->mergeObjectData($vals);
  $list->mergeStructData($vals);

  // Push to Lyris first so we can get a ListID if this is a new list.
  $lyris_status = $list->pushToLyris();
  $local_status = $list->save();

  // Set local messages.
  switch ($local_status) {
    case SAVED_NEW:
      lyris_create_field('lyris_content_docparts', TRUE);
      lyris_create_field_instance('lyris_content_docparts', 'lyris_content', $list->localVal('ListName'));

    case SAVED_UPDATED:
      drupal_set_message(t('List %title saved successfully.', array('%title' => $list->title)));
      break;

    default:
      drupal_set_message(t('Unable to save list %title.', array('%title' => $list->title)), 'error');
      break;
  }

  // Set messages about remote push.
  switch ($lyris_status) {
    case SAVED_NEW:
    case SAVED_UPDATED:
      drupal_set_message(t('List %name saved to Lyris successfully.', array('%name' => $list->structVal('ListName'))));
      break;

    default:
      drupal_set_message(t('Unable to save list %name to Lyris.', array('%name' => $list->structVal('ListName'))), 'error');
      break;
  }

  $form_state['redirect'] = 'admin/structure/lyris';
  return;
}

/**
 * LIST CRUD: Create (import).
 * FORM
 * Link to an existing Lyris list.
 */
function lyris_form_list_import($form, &$form_state) {
  $account = lyris_global_user();
  drupal_set_title(t('Import a List From Lyris'));

  if (!lyris_remote_access('UpdateList', $account)) {
    drupal_set_message(t('You do not have sufficient Lyris privileges to import a list.  Make sure !link are correct or have an administrator check your Lyris permissions.', array('!link' => l('your Lyris credentials', "user/{$account->uid}/edit", array('query' => drupal_get_destination())))), 'warning');
    drupal_goto('admin/structure/lyris');
  }

  $form['description'] = array(
    '#type' => 'markup',
    '#markup' => t('Select the list you want to import.<br />Available lists are those administered by you or by the site admin and that have not yet been imported.<br />If the mailing list you wish to import is not available, you may not be listed as an administrator.  Your Lyris admin must add you as an administrator before you can import the list.'),
    '#prefix' => '<p>',
    '#suffix' => '</p>',
  );

  if (variable_get('lyris_mode_display')) {
    $form['description']['#suffix'] .= theme('lyris_mode_indicator');
  }

  $form['ListName'] = array(
    '#type' => 'textfield',
    '#title' => t('Lyris List Name'),
    '#description' => t('Begin typing the name of a list on possible options will be provided.'),
    '#autocomplete_path' => 'lyris/ajax/list/name-autocomplete',
    '#element_validate' => array('lyris_validate_machine_name'),
    '#required' => TRUE,
  );
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('List Title'),
    '#description' => t('The human-readable name of this list.'),
    '#required' => TRUE,
    '#maxlength' => 128,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Import List'),
  );

  return $form;
}

/**
 * LIST CRUD: Create (import).
 * VALIDATE
 * Link to an existing Lyris list.
 */
function lyris_form_list_import_validate($form, &$form_state) {
  $ListName = $form_state['values']['ListName'];

  // Make sure this list hasn't been linked yet.
  $sel = db_select(LYRIS_LIST_TABLE, 'l')
    ->fields('l')
    ->condition('l.' . llf('ListName'), $ListName);
  $cnt = $sel->execute()->rowCount();

  if ($cnt > 0) {
    form_set_error('lyris_name', t('The list %name is already linked to Lyris.', array('%name' => $ListName)));
  }
}

/**
 * LIST CRUD: Create (Import).
 * SUBMIT
 * Link to an existing Lyris list.
 */
function lyris_form_list_import_submit($form, &$form_state) {
  $lyris = lyris();
  $ListName = $form_state['values']['ListName'];

  // Instantiate a new list with our default values.
  $list = lyris_list();

  // Load the list from Lyris and populate our local values.
  $struct = $lyris->loadList(array('ListName' => $ListName));
  $list->mergeObjectData($struct);

  $list->populateStruct();
  $list->mergeStructData($struct);

  // Save the list into Drupal
  $list->title  = $form_state['values']['title'];
  $list->server = $lyris->mode;
  $list->origin = LYRIS_LIST_IMPORT;

  if ($list->save()) {
    $list->setSynced(SAVED_NEW);
    lyris_create_field('lyris_content_docparts', TRUE);
    lyris_create_field_instance('lyris_content_docparts', 'lyris_content', $list->localVal('ListName'));
    drupal_set_message(t('List %name saved successfully.', array('%name' => $list->title)));
  }
  else {
    drupal_set_message(t('There was an error saving list %name.', array('%name' => $list->title)), 'error');
  }

  $form_state['redirect'] = 'admin/structure/lyris';
  return;
}

/**
 * List CRUD: Confirm Delete
 * FORM
 *
 * Confirm the deletion of a list.
 */
function lyris_list_delete_confirm($form, &$form_state, $list) {
  if (lyris_list_name_exists($list->localVal('ListName'))) {
    $form['delete_lyris'] = array(
      '#type' => 'checkbox',
      '#title' => t('Delete list and content from Lyris as well?'),
      '#description' => t('Do you want to delete %name from the Lyris server?  ALL LIST DATA WILL BE COMPLETELY DESTROYED AND CANNOT BE RECOVERED.', array('%name' => $list->localVal('ListName'))),
    );
  }
  $form['list'] = array(
    '#type' => 'value',
    '#value' => $list,
  );

  return confirm_form(
    $form,
    t('Remove list %name?', array('%name' => $list->title)),
    'admin/structure/lyris/list',
    t('This will remove the list %name and all its content from your site but not from Lyris UNLESS you check the box above.', array('%name' => $list->title)),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * List CRUD: Confirm Delete
 * SUBMIT
 *
 * Confirm the deletion of a list.
 */
function lyris_list_delete_confirm_submit($form, &$form_state) {
  $vals = $form_state['values'];
  $list = $vals['list'];
  $args = array('%name' => $list->localVal('ListName'), '%title' => $list->title);

  $ListName = $list->localVal('ListName');

  // Delete the list(s) and display the appropriate message depending on whether
  // we are expecing both or just the local to be deleted.
  entity_delete('lyris_list', $list->eid);

  // Delete the content associated with this list.
  $select = db_select(LYRIS_CONTENT_TABLE, 'lc')->fields('lc', array('eid'))->condition(llf('ListName'), $ListName)->execute()->fetchAllAssoc('eid');
  $eids = array_keys($select);

  if (!empty($eids)) {
    entity_delete_multiple('lyris_content', $eids);
  }

  drupal_set_message(t('List %title successfully removed.', $args));

  // If we are deleting the list from Lyris too.
  if (isset($vals['delete_lyris']) && $vals['delete_lyris']) {
    $lyris = lyris(NULL, $list->server);
    if ($lyris->deleteList($list->localVal('ListName'))) {
      drupal_set_message(t('List %name successfully deleted from Lyris.', $args));
    }
    else {
      drupal_set_message(t('An error occurred.  Unable to delete list %name from Lyris.', $args), 'error');
    }
  }

  drupal_goto('admin/structure/lyris/list');
}

/**
 * Lyris Content CrUD
 * FORM
 * The base form for all lyris content entry.
 */
function lyris_content_form($form, &$form_state, $lc) {
  $form = array();

  // During initial form build, add the $lyris_content entity to the form state
  // for use during form building and processing. During a rebuild, use what is
  // in the form state.
  if (!isset($form_state['lyris_content'])) {
    $form_state['lyris_content'] = $lc;
  }
  else {
    $lc = $form_state['lyris_content'];
  }

  // Set some defaults from the list if this is a new Lyris Content.
  if (!$lc->eid) {
    $list = lyris_list_load($lc->localVal('ListName'));
    $lc->setLocalVal('HeaderTo', $list->localVal('DefaultTo'));
    $lc->setLocalVal('HeaderFrom', $list->localVal('DefaultFrom'));
  }

  // Append required Lyris fields
  $form += lyris_struct('ContentStruct', 'entity_form', lyris_api_version(), $lc);

  // Create Action Buttons
  $form['#validate'][] = 'lyris_content_form_validate';
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 5,
    '#submit' => array('lyris_content_form_submit'),
  );
  if (!empty($lc->eid) && lyris_content_access('delete', $lc)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#weight' => 15,
      '#submit' => array('lyris_content_form_delete_submit'),
    );
  }

  if (!isset($form['#submit']) && function_exists($lc->localVal('ListName') . '_content_form_submit')) {
    $form['#submit'][] = $lc->localVal('ListName') . '_content_form_submit';
  }
  $form += array('#submit' => array());

  // Add fields from the Field API to the form.
  field_attach_form('lyris_content', $lc, $form, $form_state);

  // Prepopulate the form body field with the template values set in the list.
  if (!$lc->eid && $list) {
    $form['lyris_content_docparts'][$form['lyris_content_docparts']['#language']][0]['#default_value'] = $list->template;
    $form['lyris_content_docparts'][$form['lyris_content_docparts']['#language']][0]['#format'] = $list->template_format;
  }

  // Add the token reference after the body.
  // Make sure the field hasn't been deleted.
  // @todo: This field should be locked down so it cannot be deleted.
  if (isset($form['lyris_content_docparts'])) {
    $weight = (int) $form['lyris_content_docparts']['#weight'] + .1;
    $form['lyris_token_ref'] = array(
      '#type' => 'lyris_token_fieldset',
      'token_tree' => array(
        '#theme' => 'token_tree',
        '#token_types' => array('lyris', 'lyris_list', 'lyris_content', 'lyris_mailing'),
        '#weight' => $weight,
      ),
    );
  }

  return $form;
}

/**
 * Content Form
 * VALIDATE
 */
function lyris_content_form_validate($form, &$form_state) {
  $vals = $form_state['values'];
  $lc = $form_state['lyris_content'];

  // Title must be unique within the same List
  $exists = db_select(LYRIS_CONTENT_TABLE, 'lc')
              ->fields('lc')
              ->condition(llf('Title'), $vals[llf('Title')])
              ->condition(llf('ListName'), $lc->localVal('ListName'));


  if ($lc->eid) {
    $exists->condition('eid', $lc->eid, '<>');
  }

  $res = $exists->execute();

  if ($res->rowCount() > 0) {
    form_set_error('Title', t('The title %title is already taken.  Titles must be unique within their respective lists.', array('%title' => $vals[llf('Title')])));
  }
}

/**
 * Content Form
 * SUBMIT
 */
function lyris_content_form_submit($form, &$form_state) {
  $vals = $form_state['values'];

  $lc = $form_state['lyris_content'];
  entity_form_submit_build_entity('lyris_content', $lc, $form, $form_state);

  // Prepare a full struct with default data.
  $lc->populateStruct();

  // If the title has not changed, remove it from the struct or Lyris with
  // toss back an error that the name already exists.
  if ($lc->structVal('ContentID') && $vals[llf('Title')] == $lc->structVal('Title')) {
    unset($lc->struct['Title']);
  }

  // Merge the submitted Lyris form fields.
  $lc->mergeObjectData($vals);
  $lc->mergeStructData($vals);

  // We need a method to allow modules to customize the method for generating
  // the content body parts.  By default we will use the lyris_content_docpart
  // field that we create, but what if someone wants to incorporate more fields
  // or they remove the body field?
  $docparts = lyris_docparts_from_textfield($lc, 'lyris_content_docparts');
  $docparts += module_invoke_all('lyris_content_docparts', $lc, $lc->localVal('ListName'));
  foreach($docparts as $docpart) {
    $lc->addDocPart($docpart);
  }

  // Save the content locally first so any alteration hooks are implemented
  // before we push to Lyris.
  $local_status = $lc->save();
  $lyris_status = $lc->pushToLyris();

  // Set local messages.
  switch ($local_status) {
    case SAVED_NEW:
    case SAVED_UPDATED:
      drupal_set_message(t('Content %title saved successfully.', array('%title' => $lc->localVal('Title'))));
      break;

    default:
      drupal_set_message(t('Unable to save content %title.', array('%title' => $lc->localVal('Title'))), 'error');
      break;
  }

  // Set messages about remote push.
  switch ($lyris_status) {
    case SAVED_NEW:
    case SAVED_UPDATED:
      drupal_set_message(t('Content %title saved to Lyris successfully.', array('%title' => $lc->localVal('Title'))));
      break;

    default:
      drupal_set_message(t('Unable to save content %title to Lyris.', array('%title' => $lc->localVal('Title'))), 'error');
      break;
  }

  if ($lc->eid) {
    $form_state['redirect'] = 'lyris/content/' . $lc->eid;
  }
}

/**
 * Content Form
 * DELETE REDIRECT
 */
function lyris_content_form_delete_submit($form, &$form_state) {
  $destination = array();
  if (isset($_GET['destination'])) {
    $destination = drupal_get_destination();
    unset($_GET['destination']);
  }
  $lyris_content = $form_state['lyris_content'];
  $form_state['redirect'] = array('lyris/content/' . $lyris_content->eid . '/delete', array('query' => $destination));
}

/**
 * Content Form
 * DELETE CONFIRM
 */
function lyris_content_delete_confirm($form, &$form_state, $lyris_content) {
  $form_state['lyris_content'] = $lyris_content;

  $form['delete_lyris'] = array(
    '#type' => 'checkbox',
    '#title' => t('Delete content from Lyris as well?'),
    '#description' => t('Do you want to delete %title from the Lyris server?  ALL LIST DATA WILL BE COMPLETELY DESTROYED AND CANNOT BE RECOVERED.', array('%title' => $lyris_content->localVal('Title'))),
  );

  // Always provide entity id in the same form key as in the entity edit form.
  $form['eid'] = array('#type' => 'value', '#value' => $lyris_content->eid);
  return confirm_form($form,
    t('Are you sure you want to delete %title?', array('%title' => $lyris_content->localVal('Title'))),
    'lyris/' . $lyris_content->eid,
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Content Form
 * DELETE SUBMIT
 */
function lyris_content_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    $lyris_content = lyris_content_load($form_state['values']['eid']);
    lyris_content_delete($form_state['values']['eid'], $form_state['values']['delete_lyris']);

    $args = array('@list' => $lyris_content->localVal('ListName'), '%title' => $lyris_content->localVal('Title'));
    Base::watchdog('@list %title has been deleted.', WATCHDOG_INFO, $args);
    drupal_set_message(t('@list %title has been deleted.', $args));
  }

  $form_state['redirect'] = 'admin/content/lyris';
}

/**
 * Jumplist form to filter the content listing.
 * FORM
 */
function lyris_content_jump_list_form($form, &$form_state, $lists) {
  $ops = array('' => '-- ' . t('All Lists') . ' --');

  foreach ($lists as $list) {
    $ops[$list->eid] = $list->getDisplayName();
  }

  $form['container'] = array(
    '#type' => 'fieldset',
    '#title' => t('Filter Content by List'),
    '#attributes' => array('class' => array('container-inline')),
  );
  $form['container']['jump_to_list'] = array(
    '#type' => 'select',
    '#options' => $ops,
    '#attributes' => array('class' => array('container-inline')),
    '#default_value' => (int) arg(3),
  );
  $form['container']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Filter'),
  );

  return $form;
}

/**
 * Jumplist form to filter the content listing.
 * SUBMIT
 */
function lyris_content_jump_list_form_submit($form, &$form_state) {
  $go = $form_state['values']['jump_to_list'];

  if (!$go) {
    $form_state['redirect'] = 'admin/content/lyris';
  }
  elseif (is_numeric($go)) {
    $form_state['redirect'] = 'admin/content/lyris/' . $go;
  }
}

/**
 * Lyris Mailing Form
 * FORM
 */
function lyris_mailing_form($form, &$form_state, $list, $lc, $mailing) {
  $struct = lyris_struct('MailingStruct', 'entity_form', $mailing->api_version, $mailing);

  $form_state['#list']    = $list;
  $form_state['#lc']      = $lc;
  $form_state['#mailing'] = $mailing;

  $form += $struct;

  // Hide the fields that appear in the preview if they are displayed
  // in the settings.
  unset($form[llf('To')], $form[llf('From')], $form[llf('ReplyTo')]);

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['test'] = array(
    '#type' => 'submit',
    '#value' => t('Test Mailing'),
    '#weight' => 4,
    '#submit' => array('lyris_mailing_form_submit'),
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Send Mailing'),
    '#weight' => 5,
    '#submit' => array('lyris_mailing_form_submit'),
  );

  return $form;
}

/**
 * Lyris Mailing Form
 * SUBMIT
 */
function lyris_mailing_form_submit($form, &$form_state) {
  global $user;
  $vals = $form_state['values'];
  $mailing = $form_state['#mailing'];
  $lc = $form_state['#lc'];

  entity_form_submit_build_entity('lyris_mailing', $mailing, $form, $form_state);

  // Merge in any altered data.
  $mailing->mergeObjectData($vals);

  // Store the mailing with the updated settings so we can pull it in from an
  // argument in the callback confim functions.
  $mailing->save();

  // Repackage the mailing with the new values for the confirm forms.
  //$form_state['#mailing'] = $mailing;

  // Redirect to the proper confirmation form.
  if ($vals['op'] == $vals['test']) {
    $form_state['redirect'] = "lyris/mailing/{$mailing->eid}/test";
  }
  elseif ($vals['op'] == $vals['submit']) {
    $form_state['redirect'] = "lyris/mailing/{$mailing->eid}/send";
  }

  return;
}

/**
 * Lyris Mailing Form
 * CONFIRM
 */
function lyris_mailing_form_send_confirm($form, &$form_state, $mailing) {
  $lc = lyris_content_load_contentID($mailing->ContentID);
  $list = lyris_list_load($mailing->localVal('ListName'));

  $args = array(
    '%mailing'  => $mailing->getDisplayName(),
    '%name'     => $lc->getDisplayName(),
    '%listname' => $list->getDisplayName(),
  );

  $form = array();
  $form_state['#mailing'] = $mailing;
  $form_state['#lc'] = $lc;

  return confirm_form($form,
    t("Deliver '%mailing?'", $args),
    "lyris/content/{$lc->eid}/mailings",
    t('This will deliver the content %name to all subscribed members of %listname.', $args) . '<br />' . t('Do you want to continue?'),
    t('Send'),
    t('Cancel')
  );
}

/**
 * Lyris Mailing Form
 * CONFIRM SUBMIT - SEND THE MAIL
 */
function lyris_mailing_form_send_confirm_submit($form, &$form_state) {
  global $user;

  $mailing = $form_state['#mailing'];
  $lc = $form_state['#lc'];

  $mailing->populateStruct();

  $lyris = lyris(NULL, $mailing->server);
  $result = $lyris->sendMailing($mailing);

  $mailing->InMailID = $result;
  if ($result) {
    $mailing->uid = $user->uid;
    $mailing->send_date = REQUEST_TIME;
  }

  $mailing->save();

  // Log the results
  if (is_numeric($result)) {
    drupal_set_message(t('Mailing sent succesfully!'));
  }
  else {
    drupal_set_message(t('There was an error sending the mailing.'), 'error');
  }

  $form_state['redirect'] = "lyris/content/{$lc->eid}/mailings";
}

/**
 * Lyris Mailing Test Form
 * CONFIRM
 */
function lyris_mailing_form_test_confirm($form, &$form_state, $mailing) {
  $lc = lyris_content_load_contentID($mailing->ContentID);
  $list = lyris_list_load($mailing->localVal('ListName'));

  $args = array(
    '%mailing'  => $mailing->getDisplayName(),
    '%name'     => $lc->getDisplayName(),
    '%listname' => $list->getDisplayName(),
  );

  $form['subject_prefix'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject Prefix'),
    '#description' => t('Add a prefix to the e-mail subject?'),
    '#default_value' => '[' . t('TEST') . ']',
    '#field_suffix' => token_replace($lc->localVal('NativeTitle'), array('lyris_list' => $list, 'lyris_content' => $lc, 'lyris_mailing' => $mailing)),
    '#maxlength' => 32,
    '#size' => 20,
  );

  $form['recipients'] = array(
    '#type' => 'textarea',
    '#title' => t('Test Recipients'),
    '#description' => t('Enter e-mail addresses to test mailing delivery.') . ' <strong>' . t('One per line.') . '</strong><br />',
    '#rows' => 5,
    '#default_value' => variable_get('lyris_mailing_test_recipients'),
  );

  $form_state['#mailing'] = $mailing;
  $form_state['#lc'] = $lc;

  return confirm_form($form,
    t("Test Delivery of '%mailing?'", $args),
    "lyris/content/{$lc->eid}/mailings",
    t('This will deliver the content %name to only the e-mail addresses listed above.', $args) . '<br />' . t('Do you want to continue?'),
    t('Send'),
    t('Cancel')
  );
}

/**
 * Lyris Mailing Test Form
 * CONFIRM SUBMIT - SEND THE TEST MAIL
 */
function lyris_mailing_form_test_confirm_submit($form, &$form_state) {
  global $user;

  $lc = $form_state['#lc'];

  $mailing = $form_state['#mailing'];
  $mailing->populateStruct();

  // Get the test recipients
  $recipients = $form_state['values']['recipients'];
  $emails = explode("\r\n", $recipients);

  // Append a prefix if requested
  $prefix = $form_state['values']['subject_prefix'];
  if ($prefix) {
    $subject = $mailing->structVal('Subject');
    $mailing->setStructVal('Subject', "$prefix  $subject");
  }

  // Send the mailing
  $lyris = Lyris(NULL, $mailing->server);
  $result = $lyris->sendMailingTest($mailing, $emails);

  // Store the mailing result
  $mailing->InMailID = $result;
  if ($result) {
    $mailing->uid = $user->uid;
    $mailing->send_date = REQUEST_TIME;
  }

  $mailing->save();

  // Log the results
  if (is_numeric($result)) {
    drupal_set_message(t('Mailing sent succesfully!'));
  }
  else {
    drupal_set_message(t('There was an error sending the mailing.'), 'error');
  }

  $form_state['redirect'] = "lyris/content/{$lc->eid}/mailings";
}

/**
 * Field Sync Form
 * FORM
 *
 * Return a table with side-by-side comparisons of the remote and local entity
 * field values.
 */
function lyris_entity_field_comparison_form($form, &$form_state, $entity, $remote) {
  $form_state['#entity'] = $entity;
  $form_state['#remote'] = $remote;

  $struct = lyris_struct($entity->struct_type, 'local');
  $rows = array();
  foreach (array_keys($struct) as $field) {
    $field = lrf($field);

    // Create a row for each field whole local value does not match the remote
    // lyris value.
    if (isset($remote[$field]) && !lyris_values_are_equal($entity->localVal($field), $remote[$field], $field)) {
      $row = array(
        $field,
        $entity->localVal($field),
        $remote[$field],
      );
      $rows[$field] = $row;
    }
  }

  $form['description'] = array(
    '#type' => 'markup',
    '#markup' => t('Select the fields to merge from Lyris.'),
  );
  $form['fields'] = array(
    '#type' => 'tableselect',
    '#header' => array(
      t('Field'),
      t('Local Value'),
      t('Lyris Value'),
    ),
    '#options' => $rows,
    '#empty' => t('!name is in sync with Lyris.', array('!name' => $entity->getDisplayName())),
    '#attributes' => array('id' => 'lyris-entity-fields'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Merge Remote Values'),
  );

  return $form;
}

/**
 * Field Sync Form
 * SUBMIT
 *
 * Return a table with side-by-side comparisons of the remote and local entity
 * field values.
 */
function lyris_entity_field_comparison_form_submit($form, &$form_state) {
  $fields = $form_state['values']['fields'];
  $entity = $form_state['#entity'];
  $remote = $form_state['#remote'];

  // Safely merge the remote values into the local entity.
  foreach ($fields as $field => $merge) {
    if ($merge && array_key_exists($field, $remote)) {
      $entity->setLocalVal($field, $remote[$field]);
    }
  }

  if ($entity->save()) {
    drupal_set_message(t('Fields have been updated.'));
  }
  else {
    drupal_set_message(t('There was an error updating the fields.'), 'error');
  }
}

/**
 * Form to quickly populate a struct and push it to Lyris without editing values.
 * FORM
 */
function lyris_entity_quick_push_form($form, &$form_state, $entity) {
  $form_state['#entity'] = $entity;

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update Lyris')
  );

  return $form;
}

/**
 * Form to quickly populate a struct and push it to Lyris without editing values.
 * SUBMIT
 */
function lyris_entity_quick_push_form_submit($form, &$form_state) {
  $entity = $form_state['#entity'];

  $entity->populateStruct();

  if ($entity->pushToLyris()) {
    drupal_set_message(t('!name has been updated.', array('!name' => $entity->getDisplayName())));
  }
  else {
    drupal_set_message(t('There was an error sending data to Lyris.'), 'error');
  }
}
