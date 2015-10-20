<?php

/**
 * Get uid of given user, If no user is given UID of current user.
 *
 * @param mixed $user
 *   User entity or ID of them.
 * @param boolean $user
 *   Whether to check $user 'could' be a valid UID, an integer and if not,
 *   throw an exception. In access control context it's better to throw an
 *   exception rather to silently pass.
 *
 * @return int
 *   UID of user. If no user was given as an argument, UID of current user is
 *   returned.
 */
function _entityspice_user_get_uid($user = NULL, $check = FALSE) {
  if($user === TRUE) {
    $uid = $GLOBALS['user']->uid;
  }
  elseif($user) {
    $uid = is_object($user) ? $user->uid : $user;
  }
  else {
    $uid = $GLOBALS['user']->uid;
  }

  if($check && !(ctype_digit($uid) || is_int($uid))) {
    throw new Exception('Invalid user ID.');
  }

  return $uid;
}

/**
 * This helper function gathers hook result of a given info hook.
 *
 * This function is only good for "info" hooks, Which will not take any
 * argument and return an array of info.
 *
 * The info will be cached for given Fully Quallified Name of $hook, Which
 * includes name of hook, module and suffix. This cache will be reseted when
 * $reset is set to TRUE.
 *
 * It is important that each module returns an array, Otherwise it will not be
 * taken into account.
 *
 * Attention: do not change $data to &$data, because internal implemention of
 * this helper function is subject to change and may not work with variable
 * references.
 *
 * @param string $hook
 *   Name of hook to gather data from.
 * @param string $module
 *   Name of module this hook belongs to.
 * @param boolean $reset
 *   whether to reset the cache bin belonging to $hook.
 * @param string $suffix
 *   If is not NULL, will be appended to hook name.
 * @param mixed $data
 *   Data to pass to hook.
 * @param boolean $module_prefixed
 *   If is FALSE then returned result of each module will be simply sumed up
 *   with final result of hook. If is TRUE, the result of each module will be
 *   put in a key in final result array. The key is name of implementing
 *   module.
 *
 * @return mixed hook_result
 *   Result of hook gathered from modules.
 */
function _entityspice_gather_hook_info($hook, $module, $reset = FALSE, $suffix = 'info', $data = NULL, $module_prefixed = FALSE) {
  $drupal_static_fast = &drupal_static(__FUNCTION__, array('info' => array()));
  $info = &$drupal_static_fast['info'];

  $hook_name = $module . '_' . $hook;
  if($suffix) {
    $hook_name .= '_' . $suffix;
  }

  if(!isset($info[$hook_name]) || $reset) {
    $info[$hook_name] = array();
    foreach (module_implements($hook_name) as $implementing_module) {
      $function = $implementing_module . '_' . $hook_name;
      $module_info = $function($data);
      if($module_prefixed) {
        $info[$hook_name][$implementing_module] = $module_info;
      }
      elseif(is_array($module_info)) {
        $info[$hook_name] += $module_info;
      }
      else {
        $targ = array('@module' => $implementing_module, '@hook' => $hook_name);
        watchdog('entityspice', 'Malformed hook result from module: @module for hook: @hook', $targ, WATCHDOG_WARNING);
      }
    }
  }

  $ctx = array(
    'module' => $module,
    'hook' => $hook,
    'data' => $info[$hook_name],
    'hook_name' => $hook_name
  );
  drupal_alter($hook_name, $info, $ctx);

  return $info[$hook_name];
}
