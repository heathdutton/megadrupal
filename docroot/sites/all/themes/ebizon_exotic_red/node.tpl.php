<?php phptemplate_comment_wrapper(NULL, $node->type); ?>

<div id="node-<?php print $node->nid; ?>" class="node<?php if ($sticky) { print ' sticky'; } ?><?php if (!$status) { print ' node-unpublished'; } ?>">

<?php print $user_picture; ?>

<?php if ($page == 0): ?>
  <h2><a href="<?php print $node_url ?>" title="<?php print $title ?>"><?php print $title ?></a></h2>
<?php endif; ?>

  <?php if ($submitted): ?>
    <span class="submitted"><?php //print t('!date — !username', array('!username' => theme('username', $node), '!date' => format_date($node->created))); ?>
    <?php print $submitted ?>
    </span>
  <?php endif; ?>

  <div class="content">
    <?php print render($content); ?>
  </div>

  <div class="clearfix clear">
    
  </div>
<?php print render($content['comments']); ?>
</div>