<?php
/**
 * @file
 * Default theme implementation to present feeds as list items.
 *
 * Each iteration generates a single feed source or category.
 *
 * Available variables:
 * - $title: Title of the feed or category.
 * - $summary_list: Unordered list of linked feed items generated through
 *   theme_item_list().
 * - $source_url: URL to the local source or category.
 *
 * Borrowed from Boron
 * @link http://drupal.org/project/boron
 * @see template_preprocess()
 * @see template_preprocess_aggregator_summary-items()
 */
?>
<section id="feed-<?php print $title; ?>" class="feed-<?php print $title; ?> feed">
  <h3 class="feed-title"><?php print $title; ?></h3>
  <?php print $summary_list; ?>
  <div class="links feed-links">
    <a href="<?php print $source_url; ?>"><?php print t('More'); ?></a>
  </div>
</section> <!-- /.feed -->