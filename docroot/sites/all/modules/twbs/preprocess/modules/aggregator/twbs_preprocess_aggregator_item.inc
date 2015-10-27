<?php

/**
 * Processes variables for aggregator-item.tpl.php.
 *
 * @see aggregator-item.tpl.php
 */
function twbs_preprocess_aggregator_item(&$variables) {
  $item = $variables['item'];

  $variables['feed_url'] = check_url($item->link);
  $variables['feed_title'] = check_plain($item->title);
  $variables['content'] = aggregator_filter_xss($item->description);

  $variables['source_url'] = '';
  $variables['source_title'] = '';
  if (isset($item->ftitle) && isset($item->fid)) {
    $variables['source_url'] = url("aggregator/sources/$item->fid");
    $variables['source_title'] = check_plain($item->ftitle);
  }
  if (date('Ymd', $item->timestamp) == date('Ymd')) {
    $variables['source_date'] = t('%ago ago', array('%ago' => format_interval(REQUEST_TIME - $item->timestamp)));
  }
  else {
    $variables['source_date'] = format_date($item->timestamp, 'custom', variable_get('date_format_medium', 'D, m/d/Y - H:i'));
  }

  $variables['categories'] = array();
  foreach ($item->categories as $category) {
    $variables['categories'][$category->cid] = l($category->title, 'aggregator/categories/' . $category->cid);
  }
}
