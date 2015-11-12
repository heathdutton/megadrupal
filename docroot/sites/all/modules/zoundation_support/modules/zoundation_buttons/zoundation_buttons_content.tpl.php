<?php
/**
 * @file
 * Template file for zoundation content button
 */
?>
<a<?php print drupal_attributes($button_attributes); ?>><?php print $title; ?></a>
<div <?php print drupal_attributes($dropdown_attributes); ?> data-dropdown-content>
<?php print $content; ?>
</div>
