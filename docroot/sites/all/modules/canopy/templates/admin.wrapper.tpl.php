<?php
/**
 * @file
 * TPL file to render a generic wrapper to wrap content within osCaddie Alfresco UI.
 */
?>
<div id="oscaddie_alfresco-admin">
  <div class="header">
    <?php print l(t("osCaddie Alfresco"), 'admin/oscaddie_alfresco', array('attributes' => array('class' => array('logo'), 'title' => t("osCaddie Alfresco")))); ?>
  </div>
  <div class="wrapper">
    <?php drupal_set_title(''); ?>
    <?php print $content; ?>
  </div>
  <div class="footer">
    <p class="copyright"><?php print t("copyright @year by Appnovation Technologies", array('@year' => date('Y'))); ?></p>
  </div>
</div>
