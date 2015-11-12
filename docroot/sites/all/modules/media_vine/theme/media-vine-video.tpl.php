<?php
/**
 * @file
 * Template file for theme('media_vine_video').
 *
 * Variables available:
 *  $uri - The media uri for the Vine video (e.g. vine://v/hrAOEOBw0F2).
 *  $video_id - The unique identifier of the Vine video (e.g., hrAOEOBw0F2).
 *  $id - The file entity ID (fid).
 *  $url - The full url including query options for the Vine iframe.
 *  $options - An array containing the Media Vine formatter options.
 *  $width - The width value set in Media: Vimeo file display options.
 *  $height - The height value set in Media: Vimeo file display options.
 *  $title - The Media: YouTube file's title.
 *  $embed_format - Vine embed Format.
 *  $autoplay_audio - Vine autoplay audio.
 *
 *
 *  $alternative_content - Text to display for browsers
 *  that don't support iframes.
 */
?>

<div class="<?php print $classes; ?> media-vine-<?php print $id; ?>">  
  <iframe class="vine-embed" src="<?php print $url; ?>" width="<?php print $width; ?>" height="<?php print $height; ?>" frameborder="0"><?php print $alternative_content; ?></iframe><script async src="//platform.vine.co/static/scripts/embed.js" charset="utf-8"></script>
</div>
