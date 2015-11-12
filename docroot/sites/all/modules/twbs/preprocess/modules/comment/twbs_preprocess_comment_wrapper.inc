<?php

/**
 * Process variables for comment-wrapper.tpl.php.
 *
 * @see comment-wrapper.tpl.php
 */
function twbs_preprocess_comment_wrapper(&$variables) {
  // Provide contextual information.
  $variables['node'] = $variables['content']['#node'];
  $variables['display_mode'] = variable_get('comment_default_mode_' . $variables['node']->type, COMMENT_MODE_THREADED);
  // The comment form is optional and may not exist.
  $variables['content'] += array('comment_form' => array());
}
