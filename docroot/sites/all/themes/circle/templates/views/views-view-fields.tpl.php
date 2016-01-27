<?php

/**
 * @file
 * Default simple view template to all the fields as a row.
 */
?>
<?php foreach ($fields as $id => $field): ?>
  <?php if (!empty($field->separator)): ?>
    <?php print $field->separator; ?>
  <?php endif; ?>
  <?php print $field->wrapper_prefix; ?>
    <?php if (!empty($field->label_html)): ?>
      <span class="views-field-label">
        <?php print $field->label_html; ?>
      </span>
    <?php endif; ?>
    <?php print $field->content; ?>
  <?php print $field->wrapper_suffix; ?>
<?php endforeach; ?>
