<?php

/**
 * Override or insert variables into the region template.
 */
function bluemarine_preprocess_region(&$vars, $hook) {
  // Sidebar regions get an extra class.
  if (strpos($vars['region'], 'sidebar_') === 0) {
    $vars['classes_array'][] = 'column';
  }
}

/**
 * Override or insert variables into the page template.
 */
function bluemarine_preprocess_page(&$vars, $hook) {
  $vars['title_attributes_array']['class'][] = 'title';
}

/**
 * Override or insert variables into the node template.
 */
function bluemarine_preprocess_node(&$vars, $hook) {
  $vars['title_attributes_array']['class'][] = 'title';
}

/**
 * Override or insert variables into the comment template.
 */
function bluemarine_preprocess_comment(&$vars, $hook) {
  $vars['title_attributes_array']['class'][] = 'title';
}

/**
 * Override or insert variables into the block template.
 */
function bluemarine_preprocess_block(&$vars, $hook) {
  $vars['title_attributes_array']['class'][] = 'title';
}

/*
 * Process variables for page.tpl.php.
 */
function bluemarine_process_page(&$vars) {
  // Hook into color.module.
  if (module_exists('color')) {
    _color_page_alter($vars);
  }
}

/*
 * Process variables for html.tpl.php.
 */
function bluemarine_process_html(&$vars) {
  // Hook into color.module.
  if (module_exists('color')) {
    _color_html_alter($vars);
  }
}
