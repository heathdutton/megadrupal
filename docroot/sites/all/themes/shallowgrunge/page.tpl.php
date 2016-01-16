<?php
// $Id: 
?>

  <div id="page">
    <div id="header"><div id="header-inner"><div id="header-tear">
 
    <div id="header-region" class="clearfix">
      <?php if (!empty($page['header'])): ?>
        <?php print render($page['header']); ?>
      <?php endif; ?>
    </div>
    
    <div id="<?php if (!empty($page['search'])) { print "logo-title"; } else { print "logo-title-full"; } ?>">

        <?php if (!empty($logo)): ?>
          <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home" id="logo">
            <img src="<?php print $logo; ?>" alt="" />
          </a>
        <?php endif; ?>
        
        <?php if (!empty($site_name) || !empty($site_slogan)) : ?>
          <div id="name-slogan">
            <?php if (!empty($site_name)): ?>
              <div id="site-name">
                <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><?php print $site_name; ?></a>
              </div>
            <?php endif; ?>

            <?php if (!empty($site_slogan)): ?>
              <div id="site-slogan">
                <?php print $site_slogan; ?>
              </div>
            <?php endif; ?>
          </div> <!-- /name-and-slogan -->
        <?php endif; ?>  
      </div> <!-- /logo-title -->
      
      <?php if(!empty($page['search'])): ?>
        <div id="search-box">
          <?php print render($page['search']); ?>
        </div>
      <?php endif; ?>

    </div></div></div> <!-- /header -->

    <div id="container" class="clearfix">

      <div id="navigation" class="menu <?php if (!empty($main_menu)) { print "withprimary"; } if (!empty($secondary_menu)) { print " withsecondary"; } ?>"><div id="navigation-inner"><div id="navigation-tear">
        <?php if (!empty($main_menu)): ?>
          <div id="primary" class="clearfix">
            <?php print theme('links__system_main_menu', array('links' => $main_menu, 'attributes' => array('id' => 'main-menu', 'class' => array('links', 'inline', 'clearfix')), 'heading' => t('Main menu'))); ?>
          </div>
        <?php endif; ?>

      </div></div></div> <!-- /navigation -->

      <div id="main">
        <div id="<?php print (!empty($page['sidebar_first']) ? 'main-content' : 'main-content-full'); ?>" class="column">
        <?php if (!empty($breadcrumb)): ?><div id="breadcrumb"><?php print $breadcrumb; ?></div><?php endif; ?>
        <?php if (!empty($page['highlighted'])): ?><div id="mission"><?php print $page['highlighted']; ?></div><?php endif; ?>

        <div id="content">
          <?php if (!empty($tabs)): ?><div class="tabs"><?php print render($tabs); ?></div><?php endif; ?>
          <?php if (!empty($title)): ?><h1 class="title" id="page-title"><?php print $title; ?></h1><?php endif; ?>
          <?php if (!empty($messages)): print $messages; endif; ?>
          <?php if (!empty($page['help'])): print render($page['help']); endif; ?>
          <div id="content-content" class="clearfix">
            <?php print render($page['content']); ?>
          </div> <!-- /content-content -->
          <?php print $feed_icons; ?>
        </div> <!-- /content -->

      </div> <!--main -->

      <?php if (!empty($page['sidebar_first'])): ?>
        <div id="sidebar" class="column"><div id="sidebar-inner">
          <?php print render($page['sidebar_first']); ?>
        </div></div> <!-- /sidebar -->
      <?php endif; ?>

    </div></div> <!-- /main /container -->
    
  </div> <!-- /page -->
  
  <div id="footer"><div id="footer-inner">
    <?php if (!empty($page['footer'])): ?>
      <div id="footer-region">
        <?php print render($page['footer']); ?>
      </div>
    <?php endif; ?>
    <div id="attribution"><a href="http://doncoryon.com">Drupal theme created by DonCoryon.</div>
  </div></div> <!-- /footer -->
