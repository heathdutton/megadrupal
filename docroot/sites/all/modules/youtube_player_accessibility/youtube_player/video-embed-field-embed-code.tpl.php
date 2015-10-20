<?php
/**
 * @file
 * Default theme implementation to display a video embed field.
 *
 * Variables available:
 * - $url: The URL of the video to display embed code for
 * - $style: The style name of the embed code to use
 * - $style_settings: The style settings for the embed code
 * - $handler: The name of the video handler
 * - $embed_code: The embed code
 * - $data: Additional video data
 *
 * @see template_preprocess_video_embed()
 */
?>
<!-- This is where the player, buttons, and (optional) play list get rendered -->
<div class="embedded-video ytplayerbox">
  <div class="player">
    <?php print $embed_code; ?>
  </div>
  <!-- specify 'normal' for YouTube VGA aspect ratio (480 x 360) and 'wide' for YouTube HD (640 x 360) -->
  <span class="ytplayeraspect: wide">&nbsp;</span>
  <!-- list video titles and identifiers here, play list rendered only if more than one movie  -->
  <span class="ytmovieurl: <?php print $data['media$group']['yt$videoid']; ?>"><?php print $data['title']; ?></span>
</div>