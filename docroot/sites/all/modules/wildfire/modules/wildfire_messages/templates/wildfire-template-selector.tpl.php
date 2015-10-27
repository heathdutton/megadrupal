<?php
/**
 * @file
 *  Template for the template selector on the edit page of a broadcast.
 */
?>
<div class="description">
  <?php print t('Click on a template below to select it.'); ?>
</div>
<div id="wildfire-template-selector">
<ul>
  <?php if (count($templates)): ?>
    <?php foreach ($templates as $name => $template): ?>
      <?php if ($template['status'] == 1) : ?>
      <li id="wildfire-template-<?php print $name; ?>" class="unselected">
        <a href="javascript:void(0);">
          <?php print $template['image']; ?>
          <span class="wildfire-template-title"><?php print $template['title']; ?></span>
          <span class="wildfire-template-description description"><?php print $template['description']; ?></span>
        </a>
      </li>
      <?php elseif ($name == $current_template): ?>
      <li id="wildfire-template-<?php print $name; ?>" class="unselected disabled">
        <?php print $template['image']; ?>
        <span class="wildfire-template-title"><?php print $template['title']; ?> <?php print t('(disabled)'); ?></span>
        <span class="wildfire-template-description description"><?php print $template['description']; ?></span>
      </li>
      <?php endif; ?>
    <?php endforeach; ?>
  <?php else: ?>
    <li class="nonefound">
      <?php print t('There are no templates.'); ?>
    </li>
  <?php endif; ?>
</ul>
</div>
