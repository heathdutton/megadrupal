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
<?php $panel_class = check_plain((isset($settings['panel_class']) && $settings['panel_class']) ? $settings['panel_class'] : ''); ?>
<div class="panel-three-column clearfix container-fluid <?php print $panel_class; ?>" <?php if (!empty($css_id)): print "id=\"$css_id\""; endif; ?>>
<?php if ($content['one'] || $content['two'] || $content['three']): ?>
    <?php if (isset($settings['use_container']) && $settings['use_container']): ?>
      <div class="container">
    <?php endif; ?>
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
    <?php if (isset($settings['use_container']) && $settings['use_container']): ?>
      </div>
    <?php endif; ?>
  <?php endif; ?>
</div>
