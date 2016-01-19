<?php
/**
 * @file
 * Renders one node link in the big listing
 */
 ?>
 <li>
<p><?php echo l($item['title'], 'node/' . $item['nid']); ?></p>
<?php echo $item['teaser']; ?>
</li>
