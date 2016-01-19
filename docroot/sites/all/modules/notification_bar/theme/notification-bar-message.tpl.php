<div class="clearfix notification-bar-message notification-bar-<?php print $vars['type']; ?>">
  <div class="content-left"><?php print $vars['message']['message_text_left']; ?></div>
  <div class="content"><?php print $vars['message']['message_text']; ?></div>
  <div class="content-right">
    <?php if ($vars['message']['cta_text']): ?>
      <a class="btn" href="<?php print $vars['message']['cta_url']; ?>"><?php print $vars['message']['cta_text']; ?></a>
    <?php endif; ?>
  </div>
</div>
