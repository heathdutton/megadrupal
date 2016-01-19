<?php
/**
 * @file
 * Display the embed code
 *
 * @see template_preprocess_video_shortcode()
 */
?>

<div style="overflow: hidden;" class="<?php print $classes; ?>">
  <?php print $embed_code; ?>
  <?php print $caption; ?>
</div>
