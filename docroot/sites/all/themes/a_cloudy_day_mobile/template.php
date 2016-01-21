<?php // $Id$ ?>
<?php
function a_cloudy_day_mobile_feed_icon(&$variables) {  
  $text = t('Subscribe to @feed-title', array('@feed-title' => $variables['title']));
  if ($image = theme('image', array('path' => path_to_theme() . '/images/rss.png', 'alt' => $text))) {
    return l($image, $variables['url'], array('html' => TRUE, 'attributes' => array('class' => array('feed-icon'), 'title' => $text)));
  }
}