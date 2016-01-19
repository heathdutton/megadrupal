<?php

/**
 * @file
 * This file provides not working code and exists only to provide examples of
 * using the Entity Print API's.
 *
 * For further documentation see: https://www.drupal.org/node/2430561
 */

/**
 * This hook is provided to allow modules to add their own CSS files.
 *
 * Note, you can also manage the CSS files from your theme.
 * @see https://www.drupal.org/node/2430561#from-your-theme
 *
 * @param string $entity_type
 *   The entity type of the entity we're rendering.
 * @param object $entity
 *   The entity we're rending.
 */
function hook_entity_print_css($entity_type, $entity) {
  // An example of adding two stylesheets for any commerce_order entity.
  if ($entity_type === 'commerce_order') {
    $path = drupal_get_path('module', 'entity_print_commerce_order');
    entity_print_add_css($path . '/css/table.css');
    entity_print_add_css($path . '/css/commerce-order.css');
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
 * @param string $entity_type
 *   The entity type of the entity we're rendering.
 * @param $entity
 *   The entity we're rending.
 */
function hook_entity_print_pdf_alter(WkHtmlToPdf $pdf, $entity_type, $entity) {
  $terms = variable_get('terms_and_conditions', '');
  $pdf->addPage($terms);
}
