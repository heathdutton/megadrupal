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
  'title' => t('Party Attached Entity Edit Form'),
  'content_types' => 'party_attached_entity_edit_form',
  // 'single' means not to be subtyped.
  'single' => TRUE,
  // Name of a function which will render the block.
  'render callback' => 'party_attached_entity_edit_form_render',

  // Icon goes in the directory with the content type.
  'description' => t('Show a Profile2 edit form.'),
  'required context' => new ctools_context_required(t('Party'), 'entity:party'),
  'edit form' => 'party_attached_entity_edit_form_edit_form',
  'admin title' => 'party_attached_entity_edit_form_admin_title',

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
function party_attached_entity_edit_form_render($subtype, $conf, $args, $context) {
  $data_sets = party_get_data_set_info();
  $data_set = isset($conf['data_set_name']) && isset($data_sets[$conf['data_set_name']])
    ? $data_sets[$conf['data_set_name']]: NULL;

  $block = new stdClass();
  $block->title = t('Edit Party Attached Entity');
  $block->content = '';

  if (!empty($context->data) && $data_set) {
    // Get our controller
    $controller = $context->data->getDataSetController($data_set['set_name']);

    // Get our entity
    $attached_entity = $controller->getEntity(0, !empty($conf['create']));
    if (!empty($conf['create']) && !empty($attached_entity)) {
      $controller->attachEntity($attached_entity);
    }

    // If there's no attached_entity return an empty block
    if (!$attached_entity) {
      return $block;
    }

    $form = drupal_get_form('party_attached_entity_edit_form_form__' . $data_set['set_name'] . '__' . 0,
      $context->data, $data_set, 0);

    // Render it...
    $block->content = drupal_render($form);
  }

  return $block;
}

/**
 * Form
 */
function party_attached_entity_edit_form_form($form, &$form_state, $party, $data_set, $delta = 0) {
  // Load the include
  form_load_include($form_state, 'inc', 'party_dashboard', 'plugins/content_types/party_attached_entity_edit_form');
  form_load_include($form_state, 'inc', 'party', 'party.pages');

  // Get the form
  $form = party_attached_entity_edit_form($form, $form_state, $party, $data_set, $delta);

  // Add our submit handler and set the action
  $form['#submit'][] = 'party_attached_entity_edit_form_form_submit';
  $form['#action'] = base_path() . str_replace('ajax', 'nojs', request_path());

  // Remove URL Redirects
  foreach ($form as $name => &$element) {
    if (substr($name, 0, 1) != '#' && isset($element['redirect'])) {
      $element['redirect']['#access'] = FALSE;
    }
  }

  return $form;
}

/**
 * Submit Handler
 */
function party_attached_entity_edit_form_form_submit(&$form, &$form_state) {
  // Make sure we stop it redirecting anywhere it shouldn't...
  unset($form_state['redirect']);
}

/**
 * Config Form
 */
function party_attached_entity_edit_form_edit_form($form, &$form_state) {
  $data_sets = party_get_data_set_info();
  if (count($data_sets)) {
    $form['data_set_name'] = array(
      '#type' => 'select',
      '#title' => t('Data Set'),
      '#options' => array(),
      '#default_value' => !empty($form_state['conf']['data_set_name']) ? $form_state['conf']['data_set_name']: '',
    );
    foreach ($data_sets as $data_set_name => $data_set) {
      $form['data_set_name']['#options'][$data_set_name] = $data_set['label'];
    }

    $form['create'] = array(
      '#type' => 'checkbox',
      '#title' => t("Create entity if it doesn't exist"),
      '#default_value' => !empty($form_state['conf']['create']) ? $form_state['conf']['create']: '',
    );
  }
  else {
    $form['no_data_set'] = array(
      '#prefix' => '<p class="error">',
      '#markup' => t('There are no data sets configured for party.'),
      '#suffix' => '</p>',
    );
  }

  return $form;
}

function party_attached_entity_edit_form_edit_form_submit(&$form, &$form_state) {
  foreach (element_children($form) as $key) {
    if (isset($form_state['values'][$key])) {
      $form_state['conf'][$key] = $form_state['values'][$key];
    }
  }
}

/**
 * Title Callback
 */
function party_attached_entity_edit_form_admin_title($subtype, $conf, $context = NULL) {
  if ($conf['override_title'] && !empty($conf['override_title_text'])) {
    $output = format_string('"@context" !title', array(
      '@context' => $context->identifier,
      '!title' => filter_xss_admin($conf['override_title_text']),
    ));
  }
  else {
    $output = t('"@context" Attached Entity - %data_set_name', array(
      '@context' => $context->identifier,
      '%data_set_name' => isset($conf['data_set_name']) ? $conf['data_set_name']: t('Not Selected'),
    ));
  }
  return $output;
}
