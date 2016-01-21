<?php
/**
 * @file
 * Theming for wrapper wrapper of all 2gis widget pages.
 */
?>
<div class="amoCRM-widget">
  <div class="panel-amo-widget-group">
    <div class="panel-amo-widget">
        <div class="panel-amo-widget-close"></div>
        <div class="widget-2gis">
          <div class="widget-2gis-copyright">
            <?php if (!empty($logo)): ?>
              <?php print $logo; ?>
            <?php endif; ?>
            <p><?php print t('Data is provided by 2GIS') ?></p>
          </div>
          <?php if (!empty($content)): ?>
            <?php print $content; ?>
          <?php endif; ?>
        </div>
    </div>
  </div>
</div>
