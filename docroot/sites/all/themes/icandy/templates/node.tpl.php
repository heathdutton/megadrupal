<?php
/**
 * @file
 * The theme system, which controls the output of Drupal.
 *
 * The theme system allows for nearly all output of the Drupal system to be
 * customized by user themes.
 */

?>

 <div id="node-<?php print $node->nid; ?>" class="page node<?php
if ($sticky) {
        print ' sticky';
    }
    ?><?php if (!$status) {
    print ' node-unpublished';
    } ?>">

<?php if (!$page): ?>
    <h2<?php print $title_attributes; ?> class="post-title comment_title" >
      <a href="<?php print $node_url; ?>"><?php print $title; ?></a>
    </h2>
    
    <?php if ($submitted): ?>
      
      <div class = "post-meta">
        <div class = "meta-left">
          <p class="postdate"><?php print format_date($node->created, 'custom', 'M jS Y' );?></p>
          <p class="postauthor"><?php print t('By: ') ;?><?php print ($node->name);?></p>
        </div>
        <div class = "meta-right">
          <?php if ($comment_count > 0): ?>
            <p class="postcomments"><a class=" comments" href="<?php print $node_url ?>#comments"><?php print format_plural($node->comment_count, "@count ".t('comment'), "@count ".t('comments'), array('@count' => $node->comment_count)); ?></a></p>
          <?php else: ?>
            <p class="postcomments"><a class="no comments" href="<?php print $node_url ?>#comments"><?php print t('No comments')?></a></p>      
          <?php endif; ?>
        </div>
      </div>
    <?php endif; ?>    
<?php endif; ?>
  <div class="clear"></div>
  <div class="content clear-block">
    <?php 
       hide($content['links']);
       hide($content['field_tags']); 
       hide($content['comments']);
       print render($content);  
     ?>
  </div>
  
    <?php if ($field_tags): ?>
      <div class="post-tags"><?php  print render($content['field_tags']) ;?></div>
    <?php  endif;?>
 
  <div class="clearfix">
    <?php if (!empty($content['links'])): ?>
      <div class="links"><?php   print render($content['links']); ?></div>
    <?php endif; ?>

    <?php  print render($content['comments']); ?>
  </div>

 </div>
  
