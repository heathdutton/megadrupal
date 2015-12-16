<?php

/**
 * @file media_tudou/includes/themes/media-tudou-video.tpl.php
 *
 * Template file for theme('media_tudou_video').
 *
 * Variables available:
 *  $uri - The uri to the ToDou video, such as tudou://v/xsy7x8c9.
 *  $video_id - The unique identifier of the ToDou video.
 *  $width - The width to render.
 *  $height - The height to render.
 *  $autoplay - If TRUE, then start the player automatically when displaying.
 *  $fullscreen - Whether to allow fullscreen playback.
 *
 * Note that we set the width & height of the outer wrapper manually so that
 * the JS will respect that when resizing later.
 */
?>
<div class="media-tudou-outer-wrapper" id="media-tudou-<?php print $id; ?>" style="width: <?php print $width; ?>px; height: <?php print $height; ?>px;">
  <div class="media-tudou-preview-wrapper" id="<?php print $wrapper_id; ?>">
    <?php print $output; ?>
  </div>
</div>
