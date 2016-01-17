<?php
/**
 * @file party.pages.inc
 */

/**
 * Page callback to display a party.
 *
 * @todo: replace this completely now we have party pieces as 2nd level tabs.
 * this should be... what? basic info about the party like creation date,
 * label, etc?
 */
function party_page_view($party, $view_mode = 'full') {
  global $user;

  $party->content = array();
  $build = array();

  $build = module_invoke_all('party_page_view_alter', $party);

  // Demo output so we see this works.
  $build['demo'] = array(
    '#type' => 'markup',
    '#markup' => 'Party view page',
  );

  $controller = entity_get_controller('party');
  $build['party'] = $controller->view(array($party->pid => $party));
  return $build;
}


/**
 * Page callback for adding a party.
 */
function party_add() {
  $party = entity_create('party', array(
    'label' => '',
  ));

  return drupal_get_form('party_form', $party);
}

/**
 * Party edit form.
 */
function party_form($form, &$form_state, $party, $op = 'edit') {
  // Manually add this file in incase its not already here.
  $form_state['build_info']['files']['form'] = drupal_get_path('module', 'party') . '/party.pages.inc';

  $form['#party'] = $party;

  if (isset($party->pid)) {
    $form['pid'] = array(
      '#type' => 'value',
      '#value' => $party->pid,
    );

    drupal_set_title(check_plain("Edit " . $party->label));
  }

  $form_state['#party'] = $party;
  $form_state['#party_unchanged'] = $party; // So that other modules can react to changes. Must be a better way

  // Set validate and submit handlers.
  $form['#validate'][] = 'party_edit_form_validate';
  $form['#submit'][] = 'party_edit_form_submit';

  // Attach data set forms.
  $data_sets = party_get_party_data_sets($party);
  foreach ($data_sets as $data_set) {
    // Get our controller
    $controller = $party->getDataSetController($data_set);
    party_data_set_attach_form($form, $form_state, $controller);
  }

  field_attach_form('party', $party, $form, $form_state);

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 99,
  );

  $form['#submit'][] = 'party_edit_form_save';
  return $form;
}

/**
 * Validate the Party Edit Form
 *
 * @todo: Add validation for all the attached entities
 */
function party_edit_form_validate($form, &$form_state) {
  $pseudo_entity = (object) $form_state['values'];
  field_attach_form_validate('party', $pseudo_entity, $form, $form_state);
}

/**
 * Submit handler for the the Party edit form.
 *
 * The work of saving the party is split over 3 submit handlers for this form:
 *  - party_edit_form_submit() - builds the Party object and attaches field API
 *    field values.
 *  - party_data_set_attach_form_submit() - saves all the attached
 *    entities.
 *  - party_edit_form_save() - updates party label.
 * This is in order to allow plugins to use all of the party data available when
 * generating the name label.
 */
function party_edit_form_submit($form, &$form_state) {
  field_attach_submit('party', $form['#party'], $form, $form_state);
  party_save($form['#party']);
}

/**
 * Set Entity Label
 *
 * @see party_edit_form_submit
 */
function party_edit_form_save($form, &$form_state) {
  $controller = entity_get_controller('party');
  $controller->setPrimaryFields($form['#party']);
  drupal_set_message(t("@party saved successfully.", array('@party' => $form['#party']->label)));
  $form_state['redirect'] = 'party/' . $form['#party']->pid;
}

/**
 * Form constructor for the party archive form.
 *
 * @param Party $party
 *   The party we are set to archive.
 *
 * @see party_archive_form_submit().
 */
function party_archive_form($form, &$form_state, $party) {
  $form_state['party'] = $party;

  // Output the confirm form.
  if ($party->archived) {
    $yes = t('Un-archive');
    $action = t('un-archive');
    $name = 'un-archive';
  }
  else {
    $yes = t('Archive');
    $action = t('archive');
    $name = 'archive';
  }
  $message = t('Are you sure you want to @action party %label (%id)?', array(
    '@action' => t($action),
    '%label' => $party->label,
    '%id' => $party->pid,
  ));
  $form = confirm_form($form, $message, 'admin/community', '', $yes, NULL, $action);

  return $form;
}

/**
 * Submit handler for the party archive form.
 */
function party_archive_form_submit($form, &$form_state) {
  if (!empty($form_state['values']['archive'])) {
    $form_state['party']->archived = TRUE;
  }
  elseif (!empty($form_state['values']['un-archive'])) {
    $form_state['party']->archived = FALSE;
  }
  else {
    return;
  }

  $form_state['party']->save();
}

/**
 * Party delete form.
 */
