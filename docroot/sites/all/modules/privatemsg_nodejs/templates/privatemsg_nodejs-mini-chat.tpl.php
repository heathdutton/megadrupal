<?php

/**
 * @file
 * Mini chat template file.
 */
?>
<div class="pmnj-minichat wrapper-mini-chat-<?php print $thread['thread_id']; ?>">
  <div class="pmnj-minichat-inner">
    <div class="name"> <?php print $recipient; ?></div>
    <div class="pmnj-mes">
      <div class="pmnj-mes-inner">
        <?php print drupal_render($messages); ?>
      </div>
    </div>
  </div>
    <div class="form">
      <?php print drupal_render($form); ?>
    </div>
</div>
