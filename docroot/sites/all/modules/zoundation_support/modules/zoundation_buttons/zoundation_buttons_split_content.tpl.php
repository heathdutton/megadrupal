<?php
/**
 * @file
 * Template file for zoundation split content button
 */
?>
<a<?php print drupal_attributes($button_attributes); ?>><?php print $title; ?>
  <span data-dropdown="<?php print $dataid;?>"></span>
</a>
<div <?php print drupal_attributes($dropdown_attributes); ?> data-dropdown-content>
<?php print $content; ?>
</div>
