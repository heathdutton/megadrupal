<?php
/**
 * @file
 *
 * For more info on Drupal default for this template, refer to
 * http://api.drupal.org/api/drupal/modules--aggregator--aggregator-item.tpl.php/7
 */
?>
<article class="feed-item">

  <header>
    <h3>
      <a href="<?php print $feed_url; ?>"><?php print $feed_title; ?></a>
    </h3>
    <p class="submitted">
      <?php if ($source_url) : ?>
        <a href="<?php print $source_url; ?>" class="feed-item-source"><?php print $source_title; ?></a> -
      <?php endif; ?>
      <span class="feed-item-date"><?php print $source_date; ?></span>
    </p>
  </header>

  <?php if ($content) : ?>
    <div class="content">
      <?php print $content; ?>
    </div>
  <?php endif; ?>

  <?php if ($categories): ?>
    <footer>
      <p class="categories"><?php print t('Categories'); ?>: <?php print implode(', ', $categories); ?></p>
    </footer>
  <?php endif; ?>

</article>
