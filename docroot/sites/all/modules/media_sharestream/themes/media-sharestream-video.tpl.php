<?php

/**
 * @file
 * Template file for theme('media_sharestream_video').
 *
 * Variables available:
 *  $uri - The media uri for the ShareStream video
 *    (e.g., sharestream://v/xsy7x8c9).
 *  $video_id - The unique identifier of the ShareStream video (e.g., xsy7x8c9).
 *  $id - The file entity ID (fid).
 *  $url - The full url including query options for the ShareStream iframe.
 *  $options - An array containing the Media ShareStream formatter options.
 *  $width - The width value set in Media: ShareStream file display options.
 *  $height - The height value set in Media: ShareStream file display options.
 *  $title - The Media: ShareStream file's title.
 *  $alternative_content - Text to display for browsers that don't support
 *  iframes.
 */

?>
<div class="<?php print $classes; ?> media-sharestream-<?php print $id; ?>">
  <iframe class="media-sharestream-player" width="<?php print $width; ?>" height="<?php print $height; ?>" title="<?php print $title; ?>" src="<?php print $url; ?>" frameborder="0" allowfullscreen><?php print $alternative_content; ?></iframe>
</div>
