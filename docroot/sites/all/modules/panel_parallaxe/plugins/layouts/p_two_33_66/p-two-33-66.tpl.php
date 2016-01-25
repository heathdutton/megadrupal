<?php
/**
 * @file
 * Template for p-two-33-66.
 */

$panel_prefix = isset($panel_prefix) ? $panel_prefix : '';
$panel_suffix = isset($panel_suffix) ? $panel_suffix : '';
?>
<?php print $panel_prefix; ?>
<div class="two-33-66 at-panel panel-display clearfix" <?php if (!empty($css_id)): print "id=\"$css_id\""; endif; ?>>
  <?php if ($content['two_33_66_top']): ?>
    <div class="region region-two-33-66-top region-conditional-stack fullsize">
      <div class="region-inner clearfix">
        <?php print $content['two_33_66_top']; ?>
      </div>
    </div>
  <?php endif; ?>
  <div class="region region-two-33-66-first">
    <div class="region-inner clearfix">
      <?php print $content['two_33_66_first_1']; ?>
    </div>
  </div>
  <div class="region region-two-33-66-second">
    <div class="region-inner clearfix">
      <?php print $content['two_33_66_second_1']; ?>
    </div>
  </div>
  <?php if ($content['two_33_66_middle']): ?>
    <div class="region region-two-33-66-middle region-conditional-stack fullsize">
      <div class="region-inner clearfix">
        <?php print $content['two_33_66_middle']; ?>
      </div>
    </div>
  <?php endif; ?>
  <?php if ($content['two_33_66_first_2']): ?>
  <div class="region region-two-33-66-first">
    <div class="region-inner clearfix">
      <?php print $content['two_33_66_first_2']; ?>
    </div>
  </div>
  <?php endif; ?>
  <?php if ($content['two_33_66_second_2']): ?>
  <div class="region region-two-33-66-second">
    <div class="region-inner clearfix">
      <?php print $content['two_33_66_second_2']; ?>
    </div>
  </div>
  <?php endif; ?>
  <?php if ($content['two_33_66_bottom']): ?>
    <div class="region region-two-33-66-bottom region-conditional-stack fullsize">
      <div class="region-inner clearfix">
        <?php print $content['two_33_66_bottom']; ?>
      </div>
    </div>
  <?php endif; ?>
</div>
<?php print $panel_suffix; ?>
