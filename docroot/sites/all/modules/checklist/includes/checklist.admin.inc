<?php
// $Id$


/**
 * @file
 * Checklist admin pages.
 */

/**
 * The main admin form, to manage general checklist settings.
 */
function checklist_admin_form($form, $form_state) {
  $form['checklist_show_on_login'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show checklist when a user logs in?'),
    '#description' => t('A user will be immediately redirected to their checklist page upon logging in if this setting is enabled. This setting does not affect checklists that force redirect on navgiation.'),
    '#default_value' => variable_get('checklist_show_on_login', TRUE),
  );
  $form['checklist_page_title'] = array(
    '#type' => 'textfield',
    '#title' => t('User checklist page title'),
    '#default_value' => variable_get('checklist_page_title', 'Your checklists'),
    '#description' => t('The title shown on the !checklist page', array('!checklist' => l(t('checklist'), 'checklist'))),
    '#weight' => -5,
  );
  $defaults = variable_get('checklist_page_description', array('value' => '', 'format' => filter_default_format()));
  $form['checklist_page_description'] = array(
    '#type' => 'text_format',
    '#title' => t('User checklist page details'),
    '#description' => t('Additional details to assist a user on their checklist page.'),
    '#default_value' => $defaults['value'],
    '#format' => $defaults['format'],
    '#weight' => -4,
  );
  $form['checklist_page_tokens'] = _checklist_token_help(-3);
  $form['checklist_continue_text'] = array(
    '#type' => 'textfield',
    '#title' => t('"Continue" link text'),
    '#default_value' => variable_get('checklist_continue_text', 'Continue'),
    '#required' => TRUE,
    '#weight' => -2,
  );
  $form['checklist_continue_path'] = array(
    '#type' => 'textfield',
    '#title' => t('"Continue" link path'),
    '#description' => t('The path that the "Continue" link redirects a user to.'),
    '#default_value' => variable_get('checklist_continue_path', ''),
    '#weight' => -1,
  );
  return system_settings_form($form);
}

/**
 * Submit function for checklist_admin_form.
 */
function checklist_admin_form_submit($form, &$form_state) {
  variable_set('checklist_show_on_login', trim($form_state['values']['checklist_show_on_login']));
  variable_set('checklist_page_title', trim($form_state['values']['checklist_page_title']));
  variable_set('checklist_page_description', $form_state['values']['checklist_page_description']);
  variable_set('checklist_continue_text', trim($form_state['values']['checklist_continue_text']));
  variable_set('checklist_continue_path', trim($form_state['values']['checklist_continue_path']));
}

/**
 * Admin form to reorder checklists.
 */
function checklist_admin_checklists_form($form, &$form_state) {
  $cls = checklist_checklists_load(TRUE);
  foreach ($cls as $cl) {
    $form['cl_' . $cl->clid] = array(
      '#type' => 'weight',
      '#title' => check_plain($cl->title),
      '#default_value' => $cl->weight,
    );
  }
  $form['cl-table'] = array(
    '#type' => 'item',
    '#markup' => '',
    '#weight' => -5,
  );
  $form['#checklists'] = $cls;
  $form['list_submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save order'),
    '#weight' => 1,
  );
  $form['add_item'] = array(
    '#type' => 'item',
    '#markup' => l(t('Add new checklist'), 'admin/structure/checklist/add'),
    '#weight' => 2,
  );
  return $form;
}

/**
 * Submit function for checklist_admin_checklists_form().
 */
function checklist_admin_checklists_form_submit($form, &$form_state) {
  foreach ($form['#checklists'] as $cl) {
    $update = array(
      'clid' => $cl->clid,
      'weight' => $form_state['values']['cl_' . $cl->clid],
    );
    drupal_write_record('checklist_lists', $update, 'clid');
  }
  drupal_set_message(t('Checklists order updated successfully.'));
}

/**
 * Theme the checklist list form.
 */
