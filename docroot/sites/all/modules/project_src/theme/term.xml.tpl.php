<?php
/**
 * @file
 * Project Source's implementation to display project term XML.
 *
 * Available variables:
 * - $name: The name of this term.
 * - $value: The value of this term.
 *
 * @see template_preprocess_project_src_xml()
 * @see template_preprocess_project_src_release()
 * @see template_preprocess_project_src_term()
 */
?>
<term>
  <name><?php echo $name; ?></name>
  <value><?php echo $value; ?></value>
</term>
