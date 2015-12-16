<?php
/**
 * @file
 * Template for p-three-25-50-25.
 */

$panel_prefix = isset($panel_prefix) ? $panel_prefix : '';
$panel_suffix = isset($panel_suffix) ? $panel_suffix : '';
?>
<?php print $panel_prefix; ?>
<div class="three-25-50-25 at-panel panel-display clearfix" <?php if (!empty($css_id)): print "id=\"$css_id\""; endif; ?>>
  <?php if ($content['three_25_50_25_top']): ?>
    <div class="region region-three-25-50-25-top region-conditional-stack fullsize">
      <div class="region-inner clearfix">
        <?php print $content['three_25_50_25_top']; ?>
      </div>
    </div>
  <?php endif; ?>
  <div class="region region-three-25-50-25-first">
    <div class="region-inner clearfix">
      <?php print $content['three_25_50_25_first_1']; ?>
    </div>
  </div>
  <div class="region region-three-25-50-25-second">
    <div class="region-inner clearfix">
      <?php print $content['three_25_50_25_second_1']; ?>
    </div>
  </div>
  <div class="region region-three-25-50-25-third">
    <div class="region-inner clearfix">
      <?php print $content['three_25_50_25_third_1']; ?>
    </div>
  </div>
  <?php if ($content['three_25_50_25_middle']): ?>
    <div class="region region-three-25-50-25-middle region-conditional-stack fullsize">
      <div class="region-inner clearfix">
        <?php print $content['three_25_50_25_middle']; ?>
      </div>
    </div>
  <?php endif; ?>
  <?php if ($content['three_25_50_25_first_2']): ?>
  <div class="region region-three-25-50-25-first">
    <div class="region-inner clearfix">
      <?php print $content['three_25_50_25_first_2']; ?>
    </div>
  </div>
  <?php endif; ?>
  <?php if ($content['three_25_50_25_second_2']): ?>
  <div class="region region-three-25-50-25-second">
    <div class="region-inner clearfix">
      <?php print $content['three_25_50_25_second_2']; ?>
    </div>
  </div>
  <?php endif; ?>
  <?php if ($content['three_25_50_25_third_2']): ?>
  <div class="region region-three-25-50-25-third">
    <div class="region-inner clearfix">
      <?php print $content['three_25_50_25_third_2']; ?>
    </div>
  </div>  
  <?php endif; ?>
  <?php if ($content['three_25_50_25_bottom']): ?>
    <div class="region region-three-25-50-25-bottom region-conditional-stack fullsize">
      <div class="region-inner clearfix">
        <?php print $content['three_25_50_25_bottom']; ?>
      </div>
    </div>
  <?php endif; ?>
</div>
<?php print $panel_suffix; ?>
