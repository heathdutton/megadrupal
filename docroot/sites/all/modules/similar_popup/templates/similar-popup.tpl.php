<div id="similar-popup-wrapper" class="<?php print $classes; ?>">
  <span class="close"></span>

  <?php if (!empty($title)): ?>
    <h3 class="title"><?php print $title; ?></h3>
  <?php endif; ?>

  <?php if (!empty($view)): ?>
    <div class="content"><?php print $view; ?></div>
  <?php endif; ?>

  <?php if (!empty($sound_notification)): ?>
    <?php print $sound_notification; ?>
  <?php endif; ?>
</div>
