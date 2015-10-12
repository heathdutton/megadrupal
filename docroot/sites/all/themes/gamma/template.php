<?php
// $Id: template.php,v 1.1 2011/01/07 16:10:59 himerus Exp $

/**
 * @file
 * Starter template.php file for subthemes of Omega.
 */

/*
 * Add any conditional stylesheets you will need for this sub-theme.
 *
 * To add stylesheets that ALWAYS need to be included, you should add them to
 * your .info file instead. Only use this section if you are including
 * stylesheets based on certain conditions.
 */

/**
 * Implements hook_theme().
 */
function gamma_theme(&$existing, $type, $theme, $path) {
  $hooks = array();
  //$hooks = omega_theme($existing, $type, $theme, $path);
  return $hooks;
}

function gamma_field__taxonomy_term_reference($vars) {
  $output = '';
  
  // Render the label, if it's not hidden.
  if (!$vars['label_hidden']) {
    $output .= '<h3 class="field-label">' . $vars['label'] . ': </h3>';
  }

  // Render the items.
  $terms = array();
  foreach ($vars['items'] as $delta => $item) {
    $terms[$delta] = drupal_render($item);
  }
  $output .= theme('item_list', array('items' => $terms, 'attributes' => array('class' => 'taxonomy-terms')));

  // Render the top-level DIV.
  $output = '<div class="' . $vars['classes'] . (!in_array('clearfix', $vars['classes_array']) ? ' clearfix' : '') . '">' . $output . '</div>';

  return $output;
}