function theme_checklist_admin_checklists_form($variables) {
  $form = $variables['form'];
  if (isset($form['#checklists'])) {
    $rows = array();
    foreach ($form['#checklists'] as $cl) {
      unset($form['cl_' . $cl->clid]['#title']);
      if (!isset($form['cl_' . $cl->clid]['#attributes'])) {
        $form['cl_' . $cl->clid]['#attributes'] = array();
      }
      if (!isset($form['cl_' . $cl->clid]['#attributes']['class'])) {
        $form['cl_' . $cl->clid]['#attributes']['class'] = array();
      }
      $form['cl_' . $cl->clid]['#attributes']['class'] = array('cl-order');
      $row = array(
        'data' => array(),
        'class' => array('draggable'),
        'id' => 'cl-' . $cl->clid,
      );
      $row['data'][] = check_plain($cl->title);
      $row['data'][] = _checklist_status_options($cl->status);
      $row['data'][] = l(t('Edit'), 'admin/structure/checklist/' . $cl->clid);
      $row['data'][] = l(t('Delete'), 'admin/structure/checklist/' . $cl->clid . '/delete');
      $row['data'][] = drupal_render($form['cl_' . $cl->clid]);
      $rows[] = $row;
    }
    $form['cl-table']['#markup'] = theme('table', array('header' => array(t('Checklist'), t('Status'), '', '', t('Weight')), 'rows' => $rows, 'attributes' => array('id' => 'cl-lists')));
    drupal_add_tabledrag('cl-lists', 'order', 'sibling', 'cl-order');
  }
  return drupal_render_children($form);
}

/**
 * Form to create/edit a checklist
 */
function checklist_edit_form($form, $form_state, $cl = NULL) {
  $cl = is_object($cl) ? $cl : NULL;
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Checklist title'),
    '#default_value' => ($cl ? $cl->title : NULL),
    '#required' => TRUE,
    '#maxlength' => 255,
    '#weight' => -1,
  );
  $form['description'] = array(
    '#type' => 'text_format',
    '#title' => t('Checklist description'),
    '#description' => t('Any user assistance you want to provide should show up here.'),
    '#default_value' => ($cl ? $cl->description : NULL),
    '#format' => ($cl && $cl->format ? $cl->format : filter_default_format()),
    '#weight' => 0,
  );
  $form['message'] = array(
    '#type' => 'textfield',
    '#title' => t('Completion message'),
    '#description' => t('Message to display when a user completes this checklist.'),
    '#default_value' => ($cl ? $cl->message : NULL),
    '#maxlength' => 255,
    '#weight' => 2,
  );
  $form['checklist_list_tokens'] = _checklist_token_help(2);
  if ($cl) {
    $form['checklist_items'] = array(
      '#type' => 'fieldset',
      '#title' => t('Checklist items'),
      '#weight' => 3,
    );
    $cl_items = checklist_items_load($cl->clid, TRUE);
    foreach ($cl_items as $cli) {
      $form['checklist_items']['cli_' . $cli->cliid] = array(
        '#type' => 'weight',
        '#title' => $cli->title,
        '#default_value' => $cli->weight,
      );
    }
    $form['#checklist_items'] = $cl_items;
    $form['checklist_items']['add_item'] = array(
      '#type' => 'item',
      '#markup' => l(t('Add new item'), 'admin/structure/checklist/' . $cl->clid . '/items'),
      '#weight' => 2,
    );
  }
  $form['checklist_items']['force_order'] = _checklist_checkbox_form_item(t('Force item order'), ($cl) && ($cl->force_order), 25, t('Force items to be completed in the order they are listed.'));
  $roles = user_roles(TRUE);
  $form['list_roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Required roles'),
    '#description' => t('Only users of the following roles will be required to complete this checklist.'),
    '#options' => $roles,
    '#default_value' => ($cl ? $cl->roles : array()),
    '#weight' => 4,
  );
  unset($roles[DRUPAL_ANONYMOUS_RID]);
  unset($roles[DRUPAL_AUTHENTICATED_RID]);
  $roles = array(t('-- No Role --')) + $roles;
  $form['rid_on_complete'] = array(
    '#type' => 'select',
    '#title' => t('Assign role'),
    '#description' => t('Once the checklist is complete, assign this role to the user.'),
    '#options' => $roles,
    '#default_value' => ($cl ? $cl->rid_on_complete : NULL),
    '#weight' => 3,
  );
  $form['first_uid'] = array(
    '#type' => 'select',
    '#title' => t('Require users'),
    '#options' => array(0 => t('All, including user 1'), 1 => t('All, except user 1'), 2 => t('Only require new users')),
    '#description' => t('If you only require new users then to complete the checklist, then only new users that meet the group requirements will be expected to complete this checklist.'),
    '#default_value' => ($cl ? $cl->first_uid : 1),
    '#weight' => 5,
  );
  $form['restrict_nav'] = _checklist_checkbox_form_item(t('Limit navigation until complete'), ($cl) && ($cl->restrict_nav), 6, t('Checking this box will disable a user\'s access to the site until they have completed this checklist.'));
  $form['weight'] = array(
    '#type' => 'weight',
    '#title' => t('Checklist weight'),
    '#default_value' => $cl ? $cl->weight : 0,
    '#weight' => 9,
  );
  $form['status'] = array(
    '#type' => 'radios',
    '#title' => t('Status'),
    '#options' => _checklist_status_options(NULL, ($cl) && ($cl->status == CHECKLIST_DELETED)),
    '#default_value' => ($cl ? $cl->status : CHECKLIST_ENABLED),
    '#description' => t('What is the current status of the checklist?'),
    '#weight' => 10,
  );
  $form['list_submit'] = array(
    '#type' => 'submit',
    '#value' => ($cl ? t('Update checklist') : t('Add new checklist')),
    '#weight' => 11,
  );
  if ($cl) {
    $form['delete'] = array(
      '#type' => 'item',
      '#markup' => l(t('Delete'), 'admin/structure/checklist/' . $cl->clid . '/delete'),
      '#weight' => 12,
    );
  }
  $form['#checklist'] = $cl;
  return $form;
}

