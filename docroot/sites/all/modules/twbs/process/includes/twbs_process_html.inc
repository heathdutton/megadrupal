<?php

/**
 * Process variables for html.tpl.php
 *
 * Perform final addition and modification of variables before passing into
 * the template. To customize these variables, call drupal_render() on elements
 * in $variables['page'] during THEME_preprocess_page().
 *
 * @see twbs_preprocess_html()
 * @see html.tpl.php
 */
function twbs_process_html(&$variables) {
  // Render page_top and page_bottom into top level variables.
  $variables['page_top'] = drupal_render($variables['page']['page_top']);
  $variables['page_bottom'] = drupal_render($variables['page']['page_bottom']);
  // Place the rendered HTML for the page body into a top level variable.
  $variables['page']              = $variables['page']['#children'];
  $variables['page_bottom'] .= drupal_get_js('footer');

  $variables['head']    = drupal_get_html_head();
  $variables['css']     = drupal_add_css();
  $variables['styles']  = drupal_get_css();
  $variables['scripts'] = drupal_get_js();
}
