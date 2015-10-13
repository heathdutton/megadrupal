<?php
/**
 * @file
 * Displays the provider link.
 */
?>

<li class="stitchz_social_login_provider <?php print strtolower($provider_name); ?> big">
  <a href="<?php print $authentication_url; ?>">
    <span class="fa fa-fw fa-<?php print strtolower($provider_name); ?>"></span><?php print $provider_name; ?>
  </a>
</li>
