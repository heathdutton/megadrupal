<?php
/**
 * @file
 * Template for p-two-66-33.
 */

$panel_prefix = isset($panel_prefix) ? $panel_prefix : '';
$panel_suffix = isset($panel_suffix) ? $panel_suffix : '';
?>
<?php print $panel_prefix; ?>
<div class="two-66-33 at-panel panel-display clearfix" <?php if (!empty($css_id)): print "id=\"$css_id\""; endif; ?>>
  <?php if ($content['two_66_33_top']): ?>
    <div class="region region-two-66-33-top region-conditional-stack fullsize">
      <div class="region-inner clearfix">
        <?php print $content['two_66_33_top']; ?>
      </div>
    </div>
  <?php endif; ?>
  <div class="region region-two-66-33-first">
    <div class="region-inner clearfix">
      <?php print $content['two_66_33_first_1']; ?>
    </div>
  </div>
  <div class="region region-two-66-33-second">
    <div class="region-inner clearfix">
      <?php print $content['two_66_33_second_1']; ?>
    </div>
  </div>
  <?php if ($content['two_66_33_middle']): ?>
    <div class="region region-two-66-33-middle region-conditional-stack fullsize">
      <div class="region-inner clearfix">
        <?php print $content['two_66_33_middle']; ?>
      </div>
    </div>
  <?php endif; ?>
  <?php if ($content['two_66_33_first_2']): ?>
   <div class="region region-two-66-33-first">
    <div class="region-inner clearfix">
      <?php print $content['two_66_33_first_2']; ?>
    </div>
  </div>
  <?php endif; ?>
  <?php if ($content['two_66_33_second_2']): ?>
  <div class="region region-two-66-33-second">
    <div class="region-inner clearfix">
      <?php print $content['two_66_33_second_2']; ?>
    </div>
  </div>
  <?php endif; ?>
  <?php if ($content['two_66_33_bottom']): ?>
    <div class="region region-two-66-33-bottom region-conditional-stack fullsize">
      <div class="region-inner clearfix">
        <?php print $content['two_66_33_bottom']; ?>
      </div>
    </div>
  <?php endif; ?>
</div>
<?php print $panel_suffix; ?>
