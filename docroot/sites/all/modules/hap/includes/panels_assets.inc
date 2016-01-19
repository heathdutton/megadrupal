<?php

/**
 * @file
 *  Register assets to be included during render of configured Panel variant.
 */


/**
 * Implements hook_hap_info().
 */
function hap_panels_assets_hap_info() {
  return array(
    'name' => 'Panels Assets',
    'description' => 'Provides a hook to register "application-level" assets to be included during the render of any configured Panels variant.',
    'hooks' => array('panels_post_render', 'form_panels_panel_context_edit_content_alter'),
    'dependencies' => array(
      'modules' => array('panels'),
    ),
  );
}


/**
 * Retrieve a list of Panels assets.
 */
function hap_panels_assets() {
  static $assets;
  if (!isset($assets)) {
    $assets = array();
    foreach (module_implements('hap_panels_assets') as $module) {
      foreach (module_invoke($module, 'hap_panels_assets') as $name => $info) {
        $assets[$name] = $info + array(
          'label' => '',
          'description' => '',
          'module' => $module,
          'css' => array(),
          'js' => array(),
          'library' => array(),
        );
      }
    }

    drupal_alter('hap_panels_assets', $assets);
  }

  return $assets;
}


/**
 * Helper function to produce an #options array for form builders.
 */
function _hap_panels_assets_options() {
  $options = array();
  foreach (hap_panels_assets() as $name => $info) {
    $options[$name] = $info['label'];
  }
  return $options;
}


/**
 * Implementation of hook_panels_post_render().
 */
function hap_panels_assets_panels_post_render($renderer) {
  $assets = hap_panels_assets();
  foreach ($renderer->panel_settings['hap_panels_assets'] as $conf) {
    foreach (array('css', 'js', 'library') as $type) {
      if (!empty($assets[$conf][$type])) {
        $function = 'drupal_add_' . $type;
        foreach ($assets[$conf][$type] as $item) {
          call_user_func_array($function, $item);
        }
      }
    }
  }
}


/**
 * Implements hook_form_FORM_ID_alter().
 * @see panels_panel_context_edit_content().
 */
function hap_panels_assets_form_panels_panel_context_edit_content_alter(&$form, &$form_state) {
  $options = _hap_panels_assets_options();

  // Drop out if there are no options.
  if (empty($options)) return;

  $display = $form_state['display'];
  $form['#submit'][] = '_hap_panels_assets_panels_panel_context_edit_content_submit';
  $form['hap_panels_assets'] = array(
    '#type' => 'checkboxes',
    '#options' => _hap_panels_assets_options(),
    '#title' => 'Highly Attractive Panels Assets',
    '#default_value' => isset($display->panel_settings['hap_panels_assets']) ? $display->panel_settings['hap_panels_assets'] : '',
    '#description' => t('In Themepacket methodology, it is important that a theme packets is responsible for styling and adding behaviors to their OWN markup, and never assume relationships with external elements. That said, Panel layout plugins should not assume theme packets embedded within them, likewise, theme packets should not assume layout (width, float, margin, etc) in respect to its Panel layout. The solution is to include "application-level" stylesheets and scripts that act as the "integration" assets.'),
  );
}

/**
 * Submission handler: ensures we're saving the asset reference in the right place.
 * @see hap_panels_assets_form_panels_panel_context_edit_content_alter().
 */
function _hap_panels_assets_panels_panel_context_edit_content_submit($form, &$form_state) {
  $display = &$form_state['display'];
  $display->panel_settings['hap_panels_assets'] = array_filter($form_state['values']['hap_panels_assets']);
}
