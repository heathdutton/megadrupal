<?php
/**
 * @file
 * Template for p-three-25-25-50.
 */

$panel_prefix = isset($panel_prefix) ? $panel_prefix : '';
$panel_suffix = isset($panel_suffix) ? $panel_suffix : '';
?>
<?php print $panel_prefix; ?>
<div class="three-25-25-50 at-panel panel-display clearfix" <?php if (!empty($css_id)): print "id=\"$css_id\""; endif; ?>>
  <?php if ($content['three_25_25_50_top']): ?>
    <div class="region region-three-25-25-50-top region-conditional-stack fullsize">
      <div class="region-inner clearfix">
        <?php print $content['three_25_25_50_top']; ?>
      </div>
    </div>
  <?php endif; ?>
  <div class="region region-three-25-25-50-first">
    <div class="region-inner clearfix">
      <?php print $content['three_25_25_50_first_1']; ?>
    </div>
  </div>
  <div class="region region-three-25-25-50-second">
    <div class="region-inner clearfix">
      <?php print $content['three_25_25_50_second_1']; ?>
    </div>
  </div>
  <div class="region region-three-25-25-50-third">
    <div class="region-inner clearfix">
      <?php print $content['three_25_25_50_third_1']; ?>
    </div>
  </div>
  <?php if ($content['three_25_25_50_middle']): ?>
    <div class="region region-three-25-25-50-middle region-conditional-stack fullsize">
      <div class="region-inner clearfix">
        <?php print $content['three_25_25_50_middle']; ?>
      </div>
    </div>
  <?php endif; ?>
  <?php if ($content['three_25_25_50_first_2']): ?>
  <div class="region region-three-25-25-50-first">
    <div class="region-inner clearfix">
      <?php print $content['three_25_25_50_first_2']; ?>
    </div>
  </div>
  <?php endif; ?>
  <?php if ($content['three_25_25_50_second_2']): ?>
  <div class="region region-three-25-25-50-second">
    <div class="region-inner clearfix">
      <?php print $content['three_25_25_50_second_2']; ?>
    </div>
  </div>
  <?php endif; ?>
  <?php if ($content['three_25_25_50_third_2']): ?>
  <div class="region region-three-25-25-50-third">
    <div class="region-inner clearfix">
      <?php print $content['three_25_25_50_third_2']; ?>
    </div>
  </div>  
  <?php endif; ?>
  <?php if ($content['three_25_25_50_bottom']): ?>
    <div class="region region-three-25-25-50-bottom region-conditional-stack fullsize">
      <div class="region-inner clearfix">
        <?php print $content['three_25_25_50_bottom']; ?>
      </div>
    </div>
  <?php endif; ?>
</div>
<?php print $panel_suffix; ?>
