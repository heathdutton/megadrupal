<?php
/**
 * @file
 *
 * Theming dashboard related element to be compatible
 * with jQuery UI and SigmaOne.
 */


/**
 * Returns HTML for a generic dashboard region.
 *
 * @param $variables
 *   An associative array containing:
 *   - element: A render element containing the properties of the dashboard
 *     region element, #dashboard_region and #children.
 *
 * @ingroup themeable
 */
function jqueryui_vtcore_dashboard_region($variables) {
  drupal_add_library('system', 'ui.accordion');
  extract($variables);
  $output = '<div id="' . $element['#dashboard_region'] . '" class="dashboard-region">';
  $output .= '<div class="region clearfix ui-accordion-icons ui-accordion ui-helper-reset ui-widget">';
  $output .= $element['#children'];
  // Closing div.region
  $output .= '</div>';
  // Closing div.dashboard-region
  $output .= '</div>';
  return $output;
}

/**
 * Returns HTML for disabled blocks, for use in dashboard customization mode.
 *
 * @param $variables
 *   An associative array containing:
 *   - blocks: An array of block objects from _block_rehash().
 *
 * @ingroup themeable
 */
function jqueryui_vtcore_dashboard_disabled_blocks($variables) {
  extract($variables);
  $output = '<div class="canvas-content ui-widget-content ui-helper-reset ui-widget ui-corner-all clearfix">
  					 <div class="canvas-message ui-state-highlight ui-corner-all">
  					 <span class="ui-icon ui-icon-info ui-helper-float-left"></span>' .
              t('Drag and drop these blocks to the columns below. Changes are automatically saved.
              	More options are available on the <a href="@dashboard-url">configuration page</a>.',
              array('@dashboard-url' => url('admin/dashboard/configure'))) . '</div>';

  $output .= '<div id="disabled-blocks" class="ui-widget-content ui-corner-all"><div class="region disabled-blocks ui-accordion-icons ui-accordion ui-helper-reset ui-widget clearfix">';
  foreach ($blocks as $block) {
    $output .= theme('dashboard_disabled_block', array('block' => $block));
  }

  $link_attributes = array(
    'attributes' => array(
      'class' => array(
        'ui-button-text',
      ),
    ),
  );
  $output .= '<div class="clearfix"></div></div></div>';
  $output .= '<p class="dashboard-add-other-blocks ui-button ui-widget ui-state-default ui-corner-all ui-button-text-only">' . l(t('Add other blocks'), 'admin/dashboard/configure', $link_attributes) . '</p>';
  $output .= '</div>';
  return $output;
}

/**
 * Returns HTML for disabled blocks, for use in dashboard customization mode.
 *
 * @param $variables
 *   An associative array containing:
 *   - block: A block object from _block_rehash().
 *
 * @ingroup themeable
 */
function jqueryui_vtcore_dashboard_disabled_block($variables) {
  _jqueryui_load_css(BASE_THEME, 'accordion');
  extract($variables);
  $output = "";
  if (isset($block)) {
    $output .= '<div id="block-' . $block['module'] . '-' . $block['delta']
    . '" class="disabled-block block block-' . $block['module'] . '-' . $block['delta']
    . ' module-' . $block['module'] . ' delta-' . $block['delta']
    . '">'
    . '<h3 class="block-title ui-corner-all ui-accordion-header ui-state-default ui-state-default ui-helper-reset">'
    . '<span class="ui-icon ui-icon-circle-arrow-e"></span>'
    . '<a href="#" class="header">'
    . (!empty($block['title']) && $block['title'] != '<none>' ? check_plain($block['title']) : check_plain($block['info']))
    . '</a>'
    . '</h3>'
    . '<div class="block-content ui-accordion-content ui-widget-content ui-corner-bottom ui-accordion-content-active ui-helper-reset"></div>'
    . '</div>';
  }
  return $output;
}