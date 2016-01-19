<?php

/**
 * @file
 * Main file for the the webform content type definition.
 */

/**
 * Ctools plugin configuration.
 *
 * This $plugin array which will be used by the system that includes this file.
 */
$plugin = array(
  'single' => FALSE,
  'title' => t('Rendered Webform'),
  'description' => t('Shows a rendered Webform.'),
  'category' => t('Webforms'),
  'edit form' => 'webform_panels_webform_panels_edit_form',
  'render callback' => 'webform_panels_webform_panels_render',
  'admin info' => 'webform_panels_webform_panels_admin_info',
  'content types' => 'webform_panels_webform_panels_type_content_types',
  'defaults' => array(
    'selected_forms' => 1,
  ),

);

/**
 * Content types callback.
 *
 * This callback returns the list of subtypes available to the content type
 * plugin.
 */
function webform_panels_webform_panels_type_content_types() {

  $types = array();

  // Selects the node types that have webform components associated with them.
  $query = db_select('webform_component', 'w')
    ->fields('w', array('nid'))
    ->distinct();
  $query->leftjoin('node', 'n', 'n.nid = w.nid');
  $query->fields('n', array('type'));

  // Builds an array with the node types and their content type ctools info.
  foreach ($query->execute() as $result) {

    $types[$result->type] = array(
      'title' => node_type_get_name($result->type),
      'category' => t('Webforms'),
    );
  }

  return $types;
}

/**
 * Edit form callback for the content type.
 *
 * This edit form presents the options available when adding a webform to a
 * panel. It allows the user to specify a view mode and the node that is to be
 * rendered.
 */
function webform_panels_webform_panels_edit_form($form, &$form_state) {

  $conf = $form_state['conf'];

  // Sets the subtype according with the chosen value in the UI.
  $subtype = $form_state['subtype_name'];

  // Gets the info for node entity with all the available view modes.
  $entity_info = entity_get_info('node');

  // Creates the list of view modes for this bundle.
  $bundle_settings = field_bundle_settings('node', $subtype);

  $options = array(

    'default' => 'Default',
  );
  if (!empty($bundle_settings['view_modes'])) {
    foreach ($bundle_settings['view_modes'] as $mode => $settings) {

      if ($settings['custom_settings']) {

        $options[$mode] = $entity_info['view modes'][$mode]['label'];
      }
    }
  }

  // Adds the view mode options to the form.
  if (count($options) > 1) {
    $form['view_mode'] = array(
      '#type' => 'select',
      '#options' => $options,
      '#title' => t('View mode'),
      '#default_value' => $conf['view_mode'],
    );
  }
  else {
    $form['view_mode_info'] = array(
      '#type' => 'item',
      '#title' => t('View mode'),
      '#description' => t('Only one view mode is available for this entity type.'),
      '#markup' => $options ? current($options) : t('Default'),
    );

    $form['view_mode'] = array(
      '#type' => 'value',
      '#value' => $options ? key($options) : t('default'),
    );
  }

  // Select options with the nodes of the selected subtype which have webforms.
  $subtype_webforms = webform_panels_get_all_forms($subtype);
  $form['selected_forms'] = array(

    '#title' => t('Webform to embed'),
    '#description' => t('Used to embed the selected webform in the page.'),
    '#type' => 'select',
    '#options' => $subtype_webforms,
    '#default_value' => $conf['selected_forms'],
    '#required' => TRUE,
  );
  return $form;
}

/**
 * Helper function that Retrieves all nodes that have webform components.
 *
 * @return options[]
 *   An array of with the returned webforms keyed nid->title
 */
function webform_panels_get_all_forms($subtype) {

  // Select all nodes from the database that have webform components.
  $query = db_select('webform_component', 'w')->fields('n', array('nid'))->fields('n', array('title'))->distinct();
  $query->leftjoin('node', 'n', 'n.nid = w.nid');
  $query->condition('n.type', $subtype, '=');

  // Add node_access tag to show only nodes the user can access.
  $query->addTag('node_access');

  // Builds an array with all the nodes returned by the query.
  $options = $query->execute()->fetchAllKeyed();

  return $options;
}

/**
 * The edit form submit handler.
 *
 * Saves the submitted form data in the edit form.
 */
function webform_panels_webform_panels_edit_form_submit($form, &$form_state) {

  if (isset($form_state['values']['selected_forms'])) {
    $form_state['conf']['selected_forms'] = $form_state['values']['selected_forms'];
  }
  if (isset($form_state['values']['view_mode'])) {
    $form_state['conf']['view_mode'] = $form_state['values']['view_mode'];
  }
}

/**
 * Render callback for the content type plugin.
 *
 * Renders the webform in the page using the selected view mode.
 */
function webform_panels_webform_panels_render($subtype, $conf, $panel_args, $context = NULL) {

  $block = new stdClass();
  $view_mode = $conf['view_mode'];

  // Add the rendered node in the selected view mode to the block.
  $node = node_load($conf['selected_forms']);

  // Check if user has access to the node being rendered.
  $access = node_access('view',$node);

  // If the user has access to these webforms, render them.
  if($access){

    $render = node_view($node, $view_mode);
    $block->content .= render($render);
  }

  return $block;
}

/**
 * Implements hook_PLUGIN_content_type_admin_title().
 *
 * Returns the administrative title for a webform in a panel.
 */
function webform_panels_webform_panels_content_type_admin_title($entity_type, $conf, $contexts) {

  // Get all available view modes for node.
  $entity_info = entity_get_info('node');
  // Get the chosen view mode.
  $view_mode = $conf['view_mode'];
  // Get the chosen node.
  $node = node_load($conf['selected_forms']);

  if (isset($entity_info['view modes'][$view_mode])) {
    $view_mode = $entity_info['view modes'][$view_mode]['label'];
  }
  return t('Rendered webform "%title" using view mode "@view_mode"', array('%title' => $node->title, '@view_mode' => $view_mode));
}
