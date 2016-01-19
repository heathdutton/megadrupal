<?php
/**
 * @file
 * Default theme implementation for displaying the zeebox Follow Button within a block.
 *
 * Available variables:
 * - $url: The fully formed URL to the zeebox Follow Button.
 * - $message: The hyperlink alternative message to display should the iframe not load.
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
 *  @see template_preprocess_zeebox_follow_button()
 */
?>
<iframe class="<?php print $classes;?>" style="margin: 0; border: 0; padding: 0;" scrolling="no" frameborder="0" allowtransparency="true" width="110px" height="36px" src="<?php print $url; ?>"><?php print $message; ?></iframe>
