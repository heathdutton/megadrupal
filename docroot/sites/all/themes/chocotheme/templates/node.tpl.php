<?php
?>
<div id="node-<?php print $node->nid; ?>" class="node<?php if ($sticky) { print ' sticky'; } ?><?php if (!$status) { print ' node-unpublished'; } ?>">


<div class ="post">
  <?php print render($title_prefix); ?>
      <?php if (!$page): ?>
        <div class="title"><h2<?php print $title_attributes; ?>><a href="<?php print $node_url; ?>"><?php print $title; ?></a></h2></div>
      <?php endif; ?>
        <div class="date">
          <div class="bg">
            <span class="day"><?php print format_date($node->created, 'custom', 'd');?></span>
            <span><?php print format_date($node->created, 'custom', 'M');?></span>
          </div>
         </div>
      
      <?php print render($title_suffix); ?>
       <div class="meta">
         <div class ="bg">
           <span class="comments-num">
             <?php if ($comment == '2') :?>
               <span>
                 <?php if ($comment_count > 0): ?>
                   <a class=" comments" href="<?php print $node_url ?>#comments"><?php print format_plural($node->comment_count, "@count ".t('comment'), "@count ".t('comments'), array('@count' => $node->comment_count)); ?></a>
                 <?php else: ?>
                   <a class="no comments" href="<?php print $node_url ?>#comments"><?php print t('No comments')?></a>      
                 <?php endif; ?>
               </span>
                 <?php elseif($comment == '1'): ?>
                   <span><?php print t('Comments off') ;?></span>
                 <?php endif; ?>
          </span>
          <?php if ($submitted): ?>
            <p><?php print t('Posted by ') ;?><a href="user/<?php print $node->uid ;?>"><?php print ($node->name) ;?></a> at <?php print format_date($node->created, 'custom', 'H:i');?></p>
         <?php endif; ?>
         </div>
       <div class="bot"> </div>
     </div>
     <div class="entry content clear-block">
       <?php 
       hide($content['links']);
       hide($content['field_tags']); 
       hide($content['comments']);
       print render($content); ?>
     </div>
     <?//php if ($field_tags): ?>
      <div class="post-tags"><?php  print render($content['field_tags']) ;?></div>
    <?//php  endif;?>
    <div class="clearfix">
    <?php if (!empty($content['links'])): ?>
      <div class="links"><?php   print render($content['links']); ?></div>
    <?php endif; ?>

    <?php  print render($content['comments']); ?>
  </div>
  </div>
</div>
