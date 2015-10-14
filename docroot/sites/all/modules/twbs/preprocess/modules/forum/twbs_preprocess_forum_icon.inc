<?php

/**
 * Preprocesses variables for forum-icon.tpl.php.
 *
 * @param $variables
 *   An array containing the following elements:
 *   - new_posts: Indicates whether or not the topic contains new posts.
 *   - num_posts: The total number of posts in all topics.
 *   - comment_mode: An integer indicating whether comments are open, closed,
 *     or hidden.
 *   - sticky: Indicates whether the topic is sticky.
 *   - first_new: Indicates whether this is the first topic with new posts.
 *
 * @see forum-icon.tpl.php
 * @see theme_forum_icon()
 */
function twbs_preprocess_forum_icon(&$variables) {
  $variables['hot_threshold'] = variable_get('forum_hot_topic', 15);
  if ($variables['num_posts'] > $variables['hot_threshold']) {
    $variables['icon_class'] = $variables['new_posts'] ? 'hot-new' : 'hot';
    $variables['icon_title'] = $variables['new_posts'] ? t('Hot topic, new comments') : t('Hot topic');
  }
  else {
    $variables['icon_class'] = $variables['new_posts'] ? 'new' : 'default';
    $variables['icon_title'] = $variables['new_posts'] ? t('New comments') : t('Normal topic');
  }

  if ($variables['comment_mode'] == COMMENT_NODE_CLOSED || $variables['comment_mode'] == COMMENT_NODE_HIDDEN) {
    $variables['icon_class'] = 'closed';
    $variables['icon_title'] = t('Closed topic');
  }

  if ($variables['sticky'] == 1) {
    $variables['icon_class'] = 'sticky';
    $variables['icon_title'] = t('Sticky topic');
  }
}