/**
 * Submit function for checklist_edit_form().
 */
function checklist_edit_form_submit($form, &$form_state) {
  $cl = $form['#checklist'] ? $form['#checklist'] : new stdClass();

  // Determine what the first user should be.
  if ($form_state['values']['first_uid']) {
    if (isset($cl->first_uid) && $cl->first_uid > 1) {
      $form_state['values']['first_uid'] = $cl->first_uid;
    }
    else {
      $form_state['values']['first_uid'] = db_query('SELECT MAX(uid) FROM {users}')->fetchField();
    }
  }

  if ($form_state['values']['description']['format']) $form_state['values']['format'] = $form_state['values']['description']['format'];
  $form_state['values']['description'] = $form_state['values']['description']['value'];
  // Save the checklist.
  if (empty($cl->clid)) {
    $new_checklist = TRUE;
    if (drupal_write_record('checklist_lists', $form_state['values']) && !empty($form_state['values']['clid'])) {
      drupal_set_message(t('Checklist created successfully.'));
    }
  }
  else {
    $new_checklist = FALSE;
    $form_state['values']['clid'] = $cl->clid;
    if (drupal_write_record('checklist_lists', $form_state['values'], array('clid'))) {
      drupal_set_message(t('Checklist updated successfully.'));
      foreach ($form['#checklist_items'] as $cli) {
        $update = array(
          'cliid' => $cli->cliid,
          'weight' => $form_state['values']['cli_' . $cli->cliid],
        );
        drupal_write_record('checklist_items', $update, 'cliid');
      }
    }
  }

  if (!empty($form_state['values']['clid'])) {
    // Replace all of the roles that need to complete this checklist.
    db_delete('checklist_roles')
      ->condition('clid', $form_state['values']['clid'])
      ->execute();
    foreach ($form_state['values']['list_roles'] as $rid) {
      if ($rid) {
        $values = array(
          'clid' => $form_state['values']['clid'],
          'rid' => $rid,
        );
        drupal_write_record('checklist_roles', $values);
      }
    }
    if ($new_checklist) {
      $form_state['redirect'] = 'admin/structure/checklist/' . $form_state['values']['clid'] . '/items';
    }
  }
}

/**
 * Theme the checklist edit form.
 */
