<?php
/**
 * @file
 * Template for Views horizontal scroll pager.
 *
 * Available variables:
 * - $pager_items: pager layout.
 * - $total_pages: total count of pages.
 * - $pager_class: list of HTML classes in text format.
 *
 * @ingroup themeable
 */
?>
<div class="views-scroll-pager horizontal-only<?php print $pager_class; ?>">
  <div class="pager-container">
    <?php print $pager_items; ?>
  </div>
  <div class="total-pages"><?php print $total_pages; ?></div>
</div>
