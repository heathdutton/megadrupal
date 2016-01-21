<?php
// $Id: node.tpl.php,v 1.8 2010/11/16 15:36:36 shannonlucas Exp $
/**
 * @file
 * The node rendering logic for Nitobe.
 *
 * In addition to the standard variables Drupal makes available to node.tpl.php,
 * these variables are made available by the theme:
 * - $nitobe_node_author: The node's "posted by" text and author link.
 * - $nitobe_perma_title: The localized permalink text for the node.
 * - $nitobe_show_meta: Indicates whether the meta data div should be rendered.
 * - $nitobe_node_timestamp: The timestamp for this type, if one should be
 *   rendered for this type.
 */

hide($content['comments']);
hide($content['links']);
hide($content['field_tags']);
?>
<div class="<?php echo $classes; ?>">
<?php if ($page == 0): ?>
  <div class="node-headline clearfix">
    <h2><a href="<?php echo $node_url; ?>" rel="bookmark" title="<?php echo $nitobe_perma_title; ?>"><?php echo $title; ?></a></h2>
    <?php if (isset($nitobe_node_timestamp)): ?>
        <span class="timestamp"><?php echo $nitobe_node_timestamp; ?></span>
    <?php endif; ?>
  </div>
<?php endif; ?>
  <div class="content clearfix">
    <?php if (isset($nitobe_node_author)): ?>
      <div class="node-author"><?php echo $nitobe_node_author; ?></div>
    <?php endif; ?>
    <?php echo $user_picture; ?>
    <?php echo render($content); ?>
  </div>
  <?php if ($nitobe_show_meta): ?>
    <div class="meta">
      <?php
        if (!empty($content["field_tags"])) echo render($content["field_tags"]);
        if (!empty($content["links"])) echo render($content["links"]); ?>
    </div>
  <?php endif; ?>
  <div class="clearfix">
    <?php echo render($content["comments"]); ?>
  </div>
</div>
