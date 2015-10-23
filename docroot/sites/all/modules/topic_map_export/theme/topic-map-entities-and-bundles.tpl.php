<?php
/**
 * @file
 * Template for creating topic types for Drupal entities and bundles
 *
 * Available variables:
 * - $entities: List supported entities(keys) and bundles(values)
 */
?>
<?php foreach ($entities as $entity_id => $entity): ?>
  <topic id="entity_<?php echo $entity_id; ?>">
    <subjectIdentifier href="<?php echo $entity['entity_si'];  ?>" />
    <name>
      <value><?php echo $entity['label']; ?> (Entity)</value>
    </name>
    <instanceOf>
      <topicRef href="#website"/>
    </instanceOf>
  </topic>
  <topic id="entity_<?php echo $entity_id; ?>-bundle-description">
    <subjectIdentifier href="<?php echo $entity['desc_si']; ?>" />
    <name>
      <value><?php echo $entity['label']; ?> bundle description</value>
    </name>
  </topic>
  <?php foreach ($entity['bundles'] as $bundle_key => $bundle): ?>
    <topic id="entity_<?php echo $entity_id; ?>-bundle_<?php echo $bundle_key; ?>">
      <subjectIdentifier href="<?php echo $bundle['bundle_si']; ?>" />
      <name>
        <value><?php echo $bundle['label']; ?> (<?php echo $bundle['bundle_of']; ?>)</value>
      </name>
      <instanceOf>
        <topicRef href="#entity_<?php echo $entity_id; ?>"/>
      </instanceOf>
    <?php if (!empty($bundle['bundle_desc'])): ?>
      <occurrence>
        <type>
          <topicRef href="#entity_<?php echo $entity_id; ?>-bundle-description" />
        </type>
        <resourceData><?php echo $bundle['bundle_desc']; ?></resourceData>
      </occurrence>
    <?php endif; ?>
    </topic>
    <topic id="entity_<?php echo $entity_id; ?>-bundle-<?php echo $bundle_key; ?>-description">
      <subjectIdentifier href="<?php echo $bundle['bundle_desc_si']; ?>" />
      <name>
        <value><?php echo $bundle['label']; ?> description</value>
      </name>
    </topic>
  <?php endforeach; ?>
<?php endforeach; ?>
