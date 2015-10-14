<?php

/**
 * Processes variables for aggregator-feed-source.tpl.php.
 *
 * @see aggregator-feed-source.tpl.php
 */
function twbs_preprocess_aggregator_feed_source(&$variables) {
  $feed = $variables['feed'];

  $variables['source_icon'] = theme('feed_icon', array('url' => $feed->url, 'title' => t('!title feed', array('!title' => $feed->title))));

  if (!empty($feed->image) && !empty($feed->title) && !empty($feed->link)) {
    $variables['source_image'] = l(theme('image', array('path' => $feed->image, 'alt' => $feed->title)), $feed->link, array('html' => TRUE, 'attributes' => array('class' => 'feed-image')));
  }
  else {
    $variables['source_image'] = '';
  }

  $variables['source_description'] = aggregator_filter_xss($feed->description);
  $variables['source_url'] = check_url(url($feed->link, array('absolute' => TRUE)));

  if ($feed->checked) {
    $variables['last_checked'] = t('@time ago', array('@time' => format_interval(REQUEST_TIME - $feed->checked)));
  }
  else {
    $variables['last_checked'] = t('never');
  }

  if (user_access('administer news feeds')) {
    $variables['last_checked'] = l($variables['last_checked'], 'admin/config/services/aggregator');
  }
}
