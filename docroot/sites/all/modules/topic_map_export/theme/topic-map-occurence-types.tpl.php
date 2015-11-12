<?php

/**
 * @file
 * Template for creating most of the occurrance(exluding few 'description' type
 * occurences)
 *
 * Available variables:
 * - $occurrences: List occurrences
 */
?>

<?php foreach ($occurrences as $key => $value): ?>
  <topic id="field-type-<?php echo $key . '-' . $value['entity_type'] . '-' . $value['bundle']; ?>">
    <name>
      <value><?php echo $value['label'] . " (" . $value['entity_type'] . '-' . $value['bundle']; ?>)</value>
      <?php echo $value['variant_name']; ?>
    </name>
    <occurrence>
      <type>
        <topicRef href="#field-description" />
      </type>
      <resourceData><?php echo $value['description']; ?></resourceData>
    </occurrence>
  </topic>
<?php endforeach; ?>
<topic id="field-description">
  <name>
    <value>Field description</value>
  </name>
</topic>
