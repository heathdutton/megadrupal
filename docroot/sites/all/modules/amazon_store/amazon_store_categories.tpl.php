<?php
// $ID: $
/**
 * @file
 *   Present a block of Amazon store category links.
 */
$num_columns = variable_get('amazon_store_categories_block_num_columns', 2);
$categories = $GLOBALS['amazon_store_search_indexes']->getSearchIndexPulldown(TRUE);
print "<div class='amazon-store-category-blurb'>". t("Search Amazon in these categories:") ."</div>";
$td_length = (int)(count($categories)/$num_columns);
if (count($categories % $num_columns)) {
  $td_length++; // Distribute more into the first columns.
}
print "<table class='search-indexes-table'><tr><td>";
$count = 1;
$column = 1;
foreach ($categories as $category => $friendly_name) {
  $link = l($friendly_name, AMAZON_STORE_PATH, array('attributes' => array('rel' => 'nofollow'), 'query' => array('SearchIndex' => $category)));
  print "<p>$link</p>";
  if (($count % $td_length == 0) && $column < $num_columns) {
    print "</td><td>";
    $column ++;
  }
  $count++;
}
print "</td></tr></table>";
