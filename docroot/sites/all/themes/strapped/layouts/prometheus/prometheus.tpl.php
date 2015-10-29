<?php
/**
 * @file
 * Template - Hyperion
 */
?>
<div class="clearfix <?php print $classes ?>" <?php if (!empty($css_id)) {
  print "id=\"$css_id\"";
} ?>>


  <?php if (!empty($content['layer_1'])) { ?>
    <div class="row layer-1">
      <div class="col-md-12 layer-1"><?php print $content['layer_1']; ?></div>
    </div>
  <?php } ?>


  <?php if (!empty($content['layer_2_left']) || !empty($content['layer_2_right'])) { ?>
    <div class="row layer-2">
      <div
        class="col-md-<?php print (empty($content['layer_2_right']) ? 12 : 8); ?> layer-2-left"><?php print $content['layer_2_left']; ?></div>
      <?php if (!empty($content['layer_2_right'])) { ?>
        <div
          class="col-md-4 layer-2-right"><?php print $content['layer_2_right']; ?></div>
      <?php } ?>
    </div>
  <?php } ?>


  <?php if (!empty($content['layer_3_left']) || !empty($content['layer_3_right'])) { ?>
    <div class="row layer-3">
      <div class="col-md-4 layer-3-left"><?php print $content['layer_3_left']; ?></div>
      <div class="col-md-8 layer-3-right"><?php print $content['layer_3_right']; ?></div>
    </div>
  <?php } ?>

  <?php if (!empty($content['layer_4_left']) || !empty($content['layer_4_center']) || !empty($content['layer_4_right'])) { ?>
    <div class="row layer-4">
      <div class="col-md-4 layer-4-left"><?php print $content['layer_4_left']; ?></div>
      <div class="col-md-4 layer-4-center"><?php print $content['layer_4_center']; ?></div>
      <div class="col-md-4 layer-4-right"><?php print $content['layer_4_right']; ?></div>
    </div>
  <?php } ?>


  <?php if (!empty($content['layer_5_left']) || !empty($content['layer_5_right'])) { ?>
    <div class="row layer-5">
      <div class="col-md-6 layer-5-left"><?php print $content['layer_5_left']; ?></div>
      <div class="col-md-6 layer-5-right"><?php print $content['layer_5_right']; ?></div>
    </div>
  <?php } ?>


  <?php if (!empty($content['layer_6'])) { ?>
    <div class="row layer-6">
      <div class="col-md-12 layer-6"><?php print $content['layer_6']; ?></div>
    </div>
  <?php } ?>





  <?php if (!empty($content['layer_7_left']) || !empty($content['layer_7_right'])) { ?>
    <div class="row layer-7">
      <div class="col-md-8 layer-7-left"><?php print $content['layer_7_left']; ?></div>
      <div class="col-md-4 layer-7-right"><?php print $content['layer_7_right']; ?></div>
    </div>
  <?php } ?>


  <?php if (!empty($content['layer_8_left']) || !empty($content['layer_8_right'])) { ?>
    <div class="row layer-8">
      <div class="col-md-4 layer-8-left"><?php print $content['layer_8_left']; ?></div>
      <div class="col-md-8 layer-8-right"><?php print $content['layer_8_right']; ?></div>
    </div>
  <?php } ?>

  <?php if (!empty($content['layer_9_left']) || !empty($content['layer_9_center']) || !empty($content['layer_9_right'])) { ?>
    <div class="row layer-9">
      <div class="col-md-4 layer-9-left"><?php print $content['layer_9_left']; ?></div>
      <div class="col-md-4 layer-9-center"><?php print $content['layer_9_center']; ?></div>
      <div class="col-md-4 layer-9-right"><?php print $content['layer_9_right']; ?></div>
    </div>
  <?php } ?>


  <?php if (!empty($content['layer_10_left']) || !empty($content['layer_10_right'])) { ?>
    <div class="row layer-10">
      <div class="col-md-6 layer-10-left"><?php print $content['layer_10_left']; ?></div>
      <div class="col-md-6 layer-10-right"><?php print $content['layer_10_right']; ?></div>
    </div>
  <?php } ?>

  <?php if (!empty($content['layer_11'])) { ?>
    <div class="row layer-1">
      <div class="col-md-12 layer-11"><?php print $content['layer_11']; ?></div>
    </div>
  <?php } ?>
</div>

