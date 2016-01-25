<?php
/**
 * @file
 * Template for Panopoly folders.
 *
 * Variables:
 * - $css_id: An optional CSS id to use for the layout.
 * - $content: An array of content, each item in the array is keyed to one
 * panel of the layout. This layout supports the following sections:
 */
?>

<div class="panel-display folders clearfix container <?php if (!empty($classes)) { print $classes; } ?><?php if (!empty($class)) { print $class; } ?>" <?php if (!empty($css_id)) { print "id=\"$css_id\""; } ?>>

  <div class="folders-container folders-column-content clearfix row-fluid container-fluid">
    <div class="row">
      <div class="folders-inner-container-left span9 col-md-9">
        <div class="row-fluid row">
          <div class="folders-column-content-region folders-column1 panel-panel span4 col-md-4">
            <div class="folders-column-content-region-inner folders-column1-inner panel-panel-inner">
              <?php print $content['folderlayout']; ?>
            </div>
          </div>
          <div class="folders-column-content-region folders-content panel-panel span8 col-md-8">
            <div class="folders-column-content-region-inner folders-content-inner panel-panel-inner">
              <?php print $content['content']; ?>
            </div>
          </div>
        </div>
      </div>
      <div class="folders-column-content-region folders-column2 panel-panel span3 col-md-3">
        <div class="folders-column-content-region-inner folders-column2-inner panel-panel-inner">
          <?php print $content['sidebar']; ?>
        </div>
      </div>
    </div>
  </div>

</div><!-- /.folders -->
