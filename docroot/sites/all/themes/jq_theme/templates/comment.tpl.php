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

 <div class="clear-block">
  <div id="comments_box">
    <div class="avatar-box">
            <?php
                global $base_path;
                global $theme_path;
                if ($picture) {
                    print $picture;
                }
                else {
                    print '<img width="48" height="48" src="'.$base_path.$theme_path.'/img/gravatar.png" alt="photo" />';
                }
            ?>
      </div>
    <?php if ($submitted): ?>
      <div class ="comment-details"><span class="submitted"><a href="user/ <?php print $node->uid ;?>"><?php print ($node->name);?></a> . <?php print format_date($node->created, 'custom', 'j M,Y' );?> at <?php print format_date($node->created, 'custom', 'g:ha');?></span></div>
    <?php endif; ?>
    <div class="clear"></div>
    <?php if ($comment->new) : ?>
      <span class="new"><?php print drupal_ucfirst($new) ?></span>
    <?php endif; ?>
    <h3<?php print $title_attributes; ?>><?php print $title; ?></h3>
    <div class="content">
      <?php 
      hide($content['links']);
       print render ($content); ?>
      <?php if ($signature): ?>
        <div class="clear-block">
          <div>—</div>
          <?php print $signature ?>
        </div>
      <?php endif; ?>
    </div>
  </div>
 </div>
 <?php
 $links = render($content['links']);
 ?>
  <?php if ($links): ?>
    <div class="links"><?php print $links ?></div>
  <?php endif; ?>
</div>
