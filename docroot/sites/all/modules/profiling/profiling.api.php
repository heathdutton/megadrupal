<?php

/**
 * @file
 * Profiling module hook definition.
 */

/**
 * Given a full database row fetched (statistics) that may contain arbitrary
 * various data fields, attempt to determine a textual, human readable help
 * for site integrator on how to improve this and reduce time, memory, or both
 * consumption.
 * 
 * This is pure UI stuff. Useless now, but will be use in the future to create
 * small reports on table report screens that will give some information about
 * how to improve performance.
 * 
 * This hook implementation is optional.
 * 
 * @param string $name
 *   Timer name. If you don't know anything about this timer, please return
 *   NULL.
 * @param object $row
 *   Row object from database, fetched from one of the table result screen.
 *   It may contain any of the Profiling_SimpleQuery known fields. Use this
 *   data, if set, to determine problems.
 * @param array $thresholds
 *   User configured thresholds for the given report context.
 * 
 * @return array
 *   Array of human readable text, which can contain basic html and/or links.
 *   Each one of the given text in this array should be short enough so it
 *   won't bore the potential readers.
 *   NULL if you don't know anything about the given timer.
 */
function hook_profiling_analyze($name, $row) {
  // FIXME: Todo.
}

/**
 * Give your timers a human name and description for report screens.
 * 
 * This hook implementation is optional.
 * 
 * @return array
 *   Descriptive array of custom timer containing the 'title' and 'description'
 *   keys.
 */
function hook_profiling_timer_help() {
  $items = array();
  foreach (array_unique(variable_get('profiling_hooks', array())) as $hook) {
    $items['hook_' . $hook] = array(
      'title' => "hook_" . $hook . "()",
      'description' => t("Dynamically spawned <em>@hook</em> profiling, currently being implemented by <em>@modules</em> modules.", array(
        '@hook' => 'hook_' . $hook . '()',
        // Modules currently implementing these hook do not include ourself
        // since we don't spawn the eval'd code at runtime when we are in
        // administrative screens.
        '@modules' => implode(', ', module_implements($hook)),
      ))
    );
  }
  return $items;
}

/**
 * Give your collections a human name and description for report screens.
 * 
 * This hook implementation is optional.
 * 
 * @return array
 *   Descriptive array of custom timer containing the 'title' and 'description'
 *   keys.
 */
function hook_profiling_collection_help() {
  $items = array();
  $items[PROFILING_DEFAULT_COLLECTION] = array(
    'title' => t("Default"),
    'description' => t("Profiling module default timers."),
  );
  $items[PROFILING_DEFAULT_HOOK] = array(
    'title' => t("Hooks"),
    'description' => t("Hook timers measure profile some critical hooks resources consumption."),
  );
  return $items;
}
