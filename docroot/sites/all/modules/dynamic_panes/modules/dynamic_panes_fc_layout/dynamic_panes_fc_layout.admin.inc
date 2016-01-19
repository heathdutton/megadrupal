<?php

/**
 * @file
 * Administration related functionality for the module.
 */

/**
 * Form constructor for the FC Layout add/edit form.
 *
 * @see dynamic_panes_fc_layout_form_submit()
 * @see dynamic_panes_fc_layout_form_delete_submit()
 *
 * @ingroup forms
 */
function dynamic_panes_fc_layout_form($form, &$form_state, $layout, $op = 'edit') {
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => !empty($layout->title) ? $layout->title : '',
    '#required' => TRUE,
  );

  field_attach_form(DYNAMIC_PANES_FC_LAYOUT_ENTITY_TYPE_NAME, $layout, $form, $form_state);

  $form['actions'] = array(
    '#type' => 'actions',
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  if ($op == 'edit') {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#weight' => 15,
      '#submit' => array('dynamic_panes_fc_layout_form_delete_submit'),
    );
  }

  return $form;
}

/**
 * Form submission handler for dynamic_panes_fc_layout_form().
 *
 * @see dynamic_panes_fc_layout_form()
 * @see dynamic_panes_fc_layout_form_delete_submit()
 *
 * @ingroup forms
 */
function dynamic_panes_fc_layout_form_submit($form, &$form_state) {
  $layout = entity_ui_form_submit_build_entity($form, $form_state);

  $is_new = !empty($layout->is_new);

  $layout->created = $is_new ? REQUEST_TIME : $layout->created;
  $layout->changed = REQUEST_TIME;
  $layout->save();

  if ($is_new) {
    drupal_set_message(t('%title has been created.', array('%title' => $layout->title)));
  }
  else {
    drupal_set_message(t('%title has been updated.', array('%title' => $layout->title)));
  }

  $form_state['redirect'] = 'admin/structure/dynamic-panes-fc-layout';
}

/**
 * Form submission handler for dynamic_panes_fc_layout_form().
 *
 * Handles the 'Delete' button on the layout form.
 *
 * @see dynamic_panes_fc_layout_form()
 * @see dynamic_panes_fc_layout_form_submit()
 *
 * @ingroup forms
 */
function dynamic_panes_fc_layout_form_delete_submit($form, &$form_state) {
  $destination = array();
  if (isset($_GET['destination'])) {
    $destination = drupal_get_destination();
    unset($_GET['destination']);
  }

  $layout = $form_state[DYNAMIC_PANES_FC_LAYOUT_ENTITY_TYPE_NAME];
  $form_state['redirect'] = array('admin/structure/dynamic-panes-fc-layout/manage/' . $layout->id . '/delete', array('query' => $destination));
}

/**
 * Page callback for add block to layout.
 *
 * @param object $layout
 *   The layout object.
 * @param object $fc_item_1
 *   The field collection item used as region.
 * @param string $region_name
 *   The region name.
 * @param object $fc_item_2
 *   The field collection item used as level of region.
 * @param string|int $level
 *   The level value.
 *
 * @return array
 *   The form array.
 */
