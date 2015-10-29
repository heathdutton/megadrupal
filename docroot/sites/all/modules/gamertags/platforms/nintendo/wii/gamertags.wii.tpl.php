<?php
/**
 * @file
 * Theme implementation for displaying Wii Gamertags.
 *
 * Available variables:
 *  - $alt: The alternative image text to display
 *  - $classes: String of classes that can be used to style contextually through
 *      CSS. It can be manipulated through the variable $classes_array from
 *      preprocess functions
 *  - $gamertag: The users Playstation Network gamertag
 *  - $mapwii: The boolean for whether to use (TRUE) mapwii.com or not
 *  - $url: The parsed gamertag URL
 *
 * @see gamertags_preprocess_gamertags_wii()
 */
?>
<div id="wii-background" class="<?php print $classes; ?>" name="gamertags_wii">
  <div id="wii-gamertag">
    <?php if ($mapwii): ?>
      <a href="<?php print $url; ?>"><?php print $gamertag; ?></a>
    <?php else: ?>
      <?php print $gamertag; ?>
    <?php endif; ?>
  </div>
</div>
