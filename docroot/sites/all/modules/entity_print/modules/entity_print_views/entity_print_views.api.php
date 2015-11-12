<?php

/**
 * @file
 * This file provides not working code and exists only to provide examples of
 * using the Entity Print Views API's.
 *
 * For further documentation see: https://www.drupal.org/node/2430561
 */

/**
 * This hook is provided to allow modules to add their own CSS files.
 *
 * Note, you can also manage the CSS files from your theme.
 * @see https://www.drupal.org/node/2430561#from-your-theme
 *
 * @param object $view
 *   The view object.
 */
function hook_entity_print_views_css($view) {
  if ($view->name === 'some_view') {
    // Add a table.css file from my module.
    entity_print_add_css(drupal_get_path('module', 'my_module') . '/css/table.css');
  }
}

/**
 * Allows other modules to get hold of the pdf object for making changes.
 *
 * Only use this function if you're not able to achieve the right outcome with
 * a custom template and CSS.
 *
 * @param \WkHtmlToPdf $pdf
 *   The pdf object.
 * @param object $view
 *   The view object we're rendering.
 */
function hook_entity_print_views_pdf_alter(WkHtmlToPdf $pdf, $view) {
  $terms = variable_get('terms_and_conditions', '');
  $pdf->addPage($terms);
}
