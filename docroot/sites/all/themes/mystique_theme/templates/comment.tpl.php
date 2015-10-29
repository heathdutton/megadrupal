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
    <div class="comment-head comment odd alt thread-odd thread-alt depth-1 withAvatars reader name-Mr WordPress">
        <div class="avatar-box">
            <?php
                global $base_path;
                global $theme_path;
                if ($picture) {
                    print $picture;
                }
                else {
                    print '<img width="48" height="48" src="'.$base_path.$theme_path.'/images/default.png" alt="photo" />';
                }
            ?>
        </div>
        <div class="author">
            <?php if ($submitted): ?>
            <span class="submitted"><?php print $submitted; ?></span>
            <?php endif; ?>
            <?php  if ($comment->new) : ?>
            <span class="new"><?php  print drupal_ucfirst($new) ?></span>
            <?php endif; ?>
        </div>
        
    </div>
    <div class="comment-body clearfix">
        <?php print render($title_prefix); ?>
        <h3<?php print $title_attributes; ?>><?php print $title; ?></h3>
        <?php print render($title_suffix); ?>
        <div class="content">
            <?php print render($content); ?>
            <?php if ($signature): ?>
            <div class="clear-block">
                <div>—</div>
                <?php print $signature ?>
            </div>
            <?php endif; ?>
        </div>
    </div>
  </div>
</div>
