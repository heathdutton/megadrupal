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
  if (is_object($parent_entity)) {
    drupal_set_title(t('Default !name Values for !title', array('!name' => ucwords($obj_type), '!title' => $parent_entity->title)));
    $parent_eid = $parent_entity->eid;
  }
  else {
    $parent_eid = 0;
  }

  $form = array();
  $obj_func = 'lyris_' . $obj_type;

  // Load an object shell.
  $obj = (function_exists($obj_func) ? $obj_func($parent_eid) : NULL);

  // Store the list so we don't have to build it again in the submit handler.
  $form_state['object_type'] = $obj_type;
  $form_state['object']      = $obj;
  $form_state['struct_name'] = $struct_name;

  // Get the list from the struct data.
  $struct = lyris_struct($struct_name, 'defaults_form', lyris_api_version(), $obj);

  // Set field permissions for all roles
  $obj->populatePerms(array(), $parent_eid);

  foreach (element_children($struct) as $child) {
    _lyris_form_defaults_process_field($struct[$child], $child, $obj);
  }

  $form += $struct;
  $form['#attached']['css'][] = LYRIS_MOD_PATH . '/lyris.css';
  $form['#attached']['js'][]  = LYRIS_MOD_PATH . '/js/lyris.js';

  $form['parent_eid'] = array(
    '#type' => 'value',
    '#value' => $parent_eid,
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
function _lyris_form_defaults_process_field(&$field, $name, $obj) {
  // Handle the vertical tabs
  if ($field['#type'] == 'vertical_tabs') {
    foreach (element_children($field) as $child) {
      _lyris_form_defaults_process_field($field[$child], $child, $obj);
    }
  }
  // Run through each vertical tab fieldset.
  elseif ($field['#type'] == 'group' || ($field['#type'] == 'fieldset' && $field['#tab'])) {
    $field['#theme'] = array('lyris_form_default_perms_table');
    foreach (element_children($field) as $child) {
      _lyris_form_defaults_process_field($field[$child], $child, $obj);
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
    $new_field['site_default'] = $field;
    $new_field['perms'] = array();

    // Build the $ops array for field permissions per role.
    $ops = array();
    $ops[0] = t('Hidden');
    if ($field['#permission']['read']) {
      $ops[1] = t('View');
    }
    if ($field['#permission']['write']) {
      $ops[2] = t('Edit');
    }

    foreach (user_roles() as $rid => $role) {
      $new_field['perms'][$rid] = array(
        '#type' => 'radios',
        '#options' => $ops,
        '#default_value' => (array_key_exists($name, $obj->field_perms) && array_key_exists($rid, $obj->field_perms[$name]) ? $obj->field_perms[$name][$rid]['status'] : LYRIS_FIELD_PERM_DEFAULT),
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
  $obj    = $form_state['object'];
  $struct = $form_state['struct_name'];

  // Cycle through all valid struct fields.
  foreach (lyris_struct($struct, 'local', $obj->api_version) as $field => $value) {
    // If the valid field exists in the submitted values.
    if (array_key_exists($field, $vals) && is_array($vals[$field])) {
      // Compare the default values.
      if (array_key_exists('site_default', $vals[$field]) && $obj->localVal($field) != $vals[$field]['site_default']) {
        $record = array(
          'struct'        => $struct,
          'name'          => $field,
          'parent_eid'    => $vals['parent_eid'],
          'default_value' => serialize($vals[$field]['site_default']),
        );

        // Update the existing default value.
        if (array_key_exists($field, $obj->field_defaults) && $record['parent_eid'] == $obj->field_defaults[$field]['parent_eid']) {
          $record['sfid'] = $obj->field_defaults[$field]['sfid'];

          // If the new value is the same as the last inheritted value, delete
          // this record instead of creating a new default value that is
          // overriding the exact same value.
          if ($vals[$field]['site_default'] == $obj->field_defaults[$field]['previous']) {
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
        if ((!array_key_exists($field, $obj->field_perms) || !array_key_exists($rid, $obj->field_perms[$field]) || $obj->field_perms[$field][$rid]['parent_eid'] != $perm['parent_eid']) && $status != LYRIS_FIELD_PERM_DEFAULT) {
          drupal_write_record(LYRIS_PERM_TABLE, $perm);
        }
        // If the permission has been set and has changed, update the record.
        elseif (array_key_exists($field, $obj->field_perms) && array_key_exists($rid, $obj->field_perms[$field]) && $obj->field_perms[$field][$rid]['parent_eid'] == $perm['parent_eid'] && ($status !== $obj->field_perms[$field][$rid]['status'])) {

          // If the new value is the same as the last inheritted value, delete
          // this record instead of creating a new default value that is
          // overriding the exact same value.
          if ($status === $obj->field_perms[$field][$rid]['previous']) {
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

  // Add the appropriate struct fields to the form.
  $form += $struct;

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save List'),
    '#weight' => 40,
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
      lyris_create_field_instance('lyris_content_body', 'lyris_content', $list->localVal('ListName'));
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
    //lyris_create_field_instance('lyris_content_title', 'lyris_content', $list->ListName);
    lyris_create_field_instance('lyris_content_body', 'lyris_content', $list->localVal('ListName'));
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
  $form['delete_lyris'] = array(
    '#type' => 'checkbox',
    '#title' => t('Delete list and content from Lyris as well?'),
    '#description' => t('Do you want to delete %name from the Lyris server?  ALL LIST DATA WILL BE COMPLETELY DESTROYED AND CANNOT BE RECOVERED.', array('%name' => $list->localVal('ListName'))),
  );
  $form['list'] = array(
    '#type' => 'value',
    '#value' => $list,
  );

  return confirm_form(
    $form,
    t('Remove list %name?', array('%name' => $list->title)),
    'admin/structure/lyris/list',
    t('This will remove the list %name from your site but not from Lyris UNLESS you check the box above.', array('%name' => $list->title)),
    t('Remove'),
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

  // Delete the list(s) and display the appropriate message depending on whether
  // we are expecing both or just the local to be deleted.
  $list->delete();
  drupal_set_message(t('List %title successfully removed.', $args));

  // If we are deleting the list from Lyris too, attempt that first.
  if ($vals['delete_lyris']) {
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

  // Define a vertical tabs group for Lyris Content Ownership
  $form['lyris_content_owner'] = array(
    '#type' => 'vertical_tabs',
    '#weight' => 99,
  );

  // Content author information for administrators
  /*$owner = user_load($lc->uid);
  $form['owner'] = array(
    '#type' => 'fieldset',
    '#access' => user_access('administer lyris content'),
    '#title' => t('Ownership information'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#group' => 'lyris_content_owner',
    '#attributes' => array(
      'class' => array('node-form-author'),
    ),
    '#attached' => array(
      'js' => array(
        drupal_get_path('module', 'node') . '/node.js',
        array(
          'type' => 'setting',
          'data' => array('anonymous' => variable_get('anonymous', t('Anonymous'))),
        ),
      ),
    ),
    '#weight' => 90,
  );
  $form['owner']['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Content Owner'),
    '#maxlength' => 60,
    '#autocomplete_path' => 'user/autocomplete',
    '#default_value' => $owner->name,
    '#weight' => -1,
    '#description' => t('Leave blank for %anonymous.', array('%anonymous' => variable_get('anonymous', t('Anonymous')))),
  );*/

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

  field_attach_form('lyris_content', $lc, $form, $form_state);
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
      drupal_set_message(t('Content %title sent to Lyris successfully.', array('%title' => $lc->localVal('Title'))));
      break;

    default:
      drupal_set_message(t('Unable to send content %title to Lyris.', array('%title' => $lc->localVal('Title'))), 'error');
      break;
  }

  if ($lc->eid) {
    $form_state['redirect'] = 'lyris/' . $lc->eid;
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
  $form_state['redirect'] = array('lyris/' . $lyris_content->eid . '/delete', array('query' => $destination));
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
    '#description' => t('Do you want to delete %title from the Lyris server?  ALL LIST DATA WILL BE COMPLETELY DESTROYED AND CANNOT BE RECOVERED.', array('%title' => $lyris_content->Title)),
  );

  // Always provide entity id in the same form key as in the entity edit form.
  $form['eid'] = array('#type' => 'value', '#value' => $lyris_content->eid);
  return confirm_form($form,
    t('Are you sure you want to delete %title?', array('%title' => $lyris_content->Title)),
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

    $args = array('@list' => $lyris_content->ListName, '%title' => $lyris_content->Title);
    Base::watchdog('@list %title has been deleted.', WATCHDOG_INFO, $args);
    drupal_set_message(t('@list %title has been deleted.', $args));
  }

  $form_state['redirect'] = 'admin/content/lyris';
}

/**
 * Lyris Mailing Form
 * FORM
 */
function lyris_mailing_form($form, &$form_state, $lc) {
  $mailing = lyris_mailing($lc);
  $struct = lyris_struct('MailingStruct', 'entity_form', $mailing->api_version, $mailing);

  $form_state['#mailing'] = $mailing;

  $form += $struct;

  $form['actions'] = array('#type' => 'actions');
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
  $vals = $form_state['values'];
  $mailing = $form_state['#mailing'];

  // Merge in any altered data.
  $mailing->mergeObjectData($vals);
  $mailing->mergeStructData($vals);

  // Send the mailing!
  $lyris = Lyris(NULL, $mailing->server);

  $result = $lyris->sendMailing($mailing);

  $mailing->save(array('InMailID' => $result));

  // Log the results
  if (is_numeric($result)) {
    drupal_set_message(t('Mailing sent succesfully!'));
  }
  else {
    drupal_set_message(t('There was an error sending the mailing.'), 'error');
  }
}
