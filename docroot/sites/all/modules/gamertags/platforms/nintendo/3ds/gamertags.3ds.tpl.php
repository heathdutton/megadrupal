<?php
/**
 * @file
 * Theme implementation for displaying 3DS Gamertags.
 *
 * Available variables:
 *  - $classes: String of classes that can be used to style contextually through
 *      CSS. It can be manipulated through the variable $classes_array from
 *      preprocess functions
 *  - $gamertag: The users Playstation Network gamertag
 *  - $url: The parsed gamertag URL
 *
 * @see gamertags_preprocess_gamertags_3ds()
 */
?>
<div id="3ds-background" class="<?php print $classes; ?>" name="gamertags_3ds">
  <div id="3ds-gamertag">
    <?php print $gamertag; ?>
  </div>
</div>