<?php
/**
 * @file
 * Default theme implementation for displaying search results.
 *
 * This template collects each invocation of theme_search_result(). This and
 * the child template are dependent to one another sharing the markup for
 * definition lists.
 *
 * Note that modules may implement their own search type and theme function
 * completely bypassing this template.
 *
 * Available variables:
 * - $search_results: All results as it is rendered through
 *   search-result.tpl.php
 * - $module: The machine-readable name of the module (tab) being searched, such
 *   as "node" or "user".
 *
 *
 * @see template_preprocess_search_results()
 */
?>

<?php if ($search_results) : ?>
  <?php  
    $items_per_page = 10;
      if (!isset($current_page )) {
    $current_page = isset($_GET['page']) ? $_GET['page'] + 1 : 1;
  }
    $total = 0;
    if (!empty ($GLOBALS['pager_total_items'])) {
      $total = $GLOBALS['pager_total_items'][0];
    }
    $start = 10*$current_page-9;
    $end = $items_per_page * $current_page;
    if ($end>$total) $end = $total;
    if ($total > 1) {
        $message = '<p>'. t('Displaying @start - @end of @total results', array('@start' => $start, '@end' => $end, '@total' => $total)) .'</p>';
    }
    else {
    $message = '<p>'. t('Displaying @start - @end of @total result', array('@start' => $start, '@end' => $end, '@total' => $total)) .'</p>';
  }
  ?>
  <h2><?php print t('Search results');?></h2>
  <ol class="search-results <?php print $module; ?>-results">
    <?php print $message; ?>
    <?php print $search_results; ?>
  </ol>
  <?php print $pager; ?>
<?php else : ?>
  <h2><?php print t('Your search yielded no results');?></h2>
  <?php print search_help('search#noresults', drupal_help_arg()); ?>
<?php endif; ?>