function party_delete_form($form, &$form_state, $party) {
  $form_state['party'] = $party;

  $form = confirm_form($form,
    t('Are you sure you want to delete party %label (%id)?', array('%label' => $party->label, '%id' => $party->pid)),
    'admin/community',
    '<p>' . t('This action cannot be undone.') . '</p>',
    t('Delete'),
    t('Cancel'),
    'confirm'
  );

  return $form;
}

/**
 * Submit handler for the party delete form.
 */
function party_delete_form_submit($form, &$form_state) {
  $form_state['party']->delete();
}

// =================================== Party piece display.

/**
 * Page callback to view a data set.
 *
 * Used for display of data sets that don't use Views or provide their own
 * page callback.
 *
 * @see party_party_party_pieces().
 *
 * @param $party
 *   A loaded party object.
 * @param $data_set_name
 *   A data set name.
 */
function party_view_data_set($party, $data_set_name) {
  $attached_entities = $party->getDataSetController($data_set_name)->getEntities();

  $build = array();
  foreach ($attached_entities as $delta => $entity) {
    $build[$data_set_name . '_' . $delta] = array(
      // @todo: this is sort of an abuse of what fieldsets are meant for... :/
      '#type' => 'fieldset',
      '#title' => $data_set_controller->getLabel($delta),
    );

    $build[$data_set_name . '_' . $delta]['view'] = $data_set_controller->display($delta);
  }

  return $build;
}

/**
 * Show a View display plugin as a party piece.
 *
 * @param $party
 *  A party entity from the menu loader.
 * @param $view_name
 *  The machine name of a view.
 * @param $view_display_id
 *  The machine name of a view display.
 */
function party_page_view_piece_views($party, $view_name, $view_display_id) {
  if ($view = views_get_view($view_name)) {
    if ($view->access($view_display_id)) {
      $view->set_display($view_display_id);
      if (isset($view->display_handler)) {
        $view->set_arguments(array($party->pid));
        $output = $view->execute_display($view_display_id);
        // @todo:
        //views_add_block_contextual_links($output, $view, $view_display_id);
        $view->destroy();
        return $output;
      }
    }
    $view->destroy();
  }
}

// =================================== Forms for controller-based actions.

/**
 * Form builder for a generalized attached entity action.
 *
 * @todo: let this handle built-in actions: 'edit', 'delete', 'detach'.
 *
 * @param $party
 *   A loaded party object.
 * @param $data_set
 *   A loaded data set.
 *   We don't actually need this loaded, but we need a menu loader to convert
 *   the path-style string to the machine name, and a menu loader that doesn't
 *   load would be weird too.
 * @param $action
 *   The machine name of a data set action, as defined by the data set in
 *   hook_party_data_set_info().
 * @param $eid
 *   The id of the entity, if editing.
 */
function party_attached_entity_action_form($form, &$form_state, $party, $data_set, $action, $eid = NULL) {
  $action_controller = new $data_set['actions'][$action]['controller'];

  // Get the page title from the controller.
  $page_title = $action_controller->get_page_title($party, $data_set, $eid);
  drupal_set_title(check_plain($page_title));

  $form += $action_controller->action_form($form, $form_state, $party, $data_set, $eid);

  // Ensure that our submit and validate handlers are run (and run last).
  if (!isset($form['#submit']) || !in_array('party_attached_entity_action_form_submit', $form['#submit'])) {
    $form['#submit'][] = 'party_attached_entity_action_form_submit';
  }
  if (!isset($form['#validate']) || !in_array('party_attached_entity_action_form_validate', $form['#validate'])) {
    $form['#validate'][] = 'party_attached_entity_action_form_validate';
  }

  return $form;
}

/**
 * Validate handler for the general action form.
 */
function party_attached_entity_action_form_validate($form, &$form_state) {
  // Retrieve the original form parameters from the build info.
  // Minor WTF: because $eid is optional, it doesn't show in the array. Shove it
  // in there as NULL if missing, so controller handlers can rely on it.
  if (!isset($form_state['build_info']['args'][3])) {
    $form_state['build_info']['args'][3] = NULL;
  }
  list ($party, $data_set, $action, $eid) = $form_state['build_info']['args'];

  // Call the validate handler in the action controller.
  $action_controller = new $data_set['actions'][$action]['controller'];
  $action_controller->action_form_validate($form, $form_state, $party, $data_set, $eid);
}

/**
 * Submit handler for the general action form.
 */
