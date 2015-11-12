<?php

/**
 * @file
 * Admin pages for commerce extra panes.
 */

/**
 * Form of settings for the extra panes.
 */
function commerce_extra_panes_settings_form($form, &$form_state) {
  $form = array();
  $form['node-pane'] = array(
    '#type' => 'textfield',
    '#title' => t('Select node to use as checkout pane.'),
    '#description' => t('Input the title of the node that you want to add as checkout pane.'),
    '#maxlength' => 60,
    '#autocomplete_path' => 'ctools/autocomplete/node',
  );
  $form['submit'] = array('#type' => 'submit', '#value' => t('Add pane'));

  $form['extra-panes-list'] = array(
    '#type' =>  'item',
    '#title' => t('List of extra panes.'),
    '#theme' => 'commerce_extra_panes_settings_list',
  );
  return $form;
}

/**
 * Validate function for extra panes settings form.
 */
function commerce_extra_panes_settings_form_validate($form, &$form_state) {
  $preg_matches = array();
  $match = preg_match('/\[id: (\d+)\]/', $form_state['values']['node-pane'], $preg_matches);
  if (!$match) {
    form_set_error('node-pane', t('You must select a node for adding a pane.'));
  }
  else {
    $form_state['node-pane-id'] = $preg_matches[1];
  }
}

/**
 * Submit function for extra panes settings form.
 */
function commerce_extra_panes_settings_form_submit($form, &$form_state) {
  if (!empty($form_state['node-pane-id'])) {
    $panes = commerce_extra_panes_get_panes($form_state['node-pane-id']);
    if (!count($panes)) {
      db_insert('commerce_extra_panes')
        ->fields(array(
          'extra_id' => $form_state['node-pane-id'],
          'extra_type' => 'node',
          'status' => 1,
        ))
        ->execute();
      drupal_set_message(t('Extra pane added'));
    }
    else {
      drupal_set_message(t('Pane already added'), 'warning');
    }
  }
}

/**
 * Menu callback -- ask for confirmation of extra pane deletion
 */
function commerce_extra_panes_delete_confirm($form, &$form_state, $extra_id) {
  $panes = commerce_extra_panes_get_panes($extra_id);
  $extra_pane = reset($panes);
  $entity = entity_load_single($extra_pane->extra_type, $extra_id);
  $form['#entity'] = $entity;
  $form['#extra_id'] = $extra_id;
  return confirm_form($form,
    t('Are you sure you want to delete the pane %title?', array('%title' => $entity->title)),
    'admin/commerce/config/checkout/extra',
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Execute extra pane deletion
 */
function commerce_extra_panes_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    $entity = $form['#entity'];
    db_delete('commerce_extra_panes')
      ->condition('extra_id', $form['#extra_id'])
      ->execute();
    drupal_set_message(t('Extra pane %title deleted', array('%title' => $entity->title)));
  }
  $form_state['redirect'] = 'admin/commerce/config/checkout/extra';
}

/**
 * Enable/disable checkout panes.
 */
function commerce_extra_panes_change_status($extra_id, $token = '') {
  if ($extra_id && drupal_valid_token($token, $extra_id)) {
    $panes = commerce_extra_panes_get_panes($extra_id);
    $extra_pane = reset($panes);
    if ($extra_pane->status) {
      //Enabled, let's disable it.
      db_update('commerce_extra_panes')
        ->fields(array(
          'status' => 0,
        ))
        ->condition('extra_id', $extra_id)
        ->execute();
      drupal_set_message(t('Commerce extra pane disabled'));
    }
    else {
      db_update('commerce_extra_panes')
        ->fields(array(
          'status' => 1,
        ))
        ->condition('extra_id', $extra_id)
        ->execute();
      drupal_set_message(t('Commerce extra pane enabled'));
    }
  }
  drupal_goto('admin/commerce/config/checkout/extra');
}
