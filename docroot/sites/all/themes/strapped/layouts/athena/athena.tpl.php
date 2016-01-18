<div class="clearfix <?php print $classes ?>" <?php if (!empty($css_id)) {
  print "id=\"$css_id\"";
} ?>>
  <?php if (!empty($content['layer_1_left']) || !empty($content['layer_1_right'])) { ?>
    <div class="row layer-1">
      <div class="col-md-6 layer-1-left"><?php print $content['layer_1_left']; ?></div>
        <div class="col-md-6 layer-1-right"><?php print $content['layer_1_right']; ?></div>
    </div>
  <?php } ?>
</div>

