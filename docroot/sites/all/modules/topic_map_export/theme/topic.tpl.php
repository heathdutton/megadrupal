<?php

/**
 * @file
 * Template for single topic
 *
 * Available variables:
 * - $topic_id: Identifier for the topic
 * - $si: One or more Subjectidentifiers
 * - $name: Name of the topic
 * - $instance_of: Topics superclass or superclasses
 * - $occurrences: Occurrence(s) of topic. Notice that these should be
 * already rendered.
 * - $variants: Variant names of current topic
 */
?>
<topic id="<?php echo $topic_id; ?>">
  <?php foreach ($si as $value): ?>
    <subjectIdentifier href="<?php echo $value; ?>" />
  <?php endforeach; ?>
  <?php if (!empty($name)): ?>
    <name>
      <value><?php echo $name; ?></value>
      <?php echo implode("\n", $variants); ?>
    </name>  
  <?php endif; ?>  
  <?php foreach($instance_of as $value): ?>
    <instanceOf>
      <topicRef href="#<?php echo $value; ?>" />
    </instanceOf>
  <?php endforeach; ?>
  <?php echo implode("\n", $occurrences); ?>
</topic>
