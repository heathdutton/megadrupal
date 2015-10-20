<div<?php print $css_id ? " id=\"$css_id\"" : ''; ?> class="layout-one-column clearfix">
  <?php if (!empty($content['main'])): ?>
  <div class="column column-main">
    <?php print render($content['main']); ?>
  </div>
  <?php endif; ?>
</div>

