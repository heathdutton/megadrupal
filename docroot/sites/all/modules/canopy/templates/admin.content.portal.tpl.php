<?php
/**
 * @file
 * TPL file to render the osCaddie Alfresco Portal Configuration view.
 */
?>
<?php if ($alfresco): ?>
<div id="portal-alfresco" class="clearfix">
  <h3 class="icon alfresco"><?php print t("Alfresco"); ?></h3>
  <?php print $alfresco->content; ?>

  <?php if ($alfresco->buttons['add']): ?>
  <?php print l(t("Add"), 'admin/oscaddie_alfresco/portal/alfresco/add', array('attributes' => array('class' => array('button', 'add')))); ?>
  <?php else: ?>
  <span class="button add disabled">&nbsp;</span>
  <?php endif; ?>
  <div class="clear"></div>
</div>
<?php endif; ?>

<?php if ($drupal): ?>
<div id="portal-drupal" class="clearfix">
  <h3 class="icon drupal"><?php print t("Drupal"); ?></h3>
  <?php print $drupal->content; ?>

  <?php if ($drupal->buttons['add']): ?>
  <?php print l(t("Add"), 'admin/oscaddie_alfresco/portal/drupal/add', array('attributes' => array('class' => array('button', 'add')))); ?>
  <?php else: ?>
  <span class="button add disabled">&nbsp;</span>
  <?php endif; ?>
  <div class="clear"></div>
</div>
<?php endif; ?>
