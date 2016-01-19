<?php

/**
 * @file
 * Describe hooks provided by the Views Access Callback module.
 */

/**
 * @defgroup views_access_callback_hooks Views Access Callback hooks
 * @{
 * Hooks that can be implemented by other modules in order to implement the
 * Views Access Callback API.
 */

/**
 * Describes callback functions to Views Access Callback.
 *
 * This hook should be placed in MODULENAME.module.
 *
 * @return
 *   An associative array describing the callback functions. Primary key is
 *   the name of the function that should be called to check whether a user
 *   has access to the view. The value for the key entry is a translated
 *   string describing the callback function.
 *
 * Any arguments passed to the view will also be passed to the callback
 * function.
 */
function hook_views_access_callbacks() {
  $callbacks['views_access_callback_view_access'] = t('Access my view');
  return $callbacks;
}

/**
 * @}
 */

/**
 * @defgroup views_access_callback About Views Access Callback
 * @{
 * In Views Access Callback, a callback is a function that is invoked by views
 * to check whether a user may access a a given view. The callback function
 * should return a boolean. TRUE means the user may access the view, FALSE
 * means the user may not.
 *
 * @code
 * function mymodule_views_access_callbacks() {
 *   return array('mymodule_monday_access' => t('Allow access on mondays'));
 * }
 *
 * function mymodule_monday_access() {
 *   // Any view arguments are passed into this function.
 *   $views_args = func_get_args();
 *
 *   // Permit access if today is monday.
 *   if (date('N') == 1) {
 *     return TRUE;
 *   }
 *   return FALSE;
 * }
 * @endcode
 *
 * @}
 */