function theme_checklist_edit_form($variables) {
  $form = $variables['form'];
  if (isset($form['#checklist_items'])) {
    $rows = array();
    foreach ($form['#checklist_items'] as $cli) {
      unset($form['checklist_items']['cli_' . $cli->cliid]['#title']);
      if (!isset($form['checklist_items']['cli_' . $cli->cliid]['#attributes'])) {
        $form['checklist_items']['cli_' . $cli->cliid]['#attributes'] = array();
      }
      if (!isset($form['checklist_items']['cli_' . $cli->cliid]['#attributes']['class'])) {
        $form['checklist_items']['cli_' . $cli->cliid]['#attributes']['class'] = array();
      }
      $form['checklist_items']['cli_' . $cli->cliid]['#attributes']['class'][] = 'cli-order';
      $row = array(
        'data' => array(),
        'class' => array('draggable'),
        'id' => 'cli-' . $cli->cliid,
      );
      $row['data'][] = check_plain($cli->title);
      $row['data'][] = _checklist_item_type_title($cli->cltid);
      $row['data'][] = _checklist_status_options($cli->status);
      $row['data'][] = l(t('Edit'), 'admin/structure/checklist/' . $cli->clid . '/items/' . $cli->cliid);
      $row['data'][] = l(t('Delete'), 'admin/structure/checklist/' . $cli->clid . '/items/' . $cli->cliid . '/delete');
      $row['data'][] = drupal_render($form['checklist_items']['cli_' . $cli->cliid]);
      $rows[] = $row;
    }
    $form['checklist_items']['table'] = array(
      '#type' => 'item',
      '#markup' => theme('table', array('header' => array(t('Item'), t('Type'), t('Status'), '', '', t('Weight')), 'rows' => $rows, 'attributes' => array('id' => 'cl-items'))),
      '#weight' => 0,
    );
    drupal_add_tabledrag('cl-items', 'order', 'sibling', 'cli-order');
  }
  return drupal_render_children($form);
}

/**
 * Form to delete a checklist
 */
function checklist_delete_form($form, &$form_state, $cl) {
  if ($count = db_query('SELECT COUNT(uid) FROM {checklist_user_checks} WHERE cliid IN (SELECT cliid FROM {checklist_items} WHERE clid = :clid)', array(':clid' => $cl->clid))->fetchField()) {
    $message = 'There are !count items already completed by users on this checklist. This checklist will not be fully deleted, simply removed from all lists and users will no longer be presented with the option to complete it.';
    $form['#checklist_delete'] = FALSE;
  }
  else {
    $message = 'This action cannot be undone.';
    $form['#checklist_delete'] = TRUE;
  }
  $form['#checklist'] = $cl;
  return confirm_form($form, t('Are you sure you want to delete the @title checklist?', array('@title' => $cl->title)), 'admin/structure/checklist/' . $cl->clid, t($message, array('!count' => $count)));
}

/**
 * Submit function for the checklist_delete_form form
 * Deletes a checklist and all associated data
 */
function checklist_delete_form_submit($form, &$form_state) {
  if ($form['#checklist_delete']) {
    db_delete('checklist_items')
      ->condition('clid', $form['#checklist']->clid)
      ->execute();
    db_delete('checklist_roles')
      ->condition('clid', $form['#checklist']->clid)
      ->execute();
    db_delete('checklist_lists')
      ->condition('clid', $form['#checklist']->clid)
      ->execute();
  }
  else {
    $form['#checklist']->status = CHECKLIST_DELETED;
    drupal_write_record('checklist_lists', $form['#checklist'], 'clid');
  }
  drupal_set_message(t('Checklist deleted successfully.'));
  $form_state['redirect'] = 'admin/structure/checklist/list';
}

/**
 * Form to edit a checklist item
 */
