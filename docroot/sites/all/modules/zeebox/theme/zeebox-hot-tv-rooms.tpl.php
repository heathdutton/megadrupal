<?php
/**
 * @file
 * Default theme implementation for displaying the zeebox Hot TV Rooms widget within a block.
 *
 * Available variables:
 * - $url: The fully formed URL to the zeebox TV Room.
 * - $message: The hyperlink alternative message to display should the javascript not load.
 * - $region: The current region code, check_plain() before use.
 * - $width: The width of the widget container.
 * - $settings: An array of passed bean settings.
 *
 * Helper variables:
 * - $classes_array: Array of html class attribute values. It is flattened
 *   into a string within the variable $classes.
 * - $is_front: Flags true when presented in the front page.
 * - $logged_in: Flags true when the current user is a logged-in member.
 * - $is_admin: Flags true when the current user is an administrator.
 *
 *  @see template_preprocess_zeebox_hot_tv_rooms()
 */
?>
<div style="width: <?php print $width; ?>;">
  <a class="<?php print $classes;?>" href="<?php print $url; ?>" data-zeebox-limit="<?php print $settings['limit']; ?>" data-zeebox-link="<?php print $settings['tv_room_link']; ?>" data-zeebox-type="hot-rooms"><?php print $message; ?></a>
</div>
