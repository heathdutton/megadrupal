<?php
/**
 * @file
 * Block menu quick infos template.
 */
?>

<div class="">
  <?php print render($title_prefix); ?>
  <?php if ($block->subject): ?>
    <h2<?php print $title_attributes; ?>><?php print $block->subject ?></h2>
  <?php endif; ?>
  <?php print render($title_suffix); ?>


  <ul class="quick-info menu clearfix">
    <li class="expanded"><span class="menu-item-container"><?php echo t('Quick Infos') ?></span>
      <?php print $content ?>
    </li>
  </ul>
</div>
