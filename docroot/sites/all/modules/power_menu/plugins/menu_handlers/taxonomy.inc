<?php

/**
 * The taxonomy plugin....
 */
if (module_exists('taxonomy')) {
  $plugin = array(
    'title' => t("Taxonomy plugin"),
    'description' => t('Sets the active menu trail based on a taxonomy term linked to a menu link.'),
    'has edit form' => TRUE,
    'handler' => array(
      'class' => 'PowerMenuTaxonomyHandler',
    ),
  );
}

/**
 * Implements the power_menu_taxonomy_terms:#post_render from PowerMenuTaxonomyHandler::menuFormAlter().
 * 
 * Disable the select option for used terms in other menu links.
 */
function power_menu_taxonomy_terms_post_render($content, $element) {
  $terms = variable_get('power_menu_taxonomy_terms', array());
  $mlid = arg(4);

  foreach ($terms as $key => $value) {
    // Is the mlid not used for the termid, disable it
    if ($mlid != $value) {
      $content = preg_replace('/( value="' . $key . '")/', '$1 disabled="disabled"', $content, 1);
    }
  }

  return $content;
}
