<?php
?>
<div class="comment comment<?php print ($comment->new) ? ' comment-new' : ''; print ' '. $status; print ' '. $zebra; ?>">
    <div class="comment-body">
        <?php
        global $base_path;
        global $theme_path;
        if($picture) {
            print $picture;
        }
        else {
            print '<img width="70" height="70" src="'.$base_path.$theme_path.'/images/avatar.gif" alt="">';
        }
        ?>
        <?php if ($submitted): ?>
        <?php // print $submitted; ?>
          <p class="author"><a href="user/<?php print $node->uid ;?>"><?php print ($node->name);?></a></p>
          <p class="comment-meta"><?php print format_date($node->created, 'custom', 'd M ,Y');?> at <?php print format_date($node->created,'custom','H:i');?></p>
        <?php endif; ?>
        <?php if ($comment->new) : ?>
        <span class="new"><?php print drupal_ucfirst($new) ?></span>
        <?php endif; ?>
        <h3<?php print $title_attributes; ?>><?php print $title; ?></h3>
        <div class="comment-content">
            <?php  print render($content);?>
            <?php if ($signature): ?>
            <div class="clear-block">
                <div>—</div>
                <?php print $signature ?>
            </div>
            <?php endif; ?>
        </div>      
    </div>
</div>
