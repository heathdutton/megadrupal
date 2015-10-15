<?php
/**
 * @file
 * Template for a 1 column panel layout.
 *
 * This template provides a very simple "one column" panel display layout.
 *
 * Variables:
 * - $id: An optional CSS id to use for the layout.
 * - $content: An array of content, each item in the array is keyed to one
 *   panel of the layout. This layout supports the following sections:
 *   $content['middle']: The only panel in the layout.
 */
?>
<div class="panel-one-column clearfix " <?php if (!empty($css_id)): print "id=\"$css_id\""; endif; ?>>
  <div class="row">
    <?php if (isset($content['one']) && $content['one']): ?>
      <?php print $content['one']; ?>
    <?php endif; ?>
  </div>
</div>
