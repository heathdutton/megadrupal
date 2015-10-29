<?php
// $Id$

/**
 * @file
 * Theme Functions - 960 Grid Layout
 *
 * @author: Daniel Honrade http://drupal.org/user/351112
 *
 */
 
/**
 * OM Grids on process html
 *
 * - Guides
 *
 */
function om_grids_html_vars(&$vars, $info) {

  // OM Grid Guides
  if (isset($info['settings']['grid_guide']) && ($info['settings']['grid_guide'] == 'on')) {
    $om_grid_buttons = '<div id="om-grid-guide-buttons"><div class="om-grid-title">OM 960 Grids</div>';
    $om_grid_buttons .= '<label><input name="om-grid-guide-button" type="radio" id="om-grid-guide-button-disabled" class="form-radio" value="" /> Disabled</label>';
    $om_grid_buttons .= '<label><input name="om-grid-guide-button" type="radio" id="om-grid-guide-button-12" class="form-radio" value="12" /> 12 Columns</label>';
    $om_grid_buttons .= '<label><input name="om-grid-guide-button" type="radio" id="om-grid-guide-button-16" class="form-radio" value="16" /> 16 Columns</label>';
    $om_grid_buttons .= '<label><input name="om-grid-guide-button" type="radio" id="om-grid-guide-button-24" class="form-radio" value="24" /> 24 Columns</label>';
    $om_grid_buttons .= '<label><input name="om-grid-guide-overlay" type="checkbox" id="om-grid-guide-overlay" class="form-checkbox" value="1" /> Overlay</label>';
    $om_grid_buttons .= '<label><input name="om-grid-guide-info" type="checkbox" id="om-grid-guide-info" class="form-checkbox" value="1" /> Info</label>';
    $om_grid_buttons .= '</div>';
  
    if (user_access('administer themes')) $vars['page_bottom'] .= $om_grid_buttons;
  }
  } 


/**
 * OM Grids on preprocess page
 *
 * - Guides
 * - Styles
 * - Scripts
 *
 */
function om_grids_page_vars(&$vars, $info) {
  
  $om_path = OM_BASE_THEME_PATH;
  
  // calculates middle wrapper width
  $grid        = isset($info['settings']['grid']) ? $info['settings']['grid']: 0;
  $grid_first  = isset($vars['sidebar_first_grid']) ? $vars['sidebar_first_grid']: 0;
  $grid_second = isset($vars['sidebar_second_grid']) ? $vars['sidebar_second_grid']: 0;    
  $vars['wrapper_middle_grid'] = ($grid != 0) ? ' grid-' . ($grid - ($grid_first + $grid_second)): '';

  // OM Grid Guides
  if (user_access('administer themes') && isset($info['settings']['grid_guide']) && ($info['settings']['grid_guide'] == 'on')) drupal_add_js($om_path . '/js/om_grids.js'); 

  // adding grid css
  $grids_allowed = array(12, 16, 24);
  if (isset($info['settings']['grid']) && in_array($info['settings']['grid'], $grids_allowed)) drupal_add_css($om_path . '/css/om_grids.css', array('group' => CSS_THEME)); 
}



