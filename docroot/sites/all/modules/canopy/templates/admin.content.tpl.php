<?php
/**
 * @file
 * TPL file to render the content within osCaddie Alfresco UI.
 */
?>
<?php if ($sidebar): print $sidebar; endif; ?>
<div class="content">
  <h2><?php print drupal_get_title(); ?></h2>
  <p class="breadcrumb"><?php print oscaddie_alfresco__admin__breadcrumb() ?></p>
  <div class="frame">
    <?php print $content ?>
  </div>
</div>
<div class="clear"></div>