function checklist_edit_item_form($form, &$form_state, $cl, $item = NULL) {
  // Get to the correct url.
  if ($item && ($cl->clid != $item->clid)) {
    drupal_goto('admin/structure/checklist/' . $item->clid . '/items/' . $item->cliid . '/delete');
  }
  $form['checklist'] = array(
    '#type' => 'item',
    '#title' => t('Checklist'),
    '#markup' => l($cl->title, 'admin/structure/checklist/' . $cl->clid),
  );
  if (($item == NULL) && (empty($form_state['storage']) || empty($form_state['storage']['type']))) {
    $form_state['storage'] = array('step' => 1);
    $form['item-type'] = array(
      '#type' => 'select',
      '#title' => t('Item Type'),
      '#options' => _checklist_item_type_options(),
      '#required' => TRUE,
      '#default_value' => CHECKLIST_DEFAULT_TYPE,
      '#description' => t('Select the item type.'),
    );
    $form['type-submit'] = array(
      '#type' => 'submit',
      '#value' => t('Configure item'),
    );
  }
  else {
    $type = $item ? $item->cltid : $form_state['storage']['type'];

    // Make sure the handler for the item type still exists.
    if (!checklist_item_type_handlers($type)) {
      $type = CHECKLIST_DEFAULT_TYPE;
    }
    $form['item-type'] = array(
      '#type' => 'item',
      '#title' => t('Item type'),
      '#markup' => _checklist_item_type_title($type),
      '#description' => _checklist_item_type_description($type),
    );
    $form['general'] = array(
      '#type' => 'fieldset',
      '#title' => t('General settings'),
      '#tree' => TRUE,
    );
    $form['general']['title'] = array(
      '#type' => 'textfield',
      '#title' => t('Title'),
      '#default_value' => ($item ? $item->title : NULL),
      '#required' => TRUE,
    );
    $form['general']['description'] = array(
      '#type' => 'text_format',
      '#title' => t('Details'),
      '#default_value' => ($item ? $item->description : NULL),
      '#format' => ($item && $item->format ? $item->format : filter_default_format()),
    );
    $form['general']['checklist_item_tokens'] = _checklist_token_help();
    $form['general']['allow_uncheck'] = _checklist_checkbox_form_item(t('Allow users to uncheck this item'), ($item ? $item->allow_uncheck : 0));
    $form['general']['weight'] = array(
      '#type' => 'weight',
      '#title' => t('Weight'),
      '#default_value' => $item ? $item->weight : 0,
    );
    $form['general']['status'] = array(
      '#type' => 'radios',
      '#title' => t('Status'),
      '#options' => _checklist_status_options(NULL, ($item) && ($item->status == CHECKLIST_DELETED)),
      '#default_value' => ($item ? $item->status : CHECKLIST_ENABLED),
      '#description' => t('What is the current status of the item?'),
      '#weight' => 8,
    );

    // Load the item handler form for specific settings.
    $specific_form = array();
    if (_checklist_item_admin_form_type($type, 'form', $specific_form, $form_state, $cl, $item) && !empty($specific_form)) {
      $form['specific'] = array(
        '#type' => 'fieldset',
        '#title' => t('Specific settings'),
      ) + $specific_form;
    }

    $form['type-submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save item'),
    );
    if ($item) {
      $form['delete'] = array(
        '#type' => 'item',
        '#markup' => l(t('Delete'), 'admin/structure/checklist/' . $cl->clid . '/items/' . $item->cliid . '/delete'),
      );
    }
    else {
      $form['cancel'] = array(
        '#type' => 'item',
        '#markup' => l(t('Cancel'), 'admin/structure/checklist/' . $cl->clid),
      );
    }

    // Save some necessary values for the submit function.
    $form['#cl'] = $cl;
    $form['#cl_item'] = $item;
    $form['#cl_item_type'] = $type;
  }
  return $form;
}

/**
 * Validate function for the checklist_edit_item_form().
 */
function checklist_edit_item_form_validate($form, &$form_state) {
  // Step 2 - Only validate the actual form.
  if (isset($form['#cl_item_type'])) {
    $type = $form['#cl_item_type'];
    $values = _checklist_item_admin_form_type($type, 'validate', $form, $form_state, $form['#cl'], $form['#cl_item']);
  }
}

/**
 * Submit function for the checklist_edit_item_form().
 */
