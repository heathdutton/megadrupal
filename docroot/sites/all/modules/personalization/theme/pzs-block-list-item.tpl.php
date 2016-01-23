<?php
/**
 * @file
 * Renders one node link in the block
 */
 ?>
 <li>
<?php echo l($item['title'], 'node/' . $item['nid']); ?>
</li>
