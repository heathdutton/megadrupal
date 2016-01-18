<?php
/**
 * @file
 * Template for a 3 column panel layout.
 *
 * This template provides a very simple "three column" panel display layout.
 *
 * Variables:
 * - $id: An optional CSS id to use for the layout.
 * - $content: An array of content, each item in the array is keyed to one
 *   panel of the layout. This layout supports the following sections:
 *   $content['one']: First column
 *   $content['two']: Second column
 *   $content['three']: Third column
 */
?>
<div class="panel-three-column clearfix " <?php if (!empty($css_id)): print "id=\"$css_id\""; endif; ?>>
  <div class="row">
    <?php if (isset($content['one']) && $content['one']): ?>
      <?php print $content['one']; ?>
    <?php endif; ?>
    <?php if (isset($content['two']) && $content['two']): ?>
      <?php print $content['two']; ?>
    <?php endif; ?>
    <?php if (isset($content['three']) && $content['three']): ?>
      <?php print $content['three']; ?>
    <?php endif; ?>
  </div>
</div>
