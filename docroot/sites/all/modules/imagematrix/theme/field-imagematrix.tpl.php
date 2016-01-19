<div class="field-imagematrix clearfix">
  <?php foreach ($blocks as $block_id => $block): ?>
    <div class="<?php print $block_classes[$block_id]; ?>" style="<?php print $block_styles[$block_id]; ?>">
      <?php foreach ($block as $image_id => $image): ?>
        <div class="<?php print $image_classes[$block_id][$image_id]; ?>" style="<?php print $image_styles[$block_id][$image_id]; ?>">
          <?php print $image; ?>
        </div>
      <?php endforeach; ?>
    </div>
  <?php endforeach; ?>
</div>
