<?php

/**
 * Processes variables for aggregator-summary-items.tpl.php.
 *
 * @see aggregator-summary-items.tpl.php
 */
function twbs_preprocess_aggregator_summary_items(&$variables) {
  $variables['title'] = check_plain($variables['source']->title);
  $variables['summary_list'] = theme('item_list', array('items' => $variables['summary_items']));
  $variables['source_url'] = $variables['source']->url;
}
