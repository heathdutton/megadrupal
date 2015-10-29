<div<?php print $attributes; ?>>
  <div<?php print $content_attributes; ?>>
    <?php if ($main_menu_tree): ?>
    <nav class="navigation">
      <div id="superfish">
        <?php print drupal_render($main_menu_tree); ?>
      </div>
    </nav>
    <?php endif; ?>
    <?php print $content; ?>
  </div>
</div>
