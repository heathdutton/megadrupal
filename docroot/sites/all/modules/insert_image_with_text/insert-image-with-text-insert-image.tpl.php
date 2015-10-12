<?php

/**
 * @file
 * Template file for ImageCache content inserted via the Insert module.
 *
 * Available variables:
 * - $item: The complete item being inserted.
 * - $url: The URL to the image.
 * - $class: A set of classes assigned to this image (if any).
 * - $style_name: The Image style being used.
 *
 * Note that ALT and Title fields should not be filled in here, instead they
 * should use placeholders that will be updated through JavaScript when the
 * image is inserted.
 *
 * Available placeholders:
 * - __alt__: The ALT text, intended for use in the <img> tag.
 * - __title__: The Title text, intended for use in the <img> tag.
 * - __description__: A description of the image, sometimes used as a caption.
 * - __filename__: The file name.
 * - __[token]_or_filename__: Any of the above tokens if available, otherwise
 *   use the file's name. i.e. __title_or_filename__.
 *
 * @see insert_theme().
 */

?>
<div class="insert-image-with-text insert-image-with-text-<?php print $style_name ?>">
  <img src="<?php print $url ?>" alt="__alt__" title="__title__" class="insert-image-with-text_image image-<?php print $style_name ?><?php print $class ? ' ' . $class : '' ?>" />
  <div class="insert-image-with-text_description">__title__</div>
</div>
