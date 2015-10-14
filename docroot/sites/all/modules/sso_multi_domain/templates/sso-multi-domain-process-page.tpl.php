<?php
/**
 * @file
 * Template to display single sign on processing page contents.
 */
?>
<div id="dvLoading">
  <div class="sso_processing_page_text"><?php print $processing_page_text; ?></div>
  <div class="sso_loader_image"><?php print $loader_image; ?></div>
  <div class="sso_loader_image_text"><?php print check_plain(variable_get('sso_multi_domain_loader_text', t('Please wait...'))); ?></div>
</div>
<div id="sso-process"  >
  <?php print $images; ?>
</div>
