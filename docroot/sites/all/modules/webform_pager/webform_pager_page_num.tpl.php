<?php

/**
 * @file
 * Theme the webform pager page num bar.
 * Available variables:
 * - $show_page_numbers: value of the setting of show page numbers.
 * - $current_page: The number of the current page
 * - $total_pages: The total amount of pages
 * - $show_percentages: value of the setting of show percentages.
 * - $current_percentage: The current percentage.
 */
?>

<span class="<?php print $classes; ?>">
  <?php if ($show_page_numbers): ?>
    <?php print t('Page <span>!cur</span> of <span>!tot</span>', array('!cur' => $current_page, '!tot' => $total_pages)); ?>
  <?php endif; ?>
  <?php if ($show_percentages): ?>
    <?php print t('<span> (!per %)</span>', array('!per' => $current_percentage)); ?>
  <?php endif; ?>
</span>
