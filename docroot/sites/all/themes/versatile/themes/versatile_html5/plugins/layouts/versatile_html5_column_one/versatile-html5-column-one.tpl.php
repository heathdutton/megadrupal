<div<?php print $css_id ? " id=\"$css_id\"" : ''; ?> class="layout-one-column clearfix">
  <?php if (!empty($content['main'])): ?>
  <section class="column column-main clearfix">
    <?php print render($content['main']); ?>
  </section>
  <?php endif; ?>
</div>

