<?php

/**
 * Returns HTML for a feed icon.
 *
 * @param $variables
 *   An associative array containing:
 *   - url: An internal system path or a fully qualified external URL of the
 *     feed.
 *   - title: A descriptive title of the feed.
 */
function twbs_feed_icon($variables) {
  $text = t('Subscribe to !feed-title', array('!feed-title' => $variables['title']));
  $icon = '<i class="fa fa-rss-square fa-lg"></i>';
  return l($icon, $variables['url'], array('html' => TRUE, 'attributes' => array('title' => $text)));
}
