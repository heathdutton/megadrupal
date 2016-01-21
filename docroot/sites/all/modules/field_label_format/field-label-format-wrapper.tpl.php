<?php
/**
 * @file
 * Custom label format template.
 *
 * Available variables:
 * - $label_element: The HTML element for the label.
 * - $label_attributes: HTML attributes string for the label.
 * - $label_text: Sanitized label text.
 * - $wrapper_attributes: HTML attributes string for the wrapper.
 * - $field_content: Rendered field HTML content.
 */
?>
<div<?php print $wrapper_attributes; ?>>
  <<?php print $label_element . $label_attributes; ?>>
    <?php print $label_text; ?>
  </<?php print $label_element; ?>>
  <?php print $field_content; ?>
</div>
