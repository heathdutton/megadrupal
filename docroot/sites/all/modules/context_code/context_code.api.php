<?php

/**
 * @file
 * Hooks provided by Context code module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Implements hook_context_code().
 */
function hook_context_code() {
  return array(
    'context_code' => t('Context code')
  );
}

/**
 * @} End of "addtogroup hooks".
 */
