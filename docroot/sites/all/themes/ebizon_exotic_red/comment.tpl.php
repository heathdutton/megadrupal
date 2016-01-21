<div class="comment<?php print ($new) ? ' comment-new' : ''; print ($comment->status == COMMENT_NOT_PUBLISHED) ? ' comment-unpublished' : ''; print ' '. $zebra; ?>">

  <div class="clearfix">
  <?php if ($submitted): ?>
    <span class="submitted"><?php //print t('!date — !username', array('!username' => theme('username', $comment), '!date' => format_date($comment->timestamp))); ?>
    <?php print $submitted; ?>
    </span>
  <?php endif; ?>

  <?php if ($new) : ?>
    <a id="new"></a>
    <span class="new"><?php print drupal_ucfirst($new) ?></span>
  <?php endif; ?>

  <?php print $picture ?>

    <h3><?php print $title ?></h3>

    <div class="content">
      <?php
        
        print render($content); ?>
    </div>

  </div>

    
</div>
