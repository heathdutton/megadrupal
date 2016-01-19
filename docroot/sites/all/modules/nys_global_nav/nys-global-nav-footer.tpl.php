<?php

/**
 * @file
 * New York State Global Navigation Header iFrame embed code.
 *
 * See https://github.com/ny/global-navigation for documentation.
 *
 * Variables:
 * - none.
 *
 * @ingroup themeable
 */
?>
<div class="nys-global-footer">
  <div class="footer-container nys-global-footer-cols-<?php print _nys_global_nav_menu_count_top(variable_get('nys_global_nav_footer_menu', '')); ?>">
    <h3><?php print l(filter_xss(variable_get('nys_global_nav_agency_name', ''), array('br')), '<front>', array('html' => TRUE)); ?></h3>
    <?php print theme('nys_global_nav_footer_menu', array(
        'menu_name' => $footer_menu_name,
        'id' => 'nys-global-nav-footer',
        )
      );
    ?>
    <?php
      // $social_media_urls = _nys_global_nav_social_media_urls();
      if (!empty($social_media_urls)) {
        print theme('nys_global_nav_footer_social_media', array(
            'social_media_urls' => $social_media_urls,
          )
        );
      }
    ?>
    <?php print ''; ?>  </div>
</div>
