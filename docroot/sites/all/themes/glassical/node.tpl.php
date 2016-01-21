<?php
// $Id: node.tpl.php $
?>
<div id="node-<?php print $node->nid; ?>" class="node<?php if ($sticky) { print ' sticky'; } ?><?php if (!$status) { print ' node-unpublished'; } ?> post">
  <?php print $picture ?>
  <?php if ($page == 0): ?>
  <?php endif; ?>
  <?php if ($submitted): ?>
    <div class="meta submitted details"><?php print $submitted; ?></div>
  <?php endif; ?>
  <h2><a href="<?php print $node_url ?>" title="<?php print $title ?>"><?php print $title ?></a></h2>
  <div class="content clear-block">
    <?php print $content ?>
  </div>
  <div class="clear-block">
    <div class="meta">
    <?php if ($taxonomy): ?>
      <div class="terms"><?php print $terms ?></div>
    <?php endif;?>
    </div>
    <?php if ($links): ?>
      <div class="links controls"><?php print $links; ?></div>
    <?php endif; ?>
  </div>
</div>
