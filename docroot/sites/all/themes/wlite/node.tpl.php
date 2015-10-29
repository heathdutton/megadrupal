
<div id="node-<?php print $node->nid; ?>" class="node<?php if ($sticky) { print ' sticky'; } ?><?php if (!$status) { print ' node-unpublished'; } if($teaser) print ' teaser'; ?>">
<?php if ($page == 0): ?>
<?php print $user_picture ?>
<?php endif; ?>
<?php if ($page == 0): ?>
  <h2 <?php print $title_attributes; ?>  class="title"><a href="<?php print $node_url ?>" title="<?php print $title ?>"><?php print render($title) ?></a></h2>
<?php endif; ?>

  <?php if ($submitted): ?>
    <div class="submitted"><?php print $submitted; ?> </div>
  <?php endif; ?>
  
     <?php if (!empty($content['field_tags'])): ?>
      <div class="terms nodeterms"><?php print render($content['field_tags']); ?></div>
    <?php endif;?>
 


 
  <div class="content clearfix">
    <?php
      // We hide the comments and links now so that we can render them later.
      hide($content['comments']);
      hide($content['links']);
      print render($content);
    
    ?>
  </div>

  <div class="clearfix">
    <?php  if ($content['links']): ?>
      <div class="links"><?php print render($content['links']) ?></div>
    <?php  endif; ?>
  </div>
  

        <?php print render($content['comments']); ?>
  


</div>

