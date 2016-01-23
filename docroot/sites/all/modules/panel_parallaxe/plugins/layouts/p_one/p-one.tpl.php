<?php
/**
 * @file
 * Template for p-one.
 */

$panel_prefix = isset($panel_prefix) ? $panel_prefix : '';
$panel_suffix = isset($panel_suffix) ? $panel_suffix : '';
?>
<?php print $panel_prefix; ?>
<div class="one-column at-panel panel-display clearfix" <?php if (!empty($css_id)): print "id=\"$css_id\""; endif; ?>>
  <?php if ($content['one_main_top']): ?>
    <div class="region region-one-main region-conditional-stack fullsize">
      <div class="region-inner clearfix">
        <?php print $content['one_main_top']; ?>
      </div>
    </div>
  <?php endif; ?>	
  <div class="region region-one-main">
    <div class="region-inner clearfix">
      <?php print $content['one_main_1']; ?>
    </div>
  </div>
  <?php if ($content['one_main_middle']): ?>
  <div class="region region-one-main region-conditional-stack fullsize">
    <div class="region-inner clearfix">
      <?php print $content['one_main_middle']; ?>
    </div>
  </div>
  <?php endif; ?>
  <?php if ($content['one_main_2']): ?>
  <div class="region region-one-main">
    <div class="region-inner clearfix">
      <?php print $content['one_main_2']; ?>
    </div>
  </div>
  <?php endif; ?>
  <?php if ($content['one_main_bottom']): ?>
  <div class="region region-one-main region-conditional-stack fullsize">
    <div class="region-inner clearfix">
      <?php print $content['one_main_bottom']; ?>
    </div>
  </div>
  <?php endif; ?>  
</div>

<?php print $panel_suffix; ?>