function dynamic_panes_fc_layout_add_block($layout, $fc_item_1, $region_name, $fc_item_2, $level) {
  // Creating new field_collection_item used as region if needed.
  if ($fc_item_1 == 'new') {
    $fc_item_1 = entity_create('field_collection_item', array('field_name' => DYNAMIC_PANES_FC_LAYOUT_FIELD_LAYOUT_REGIONS));
    $fc_item_1->setHostEntity(DYNAMIC_PANES_FC_LAYOUT_ENTITY_TYPE_NAME, $layout, LANGUAGE_NONE, FALSE);
    $fc_item_1_wrapper = entity_metadata_wrapper('field_collection_item', $fc_item_1);
    $fc_item_1_wrapper->{DYNAMIC_PANES_FC_LAYOUT_FIELD_LAYOUT_REGIONS_REGION}->set($region_name);
  }

  // Creating new field_collection_item used as level of region if needed.
  if ($fc_item_2 == 'new') {
    $fc_item_2 = entity_create('field_collection_item', array('field_name' => DYNAMIC_PANES_FC_LAYOUT_FIELD_LAYOUT_REGIONS_BLOCKS));
    $fc_item_2->setHostEntity('field_collection_item', $fc_item_1, LANGUAGE_NONE, FALSE);

    if ($level != 'all') {
      $fc_item_2_wrapper = entity_metadata_wrapper('field_collection_item', $fc_item_2);
      $fc_item_2_wrapper->{DYNAMIC_PANES_FC_LAYOUT_FIELD_LAYOUT_BLOCKS_LEVEL}->set($level);
    }
  }

  $form_state = array(
    'dynamic_panes_fc_layout' => array('op' => 'add'),
    'build_info' => array(
      'args' => array($fc_item_2),
    ),
  );

  form_load_include($form_state, 'inc', 'field_collection', 'field_collection.pages');
  return drupal_build_form('field_collection_item_form', $form_state);
}

/**
 * Page callback for edit block in layout.
 *
 * @param object $field_collection_item
 *   The field collection item.
 * @param int $delta
 *   The block delta.
 *
 * @return array
 *   The form array.
 */
function dynamic_panes_fc_layout_edit_block($field_collection_item, $delta) {
  $form_state = array(
    'dynamic_panes_fc_layout' => array('op' => 'edit', 'delta' => $delta),
    'build_info' => array(
      'args' => array($field_collection_item),
    ),
  );

  form_load_include($form_state, 'inc', 'field_collection', 'field_collection.pages');
  return drupal_build_form('field_collection_item_form', $form_state);
}

/**
 * Form constructor for the block deletion from layout confirmation form.
 *
 * @see dynamic_panes_fc_layout_delete_confirm_submit()
 */
function dynamic_panes_fc_layout_delete_confirm($form, &$form_state, $field_collection_item, $delta) {
  $path = isset($_GET['destination']) ? $_GET['destination'] : '<front>';

  $wrapper = entity_metadata_wrapper('field_collection_item', $field_collection_item);
  if (isset($wrapper->{DYNAMIC_PANES_FC_LAYOUT_FIELD_LAYOUT_BLOCKS_BLOCK}[$delta])) {
    $block_wrapper = $wrapper->{DYNAMIC_PANES_FC_LAYOUT_FIELD_LAYOUT_BLOCKS_BLOCK}[$delta];

    $form['field_collection_id'] = array(
      '#type' => 'value',
      '#value' => $wrapper->getIdentifier(),
    );
    $form['delta'] = array(
      '#type' => 'value',
      '#value' => $delta,
    );

    return confirm_form($form,
      t('Are you sure you want to delete %title?', array('%title' => $block_wrapper->title->value())),
      $path,
      t('This action cannot be undone.'),
      t('Delete'),
      t('Cancel')
    );
  }

  drupal_goto($path);
}

/**
 * Executes block deletion from layout.
 *
 * @see dynamic_panes_fc_layout_delete_confirm_confirm()
 */
function dynamic_panes_fc_layout_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    $field_collection_id = $form_state['values']['field_collection_id'];
    $delta = $form_state['values']['delta'];
    if ($field_collection_item = field_collection_item_load($field_collection_id)) {
      $wrapper = entity_metadata_wrapper('field_collection_item', $field_collection_item);
      if (isset($wrapper->{DYNAMIC_PANES_FC_LAYOUT_FIELD_LAYOUT_BLOCKS_BLOCK}[$delta])) {
        $wrapper->{DYNAMIC_PANES_FC_LAYOUT_FIELD_LAYOUT_BLOCKS_BLOCK}[$delta]->set(NULL);
        $raw = $wrapper->{DYNAMIC_PANES_FC_LAYOUT_FIELD_LAYOUT_BLOCKS_BLOCK}->raw();
        if (empty($raw)) {
          field_collection_item_delete($field_collection_id);
        }
        else {
          $wrapper->save();
        }
      }
    }
  }
  $form_state['redirect'] = isset($_GET['destination']) ? $_GET['destination'] : '<front>';
}
