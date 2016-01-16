<?php
/**
 * @file
 *
 * For more info on Drupal default for this template, refer to
 * http://api.drupal.org/api/drupal/modules--aggregator--aggregator-feed-source.tpl.php/7
 */
?>
<header class="feed-source">
  <?php print $source_icon; ?>
  <?php print $source_image; ?>
  <?php if ($source_description): ?>
    <div class="feed-description">
      <?php print $source_description; ?>
    </div>
  <?php endif; ?>
  <p class="feed-url">
    <em><?php print t('URL:'); ?></em> <a href="<?php print $source_url; ?>"><?php print $source_url; ?></a>
  </p>
  <p class="feed-updated">
    <em><?php print t('Updated:'); ?></em> <?php print $last_checked; ?>
  </p>
</header>
