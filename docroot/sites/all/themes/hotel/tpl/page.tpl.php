<div id="page" class="page-default"> <a id="Top"></a>
  <?php if(isset($page['show_skins_menu']) && $page['show_skins_menu']):?>
  <?php print $page['show_skins_menu'];?>
  <?php endif;?>
  
  <!-- HEADER -->
  <div id="header-wrapper" class="wrapper">
    <div class="container <?php print $grid; ?>">
      <div class="grid-inner clearfix">
        <div id="header" class="clearfix">
          <?php if ($logo): ?>
          <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" id="logo"> <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" /> </a>
          <?php endif; ?>
          <?php if ($site_name || $site_slogan): ?>
          <div id="name-and-slogan" class="hgroup">
            <?php if ($site_name): ?>
            <h1 class="site-name"> <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>"> <?php print $site_name; ?> </a> </h1>
            <?php endif; ?>
            <?php if ($site_slogan): ?>
            <p class="site-slogan"><?php print $site_slogan; ?></p>
            <?php endif; ?>
          </div>
          <?php endif; ?>
          <?php if ($header = render($page['header'])):?>
          <?php print $header; ?>
          <?php endif; ?>
          <?php if ($secondary_menu): ?>
          <div id="secondary-menu" class="navigation"> <?php print theme('links__system_secondary_menu', array(
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
              )); ?> </div>
          <!-- /#secondary-menu -->
          <?php endif; ?>
        </div>
      </div>
    </div>
  </div>
  <!-- /#HEADER -->
  
  <?php if($menu_bar = render($page['menu_bar'])): ?>
  <!-- MAIN NAV -->
  <div id="menu-bar-wrapper" class="wrapper">
    <div class="container <?php print $grid; ?>">
      <div class="grid-inner clearfix">
        <a title="Navigation Icon" href="javascript:void(0);" class="responsive-menu-button">Menu</a>
        <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" id="fixedLogo" style="display:none;">
          <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
        </a>
        <?php print $menu_bar; ?>
      </div>
    </div>
  </div>
  <!-- /#MAIN NAV -->
  <?php endif; ?>
  <?php if($slideshow = render($page['slideshow'])): ?>
  <!-- SLIDESHOW -->
  <div id="slideshow-wrapper" class="wrapper">
    <div class="container <?php print $grid; ?>">
      <div class="grid-inner clearfix"> <?php print $slideshow; ?> </div>
    </div>
  </div>
  <!-- /#SLIDESHOW -->
  <?php endif; ?>
  <?php if($messages || $page['help']): ?>
  <!-- HELP & MESSAGES -->
  <div id="system-messages-wrapper" class="wrapper container-16">
    <div class="container <?php print $grid; ?>">
      <div class="grid-inner clearfix"> <?php print $messages . render($page['help']); ?> </div>
    </div>
  </div>
  <!-- /#HELP & MESSAGES -->
  <?php endif; ?>
  <?php if($panel_first): ?>
  <!-- PANEL FIRST -->
  <div id="panel-first-wrapper" class="wrapper panel panel-first">
    <div class="container <?php print $grid; ?> clearfix"> <?php print $panel_first; ?> </div>
  </div>
  <!-- /#PANEL FIRST -->
  <?php endif; ?>
  <div id="main-wrapper" class="wrapper">
    <div class="container <?php print $grid; ?> clearfix">
      <div class="<?php print nucleus_group_class("content, sidebar_first"); ?>">
        <?php if ($content_width) :?>
        <!-- MAIN CONTENT -->
        <div id="main-content" class="<?php print $content_width; ?> section " style="float: right;">
          <div class="grid-inner clearfix">
            <?php if ($tabs = render($tabs)): ?>
            <div class="tabs"><?php print $tabs; ?></div>
            <?php endif; ?>
            <?php if ($highlighted = render($page['highlighted'])):?>
            <?php print $highlighted; ?>
            <?php endif; ?>
            <?php print render($title_prefix); ?>
            <?php if ($title): ?>
            <h1 id="page-title"><?php print $title; ?></h1>
            <?php endif; ?>
            <?php print render($title_suffix); ?>
            <?php if ($action_links = render($action_links)): ?>
            <ul class="action-links">
              <?php print $action_links; ?>
            </ul>
            <?php endif; ?>
            <?php if ($content = render($page['content'])):?>
            <?php print $content; ?>
            <?php endif; ?>
            
            <!--?php print $feed_icons; ?--> 
          </div>
        </div>
        <!-- /#MAIN CONTENT -->
        <?php endif; ?>
        <?php if (($sidebar_first = render($page['sidebar_first'])) && $sidebar_first_width) : ?>
        <!-- SIDEBAR FIRST -->
        <div id="sidebar-first-wrapper" class="sidebar tb-main-box <?php print $sidebar_first_width; ?> grid-last">
          <div class="grid-inner clearfix"> <?php print $sidebar_first; ?> </div>
        </div>
        <!-- /#SIDEBAR FIRST -->
        <?php endif; ?>
      </div>
      <?php if (($sidebar_second = render($page['sidebar_second'])) && $sidebar_second_width) : ?>
      <!-- SIDEBAR SECOND -->
      <div id="sidebar-second-wrapper" class="sidebar tb-main-box <?php print $sidebar_second_width; ?> grid-last">
        <div class="grid-inner clearfix"> <?php print $sidebar_second; ?> </div>
      </div>
      <!-- /#SIDEBAR SECOND -->
      <?php endif; ?>
    </div>
  </div>
  <?php if($panel_second): ?>
  <!-- PANEL SECOND -->
  <div id="panel-second-wrapper" class="wrapper panel panel-second">
    <div class="container <?php print $grid; ?> clearfix"> <?php print $panel_second; ?> </div>
  </div>
  <!-- /#PANEL SECOND -->
  <?php endif; ?>
  <?php if ($footer = render($page['footer'])): ?>
  <!-- FOOTER -->
  <div id="footer-wrapper" class="wrapper">
    <div class="container <?php print $grid; ?>">
      <div class="grid-inner clearfix">
        <?php if($breadcrumb): ?>
        <div id="breadcrumb-wrapper" class="wrapper clearfix"> <?php print $breadcrumb; ?> </div>
        <?php endif; ?>
        <div id="footer" class="clearfix"> <?php print $footer; ?>          
        </div>
      </div>
    </div>
  </div>
  <!-- /#FOOTER -->
  <?php endif; ?>
  <?php if ($footer_bottom = render($page['footer_bottom'])): ?>
  <!-- FOOTER -->
  <footer id="footer-bottom-wrapper" class="wrapper">
    <div class="container <?php print $grid; ?>">
      <div class="grid-inner clearfix">
        <div id="footer-bottom" class="clearfix"> <?php print $footer_bottom; ?> </div>
      </div>
    </div>
  </footer>
  <!-- /#FOOTER -->  
  <?php endif; ?>  
  <?php if($back_to_top_display): ?>
          <a title="<?php print t('Back to Top'); ?>" class="btn-btt" href="#Top">▲ <?php print t('Top'); ?></a>
          <?php endif; ?>
  <div id="credits">Hotel - This is a contributing Drupal Theme<br/>Design by <a href="http://www.weebpal.com/" target="_blank">WeebPal</a>.</div>          
</div>
