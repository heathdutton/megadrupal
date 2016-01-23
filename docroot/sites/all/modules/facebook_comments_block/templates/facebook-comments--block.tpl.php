<?php

/**
 * @file
 * Default theme implementation to display a facebook comments block.
 *
 * Available variables:
 * - $facebook: array contains all available variables.
 * - url: the page url.
 *   the fb script to include fb app id.
 * - facebook_comments_block_settings_width: The width of the plugin.
 * - facebook_comments_block_settings_number_of_posts: The number of comments
 *   to show by default.
 * - facebook_comments_block_settings_color_schema:
 *   The color scheme used by the plugin.
 */
?>

<div id="fb-root"></div>
<div class="fb-comments" <?php print drupal_attributes($facebook['data_attributes']); ?>></div>
