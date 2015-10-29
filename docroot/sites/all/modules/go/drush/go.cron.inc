<?php

/**
 * Callback function to send a message to Hipchat room.
 *
 * @param  string $module
 */
function drush_go_cron($module) {
  if (module_exists($module)) {
    timer_start('go_cron_links');
    $function = "{$module}_cron";
    $function();
    $time = timer_read('go_cron_links');
    drupal_set_message("Ran <strong>{$function}()</strong> in {$time} ms");
  }
}
