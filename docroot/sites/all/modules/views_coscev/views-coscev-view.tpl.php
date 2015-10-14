<?php
/**
 * @file
 * Default simple view template to display a section of items.
 *
 * - $title : The title of this group of rows.  empty if only 1 display.
 * - $directions are the 8 different directions
 * - $lines are the directions currently used by the items in the display
 * - $rows
 * @ingroup views_templates
 */

?>
<div class="cont-views-coscev">
  <?php foreach($all_items_direction as $display_name => $lines): ?>
    <div class="coscev-display-<?php print $display_name ?>">
      <div class="views-coscev">

        <?php if (!empty($title)) :  ?>
          <h3 class="coscev-title"><?php print $title; ?></h3>
        <?php endif; ?>

        <?php foreach($directions as $direction): ?>
          <div class="<?php print $direction ?> scrolling_content">
            <?php if(isset($lines[$direction])): ?>
                <?php foreach ($lines[$direction] as $item): ?>
                  <?php if($rows[$item['order']]['type'] == 'title') : ?>
                    <h3 class="coscev-item coscev-inline-title order-<?php print $item['order'] ?>">
                      <?php print $rows[$item['order']]['content']; ?>
                    </h3>
                  <?php else: ?>
                    <div class="order-<?php print $item['order'] ?> coscev-content coscev-item">
                        <?php print $rows[$item['order']]['content']; ?>
                    </div>
                  <?php endif; ?>
                <?php endforeach; ?>
            <?php endif; ?>
           </div>
        <?php endforeach; ?>
      </div>
    </div>
  <?php endforeach; ?>
</div>
