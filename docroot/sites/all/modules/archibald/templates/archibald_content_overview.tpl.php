<?php
/**
 * @file
 * archibald_content_overview.tpl.php
 * Display the archibald content overview resultset
 *
 * - $search_form: a drupal from array
 * - $search_result: array with list of result objects
 * - $search_result_count: integer
 */
echo '<div id="archibald_content_overview">';

echo drupal_render($search_form);

if (empty($search_result)) { ?>
  <b class="archibald_content_overview_result_count"><?PHP print t('No search results'); ?></b>
<?PHP
}
else {
  ?><b class="archibald_content_overview_result_count"><?PHP
  print t(
    'Search results (@count)',
    array(
     '@count' => $search_result_count,
    )
  ); ?></b>
<?PHP
  foreach ($search_result as $res_row) {
    echo '<div class="archibald_content_overview_row">' . theme('archibald_content_overview_row', array('row' => $res_row)) . '</div>';
  }
}

echo theme('pager');
echo '</div>';
