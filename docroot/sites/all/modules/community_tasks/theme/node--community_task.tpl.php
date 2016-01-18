<?php
/*
 * node template
 * available variables, as with node.tpl.php, but also
 * $button = a context sensitive form allowing the current user to change the state of the task
 * $description = a brief description of the state of the task.
 */

?>
<!-- start node-community_task.tpl.php -->
<div id="node-<?php print $node->nid; ?>" class="node<?php print " node-" . $node->type; ?> <?php print implode(' ', $classes_array) ?>">
<?php if (!$page && $title): ?>
  <h3 class="title"><a href="<?php print $node_url; ?>" title="<?php print $title; ?>"><?php print $title; ?></a></h3>
<?php endif; ?>
  <div style = "float:right"><strong><?php print $description; ?></strong></div>
<?php if ($submitted && $picture): ?>
  <div class="info"><?php print $picture; ?></div>
<?php endif; ?>
<div class="content"><?php print render($content); ?></div>
</div><!-- end node-community_task.tpl.php -->