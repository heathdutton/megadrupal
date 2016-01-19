<?php

/**
 * @file
 * Hooks provided by Instance settings module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Provides logic that should be executed when instance settings are applied.
 */
function hook_instance_settings_apply() {
  drupal_set_message(t('Instance settings applied. Please make sure you cleared a cache.'));
}

/**
 * @} End of "addtogroup hooks".
 */
