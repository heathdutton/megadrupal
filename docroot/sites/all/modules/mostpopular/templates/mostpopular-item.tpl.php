<?php
// $Id$
/**
* @file
* HTML for each most popular item in a list.
*
* Available variables:
* - $classes: A list of classes generated from $classes_array.
* - $attributes: A list of tag attributes generated from $attributes_array.
* - $item: The MostPopularItem being rendered, containing the following fields:
*   - $sid: The ID of the service.
*   - $iid: The ID of the interval.
*   - $nid: If specified, the node ID of this content.
*   - $url: The local path to the content.
*   - $title: The title of the content.
*   - $count: The number of times this content was viewed.
* - $node: The complete node, if this content is a node.
* - $show_count: True if counts have been enabled in the config. 
*/
?>
<div class="<?php print $classes; ?>" <?php print $attributes; ?>>
  <a href="<?php print url($item->url); ?>">
    <span class="title"><?php print check_plain($item->title); ?></span>
    <?php if ($show_count): ?>
      <span class="count">
        <?php print format_plural($item->count, '(1 time)', '(@count times)'); ?>
      </span>
    <?php endif; ?>
  </a>
</div>