<?php
?>

<div class="<?php print $classes . ' ' . $zebra; ?> <?php if ($comment->status == COMMENT_NOT_PUBLISHED) print ' comment-unpublished'; ?>" <?php print $attributes; ?>>
<?php if ($picture) { print $picture; } ?>
    <span class="submitted"><?php print $created; ?> &mdash; <?php print $author; ?></span>&nbsp;<span class="this-link 	 <?php if ($new != '') { ?>new <?php } ?>"><?php echo "<a href=\"#comment-$comment->cid\">#</a>"; ?></span>
<?php if ($new) : ?>
    <span class="new"><?php print drupal_ucfirst($new) ?></span>
  <?php endif; ?>

 <?/*php print render($title_prefix); ?>
    <h3<?php print $title_attributes; ?>><?php print $title ?></h3>
    <?php print render($title_suffix); */?>


<div class="content"<?php print $content_attributes; ?>>
      <?php hide($content['links']); print render($content); ?>
      <?php if ($signature): ?>
      <div class="clearfix">
        <div>&mdash;</div>
        <?php print $signature ?>
      </div>
      <?php endif; ?>
    </div>





<div class="links-comment "><?php print render($content['links']) ?></div> &nbsp;




</div>
