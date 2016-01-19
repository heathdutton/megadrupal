<?php

/**
 * @file
 * Plugin definition.
 */
 
if (module_exists('blog')) {
  // Array $plugin contains the description of the plugin.
  $plugin = array(
    'title' => t('Site Map (blogs)'),
    'description' => t('Displays site map (active blog authors).'),
    'category' => t('Miscellaneous'),
    'defaults' => array(),
    'single' => TRUE,
  );
}

/**
 * Render callback.
 * 
 * The function to define the pane when it is included on a panel.
 *
 * @param $subtype
 * @param $conf
 * @param $args
 * @param $context
 * @return stdClass
 */
function site_map_panels_site_map_panels_blogs_content_type_render($subtype, $conf, $args, $contexts) {
  $block = new stdClass();

  $block->title = !empty($conf['override_title']) ? check_plain($conf['override_title_text']) : '';
  // _site_map_blogs() is function from module Site Map. It provides display
  // the latest blogs.
  $block->content = _site_map_blogs();

  return $block;
}

/**
 * Edit form callback.
 *
 * @param $form_state
 * @param $form
 * @return array
 */
function site_map_panels_site_map_panels_blogs_content_type_edit_form($form, &$form_state) {
  return $form;
}

/**
 * Submit handler for edit form.
 * 
 * @see site_map_panels_site_map_panels_blogs_content_type_edit_form()
 *
 * @param $form_state
 * @param $form
 */
function site_map_panels_site_map_panels_blogs_content_type_edit_form_submit($form, &$form_state) {
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}