function party_attached_entity_action_form_submit($form, &$form_state) {
  // Retrieve the original form parameters from the build info.
  // Minor WTF: because $eid is optional, it doesn't show in the array. Shove it
  // in there as NULL if missing, so controller handlers can rely on it.
  if (!isset($form_state['build_info']['args'][3])) {
    $form_state['build_info']['args'][3] = NULL;
  }
  list ($party, $data_set, $action, $eid) = $form_state['build_info']['args'];

  // Call the submit handler in the action controller.
  $action_controller = new $data_set['actions'][$action]['controller'];
  $action_controller->action_form_submit($form, $form_state, $party, $data_set, $eid);

  // Redirect to the set piece.
  $form_state['redirect'] = 'party/' . $party->pid . '/' . $data_set['path element'];
}

// =================================== Attached entity action forms.

/**
 * Form to edit or create an attached entity.
 *
 * @param $party
 *    A loaded party object.
 * @param $data_set
 *   A loaded data set.
 *   We don't actually need this loaded, but we need a menu loader to convert
 *   the path-style string to the machine name, and a menu loader that doesn't
 *   load would be weird too.
 * @param $delta
 *    The id of the entity, if editing.
 *
 * @todo: Reimplement actions and edit overlay stuff
 */
function party_attached_entity_edit_form($form, &$form_state, $party, $data_set, $delta = NULL) {
  // Add the file to build info incase this has been embedded from somewhere else.
  $form_state['build_info']['files'][] = drupal_get_path('module', 'party') . '/party.pages.inc';

  $data_set_controller = $party->getDataSetController($data_set['set_name']);

  if (isset($delta)) {
    $entity = $data_set_controller->getEntity($delta, TRUE);
    $form_title = t("Edit @data-set-label (@entity-label)", array(
      '@data-set-label' => $data_set['label'],
      '@entity-label' => entity_label($data_set['entity type'], $entity),
    ));
  }
  else {
    $entity = $data_set_controller->createEntity();
    $data_set_controller->attachEntity($entity);
    $delta = $data_set_controller->getEntityDelta($entity);
    $form_title = t("Add @data-set-label to @party-label", array(
      '@data-set-label' => $data_set_controller->getDataInfo('label'),
      '@party-label' => $party->label,
    ));
  }
  drupal_set_title($form_title);

  // Get the attached entity form as a standalone, as we have just the one.
  party_data_set_attach_form($form, $form_state, $data_set_controller, array($delta), TRUE);

  // As we know we're only dealine with one item remove the fieldset.
  $form_key = $data_set_controller->getDataInfo('name') . ':' . $delta;
  unset($form[$form_key]['#type'], $form[$form_key]['#title']);

  $form['#submit'][] = 'party_attached_entity_edit_form_submit';

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 99,
  );
  return $form;
}

/**
 * Validation handler for the data set edit form.
 */
function party_attached_entity_edit_form_validate($form, &$form_state) {
  return TRUE;
}

/**
 * Submit handler for the data set edit form.
 */
function party_attached_entity_edit_form_submit($form, &$form_state) {
  // Get the data set controller
  $data_set_controller = reset($form['#data_set_controllers']);

  // Redirect to the set piece.
  $form_state['redirect'] = 'party/' . $data_set_controller->getParty()->pid . '/' . $data_set_controller->getDataInfo('path element');
}

/**
 * Form to confirm removing a data set from a party.
 *
 * @todo: pass this the delta rather than the entity id.
 */
function party_attached_entity_remove_confirm($form, &$form_state, $party, $data_set, $eid) {
  $data_set_controller = $party->getDataSetController($data_set['set_name']);

  $form['#party'] = $party;
  $form['#data_set'] = $data_set_controller;
  $form['#entity'] = reset(entity_load($data_set_controller->getDataInfo('entity type'), array($eid)));

  return confirm_form($form,
    t('Are you sure you want to remove %attached from %party?', array(
      '%attached' => $data_set_controller->getDataInfo('label'),
      '%party' =>  $party->label,
    )),
    'party/' . $party->pid, // @todo: go back to the dataset page.
    t('Are you sure you want to remove this data set from this party?') . ' ' . t('This action cannot be undone.'),
    t('Remove'),
    t('Cancel')
  );
}

/**
 * Submit handler for the data set remove form.
 */
function party_attached_entity_remove_confirm_submit($form, &$form_state) {
  $party = $form['#party'];
  $data_set_controller = $form['#data_set'];

  $data_set_controller->detachEntity($form['#entity']);
  $data_set_controller->save();

  // Redirect to the set piece.
  $form_state['redirect'] = 'party/' . $party->pid . '/' . $data_set_controller->getDataInfo('path element');
}
