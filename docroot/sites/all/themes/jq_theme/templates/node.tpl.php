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
  <div class="headline clearfix" >
    <div class="date">
      <?php print format_date($node->created, 'custom', 'M/y'); ?>
      <p class="date-month"><?php print format_date($node->created, 'custom', 'd'); ?></p>
    </div>
    <h2<?php print $title_attributes; ?>>
      <a href="<?php print $node_url; ?>"><?php  print $title; ?></a>
    </h2>
    <div class="preview"><a class="view-excerpt" href="#"></a></div>
    <?php // print_r($comment) ;?>
    <p class="post_info">
      <?php if ($comment == '2') :?>
        <span>
          <?php if ($comment_count > 0): ?>
           <a class=" comments" href="<?php print $node_url ?>#comments"><?php print format_plural($node->comment_count, "@count".t('comment'), "@count ".t('comments'), array('@count' => $node->comment_count)); ?></a>
          <?php else: ?>
          <a class="no comments" href="<?php print $node_url ?>#comments"><?php print t('No comments')?></a>      
          <?php endif; ?>
        </span>
       <?php elseif($comment == '1'): ?>
        <span><?php print t('Comments off') ;?></span>
       <?php endif; ?> 
       <?php print t('. Posted by ') ;?><?php print ("<i>".$node->name."</i>") ;?> at <?php print format_date($node->created, 'custom', 'H:i');?></p>
      
    <p class="button"></p>
    
    <div class="clearfix"></div>
  </div>
  <div class="excerpt content clear-block">
    <?php
      // We hide the comments and links now so that we can render them later.
      hide($content['comments']);
      hide($content['links']);
      hide($content['field_tags']);
      print render($content);
      ?>
      <?php // dsm($node); ?>
      <div class="post-tags"><?php  print render($content['field_tags']) ;?></div>
  </div>
  <div class="borderbottom clear-block">
    
    <?php
    // Remove the "Add new comment" link on the teaser page or if the comment
      // form is being displayed on the same page.
      if (!empty($content['comments']['comment_form'])) {
        unset($content['links']['comment']['#links']['comment-add']);
      }
      // Only display the wrapper div if there are links.
      $links = render($content['links']);
      if ($links):
     ?>
     <div class="link-wrapper">
       <?php  print render($links); ?>
     </div>
     <?php endif; ?>
  </div>
   <?php endif; ?>
   <?php if ($page): ?>
     <div class="headline clearfix" >
       <?php  if($article_date == 1): ?>
         <div class="date">
           <?php print format_date($node->created, 'custom', 'M/y');?>
           <p class="date-month"><?php print format_date($node->created, 'custom', 'd');?></p>
         </div>
       <?php  endif; ?>
       <p class="button"></p>
       <div class="preview"><a class="view-excerpt" href="#"></a></div>
       <div class="clearfix"></div>
     </div>
    <div class=" excerpt content clear-block"<?php print $content_attributes; ?>>
      <?php
        // We hide the comments and links now so that we can render them later.
        hide($content['comments']);
        hide($content['links']);
        hide($content['field_tags']);
        print render($content);
      ?>
      <div class="post-tags"><?php  print render($content['field_tags']) ;?></div>
    </div>
    <div class="clearfix">
      <?php if (!empty($content['links'])): ?>
        <div class="links"><?php print render($content['links']); ?></div>
      <?php endif; ?>
      <?php print render($content['comments']); ?>
    </div>
   <?php endif; ?>
 </div>
  
