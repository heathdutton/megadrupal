<?php
/**
 * @file
 * Returns the HTML for a single Drupal page.
 *
 * Complete documentation for this file is available online.
 * @see https://drupal.org/node/1728148
 */
?>
<div id="pointnet_red_sun_container">
    <div id="page">

        <header class="header" id="header" role="banner">

            <?php if ($logo): ?>
            <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home" class="header__logo" id="logo"><img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" class="header__logo-image" /></a>
            <?php endif; ?>

            <?php if ($site_name || $site_slogan): ?>
            <div class="header__name-and-slogan" id="name-and-slogan">
                <?php if ($site_name): ?>
                <h1 class="header__site-name" id="site-name">
            <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" class="header__site-link" rel="home"><span><?php print $site_name; ?></span></a>
          </h1>
                <?php endif; ?>

        <?php if ($site_slogan): ?>
          <div class="header__site-slogan" id="site-slogan"><?php print $site_slogan; ?></div>
        <?php endif; ?>
      </div>
    <?php endif; ?>

    <?php if ($secondary_menu): ?>
      <nav class="header__secondary-menu" id="secondary-menu" role="navigation">
        <?php print theme('links__system_secondary_menu', array(
          'links' => $secondary_menu,
          'attributes' => array(
            'class' => array('links', 'inline', 'clearfix'),
          ),
          'heading' => array(
            'text' => $secondary_menu_heading,
            'level' => 'h2',
            'class' => array('element-invisible'),
          ),
        )); ?>
      </nav>
    <?php endif; ?>
    <?php print render($page['header']); ?>

  </header>
  
    <div id="navigation">
      <?php print render($page['navigation']); ?>
    </div>
    <div id="pointnet_red_sun_breadcrumb">
    <?php print $breadcrumb; ?>
    </div>
  <div id="main">
  <?php print render($page['home_slider']); ?>
    <div id="content" class="column" role="main">
      <?php print render($page['highlighted']); ?>
      
      <a id="main-content"></a>
      <?php print render($title_prefix); ?>
      <?php if ($title): ?>
        <h1 class="page__title title" id="page-title"><?php print $title; ?></h1>
      <?php endif; ?>
      <?php print render($title_suffix); ?>
      <?php print $messages; ?>
      <?php print render($tabs); ?>
      <?php print render($page['help']); ?>
      <?php if ($action_links): ?>
        <ul class="action-links"><?php print render($action_links); ?></ul>
      <?php endif; ?>
      <?php print render($page['content']); ?>
    </div>



    <?php
      // Render the sidebars to see if there's anything in them.
      $sidebar_first  = render($page['sidebar_first']);
      $sidebar_second = render($page['sidebar_second']);
    ?>

    <?php if ($sidebar_first || $sidebar_second): ?>
      <aside class="sidebars">
        <?php print $sidebar_first; ?>
        <?php print $sidebar_second; ?>
      </aside>
    <?php endif; ?>

  </div>


</div><!-- /#page -->
    
 <div id="footer-wrapper"><div class="section">

    <?php if ($page['footer_firstcolumn'] || $page['footer_secondcolumn'] || $page['footer_thirdcolumn'] || $page['footer_fourthcolumn']): ?>
      <div id="footer-columns" class="clearfix">
        <?php print render($page['footer_firstcolumn']); ?>
        <?php print render($page['footer_secondcolumn']); ?>
        <?php print render($page['footer_thirdcolumn']); ?>
        <?php print render($page['footer_fourthcolumn']); ?>
      </div> <!-- /#footer-columns -->
    <?php endif; ?>

    <?php if ($page['footer']): ?>
      <footer id="footer" role="contentinfo" class="clearfix">
        <?php print render($page['footer']); ?>
        <div id="PointNet_credits"><a href="http://www.web-design.pointnet.it" title="Siti internet toscana Lucca" target="_blank"><h3>web & design</h3></a> by <a href="http://www.pointnet.it" title="Pagine web Toscana Lucca" target="_blank"><h3>PointNet</h3></a></div>
      </footer> <!-- /#footer -->
    <?php endif; ?>

  </div></div> <!-- /.section, /#footer-wrapper --> 
</div><!-- /#pointnet_red_sun_container -->
<?php print render($page['bottom ']); ?>
