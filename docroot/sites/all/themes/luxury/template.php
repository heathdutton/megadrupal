<?php
function luxury_preprocess_comment(&$variables) {
  $comment = $variables['elements']['#comment'];
  $node = $variables['elements']['#node'];
  $variables['created']   = format_date($comment->created, 'custom', 'j M, Y g:ia');
  $variables['changed']   = format_date($comment->changed, 'custom', 'j M, Y g:ia');

  $variables['submitted'] = t('Posted by !username on !datetime', array('!username' => $variables['author'], '!datetime' => $variables['created']));
}
?>