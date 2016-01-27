<?php

/**
 * @file
 * Hooks provided by View Mode Templates module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Implements hook_view_mode_templates().
 */
function hook_view_mode_templates() {
  return array(
    'ENTITY_TYPE__BUNDLE__VIEW_MODE', // example node__article__teaser
    'ENTITY_TYPE__BUNDLE' // example node__article
    'ENTITY_TYPE__VIEW_MODE' // example node__teaser
  );
}

/**
 * @} End of "addtogroup hooks".
 */
