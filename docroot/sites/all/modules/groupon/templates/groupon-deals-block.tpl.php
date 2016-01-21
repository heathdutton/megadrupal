<?php
/**
 * @file
 * Default theme implementation to display groupon deals.
 *
 * Available variables:
 * - $deals: The array of groupon deals.
 */

$num_of_deals = variable_get('groupon_block_num_deals');
?>
<ul>
  <?php foreach ($deals as $key => $value) : ?>
    <li><?php print l($value['title'], check_url($value['url']), array('attributes' => array('target' => '_blank')))?></li>
  <?php 
    if ($num_of_deals == $key)
      break;
   endforeach;
  ?>
</ul>
<div class="more-wrapper"><span class="more-link"><?php print l('more>>', 'groupon/deals')?></span></div>
