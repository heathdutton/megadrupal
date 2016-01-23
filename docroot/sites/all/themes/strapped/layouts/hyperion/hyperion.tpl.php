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

    <div class="row layer-0">
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

  <?php if (!empty($content['layer_3'])) { ?>

    <div class="row layer-3">
      <div class="col-md-12 layer-3"><?php print $content['layer_3']; ?></div>
    </div>

  <?php } ?>



  <?php if (!empty($content['layer_4_left']) || !empty($content['layer_4_right'])) { ?>

    <div class="row layer-4">
      <div
        class="col-md-8 layer-4-left"><?php print $content['layer_4_left']; ?></div>

      <div
        class="col-md-4 layer-4-right"><?php print $content['layer_4_right']; ?></div>
    </div>

  <?php } ?>

  <?php if (!empty($content['layer_5_left']) || !empty($content['layer_5_right'])) { ?>

    <div class="row layer-5">
      <div
        class="col-md-6 layer-5-left"><?php print $content['layer_5_left']; ?></div>

      <div
        class="col-md-6 layer-5-right"><?php print $content['layer_5_right']; ?></div>
    </div>

  <?php } ?>

  <?php if (!empty($content['layer_6_left']) || !empty($content['layer_6_center']) || !empty($content['layer_6_right'])) { ?>

    <div class="row layer-6">
      <div
        class="col-md-4 layer-6-left"><?php print $content['layer_6_left']; ?></div>
      <div
        class="col-md-4 layer-6-center"><?php print $content['layer_6_center']; ?></div>
      <div
        class="col-md-4 layer-6-right"><?php print $content['layer_6_right']; ?></div>
    </div>

  <?php } ?>

</div>

