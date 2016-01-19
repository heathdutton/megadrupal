<?php
/**
 * @file
 * Template file for zoundation list button
 */
?>
<a<?php print drupal_attributes($button_attributes); ?>><?php print $title; ?></a>
<ul <?php print drupal_attributes($dropdown_attributes); ?> data-dropdown-content>
<?php print $items; ?>
</ul>
