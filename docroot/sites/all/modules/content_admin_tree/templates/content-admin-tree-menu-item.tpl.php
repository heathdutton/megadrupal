<?php
/**
 * @file
 * This file contains the markup for each content admin tree menu.
 *
 * These items contain the node type which acts as a root of
 * a taxonomy tree. It will simply render the node
 * type as an anchor link if there aren't taxonomy
 * terms that are attached to that particular node type.
 */
?>
<?php if (($start == TRUE || $start != NULL) && ($active == TRUE || $active != NULL)) : ?>
  <div class="cat-menu"><ul class="cat-taxonomy-tree root"><li class="cat-node-type expanded active-trail">
  <?php elseif (($start == FALSE || $start == NULL) && ($active == TRUE || $active != NULL)) : ?>
    <li class="cat-node-type expanded active">
  <?php elseif (($start == FALSE || $start == NULL) && ($active == FALSE) || $active != NULL) : ?>
    <li class="cat-node-type leaf">
  <?php elseif (($start == TRUE || $start != NULL) && ($active == FALSE) || $active == NULL) : ?>
    <div class="cat-menu"><ul class="cat-taxonomy-tree root"><li class="cat-node-type leaf">
<?php endif; ?>
<a href="<?php print $node_type_url ?>"><?php print $node_type_name ?></a>
<?php if (isset($taxonomy_tree_html)) : ?>
<?php print $taxonomy_tree_html; ?>
<?php endif; ?>
<?php if (($start == FALSE || $start == NULL) && ($end == FALSE || $end == NULL)) : ?>
  </li>
  <?php elseif ($end == TRUE || $end != NULL) : ?>
    </li></ul></div>
<?php endif;
