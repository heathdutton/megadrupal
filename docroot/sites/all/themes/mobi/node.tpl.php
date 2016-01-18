<?php if ($page == 0): ?>
    <h1><a href="<?php print $node_url ?>" title="<?php print $title ?>"><?php print $title ?></a></h1>
<?php endif; ?>
  <?php print $content ?>
  <p><?php print $submitted ?></p>
<?php if ($links): ?>
  <?php print $links ?>
<?php endif; ?>
