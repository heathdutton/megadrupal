<?php 
// $Id:$
?>
<div class="comment<?php if ($comment->status == COMMENT_NOT_PUBLISHED) print ' comment-unpublished'; ?>">

  <?php if ($new) : ?>
    <span class="new"><?php print drupal_ucfirst($new) ?></span>
  <?php endif; ?>

  <?php print render($title_prefix); ?>
  <h3<?php print $title_attributes; ?>><?php print $title ?></h3>
  <?php print render($title_suffix); ?>
	  
	<div class="submitted">
    <?php
      print t('Posted on !datetime by !username',
        array('!username' => $author, '!datetime' => $created));
    ?>
  </div>
  <div class="content">
    <?php
      // We hide the comments and links now so that we can render them later.
      hide($content['links']);
      print render($content);
    ?>
  </div>
  <div class="links"><?php print render($content['links']) ?></div>

</div>
