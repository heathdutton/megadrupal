<?php

/**
 * @file
 * Hooks provided by the team module.
 */

/**
 * Act on a team being inserted or updated.
 *
 * This hook is invoked from team_save() before the team is saved to the
 * database.
 *
 * @param $team
 *   The team that is being inserted or updated.
 *
 * @ingroup team_api_hooks
 */
function hook_team_presave($team) {
  if (!empty($team->tid)) {
    db_insert('mytable')
      ->fields(array(
        'tid' => $team->tid,
        'extra' => $team->extra,
      ))
      ->execute();
  }
}

/**
 * Act on a team that is being assembled before rendering.
 *
 * The module may add elements to $team->content prior to rendering. This hook
 * will be called after hook_view(). The structure of $team->content is a
 * renderable array as expected by drupal_render().
 *
 * @param $team
 *   The team that is being assembled for rendering.
 * @param $view_mode
 *   The $view_mode parameter from team_view().
 * @param $langcode
 *   The language code used for rendering.
 *
 * @see hook_entity_view()
 *
 * @ingroup team_api_hooks
 */
function hook_team_view($team, $view_mode, $langcode) {
  $team->content['my_additional_field'] = array(
    '#markup' => $additional_field,
    '#weight' => 10,
    '#theme' => 'mymodule_my_additional_field',
  );
}

/**
 * Alter the results of team_view().
 *
 * This hook is called after the content has been assembled in a structured
 * array and may be used for doing processing which requires that the complete
 * team content structure has been built.
 *
 * If the module wishes to act on the rendered HTML of the team rather than the
 * structured content array, it may use this hook to add a #post_render
 * callback.
 * See drupal_render() and theme() documentation respectively for details.
 *
 * @param $build
 *   A renderable array representing the team content.
 *
 * @see team_view()
 *
 * @ingroup team_api_hooks
 */
function hook_team_view_alter(&$build) {
  if ($build['#view_mode'] == 'full' && isset($build['an_additional_field'])) {
    // Change its weight.
    $build['an_additional_field']['#weight'] = -10;
  }

  // Add a #post_render callback to act on the rendered HTML of the team.
  $build['#post_render'][] = 'my_module_team_post_render';
}

/**
 * Act on a team being displayed as a search result.
 *
 * This hook is invoked from team_search_execute(), after team_load()
 * and team_view() have been called.
 *
 * @param $team
 *   The team being displayed in a search result.
 *
 * @return array
 *   Extra information to be displayed with search result. This information
 *   should be presented as an associative array. It will be concatenated
 *   with the post information (last updated, author) in the default search
 *   result theming.
 *
 * @see template_preprocess_search_result()
 * @see search-result.tpl.php
 *
 * @ingroup team_api_hooks
 */
function hook_team_search_result($team) {
  // Example taken from node.api.php
  $comments = db_query('SELECT comment_count FROM {node_comment_statistics} WHERE nid = :nid', array('nid' => $node->nid))->fetchField();
  return array('comment' => format_plural($comments, '1 comment', '@count comments'));
}
