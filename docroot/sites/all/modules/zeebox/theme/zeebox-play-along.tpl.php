<?php
/**
 * @file
 * Default theme implementation for displaying the zeebox Synchronised play-along zone within a block.
 *
 * Available variables:
 * - $url: The fully formed URL to the zeebox Play-along zone.
 * - $message: The hyperlink alternative message to display should the javascript not load.
 * - $region: The current region code, check_plain() before use.
 * - $settings: An array of passed bean settings.
 *
 * Helper variables:
 * - $classes_array: Array of html class attribute values. It is flattened
 *   into a string within the variable $classes.
 * - $is_front: Flags true when presented in the front page.
 * - $logged_in: Flags true when the current user is a logged-in member.
 * - $is_admin: Flags true when the current user is an administrator.
 *
 *  @see template_preprocess_zeebox_play_along()
 */
?>
<a class="<?php print $classes;?>" href="<?php print $url; ?>" data-zeebox-type="sync-widget"><?php print $message; ?></a>
