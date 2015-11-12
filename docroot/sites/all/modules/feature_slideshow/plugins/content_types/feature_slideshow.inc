<?php

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
    'title' => t('Slideshow Embed'),
    'description' => t('Add a slideshow to a page'),
    // 'single' => TRUE means has no subtypes.
    'single' => TRUE,
    // Constructor.
    'content_types' => array('feature_slideshow_content_type'),
    // Name of a function which will render the block.
    'render callback' => 'feature_slideshow_content_type_render',
    // The default context.
    'defaults' => array(),
    'admin info' => 'feature_slideshow_admin_info',
    // This explicitly declares the config form. Without this line, the func would be
    // ctools_plugin_example_no_context_content_type_edit_form.
    'edit form' => 'feature_slideshow_content_type_edit_form',
    'category' => array(t('Custom Widgets'), -9),
        // this example does not provide 'admin info', which would populate the
        // panels builder page preview.
);

/**
 * Run-time rendering of the body of the block.
 *
 * @param $subtype
 * @param $conf
 *   Configuration as done at admin time.
 * @param $args
 * @param $context
 *   Context - in this case we don't have any.
 *
 * @return
 *   An object with at least title and content members.
 */
function feature_slideshow_content_type_render($subtype, $conf, $args, $context) {
  $block = new stdClass();

  // Extract the node id.
  $nid = entity_autocomplete_get_id($conf['slideshow']);

  // Load the slideshow
  $slideshow = node_load($nid);

  $block->title = 'Featured';
  $block->content = _feature_slideshow_generate_carousel( $slideshow);

  return $block;
}

/**
 * 'Edit form' callback for the content type.
 * This example just returns a form; validation and submission are standard drupal
 * Note that if we had not provided an entry for this in hook_content_types,
 * this could have had the default name
 * ctools_plugin_example_no_context_content_type_edit_form.
 *
 */
function feature_slideshow_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];



   $form['slideshow'] = array(
    '#type' => 'entity_autocomplete',
    '#title' => 'Select a slideshow to display in this panel region',
    '#entity_type' => 'node',
     '#bundles' => array('slideshow'),
     '#default_value' => $conf['slideshow']
  );



  $form['override_title']['#access'] = false;
  $form['override_title_text']['#access'] = false;
  $form['override_title_markup']['#access'] = false;


  return $form;
}

function feature_slideshow_content_type_edit_form_submit(&$form, &$form_state) {
  $keys = array('slideshow');
  foreach ($keys as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}


/**
 * Ctools plugin function hook_admin_info.
 */
function feature_slideshow_admin_info($subtype, $conf, $context = NULL) {



  if (empty($output) || !is_object($output)) {
    $output = new stdClass();
    $output->title = 'You have selected a slideshow: ' . $conf['slideshow'] ;
    $output->content = t('No info available.');
  }

  return $output;
}

