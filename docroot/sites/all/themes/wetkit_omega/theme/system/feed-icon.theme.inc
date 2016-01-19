<?php
/**
 * @file
 * feed-icon.theme.inc
 */


/**
 * Theme the 'feed icon' for aggegrated feeds.
 *
 * @param array $variables
 *   The array used to modify the panels ipe placeholder pane.
 */
function wetkit_omega_feed_icon($variables) {
  $text = t('Subscribe to !feed-title', array('!feed-title' => $variables['title']));
  if ($image = theme('image', array('path' => 'misc/feed.png', 'attributes' => array('class' => 'image-actual'), 'width' => 16, 'height' => 16, 'alt' => $text))) {
    return l($image, $variables['url'], array('html' => TRUE, 'attributes' => array('class' => array('feed-icon'), 'title' => $text)));
  }
}
