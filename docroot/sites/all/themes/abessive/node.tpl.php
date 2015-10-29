<?php // $Id: node.tpl.php,v 1.2 2009/02/16 16:18:13 njt1982 Exp $ ?>
<div id="node-<?php print $node->nid; ?>" class="node<?php print $sticky ? ' sticky' : ''; print !$status ? ' node-unpublished' : ''; ?> clear-block">
<?php print $picture ?>
<?php if ($page == 0): ?>
  <h2><a href="<?php print $node_url ?>" title="<?php print $title ?>"><?php print $title ?></a></h2>
<?php endif; ?>

  <div class="meta">
  <?php if ($submitted): ?>
    <span class="submitted"><?php print $submitted ?></span>
  <?php endif; ?>

  <?php if ($terms): ?>
    <div class="terms"><?php print $terms ?></div>
  <?php endif;?>
  </div>

  <div class="content">
    <?php print $content ?>
  </div>

  <?php if ($links): print $links; endif; ?>
</div>
