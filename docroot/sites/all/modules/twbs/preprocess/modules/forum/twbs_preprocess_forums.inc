<?php

/**
 * Preprocesses variables for forums.tpl.php.
 *
 * @param $variables
 *   An array containing the following elements:
 *   - forums: An array of all forum objects to display for the given taxonomy
 *     term ID. If tid = 0 then all the top-level forums are displayed.
 *   - topics: An array of all the topics in the current forum.
 *   - parents: An array of taxonomy term objects that are ancestors of the
 *     current term ID.
 *   - tid: Taxonomy term ID of the current forum.
 *   - sortby: One of the following integers indicating the sort criteria:
 *     - 1: Date - newest first.
 *     - 2: Date - oldest first.
 *     - 3: Posts with the most comments first.
 *     - 4: Posts with the least comments first.
 *   - forum_per_page: The maximum number of topics to display per page.
 *
 * @see forums.tpl.php
 */
function twbs_preprocess_forums(&$variables) {
  global $user;

  $vid = variable_get('forum_nav_vocabulary', 0);
  $vocabulary = taxonomy_vocabulary_load($vid);
  $title = !empty($vocabulary->name) ? $vocabulary->name : '';

  // Breadcrumb navigation:
  $breadcrumb[] = l(t('Home'), NULL);
  if ($variables['tid']) {
    $breadcrumb[] = l($vocabulary->name, 'forum');
  }
  if ($variables['parents']) {
    $variables['parents'] = array_reverse($variables['parents']);
    foreach ($variables['parents'] as $p) {
      if ($p->tid == $variables['tid']) {
        $title = $p->name;
      }
      else {
        $breadcrumb[] = l($p->name, 'forum/' . $p->tid);
      }
    }
  }
  drupal_set_breadcrumb($breadcrumb);
  drupal_set_title($title);

  if ($variables['forums_defined'] = count($variables['forums']) || count($variables['parents'])) {
    if (!empty($variables['forums'])) {
      $variables['forums'] = theme('forum_list', $variables);
    }
    else {
      $variables['forums'] = '';
    }

    if ($variables['tid'] && !in_array($variables['tid'], variable_get('forum_containers', array()))) {
      $variables['topics'] = theme('forum_topic_list', $variables);
      drupal_add_feed('taxonomy/term/' . $variables['tid'] . '/feed', 'RSS - ' . $title);
    }
    else {
      $variables['topics'] = '';
    }

    // Provide separate template suggestions based on what's being output. Topic id is also accounted for.
    // Check both variables to be safe then the inverse. Forums with topic ID's take precedence.
    if ($variables['forums'] && !$variables['topics']) {
      $variables['theme_hook_suggestions'][] = 'forums__containers';
      $variables['theme_hook_suggestions'][] = 'forums__' . $variables['tid'];
      $variables['theme_hook_suggestions'][] = 'forums__containers__' . $variables['tid'];
    }
    elseif (!$variables['forums'] && $variables['topics']) {
      $variables['theme_hook_suggestions'][] = 'forums__topics';
      $variables['theme_hook_suggestions'][] = 'forums__' . $variables['tid'];
      $variables['theme_hook_suggestions'][] = 'forums__topics__' . $variables['tid'];
    }
    else {
      $variables['theme_hook_suggestions'][] = 'forums__' . $variables['tid'];
    }

  }
  else {
    drupal_set_title(t('No forums defined'));
    $variables['forums'] = '';
    $variables['topics'] = '';
  }
}
