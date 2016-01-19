<?php

/**
 * @file
 * ctools content type that exposes the node add/edit
 * form vertical tab elements to panels pane.
 * This example uses the node context
 */

/**
 * $plugin array which will be used by the system that includes this file.
 */
$plugin = array(
  'title' => t('Node Vertical tab Elements To Hide'),
  'content_types' => 'node_vertical_tab_elements_hide',
  'single' => TRUE,
  'render callback' => 'node_vertical_tab_elements_hide_render',
  'icon' => 'icon.png',
  'description' => t('Provides elements of the node form vertical tab that are to be hidden'),
  'required context' => new ctools_context_required(t('Form'), 'node_form'),
  'edit form' => 'node_vertical_tab_elements_hide_edit_form',
  'admin title' => 'node_vertical_tab_elements_hide_admin_title',
  'category' => array(t('Form'), -9),
  'admin info' => 'node_vertical_tab_elements_hide_admin_info',
);

/**
 * Set the title of the vertical tab.
 */
function node_vertical_tab_elements_hide_admin_title($subtype, $conf, $context = NULL) {
  $output = t('Node Form Vertical Tabs');
  if ($conf['override_title'] && !empty($conf['override_title_text'])) {
    $output = filter_xss_admin($conf['override_title_text']);
  }
  return $output;
}

/**
 * Callback to provide administrative info.
 */
function node_vertical_tab_elements_hide_admin_info($subtype, $conf, $context = NULL) {
  return "Node Vertical Tab Elements Hidden";
}

/**
 * Run-time rendering of the body of the block (content type).
 *
 * @param array $subtype
 *   the subtype array
 * @param array $conf
 *   Configuration as done at admin time
 * @param array $args
 *   the arguments array
 * @param object $context
 *   Context - in this case we don't have any
 *
 * @return object
 *   An object with at least title and content members
 */
function node_vertical_tab_elements_hide_render($subtype, $conf, $args, &$context) {
  $block = new stdClass();

  foreach ($conf['hidden_tab_compononts'] as $key => $value) {
    if ($value !== 0) {
      $context->form[$key]['#access'] = FALSE;
    }
  }

  $block->title = "Node Vertical Tab Elements";
  return $block;
}

/**
 * Edit callback for the content type.Just returns a form.
 */
function node_vertical_tab_elements_hide_edit_form($form, &$form_state, $context) {

  $options = array('revision_information' => 'Revision information',
                   'menu' => 'Menu settings',
                   'path' => 'URL path settings',
                   'comment_settings' => 'Comment settings',
                   'author' => 'Authoring information',
                   'options' => 'Publishing options',
                   );

  $form['hidden_tab_compononts'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Select Vertical tab Compononts to Hide'),
    '#default_value' => !empty($form_state['conf']['hidden_tab_compononts'])?$form_state['conf']['hidden_tab_compononts']: $options,
    '#options' => $options,
    '#description' => t('The selected items will be hidden from the Form.'),
    );
       
  return $form;
}

/**
 * Submit callback for the content type edit form.
 */
function node_vertical_tab_elements_hide_edit_form_submit($form, &$form_state) {
  foreach (element_children($form) as $key) {
    if (!empty($form_state['values'][$key])) {
      $form_state['conf'][$key] = $form_state['values'][$key];
    }
  }
}
