<?php foreach ($images as $image_id => $image): ?>
  <div class="<?php print $image_classes[$image_id]; ?>" style="<?php print $image_styles[$image_id]; ?>">
    <?php print $image; ?>
    <?php if (isset($image_alts[$image_id])): ?>
      <div class="image-alt"><?php print $image_alts[$image_id]; ?></div>
    <?php endif; ?>
    <?php if (isset($image_titles[$image_id])): ?>
      <div class="image-title"><?php print $image_titles[$image_id]; ?></div>
    <?php endif; ?>
  </div>
<?php endforeach; ?>
