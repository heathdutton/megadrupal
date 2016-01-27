<?php
/**
 * @file
 * Theme implementation to display the Mobile Switch Block switch content.
 *
 * Available variables:
 * - $content: The complete switch content. Possible content variants:
 *   - Switch link
 *   - Message and switch link.
 *
 * - $mobile_switch_block_content: Parameter from the Mobile Switch block
 *   configuration to display the block content. Possible values:
 *   - link: Display only the switch link.
 *   - message_link: Display a message and the switch link
 * - $query_value: Used as query value in the switch link URL. Possible values:
 *   - mobile
 *   - standard
 * - $version: A string, used in the switch link text and the switch link URL.
 *   Possible values:
 *   - mobile
 *   - standard or desktop
 * - $version_text: A string, used in the switch message. Possible values:
 *   - mobile
 *   - standard or desktop
 * - $class: A class string to use for the identification of the switch link.
 *   Possible values:
 *   - mobile-switch-to-mobile
 *   - mobile-switch-to-standard
 *
 * - $switch_link: HTML content of the switch link.
 * - $switch_message: HTML content of the additional message, prepended to the
 *   switch link.
 *
 * Helper variables:
 * - $user: The user object.
 * - $mobile_switch_ismobiledevice: Boolean value.
 * - $classes_array: Array of html class attribute values. It is flattened
 *   into a string within the variable $classes.
 * - $zebra: Outputs 'odd' and 'even' dependent on each block region.
 * - $id: Counter dependent on each block region independent of any block region.
 * - $is_front: Flags true when presented in the front page.
 * - $logged_in: Flags true when the current user is a logged-in member.
 * - $is_admin: Flags true when the current user is an administrator.
 *
 * Usage with a 'Mobile jQuery' theme:
 *
 *   For more informations please read the module README.txt.
 *
 * @see template_preprocess_mobile_switch_block()
 *
 * @ingroup themeable
 */
?>
<div class="<?php print $class ?>">
  <?php print $content ?>
</div>
