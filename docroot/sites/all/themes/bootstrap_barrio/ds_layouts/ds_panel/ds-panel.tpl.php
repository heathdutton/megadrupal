<div class="<?php print $classes;?> panel panel-default">
  <?php if (isset($title_suffix['contextual_links'])): ?>
  <?php print render($title_suffix['contextual_links']); ?>
  <?php endif; ?>
  <?php if ($ds_panel_heading): ?>
    <div class="panel-heading<?php print $ds_panel_heading_classes; ?>">
      <?php print $ds_panel_heading; ?>
    </div>
  <?php endif; ?>
  <?php if ($ds_panel_body): ?>
    <div class="panel-body<?php print $ds_panel_body_classes; ?>">
      <?php print $ds_panel_body; ?>
    </div>
  <?php endif; ?>
  <?php if ($ds_panel_footer): ?>
    <div class="panel-footer<?php print $ds_panel_footer_classes; ?>">
      <?php print $ds_panel_footer; ?>
    </div>
  <?php endif; ?>
</div>

<?php if (!empty($drupal_render_children)): ?>
  <?php print $drupal_render_children ?>
<?php endif; ?>