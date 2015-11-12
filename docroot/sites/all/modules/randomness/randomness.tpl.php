<?php

/**
 * @file
 * HTML for randomness.
 *
 * @see template_preprocess_randomness()
 */
?>
<script type="text/javascript">
  jQuery(document).ready(function() {
    jQuery.randomness({
      canvas: '<?php print $canvas; ?>',
      particle_count : <?php print $particle_count; ?>,
      particle_random : '<?php print $particle_random; ?>',
      offset_top: <?php print $offset_top; ?>,
      offset_left: <?php print $offset_top; ?>,
      z_index_mod: <?php print $z_index_mod; ?>,
      mode: '<?php print $mode; ?>',
      acceleration: '<?php print $acceleration; ?>',
      easing: '<?php print $easing; ?>',
      max_dimension_image: <?php print $max_dimension_image; ?>,
      max_dimension_link_custom: <?php print $max_dimension_link_custom; ?>,
      images: <?php if (!empty($images)): print $images; endif ?>,
      links_custom: <?php if (!empty($links_custom)): print $links_custom; endif ?>
  });
});
</script>
<style type="text/css">
#randomness-canvas {
  width: <?php print $canvas_width; ?>px; 
  height: <?php print $canvas_height; ?>px;
}
</style>
<div class="<?php print $classes; ?>">
  <div id="randomness-canvas"></div>
</div>
