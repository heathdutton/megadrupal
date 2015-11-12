<?php

/**
 * @file
 * Default theme implementation to display node accessibility information.
 *
 * Available Variables:
 *
 * Content Variables:
 * - $node: The node whose information is to be displayed.
 */
?>
<div class="node_accessibility-node_information node_accessibility-items">
  <div class="node_accessibility-node_information-nid node_accessibility-node_information-item">
    <span class="node_accessibility-node_information-nid-object node_accessibility-node_information-object"><?php print(t("Node ID")); ?></span>
    <span class="node_accessibility-node_information-nid-content node_accessibility-node_information-content"><a href="<?php print(url('node/' . $node->nid)); ?>"><?php print($node->nid); ?></a></span>
  </div>
  <div class="node_accessibility-node_information-vid node_accessibility-node_information-item">
    <span class="node_accessibility-node_information-vid-object node_accessibility-node_information-object"><?php print(t("Revision ID")); ?></span>
    <span class="node_accessibility-node_information-vid-content node_accessibility-node_information-content"><?php print($node->vid); ?></span>
  </div>
  <div class="node_accessibility-node_information-author node_accessibility-node_information-item">
    <span class="node_accessibility-node_information-author-object node_accessibility-node_information-object"><?php print(t("Revision Author")); ?></span>
    <span class="node_accessibility-node_information-author-content node_accessibility-node_information-content"><?php print(theme('username', array('account' => $node))); ?></span>
  </div>
  <div class="node_accessibility-node_information-date node_accessibility-node_information-item">
    <span class="node_accessibility-node_information-date-object node_accessibility-node_information-object"><?php print(t("Revision Date")); ?></span>
    <span class="node_accessibility-node_information-date-content node_accessibility-node_information-content"><?php print(format_date($node->revision_timestamp, 'short')); ?></span>
  </div>
  <div class="node_accessibility-node_information-published node_accessibility-node_information-item">
    <span class="node_accessibility-node_information-published-object node_accessibility-node_information-object"><?php print(t("Published")); ?></span>
    <span class="node_accessibility-node_information-published-content node_accessibility-node_information-content"><?php print($published); ?></span>
  </div>
  <?php if ($workbench_moderation) { ?>
    <div class="node_accessibility-node_information-live node_accessibility-node_information-item">
      <span class="node_accessibility-node_information-live-object node_accessibility-node_information-object"><?php print(t("Live")); ?></span>
      <span class="node_accessibility-node_information-live-content node_accessibility-node_information-content"><?php print($live); ?></span>
    </div>
    <div class="node_accessibility-node_information-current node_accessibility-node_information-item">
      <span class="node_accessibility-node_information-current-object node_accessibility-node_information-object"><?php print(t("Current")); ?></span>
      <span class="node_accessibility-node_information-current-content node_accessibility-node_information-content"><?php print($current); ?></span>
    </div>
  <?php } ?>
</div>
