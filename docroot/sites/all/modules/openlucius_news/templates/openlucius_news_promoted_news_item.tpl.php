<?php
/**
 * @file
 * Template file for the promoted news items.
 */
?>
<a href="node/<?php print $vars['nid']; ?>" class="list-group-item promoted-item">
  <div class="content-inner">
    <div class="content-header">
      <div class="title">
        <?php print $vars['title']; ?>
      </div>
      <div class="badge">
        <span class="glyphicon glyphicon-transfer"></span>
        <?php print $vars['time_ago']; ?>
      </div>
    </div>
    <div class="content-body-wrapper">
      <?php if (!empty($vars['image'])): ?>
        <div class="content-image">
          <?php print $vars['image']; ?>
        </div>
      <?php endif; ?>
      <div class="content-text"><?php print $vars['intro']; ?></div>
    </div>
  </div>
</a>
