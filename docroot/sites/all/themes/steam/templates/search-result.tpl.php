<?php
/**
 * @file
 * Steam theme implementation for displaying a single search result.
 *
 * @see steam_preprocess_search_result()
 */
?>
<li class="<?php print $classes; ?>"<?php print $attributes; ?>>
  <?php print render($result); ?>
</li>
