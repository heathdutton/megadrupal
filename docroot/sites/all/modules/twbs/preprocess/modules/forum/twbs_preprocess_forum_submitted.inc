<?php

/**
 * Preprocesses variables for forum-submitted.tpl.php.
 *
 * The submission information will be displayed in the forum list and topic
 * list.
 *
 * @param $variables
 *   An array containing the following elements:
 *   - topic: The topic object.
 *
 * @see forum-submitted.tpl.php
 * @see theme_forum_submitted()
 */
function twbs_preprocess_forum_submitted(&$variables) {
  $variables['author'] = isset($variables['topic']->uid) ? theme('username', array('account' => $variables['topic'])) : '';
  $variables['time'] = isset($variables['topic']->created) ? format_interval(REQUEST_TIME - $variables['topic']->created) : '';
}
