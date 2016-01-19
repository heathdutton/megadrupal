<?php
/**
 * @file
 * Title template for work record
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */
?>
<?php if ($element['title']): ?>
  <div class="xc-title">
    <?php print l($element['title'], $xc_record['full_display_link']); ?>
  </div>
<?php endif; ?>

<?php if ($element['creator']): ?>
  <div class="xc-creator">
    by <?php print $element['creator']; ?>
  </div>
<?php endif; ?>

