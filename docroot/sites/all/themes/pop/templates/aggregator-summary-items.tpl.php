<?php
/**
 * @file
 *
 * For more info on Drupal default for this template, refer to
 * http://api.drupal.org/api/drupal/modules--aggregator--aggregator-summary-item.tpl.php/7
 */
?>
<section id="feed-<?php print $title; ?>" class="feed">
  <h3><?php print $title; ?></h3>
  <?php print $summary_list; ?>
  <div class="links">
    <a href="<?php print $source_url; ?>"><?php print t('More'); ?></a>
  </div>
</section>
