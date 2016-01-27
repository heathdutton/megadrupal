<div class="<?php print $classes;?> thumbnail">
  <?php if (isset($title_suffix['contextual_links'])): ?>
  <?php print render($title_suffix['contextual_links']); ?>
  <?php endif; ?>
  <?php print $ds_thumbnail; ?>
  <?php if ($ds_caption): ?>
    <div class="caption<?php print $ds_caption_classes; ?>">
      <?php print $ds_caption; ?>
    </div>
  <?php endif; ?>
</div>
<?php if (!empty($drupal_render_children)): ?>
  <?php print $drupal_render_children ?>
<?php endif; ?>