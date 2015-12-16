<?php

/**
 * @file
 * Default theme implementation for displaying search results.
 */

?>

<?php if ($search_results): ?>
  <h2><?php print t('Search results');?></h2>
  <section>
    <?php print $search_results; ?>
  </section>
  <?php print $pager; ?>
<?php else : ?>
  <h2><?php print t('Your search yielded no results');?></h2>
  <?php print search_help('search#noresults', drupal_help_arg()); ?>
<?php endif; ?>
