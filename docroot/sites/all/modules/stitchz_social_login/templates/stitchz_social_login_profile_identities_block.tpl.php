<?php
/**
 * @file
 * Displays an identity link.
 */
?>

<li class="stitchz_social_login_identity">
  <div class="stitchz_social_login_identity_provider <?php print strtolower($provider_name); ?>">
    <span class="fa fa-fw fa-<?php print strtolower($provider_name); ?>"></span>
    <span class="stitchz_social_login_identity_provider_name"><?php print $provider_name; ?></span>
  </div>
  <div class="stitchz_social_login_identity_provider_actions">
    <a href="<?php print $authentication_url; ?>">
      <span class="fa fa-times" title="<?php print t('remove this identity provider') ?>"></span>
    </a>
  </div>
  <br style="clear: left;" />
</li>
