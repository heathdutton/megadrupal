<?php

/**
 * @file
 * New York State Global Navigation Header.
 *
 * See https://github.com/ny/global-navigation for documentation.
 *
 * Variables:
 * - none.
 *
 * @ingroup themeable
 */
?>
<div id="nys-global-header" class="nys-global-header <?php print check_plain(variable_get('nys_global_nav_header_format', '')); ?>">
  <!--  Using github.com/ny/global-navigation library version <?php print $library_information['version']; ?> -->
  <div class="nav-toggle">
    <a href="#" role="button" id="nys-menu-control">Navigation menu</a>
  </div>
  <h1><a href="/">
  <?php print filter_xss(variable_get('nys_global_nav_agency_name', ''), array('br')); ?>
  </a></h1>
  <?php print theme('nys_global_nav_header_menu', array(
        'menu_name' => $header_menu_name,
        'id' => 'nys-global-nav',
      )
    );
  ?>
</div>
