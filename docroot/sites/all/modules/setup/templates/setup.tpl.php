<?php
/**
 * @file
 * Setup's module implementation to display a single Drupal page.
 */
?>
<div id='page'>
  <div id='branding'>
    <?php if (isset($logo)) : ?>
      <?php echo $logo ?>
    <?php endif; ?>

    <?php if (isset($title)) : ?>
      <h1 class='page-title'><?php echo $title ?></h1>
    <?php endif; ?>
  </div>

  <div id='content'>
    <?php if (isset($messages)) { echo $messages; } ?>
    <?php echo render($page['content']) ?>
  </div>
</div>
