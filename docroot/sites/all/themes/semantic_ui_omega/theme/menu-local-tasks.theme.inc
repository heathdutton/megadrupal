<?php

/**
 * @file
 * Contains the theme function override for 'menu_local_task'.
 */

/**
 * Returns HTML for primary and secondary local tasks.
 *
 * @ingroup themeable
 */

/**
 * Overrides theme_menu_local_tasks().
 */
function semantic_ui_omega_menu_local_tasks(&$variables) {
  $primary = $variables['primary'];
   if (isset($variables['primary']) && !empty($primary)) {
    foreach($primary as $menu_item_key => $menu_attributes) {
    $variables['primary'][$menu_item_key]['#link']['localized_options'] = array(
      'attributes' => array('class' => array('item')),
    );
    }
  }
  $output = '';

  if (!empty($variables['primary'])) {
    $variables['primary']['#prefix'] = '<h2 class="element-invisible">' . t('Primary tabs') . '</h2>';
    $variables['primary']['#prefix'] .= '<ul class="ui top attached tabular menu">';
    $variables['primary']['#suffix'] = '</ul>';
    $output .= drupal_render($variables['primary']);
  }
  if (!empty($variables['secondary'])) {
    $variables['secondary']['#prefix'] = '<h2 class="element-invisible">' . t('Secondary tabs') . '</h2>';
    $variables['secondary']['#prefix'] .= '<ul class="ui sub menu">';
    $variables['secondary']['#suffix'] = '</ul>';
    $output .= drupal_render($variables['secondary']);
  }
 return $output;

}
