<?php

/**
 * @file media_ted/includes/themes/media-ted-video.tpl.php
 *
 * Template file for theme('media_ted_video').
 *
 * Variables available:
 *  $uri - The uri to the Ted video, such as ted://v/xsy7x8c9.
 *  $video_id - The unique identifier of the Ted video.
 *  $width - The width to render.
 *  $height - The height to render.
 *  $autoplay - If TRUE, then start the player automatically when displaying.
 *  $fullscreen - Whether to allow fullscreen playback.
 *  $flash_embed - The old flash-based embed code
 */
?>
<?php if (isset($flash_embed)) : ?>
<div class="media-ted-outer-wrapper" id="media-ted-<?php print $id; ?>" style="width: <?php print $width; ?>px; height: <?php print $height; ?>px;">
  <div class="media-ted-preview-wrapper" id="<?php print $wrapper_id; ?>">
    <?php print $flash_embed; ?>
  </div>
</div>
<?php else : ?>
<div class="<?php print $classes ?>">
  <iframe class="media-ted-player" src="<?php print $url ?>" width="<?php print $width ?>" height="<?php print $height ?>" allowfullscreen frameborder="0" scrolling="no"></iframe>
</div>
<?php endif; ?>
