<?php

/**
 * @file
 * Default theme implementation to display a wishlist.
 *
 * Available variables:
 * - $settings_form: the settings_form to change wishlist settings.
 * - $message: The message indicating if wishlist is either valid or expired.
 * - $wishlist: the wishlist itself.
 *
 * Other variables:
 * - $expired: boolean, indicating if wishlist is expired.
 * - $expiration_date: date wishlist expires.
 * - $email_link: url to follow to access the email wishlist functionality.
 */
?>
<?php print drupal_render($settings_form) ?>
<p><?php print $message; ?></p>
<?php print drupal_render ($wishlist); ?>
