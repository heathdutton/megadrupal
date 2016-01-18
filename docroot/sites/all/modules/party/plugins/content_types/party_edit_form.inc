<?php

/**
 * @file
 * CTools content for user edit form
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t('Party Edit Form'),
  'content_types' => 'party_edit_form',
  // 'single' means not to be subtyped.
  'single' => TRUE,
  // Name of a function which will render the block.
  'render callback' => 'party_edit_form_render',

  // Icon goes in the directory with the content type.
  'description' => t('Show a party edit form.'),
  'required context' => new ctools_context_required(t('Party'), 'entity:party'),
  'edit form' => 'party_edit_form_edit_form',
  'admin title' => 'party_edit_form_admin_title',

  // presents a block which is used in the preview of the data.
  // Pn Panels this is the preview pane shown on the panels building page.
  'category' => array(t('Party'), 0),
);

/**
 * Render the Party Attached Entity Edit Form
 *
 * @param $subtype
 * @param $conf
 *   Configuration as done at admin time
 * @param $args
 * @param $context
 *   Context - in this case we don't have any
 *
 * @return
 *   An object with at least title and content members
 */
function party_edit_form_render($subtype, $conf, $args, $context) {
  $block = new stdClass();
  $block->title = t('Edit Party');
  $block->content = '';

  if (!empty($context->data)) {
    // Get hold of the form
    $form = drupal_get_form('party_edit_form_form', $context->data, $conf);

    // Render it...
    $block->content = drupal_render($form);
  }

  return $block;
}

/**
 * Form
 */
function party_edit_form_form($form, &$form_state, $party, $conf = NULL) {
  form_load_include($form_state, 'inc', 'party', 'party.pages');
  form_load_include($form_state, 'inc', 'party', 'plugins/content_types/party_edit_form');

  $form['#party'] = $party;

  if (isset($party->pid)) {
    $form['pid'] = array(
      '#type' => 'value',
      '#value' => $party->pid,
    );
  }

  $form_state['#party'] = $party;
  $form_state['#party_unchanged'] = $party; // So that other modules can react to changes. Must be a better way
  $form_state['#data_set_controllers'] =
  $form_state['#attached_entities'] = array();

  $form['#validate'][] = 'party_edit_form_validate';
  $form['#submit'][] = 'party_edit_form_submit';

  field_attach_form('party', $party, $form, $form_state);

  // Attach data set forms.
  if (!empty($conf['show_data_sets'])) {
    $data_sets = party_get_party_data_sets($party);
    foreach ($data_sets as $data_set) {
      // Get our controller
      $controller = $party->getDataSetController($data_set);
      $form_state['#data_set_controllers'][$data_set] = $controller;

      party_data_set_attach_form($form, $form_state, $controller);
    }
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 99,
  );

  // Add our submit handler and set the action
  $form['#submit'][] = 'party_edit_form_save';
  $form['#submit'][] = 'party_edit_form_form_submit';
  $form['#action'] = base_path() . str_replace('ajax', 'nojs', request_path());

  return $form;
}

/**
 * Submit Handler
 */
function party_edit_form_form_submit(&$form, &$form_state) {
  // Make sure we stop it redirecting anywhere it shouldn't...
  unset($form_state['redirect']);

  $conf = $form_state['build_info']['args'][1];
  $form_state['redirect'] = 'admin/party/' . $form['#party']->pid;
}

/**
 * Config Form
 */
function party_edit_form_edit_form($form, &$form_state) {
  $form['show_data_sets'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show Data Sets'),
    '#default_value' => $form_state['conf']['show_data_sets'],
  );

  return $form;
}

function party_edit_form_edit_form_submit(&$form, &$form_state) {
  foreach (element_children($form) as $key) {
    if (!empty($form_state['values'][$key])) {
      $form_state['conf'][$key] = $form_state['values'][$key];
    }
  }
}

/**
 * Title Callback
 */
function party_edit_form_admin_title($subtype, $conf, $context = NULL) {
  if ($conf['override_title'] && !empty($conf['override_title_text'])) {
    $output = format_string('"@context" !title', array(
      '@context' => $context->identifier,
      '!title' => filter_xss_admin($conf['override_title_text']),
    ));
  }
  else {
    $output = t('"@context" Edit Party', array(
      '@context' => $context->identifier,
    ));
  }
  return $output;
}
