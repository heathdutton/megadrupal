<?php
/**
 * @file
 * View template to display view rows in a tree structure.
 *
 * @ingroup views_templates
 */
?>
<?php if (!empty($title)): ?>
<<?php print $group_element; ?><?php print drupal_attributes($group_attributes); ?>>
<?php print $title; ?>
</<?php print $group_element; ?>>
<?php endif; ?>
<?php print $rows_content; ?>
