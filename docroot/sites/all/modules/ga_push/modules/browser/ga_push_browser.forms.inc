<?php
/**
 * @file
 * Forms for the ga_push_browser module.
 */

/**
 * Add new GA PUsh Browser Event page callback.
 */
function ga_push_browser_event_add() {

  $gapb_event = entity_create('ga_push_browser_event', array());
  drupal_set_title(t('Create GA Push Browser Event'));

  $output = drupal_get_form('ga_push_browser_event_form', $gapb_event);

  return $output;
}

/**
 * GA Push browser event create/edit form.
 *
 * @param object $gapb_event
 *   GA Push browser event.
 *
 * @return array
 *   Form.
 */
function ga_push_browser_event_form($form, &$form_state, $gapb_event) {
  $form_state['gapb_event'] = $gapb_event;

  $form = array();

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#description' => t('Administration name'),
    '#required' => TRUE,
    '#default_value' => isset($gapb_event->name) ? $gapb_event->name : NULL,
  );

  $form['status'] = array(
    '#type' => 'checkbox',
    '#title' => t('Active'),
    '#default_value' => isset($gapb_event->status) ? $gapb_event->status : TRUE,
  );

  $form['selector'] = array(
    '#type' => 'textarea',
    '#title' => t('jQuery selector'),
    '#rows' => 1,
    '#description' => t('jQuery selector'),
    '#required' => TRUE,
    '#default_value' => isset($gapb_event->selector) ? $gapb_event->selector : NULL,
  );

  $form['bind'] = array(
    '#type' => 'textfield',
    '#title' => t('jQuery bind'),
    '#description' => t('jQuery bind trigger handler: click, mouseenter, mouseleave... See documentation on !link', array('!link' => l('http://api.jquery.com/bind/', 'http://api.jquery.com/bind/'))),
    '#default_value' => 'click',
    '#required' => TRUE,
    '#default_value' => isset($gapb_event->bind) ? $gapb_event->bind : NULL,
  );

  $form['ga_category'] = array(
    '#type' => 'textfield',
    '#title' => t('GA Category'),
    '#description' => t('Google analytics category event'),
    '#required' => TRUE,
    '#default_value' => isset($gapb_event->ga_category) ? $gapb_event->ga_category : NULL,
  );

  $form['ga_action'] = array(
    '#type' => 'textfield',
    '#title' => t('GA Action'),
    '#description' => t('Google analytics action event'),
    '#required' => TRUE,
    '#default_value' => isset($gapb_event->ga_action) ? $gapb_event->ga_action : NULL,
  );

  $form['ga_label'] = array(
    '#type' => 'textfield',
    '#title' => t('GA Label'),
    '#description' => t('Google analytics label event'),
    '#default_value' => isset($gapb_event->ga_label) ? $gapb_event->ga_label : NULL,
  );

  $form['ga_value'] = array(
    '#type' => 'textfield',
    '#title' => t('GA Value'),
    '#description' => t('Google analytics value event'),
    '#default_value' => isset($gapb_event->ga_value) ? $gapb_event->ga_value : NULL,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  $form['#redirect'] = array();

  return $form;
}

/**
 * Form submit: ga_push_browser_event_form().
 */
function ga_push_browser_event_form_submit($form, &$form_state) {
  entity_form_submit_build_entity('ga_push_browser_event', $form_state['gapb_event'], $form, $form_state);
  if (ga_push_browser_event_save($form_state['gapb_event'])) {
    drupal_set_message(t('GA Push Browser event has been saved correctly.'));
  }

  $form_state['redirect'] = 'admin/config/system/ga-push/browser';
}

/**
 * Delete GA Push browser event confirm form.
 *
 * @param object $ga_push_browser_event
 *   GA Push browser evento to delete.
 *
 * @return array
 *   Form.
 */
function ga_push_browser_event_confirm_delete_form($form, &$form_state, $ga_push_browser_event) {

  $form['ga_push_browser_id'] = array('#type' => 'value', '#value' => $ga_push_browser_event->id);
  return confirm_form($form,
    t('Are you sure you want to delete %title?', array('%title' => $ga_push_browser_event->name)),
    '/admin/config/system/ga-push/browser/',
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Submit function for GA Push browser event delete confirm form.
 */
function ga_push_browser_event_confirm_delete_form_submit($form, &$form_state) {
  ga_push_browser_delete($form['ga_push_browser_id']['#value']);
  $form_state['redirect'] = '/admin/config/system/ga-push/browser/';
}
