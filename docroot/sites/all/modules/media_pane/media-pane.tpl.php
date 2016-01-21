<?php

/**
 * @file
 * A basic template for media_block entities.
 *
 * Gets information from the settings fields on the fieldable panels pane and 
 * modifies the rendered output accordingly.
 */

// Get field values.
$media_pane_link = $elements['#media_pane_link'];
$media_pane_link_text = $elements['#media_pane_link_text'];
?>

<div class="<?php print $classes; ?>"<?php print $attributes; ?>>
  <?php print render($title_suffix);
  if ($media_pane_link) : ?>
    <div class="media-pane-link-wrapper">
      <?php print render($content); ?>
    </div>
    <?php print theme('link', $media_pane_link_text) ?>
  <?php else :
    print render($content);
  endif;
 ?>
</div>
