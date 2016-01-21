<?php

/**
 * @file
 * This file contains no working PHP code; it exists to provide additional
 * documentation for doxygen as well as to document hooks in the standard
 * Drupal manner.
 */

/**
 * Allows modules to alter MagazineLayout options.
 *
 * @param $options
 *   Array of options used while instantiating MagazineLayout class.
 * @param $context
 *   Keyed array of additional data specific to where the hook was called from.
 *   It will include name of caller function, plus additionally all parameters
 *   of imagematrix_field_formatter_view() function (in case of entity display),
 *   or full view object (when called from within a view).
 */
function hook_imagematrix_layout_options_alter(&$options, $context) {
  // If hook was invoked from within entity display.
  if ($context['function'] == 'imagematrix_field_formatter_view') {
    // If entity is a node of type "article", and its nid == 5,
    // then let's randomize its layout.
    if (
      $context['entity_type'] == 'node'
      && $context['entity']->type == 'article'
      && $context['entity']->nid == 5
    ) {
      $options['randomize_layout'] = TRUE;
    }
  }
  // Alternatively, if hook was invoked from within the view.
  elseif ($context['function'] == 'pre_render') {
    // Let's give all blocks the same height.
    $options['block_height'] = 600;
  }
}
