<?php
/**
 * @file
 * Bean theme templates for Context Bean children.
 */

/**
 * Define theme templates.
 */
function theme_context_bean_child(&$variables) {
  $child_bean = entity_view('bean', array($variables['entity']), $variables['view_mode']);
  $content = '<div id="block-bean-' . $variables['entity']->delta . '" class="block block-bean ' . $variables['css_class'] . '">';
  $content .= '<h2>' . $variables['entity']->title . '</h2>';
  $content .= drupal_render($child_bean);
  $content .= '</div>';
  return $content;
}
