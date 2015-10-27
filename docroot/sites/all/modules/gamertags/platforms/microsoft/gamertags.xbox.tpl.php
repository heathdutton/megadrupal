<?php
/**
 * @file
 * Theme implementation for displaying Xbox Gamertags.
 *
 * Available variables:
 *  - $classes: String of classes that can be used to style contextually through
 *      CSS. It can be manipulated through the variable $classes_array from
 *      preprocess functions
 *  - $gamertag: The users Xbox gamertag
 *  - $no_iframe_message: The message to display
 *  - $url: The parsed gamertag URL
 *
 * @see gamertags_preprocess_gamertags_xbox()
 */
?>
<iframe name="gamertags_xbox" src="<?php print $url; ?>" class="<?php print $classes; ?>" title="<?php print $gamertag; ?>" scrolling="no" width="204" height="140" frameborder="0" marginwidth="0" marginheight="0">
  <?php print $no_iframe_message; ?>
</iframe>
