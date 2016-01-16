<div class="<?php print $classes;?> media">
  <?php if (isset($title_suffix['contextual_links'])): ?>
  <?php print render($title_suffix['contextual_links']); ?>
  <?php endif; ?>
  <div class="pull-left<?php print $ds_media_object_classes; ?>">
    <?php print $ds_media_object; ?>
  </div>
  <?php if ($ds_media_body): ?>
    <div class="media-body<?php print $ds_media_body_classes; ?>">
      <?php print $ds_media_body; ?>
    </div>
  <?php endif; ?>
</div>

<?php if (!empty($drupal_render_children)): ?>
  <?php print $drupal_render_children ?>
<?php endif; ?>