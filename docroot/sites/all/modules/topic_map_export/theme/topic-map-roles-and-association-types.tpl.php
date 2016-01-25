<?php

/**
 * @file
 * Template for creating association types and roles(both of course are topics)
 *
 * Available variables:
 * - $association_definitions: List used association definitions.
 */
?>

<?php foreach ($association_definitions as $key => $value): ?>
  <topic id="<?php echo $key; ?>">
    <name>
      <value><?php echo $value; ?></value>
    </name>
  </topic>
<?php endforeach; ?>
