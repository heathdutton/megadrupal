<?php if (!empty($title)): ?>
  <h3><?php print $title; ?></h3>
<?php endif; ?>
<div class="views-view-imagematrix">
  <?php foreach ($blocks as $block_id => $block): ?>
    <div class="<?php print $block_classes[$block_id]; ?>" style="<?php print $block_styles; ?>">
      <?php foreach ($block as $field_group_id => $field_group): ?>
        <div class="<?php print $field_group_classes[$block_id][$field_group_id]; ?>" style="<?php print $field_group_styles[$block_id][$field_group_id]; ?>">
          <?php print $field_group; ?>
        </div>
      <?php endforeach; ?>
    </div>
  <?php endforeach; ?>
</div>
