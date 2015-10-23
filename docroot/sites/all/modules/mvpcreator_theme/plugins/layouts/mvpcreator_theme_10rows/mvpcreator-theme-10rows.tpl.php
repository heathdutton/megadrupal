<?php
/**
 * @file
 * Template for MVPCreator Theme's Ten Rows layout.
 *
 * Variables:
 * - $css_id: An optional CSS id to use for the layout.
 * - $content: An array of content, each item in the array is keyed to one
 * panel of the layout.
 */
?>

<div class="panel-display clearfix <?php if (!empty($classes)) { print $classes; } ?><?php if (!empty($class)) { print $class; } ?>" <?php if (!empty($css_id)) { print "id=\"$css_id\""; } ?>>
  <?php if (!$full_width): ?>
    <div class="container-fluid">
  <?php else: ?>
    <div class="mvpcreator-theme-container-full-width">
  <?php endif; ?>
  <?php foreach (array_keys($layout['regions']) as $region): ?>
    <?php if (!empty($content[$region])): ?>
      <div class="row">
        <div class="col-md-12 panel-panel">
          <div class="panel-panel-inner">
            <?php print $content[$region]; ?>
          </div>
        </div>
      </div>
    <?php endif; ?>
  <?php endforeach; ?>
  </div>
</div>
