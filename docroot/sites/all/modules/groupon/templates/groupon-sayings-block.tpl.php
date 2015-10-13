<?php
/**
 * @file
 * Default theme implementation to display groupon sayings in block.
 *
 * Available variables:
 * - $sayings: The array of groupon sayings.
 */
?>
<ul>
  <li><b><?php print $sayings[0]['title'];?>: </b><span><?php print $sayings[0]['desc'];?></span></li>
</ul>
<div class="more-wrapper"><span class="more-link"><?php print l('more>>', 'groupon')?></span></div>
