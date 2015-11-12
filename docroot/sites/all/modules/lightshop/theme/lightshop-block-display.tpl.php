<?php

/**
 * @file
 * Basket block content.
 *
 * Available variables:
 * - $not_empty: Flag availability of items in the basket.
 * - $basket_items_list: List the contents of the basket.
 * - $total_sum: Total amount of items in the basket.
 * - $basket_page_link: Link to the basket page.
 * - $count_items: Number of items in basket.
 */
?>
<?php
if ($not_empty): ?>
<div>
  <?php print $basket_items_list; ?>
  </div>
  <?php if ($total_sum): ?>
<div>
<?php print t('Total price: ') . $total_sum; ?>

</div>
<?php endif; ?>
<div>
  <?php print $basket_page_link; ?>
  </div>
<?php
else : ?>
<div>
<?php print t('Your basket is empty'); ?>
 </div>
 <?php endif;
