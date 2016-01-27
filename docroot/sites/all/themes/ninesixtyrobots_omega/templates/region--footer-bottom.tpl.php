<div<?php print $attributes; ?>>
  <div<?php print $content_attributes; ?>>
      <div id="footer-meta" class="clear-block"> 
        <?php if ($secondary_menu): ?>
          <div id="secondary-menu" class="navigation">
            <?php print theme('links__system_secondary_menu', array(
              'links' => $secondary_menu,
              'attributes' => array(
                'id' => 'secondary-menu-links',
                'class' => array('links', 'inline', 'clearfix'),
              ),
              'heading' => array(
                'text' => t('Secondary menu'),
                'level' => 'h2',
                'class' => array('element-invisible'),
              ),
            )); ?>
            </div> <!-- /#secondary-menu -->
          <?php endif; ?>
      </div>

      <?php if ($content): ?>
      <div id="footer-bottom-content">
        <?php print $content; ?>
      </div>
      <?php endif; ?>

  </div>
</div>