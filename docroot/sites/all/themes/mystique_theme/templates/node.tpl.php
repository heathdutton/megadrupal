<?php
global $base_url;
?>
<div id="node-<?php print $node->nid; ?>" class="page node<?php
if ($sticky) {
        print ' sticky';
    }
    ?><?php if (!$status) {
    print ' node-unpublished';
    } ?>">

  <?php if (!$page): ?>
    <h2<?php print $title_attributes; ?>>
      <a href="<?php print $node_url; ?>"><?php print $title; ?></a>
    </h2>    
  <?php endif; ?>
  <?php if ($submitted): ?>
    <div class="post-date">
      <p class="day"><?php print format_date($node->created, 'custom', 'D, j M, Y' );?></p>
    </div>
    <div class="post-info clearfix ">
      
        <p class="author alignleft"><?php print t('Posted by ') ;?><a href="<?php print $base_url; ?>/user/<?php print $node->uid ;?>"><?php print ($node->name);?></a> at <?php print format_date($node->created, 'custom', 'H:i');?></p>
        <?php if ($comment_count > 0): ?>
        <p class="comments alignright"><a class=" comments" href="<?php print $node_url ?>#comments"><?php print format_plural($node->comment_count, "@count".t('comment'), "@count ".t('comments'), array('@count' => $node->comment_count)); ?></a></p>
        <?php else: ?>
        <p class="comments alignright"><a class="no comments" href="<?php print $node_url ?>#comments"><?php print t('No comments')?></a></p>      
        <?php endif; ?>
    </div>
  <?php endif; ?>
  <div class="content clear-block">
    <?php 
       hide($content['links']);
       hide($content['field_tags']); 
       hide($content['comments']);
       print render($content);  
     ?>
  </div>
  <div class="meta">
    <?php if (!empty($content['field_tags'])): ?>
      <div class="post-tags"><?php  print render($content['field_tags']) ;?></div>
    <?php  endif;?>
  </div>
  <div class="clearfix">
    <?php if (!empty($content['links'])): ?>
      <div class="links"><?php  print render($content['links']); ?></div>
    <?php endif; ?>

    <?php  print render($content['comments']); ?>
  </div>
 </div>
