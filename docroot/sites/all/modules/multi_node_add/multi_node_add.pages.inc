<?php
/**
 * @file
 * Contains the logic of the configuration form
 * and the iframe-related codes.
 */

/**
 * Provides a page where the fields to use to create the nodes can be selected.
 */
function multi_node_add_page($form, &$form_state, $type = FALSE) {
  if ($type == FALSE) {
    $all_types = array_keys(node_type_get_types());
    $type = end($all_types);
    drupal_goto('multi_node_add/' . str_replace('_', '-', $type));
  }

  drupal_add_js(drupal_get_path('module', 'multi_node_add') . '/multi_node_add.js');
  $settings = array(
    'callback' => url('multi_node_add/frame/' . $type, array('absolute' => TRUE)),
  );
  drupal_add_js(array('multiNodeAdd' => $settings), 'setting');

  $prefilled = FALSE;
  if (isset($_GET['fields']) && isset($_GET['num'])) {
    drupal_add_js(
      array(
        'multiNodeAddPreload' => array(
          'fields' => explode(',', $_GET['fields']),
          'num' => $_GET['num'],
          'view' => !empty($_GET['view']) ? 1 : 0,
        ),
      ),
      'setting'
    );
    $prefilled = TRUE;
  }

  if (!$prefilled) {

    $type = str_replace('-', '_', $type);
    $fields = field_info_instances('node', $type);
    $extra = field_info_extra_fields('node', $type, 'form');
    $req_val = array();
    $field_req = array();
    $field_names = array();

    foreach ($extra as $field_name => $entry) {
      $field_req[$field_name] = $entry['label'];
      $req_val[$field_name] = $field_name;
    }

    foreach ($fields as $field_name => $entry) {
      if ($entry['required']) {
        $field_req[$field_name] = $entry['label'];
        $req_val[$field_name] = $field_name;
      }
      else {
        $field_names[$field_name] = $entry['label'];
      }
    }

    $form = array();
    $form['hint']['#markup'] = '<noscript><div class="warning messages">' . t('Multi Node Add requires Javascript to provide the needed functionality') . '</div></noscript>';
    $form['info']['#value'] = t('Current content-type: %type', array('%type' => $type));
    if (!empty($field_req)) {
      $form['fields_req'] = array(
        '#type' => 'checkboxes',
        '#options' => $field_req,
        '#default_value' => $req_val,
        '#title' => t('Mandatory fields'),
        '#attributes' => array('class' => array('multi-node-add')),
        '#disabled' => TRUE,
      );
    }
    if (!empty($field_names)) {
      $form['fields_to_utilize'] = array(
        '#type' => 'checkboxes',
        '#options' => $field_names,
        '#title' => t('Fields to manage'),
        '#attributes' => array('class' => array('multi-node-add')),
        '#description' => t('Choose those fields that you would like to edit on the new nodes'),
      );
    }

    // If there are no available fields, we should not offer a form.
    if (empty($field_names) && empty($field_req)) {
      drupal_set_message(t('Unable to generate multiple nodes for this content type (failed to detect usable fields).'), 'warning');
      return $form;
    }

    $form['number'] = array(
      '#type' => 'textfield',
      '#default_value' => 2,
      '#size' => 2,
      '#required' => TRUE,
      '#title' => t('Number of rows'),
    );
    $form['view'] = array(
      '#type' => 'checkbox',
      '#default_value' => FALSE,
      '#title' => t('Display node after saving it'),
    );
    $form['show'] = array(
      '#type' => 'button',
      '#value' => t('Show'),
    );
    $form['shortcut'] = array(
      '#type' => 'button',
      '#value' => t('Get shortcut URL'),
    );
  }

  $common_attr = array(
    '#attributes' => array(
      'class' => array(
        'second-step',
      ),
    ),
  );
  $form['addmore'] = array(
    '#type' => 'button',
    '#value' => t('Add 2 more nodes'),
  ) + $common_attr;
  $form['create'] = array(
    '#type' => 'button',
    '#value' => t('Create all nodes'),
  ) + $common_attr;
  $form['prepopulate'] = array(
    '#type' => 'button',
    '#value' => t('Prepopulate based on first form'),
  ) + $common_attr;
  $form['placeholder']['#markup'] = '<div id="multi_node_add_frames"></div>';

  return $form;
}

/**
 * Shows the node-tabled form in an iFrame.
 */
function multi_node_add_frame_page($type) {
  global $user;
  $node = (object) array(
    'uid' => $user->uid,
    'name' => (isset($user->name) ? $user->name : ''),
    'type' => $type,
    'language' => LANGUAGE_NONE,
  );
  $form_state = array();
  $form_state['build_info'] = array();
  $form_state['build_info']['args'] = array($node);
  form_load_include($form_state, 'inc', 'node', 'node.pages');
  _multi_node_add_bare_page(drupal_build_form($type . '_node_form', $form_state));
}

/**
 * Shows the status after node creation.
 */
function multi_node_add_frame_status($node, $view) {
  if (empty($view)) {
    $page_content = t('The node is created. Title: %title , node id: !nid',
      array(
        '%title' => $node->title,
        '!nid' => l($node->nid, 'node/' . $node->nid,
          array(
            'attributes' => array(
              'target' => '_blank',
            ),
          )
        ),
      )
    );
  }
  else {
    $page_content = node_view($node);
  }
  _multi_node_add_bare_page($page_content);
}

/**
 * Outputs a bare HTML page with minimal content (no blocks, etc).
 */
function _multi_node_add_bare_page($output) {

  // It's needed to invoke as we use the html template, not the full-blown page
  // error messages must not be discarded.
  if (is_array($output)) {
    $output['messages'] = array(
      '#markup' => theme('status_messages'),
      '#weight' => -99,
    );
  }
  else {
    $output = theme('status_messages') . $output;
  }

  drupal_set_page_content($output);
  drupal_add_css(drupal_get_path('module', 'multi_node_add') . '/multi_node_add.css');
  $page = array();
  $page['page'] = drupal_set_page_content();
  $page['#theme_wrappers'] = array('html');
  print drupal_render($page);
  module_invoke_all('exit');
  exit();
}
