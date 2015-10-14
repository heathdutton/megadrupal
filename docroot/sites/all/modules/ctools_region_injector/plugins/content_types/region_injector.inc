<?php
/**
 * @file
 * Code for the CTools region injector plugin.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'single' => TRUE,
  'title' => t('Region Content Injector'),
  'description' => t('If the displayed page has the specified region, it extracts that content and displays it in this pane.'),
  'required context' => new ctools_context_required(t('Managed Page'), 'managed_page'),
  'category' => t('Page elements'),
);

/**
 * Returns an edit form for the custom type.
 */
function ctools_region_injector_region_injector_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];

  $region_name = isset($conf['region_name']) ? $conf['region_name'] : '';

  $form['region_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Region Machine Name'),
    '#description' => t('Please specify the region machine name, from which the content should be taken.'),
    '#default_value' => $region_name,
  );

  return $form;
}

/**
 * Plugin configuration form.
 */
function ctools_region_injector_region_injector_content_type_edit_form_submit($form, &$form_state) {
  $form_state['conf']['region_name'] = $form_state['values']['region_name'];
}

/**
 * Render the content.
 */
function ctools_region_injector_region_injector_content_type_render($subtype, $conf, $panel_args, $context) {
  if (!empty($context) && empty($context->data)) {
    return NULL;
  }

  $block = new stdClass();
  $block->module = 'ctools_region_injector';
  $block->title = '';

  $content = '';

  $region_name = $conf['region_name'];

  // If the managed page contains a certain region, we want to take the output
  // of that region, and display it in this pane.
  // This way we can inject context-aware content from inside a panel page,
  // outside of it, in the site template.
  if (isset($context->data['handler']->conf['display']->renderer_handler->rendered['regions'][$region_name])) {
    $content = $context->data['handler']->conf['display']->renderer_handler->rendered['regions'][$region_name];
    $block->css_class = 'region-injected-' . $region_name;
  }

  $block->content = $content;
  return $block;
}

/**
 * Returns the region which is injected into the pane.
 */
function ctools_region_injector_region_injector_content_type_admin_title($subtype, $conf) {
  if (isset($conf['region_name'])) {
    $output = t('Content injected from region @var', array('@var' => $conf['region_name']));
  }
  else {
    $output = t('No region given from which to inject content.');
  }
  return $output;
}