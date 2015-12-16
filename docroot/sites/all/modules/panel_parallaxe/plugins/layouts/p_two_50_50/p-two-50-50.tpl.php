<?php
/**
 * @file
 * Template for p-two-50-50.
 */

$panel_prefix = isset($panel_prefix) ? $panel_prefix : '';
$panel_suffix = isset($panel_suffix) ? $panel_suffix : '';
?>
<?php print $panel_prefix; ?>
<div class="two-brick at-panel panel-display clearfix" <?php if (!empty($css_id)): print "id=\"$css_id\""; endif; ?>>
  <?php if ($content['two_50_50_top']): ?>
    <div class="region region-two-brick-top region-conditional-stack fullsize">
      <div class="region-inner clearfix">
        <?php print $content['two_50_50_top']; ?>
      </div>
    </div>
  <?php endif; ?>
  <div class="panel-row row-1 clearfix">
    <div class="region region-two-brick-left-1">
      <div class="region-inner clearfix">
        <?php print $content['two_50_50_left_1']; ?>
      </div>
    </div>
    <div class="region region-two-brick-right-1">
      <div class="region-inner clearfix">
        <?php print $content['two_50_50_right_1']; ?>
      </div>
    </div>
  </div>
  <?php if ($content['two_50_50_middle']): ?>
    <div class="region region-two-brick-middle region-conditional-stack fullsize">
      <div class="region-inner clearfix">
        <?php print $content['two_50_50_middle']; ?>
      </div>
    </div>
  <?php endif; ?>
  <?php if ($content['two_50_50_left_2']): ?>
  <div class="panel-row row-2 clearfix">
    <div class="region region-two-brick-left-2">
      <div class="region-inner clearfix">
        <?php print $content['two_50_50_left_2']; ?>
      </div>
    </div>
    <?php endif; ?>
    <?php if ($content['two_50_50_right_2']): ?>
    <div class="region region-two-brick-right-2">
      <div class="region-inner clearfix">
        <?php print $content['two_50_50_right_2']; ?>
      </div>
    </div>
  </div>
  <?php endif; ?>
  <?php if ($content['two_50_50_bottom']): ?>
    <div class="region region-two-brick-bottom region-conditional-stack fullsize">
      <div class="region-inner clearfix">
        <?php print $content['two_50_50_bottom']; ?>
      </div>
    </div>
  <?php endif; ?>
</div>
<?php print $panel_suffix; ?>
