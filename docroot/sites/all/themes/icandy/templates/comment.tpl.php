<?php
/**
 * @file
 * The theme system, which controls the output of Drupal.
 *
 * The theme system allows for nearly all output of the Drupal system to be
 * customized by user themes.
 */

?>
<div class="comment<?php print($comment->new) ? ' comment-new' : ''; print ' '. $status; print ' '. $zebra; ?>">
  <div class="avatar-box">
            <?php
                global $base_path;
                global $theme_path;
                if ($picture) {
                    print $picture;
                }
                else {
                    print '<img width="48" height="48" src="'.$base_path.$theme_path.'/images/gravatar.png" alt="photo" />';
                    
                }
            ?>
        </div>
  
    <div class="commentmeta">
      <?php if ($submitted): ?>
        <span class="submitted"><?php // print $submitted; ?></span>
      <?php endif; ?>
      
      <p class="commentauthor"><a href="user/ <?php print $node->uid ;?>"><?php print ($node->name);  ?></a></p>
      <p><?php print format_date($comment->created, 'custom', 'F j, Y');?> at <?php print format_date($comment->created, 'custom', 'h:i');?></p>
    </div><div class="clear"></div>
  <?php if ($comment->new) : ?>   
    <span class="new"><?php print  drupal_ucfirst($new) ?></span>
  <?php endif; ?>
  <h3<?php print $title_attributes; ?>><?php print $title; ?></h3>

    <div class="content">
      <?php  print render($content) ; ?>
      <?php if ($signature): ?>
      <div class="clear-block">
        <div>—</div>
        <?php print $signature ?>
      </div>
      <?php endif; ?>
    </div>
  

  
</div>
