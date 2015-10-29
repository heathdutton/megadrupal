<?php
/**
 * @file
 * Contains the team from URL argument default plugin.
 */

/**
 * Default argument plugin to extract a team via menu_get_object
 */
class team_plugin_argument_default_team extends views_plugin_argument_default {
  function get_argument() {
    foreach (range(1, 3) as $i) {
      $team = menu_get_object('team', $i);
      if (!empty($team)) {
        return $team->tid;
      }
    }

    if (arg(0) == 'team' && is_numeric(arg(1))) {
      return arg(1);
    }

    // If the current page is a view that takes tid as an argument, return the tid.
    $view = views_get_page_view();

    if ($view && isset($view->argument['tid'])) {
      return $view->argument['tid']->argument;
    }
  }
}
