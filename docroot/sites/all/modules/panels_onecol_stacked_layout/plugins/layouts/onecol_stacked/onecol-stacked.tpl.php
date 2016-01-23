<?php
/**
 * @file
 * Template for one column stacked layout
 */
?>
<link rel="stylesheet" type="text/css" href="<?= drupal_get_path('module', 'panels_extra_layouts') . '/css/base-grid.css' ?>">
<div class="panel-display panel-onecol-stacked clear-block" <?php if (!empty($css_id)) { print "id=\"$css_id\""; } ?>>
  <div class="panel-panel line">
    <div class="panel-panel unit panel-header lastUnit">
      <?php print $content['header']; ?>
    </div>
  </div>

  <div class="panel-panel line">
    <div class="panel-panel unit panel-top lastUnit">
      <?php print $content['top']; ?>
    </div>
  </div>

  <div class="panel-panel line">
    <div class="panel-panel unit panel-col-onehundred firstUnit">
      <div class="inside">
        <?php print $content['center']; ?>
      </div>
    </div>
  </div>

  <div class="panel-panel panel-line">
    <div class="panel-panel unit panel-footer lastUnit">
      <?php print $content['footer']; ?>
    </div>
  </div>
</div>
