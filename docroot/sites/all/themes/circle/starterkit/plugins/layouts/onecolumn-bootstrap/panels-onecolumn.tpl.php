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
<?php $panel_class = check_plain((isset($settings['panel_class']) && $settings['panel_class']) ? $settings['panel_class'] : ''); ?>
<div class="panel-one-column clearfix container-fluid <?php print $panel_class; ?>" <?php if (!empty($css_id)): print "id=\"$css_id\""; endif; ?>>
  <?php if ($content['one']): ?>
    <?php if (isset($settings['use_container']) && $settings['use_container']): ?>
      <div class="container">
    <?php endif; ?>
      <div class="row">
        <div class="col-md-12">
          <?php if (isset($content['one']) && $content['one']): ?>
            <?php print $content['one']; ?>
          <?php endif; ?>
        </div>
      </div>
    <?php if (isset($settings['use_container']) && $settings['use_container']): ?>
      </div>
    <?php endif; ?>
  <?php endif; ?>
</div>
