<?php

/**
 * @file
 * Default theme implementation for Disclaimer content.
 *
 * Available variables:
 * - $hidden: Style display hidden.
 * - $content: Disclaimer content.
 * - $age_form: Age validation form if enable.
 * - $footer: Disclaimer footer.
 * - $enter_link: Enter text link or image link.
 * - $exit_link: Enter text link or image link.
 */
?>
<div <?php if ($hidden):?> style="display: none;" <?php endif; ?>>
  <div id="disclaimer">
    <?php if ($content):?>
    <div class="content">
      <?php print $content; ?>
    </div>
    <?php endif; ?>
    <?php if ($age_form):?>
    <div class="age">
      <?php print $age_form; ?>
    </div>
    <?php endif; ?>
    <div class="actions">
      <div class="enter"><?php print $enter_link; ?></div>
      <div class="exit"><?php print $exit_link; ?></div>
    </div>
    <?php if ($footer):?>
    <div class="footer">
      <?php print $footer; ?>
    </div>
    <?php endif; ?>
  </div>
</div>
