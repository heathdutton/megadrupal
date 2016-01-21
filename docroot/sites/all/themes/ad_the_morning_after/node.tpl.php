<div class="node" id="node-<?php print $node->nid; ?>"><div class="node-inner">

  <?php if ($page == 0): ?>
    <h2 class="title">
      <a href="<?php print $node_url; ?>"><?php print $title; ?></a>
    </h2>
  <?php endif; ?>



  <?php if ($picture) print $picture; ?>

  <?php if ($submitted): ?>
    <div class="submitted">
      <?php print $submitted; ?>
    </div>
  <?php endif; ?>

  <div class="content">
    <?php print $content; ?>
  </div>

  <?php if ($links  || count($taxonomy)): ?>
    <div class="links">
      <?php print $links; ?><div class="taxonomy-terms"><?php print $terms; ?></div>
      <span class="clear"></span>
    </div>
  <?php endif; ?>

</div></div> <!-- /node-inner, /node -->
