<?php
/**
 * @file
 * Add To Stackly Widget.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'single' => TRUE,
  'icon' => 'icon_node_form.png',
  'title' => t('stackly: Add To stackly Link'),
  'description' => t('Provides a link to add an item to stackly'),
  'category' => t('stackly'),
  'render callback'  => 'stackly_add_to_stack_render',
  'edit form'        => 'stackly_add_to_stack_edit_form',
);

/**
 * Output function for the 'add_to_stackly' content type.
 */
function stackly_add_to_stack_render($subtype, $conf, $panel_args, &$context) {
  $block = new stdClass();
  $block->title = '';
  $theme_vars = array();

  $node = menu_get_object('node', 1);
  if ($node) {
    // Replace to token, with the node value.
    if ($conf['stackly_doi']) {
      $theme_vars['data-id-doi'] = token_replace($conf['stackly_doi'], array('node' => $node));
    }
  }

  $block->content = theme('add_to_stackly', $theme_vars);
  return $block;
}

/**
 * Configuration form.
 */
function stackly_add_to_stack_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];

  $form['stackly_for_publishers'] = array(
    '#type' => 'fieldset',
    '#title' => t('Stackly Publisher Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['stackly_for_publishers']['stackly_doi'] = array(
    '#type' => 'textfield',
    '#title' => t('DOI'),
    '#description' => t('The DOI for content, tokens available for use.'),
    '#default_value' => $conf['stackly_doi'],
  );

  if (module_exists('token')) {
    $form['token_tree'] = array(
      '#theme' => 'token_tree',
      '#token_types' => array('node'),
    );
  }
  else {
    $form['token_tree'] = array(
      '#markup' => '<p>' . t('Enable the <a href="@drupal-token">Token module</a> to view the available token browser.', array('@drupal-token' => 'http://drupal.org/project/token')) . '</p>',
    );
  }

  return $form;
}

/**
 * Configuration form submit handler.
 */
function stackly_add_to_stack_edit_form_submit(&$form, &$form_state) {
  foreach ($form_state['values'] as $key => $value) {
    $form_state['conf'][$key] = $value;
  }
}
