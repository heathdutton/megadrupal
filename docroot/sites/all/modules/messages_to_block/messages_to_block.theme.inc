<?php
/**
 * @file
 * Theme functions for messages_to_block.
 */

/**
 * Theme a message.
 */
function theme_messages_to_block_message($variables) {
  $mid = $variables['mid'];
  $output = '<div class="messages-to-block-wrapper">';
  $output .= '<span class="messages-to-block-message">';
  $output .= $variables['message'];
  $output .= '</span>';
  // Generate an ajax delete link.
  $delete_link = l(t('Delete'), "messages-to-block/{$mid}/nojs", array(
    'attributes' => array('class' => array('use-ajax', 'messages-to-block-delete', "mid-{$mid}"))));
  $output .= $delete_link;
  $output .= '</div>';

  return $output;
}