function checklist_edit_item_form_submit($form, &$form_state) {
  // Step 1. Select the item type.
  if (!isset($form['#cl_item_type'])) {
    $form_state['storage']['type'] = $form_state['values']['item-type'];
    $form_state['rebuild'] = TRUE;
  }
  // Step 2. Add/edit the item properties.
  else {
    $type = $form['#cl_item_type'];
    $values = _checklist_item_admin_form_type($type, 'submit', $form, $form_state, $form['#cl'], $form['#cl_item']);
    if (!$values) {
      $values = array();
    }
    $item = $form['#cl_item'] ? $form['#cl_item'] : new stdClass();
    $item->title = trim($form_state['values']['general']['title']);
    $item->description = trim($form_state['values']['general']['description']['value']);
    if ($form_state['values']['general']['description']['format']) {
      $item->format = $form_state['values']['general']['description']['format'];
    }
    $item->allow_uncheck = $form_state['values']['general']['allow_uncheck'];
    $item->weight = $form_state['values']['general']['weight'];
    $item->status = $form_state['values']['general']['status'];
    if (empty($item->data) || !is_array($item->data)) {
      $item->data = array();
    }

    // Save any old values that were in the data variable.
    // If for some reason a handler isn't currently available,
    // keep its values just in case the handler comes back on.
    // Don't serialize because the schema declaration handles this.
    $item->data = $values + $item->data;

    // Set some necessary values for new items.
    if (empty($item->cliid)) {
      $item->clid = $form['#cl']->clid;
      $item->cltid = $type;
      $update = array();
    }
    else {
      $update = 'cliid';
    }
    if (drupal_write_record('checklist_items', $item, $update)) {
      if (!$update) {
        drupal_set_message(t('Item added successfully.'));
        $form_state['redirect'] = 'admin/structure/checklist/' . $item->clid . '/items/' . $item->cliid;
      }
      else {
        drupal_set_message(t('Item updated successfully.'));
      }
    }

    // Remove the storage variable so the form doesn't rebuild.
    if (isset($form_state['storage'])) {
      unset($form_state['storage']);
    }
    $form_state['rebuild'] = FALSE;
  }
}

/**
 * Helper function to call the item form handler
 */
function _checklist_item_admin_form_type($type, $action, &$form, &$form_state, $cl, $item) {
  // This should always be true, but just to be sure.
  if ($handler = checklist_item_type_handlers($type)) {

    // Create function name and call it if it exists.
    $function = $handler['module'] . '_checklist_item_' . $type . '_admin_form_' . $action;
    _checklist_item_load_include_file($handler);
    if (function_exists($function)) {
      $return = $function($form, $form_state, $cl, $item);
      return $return;
    }
  }
  return FALSE;
}

/**
 * Form to delete a checklist item.
 */
function checklist_delete_item_form($form, &$form_state, $cl, $item) {
  // Get to the correct url.
  if ($cl->clid != $item->clid) {
    drupal_goto('admin/structure/checklist/' . $item->clid . '/items/' . $item->cliid . '/delete');
  }
  if ($count = db_query('SELECT COUNT(uid) FROM {checklist_user_checks} WHERE cliid = :cliid', array(':cliid' => $item->cliid))->fetchField()) {
    $message = 'There are !count users who have already completed this item. This item will not be fully deleted, simply removed from all lists and users will no longer be presented with the item to complete it.';
    $form['#item_delete'] = FALSE;
  }
  else {
    $message = 'This action cannot be undone.';
    $form['#item_delete'] = TRUE;
  }
  $form['#checklist'] = $cl;
  $form['#checklist_item'] = $item;
  return confirm_form($form, t('Are you sure you want to delete the @item_title item from the @title checklist?', array('@item_title' => $item->title, '@title' => $cl->title)), 'admin/structure/checklist/' . $item->clid . '/items/' . $item->cliid, t($message, array('!count' => $count)));
}

/**
 * Submit function for checklist_delete_item_form().
 */
function checklist_delete_item_form_submit($form, &$form_state) {
  if ($form['#item_delete']) {
    db_delete('checklist_items')
      ->condition('cliid', $form['#checklist_item']->cliid)
      ->execute();
  }
  else {
    $form['#checklist_item']->status = CHECKLIST_DELETED;
    drupal_write_record('checklist_items', $form['#checklist_item'], 'cliid');
  }
  drupal_set_message(t('Checklist item deleted successfully.'));
  $form_state['redirect'] = 'admin/structure/checklist/' . $form['#checklist_item']->clid . '/items';
}

/**
 * Status options for a checklist or checklist item.
 */
function _checklist_status_options($status = NULL, $include_deleted = TRUE) {
  $statuses = array(
    CHECKLIST_ENABLED => t('Active'),
    CHECKLIST_DISABLED => t('Disabled'),
  );
  if ($include_deleted) {
    $statuses[CHECKLIST_DELETED] = t('Deleted');
  }
  if ($status !== NULL) {
    return isset($statuses[$status]) ? $statuses[$status] : '';
  }
  return $statuses;
}

function _checklist_item_type_options() {
  $options = array();
  foreach (checklist_item_type_handlers() as $type => $handler) {
    $options[$type] = $handler['title'];
  }
  return $options;
}
