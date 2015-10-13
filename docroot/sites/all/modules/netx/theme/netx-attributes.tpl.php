<?php

/**
 * @file
 * This template handles the layout of the NetX attributes.
 *
 * Variables available:
 * - $netx_attributes: An array of attributes to display.
 */
?>
<div class="netx-attributes">
  <?php foreach ($netx_attributes as $label => $value): ?>
    <div class="netx-attribute"><span class="label"><?php print $label;?></span>:&nbsp;<?php print $value; ?></div>
  <?php endforeach; ?>
</div>
