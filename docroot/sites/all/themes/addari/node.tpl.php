 <div class="<?php if ($page): ?>typepage<?php endif; ?> <?php if ($page == 0): ?>typeteaser<?php endif; ?>">
 
  <div class="node<?php if ($sticky) { print " sticky"; } ?><?php if (!$status) { print " node-unpublished"; } ?>">
    <?php if ($user_picture) {
      print $user_picture;
    }?>
    <?php if ($page == 0) { ?><h2 class="title"><a href="<?php print $node_url?>"><?php print $title?></a></h2><?php }; ?>
   <?php if ($submitted || $terms): ?> <div class="submitted">  <?php if ($submitted): ?><?php print $date?> by <?php print $name ?> <?php endif; ?>&nbsp; <?php if ($content['field_tags']): ?><?php print '<div class="taxonomy"> ' . render($content['field_tags']) . '</div>'; ?><?php endif; ?> </div><?php endif; ?>
    
    
    <div class="taxonomy"></div>
    <div class="content">
	  <?php
      hide($content['comments']);
      hide($content['links']);
      print render($content);
      ?>
	</div>
	
	<div class="clearfix">
    <?php  if ($content['links']): ?>
      <div class="links">&raquo; <?php print render($content['links']) ?></div>
    <?php  endif; ?>
    
        <?php print render($content['comments']); ?>

    </div>
  </div>
</div>