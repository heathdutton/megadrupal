<?php

/**
 * Preprocesses variables for forum-list.tpl.php.
 *
 * @param $variables
 *   An array containing the following elements:
 *   - forums: An array of all forum objects to display for the given taxonomy
 *     term ID. If tid = 0 then all the top-level forums are displayed.
 *   - parents: An array of taxonomy term objects that are ancestors of the
 *     current term ID.
 *   - tid: Taxonomy term ID of the current forum.
 *
 * @see forum-list.tpl.php
 * @see theme_forum_list()
 */
function twbs_preprocess_forum_list(&$variables) {
  global $user;
  $row = 0;
  // Sanitize each forum so that the template can safely print the data.
  foreach ($variables['forums'] as $id => $forum) {
    $variables['forums'][$id]->description = !empty($forum->description) ? filter_xss_admin($forum->description) : '';
    $variables['forums'][$id]->link = url("forum/$forum->tid");
    $variables['forums'][$id]->name = check_plain($forum->name);
    $variables['forums'][$id]->is_container = !empty($forum->container);
    $variables['forums'][$id]->zebra = $row % 2 == 0 ? 'odd' : 'even';
    $row++;

    $variables['forums'][$id]->new_text = '';
    $variables['forums'][$id]->new_url = '';
    $variables['forums'][$id]->new_topics = 0;
    $variables['forums'][$id]->old_topics = $forum->num_topics;
    $variables['forums'][$id]->icon_class = 'default';
    $variables['forums'][$id]->icon_title = t('No new posts');
    if ($user->uid) {
      $variables['forums'][$id]->new_topics = _forum_topics_unread($forum->tid, $user->uid);
      if ($variables['forums'][$id]->new_topics) {
        $variables['forums'][$id]->new_text = format_plural($variables['forums'][$id]->new_topics, '1 new', '@count new');
        $variables['forums'][$id]->new_url = url("forum/$forum->tid", array('fragment' => 'new'));
        $variables['forums'][$id]->icon_class = 'new';
        $variables['forums'][$id]->icon_title = t('New posts');
      }
      $variables['forums'][$id]->old_topics = $forum->num_topics - $variables['forums'][$id]->new_topics;
    }
    $variables['forums'][$id]->last_reply = theme('forum_submitted', array('topic' => $forum->last_post));
  }
  // Give meaning to $tid for themers. $tid actually stands for term id.
  $variables['forum_id'] = $variables['tid'];
  unset($variables['tid']);
}
