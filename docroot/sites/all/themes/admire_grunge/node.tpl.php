<div id="node-<?php print $node->nid; ?>" class="node<?php if ($sticky) { print ' sticky'; } ?><?php if (!$status) { print ' node-unpublished'; } ?>">


<div class="clearfix">
  <?php if ($submitted): ?>
    <div class="submitted"><?php print $submitted; ?></div>
  <?php endif; ?>
<?php print $user_picture ?>
<?php if ($page == 0): ?>
  <h2><a href="<?php print $node_url ?>" title="<?php print $title ?>"><?php print $title ?></a></h2>
<?php endif; ?>

</div>


  <div class="content clearfix">
      <?php
      hide($content['comments']);
      hide($content['links']);
      print render($content);
      ?>
  </div>

  <div class="clearfix">

    <?php  if ($content['links']): ?>
      <div class="links"><?php print render($content['links']) ?></div>
    <?php  endif; ?>
    <?php print render($content['comments']); ?>
  </div>
</div>

