<?php
/**
 * @file
 * Title template for manifestation
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */
?>
<?php timer_start('theme_xc_search_metadata/manifestation_title/theme'); ?>
<?php if (!empty($title_link)) : ?>
  <div class="xc-title">
    <?php print $title_link; ?>
  </div>
<?php endif ?>

<?php if (!empty($creator_link)) : ?>
  <div class="xc-creator">
    by <span class="xc-creator-name"><?php print $creator_link; ?></span>
  </div>
<?php endif ?>
<?php timer_stop('theme_xc_search_metadata/manifestation_title/theme'); ?>
