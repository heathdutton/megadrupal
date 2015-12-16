<?php

/**
 * @file
 * Template file for theme('media_solidtango_video').
 *
 * Variables available:
 * - $uri: The media uri for the Solidtango video
 *   (e.g., solidtango://host/example.solidtango.com/type/video/slug/x1y3z5w7).
 * - $video_id: The unique identifier of the Solidtango video (e.g., x1y3z5w7).
 * - $id: The file entity ID (fid).
 * - $url: The full url including query options for the Solidtango iframe.
 * - $options: An array containing the Media Solidtango formatter options.
 * - $width: The width value set in Media: Solidtango file display options.
 * - $height: The height value set in Media: Solidtango file display options.
 * - $title: The Media: YouTube file's title.
 * - $alternative_content: Text to display for browsers that don't support
 *   iframes.
 */

?>
<div class="<?php print $classes; ?> media-solidtango-<?php print $video_id; ?>">
  <iframe class="media-solidtango-player" width="<?php print $width; ?>" height="<?php print $height; ?>" title="<?php print $title; ?>" src="<?php print $url; ?>" frameborder="0" id="media-solidtango-<?php print $video_id; ?>" allowfullscreen><?php print $alternative_content; ?></iframe>
</div>
