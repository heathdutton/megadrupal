<?php
/**
 * @file
 * Default theme implementation to display groupon deals.
 *
 * Available variables:
 * - $channels: The array of groupon channel deals.
 */

$num_of_channels = variable_get('groupon_block_num_deals');
?>
<ul>
  <?php foreach ($channels as $key => $value) : ?>
    <li><?php print l($value['title'], check_url($value['url']), array('attributes' => array('target' => '_blank')))?></li>
  <?php 
    if ($num_of_channels == $key)
      break;
   endforeach;
  ?>
</ul>
<div class="more-wrapper"><span class="more-link"><?php print l('more>>', 'groupon/channels')?></span></div>
