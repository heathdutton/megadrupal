<?php // $Id$  ?>

<div id="node-<?php print $node->nid ?>" class="node node-<?php print $node->type ?>">

<?php print render($title_prefix); ?>
<?php if (!$page): ?>
  <h2<?php print $title_attributes; ?>><a href="<?php print $node_url ?>" title="<?php print $title ?>">
    <?php print $title ?></a></h2>
<?php endif; ?>
<?php print render($title_suffix); ?>

<?php if ($submitted || $terms): ?>
  <div class="meta">
    <?php if ($display_submitted): ?>
      <div class="submitted"><?php print $submitted ?></div>
    <?php endif; ?>
    <?php if (!empty($terms)): ?>
      <div class="terms"><?php print $terms ?></div>
    <?php endif;?>
  </div>
<?php endif; ?>
  <div class="content clear-block <?php print $content_attributes; ?>">
    <?php print $user_picture; ?>
    <?php
      // We hide the comments and links now so that we can render them later.
      hide($content['comments']);
      hide($content['links']);
      print render($content);
    ?>
  </div>

<?php if (!empty($content['links'])): ?>
  <div class="node-links"><?php print render($content['links']); ?></div>
<?php endif; ?>
<?php print render($content['comments']); ?>

</div>
