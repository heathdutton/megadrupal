<?php
/**
 * @file
 * Returns the HTML for a single Drupal page.
 *
 * Complete documentation for this file is available online.
 * @see https://drupal.org/node/1728148
 */
?>
<div id="page">
  <?php if ($page['header_upper']): ?>
    <div class="header-upper">
	  <?php print render($page['header_upper']); ?>
	</div>
  <?php endif; ?>
  <div class="page-inner">
    <header class="header" id="header" role="banner">
      <div class="header-top">
        <div class="header-left">
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
	    </div>
	    <div class="header-last">
          <div id="search-box">
	        <?php print $search_box;  ?>
	      </div>
          <?php print render($page['header_last']); ?>
        </div>
      </div>
      <?php print render($page['header']); ?>
	  <a href="#" class="nav-toggles">Main Navigation</a>
	  <div id="navigation">
        <?php if ($main_menu) {
			  $pid = variable_get('menu_main_links_source', 'main-menu');
			  $tree = menu_tree($pid);
			  $tree = str_replace(' class="menu"', '', $tree);
			  $main_menu = drupal_render($tree);
			  }else{
			  $main_menu = FALSE;
			  }
	    ?>
        <?php if ($main_menu): ?>
          <nav id="main-menu" role="navigation" tabindex="-1">
            <?php print $main_menu; ?>
          </nav>
        <?php endif; ?>
        <?php print render($page['navigation']); ?>
      </div>
    </header>
	<?php  if(drupal_is_front_page()) { ?>
      <div id="header-slide" class="region region-header">
	    <div class="slides">
	      <img id="0" class="slider-0 active" src="<?php print base_path(); ?>sites/all/themes/fabist/slider/slide1.jpg" />
	      <img id="1" class="slider-1" src="<?php print base_path(); ?>sites/all/themes/fabist/slider/slide2.jpg" />
	      <img id="2" class="slider-2" src="<?php print base_path(); ?>sites/all/themes/fabist/slider/slide3.jpg" />
	      <img id="3" class="slider-3" src="<?php print base_path(); ?>sites/all/themes/fabist/slider/slide4.jpg" />
	    </div>
	    <div class="dots">
	      <span id="0" class="slider-0 active"></span>
	      <span id="1" class="slider-1"></span>
	      <span id="2" class="slider-2"></span>
	      <span id="3" class="slider-3"></span>
	    </div>
	  </div>
    <?php }?>
    <?php print $breadcrumb; ?>
	<div class="content-top">
     <?php print render($page['content_top_1']); ?>
	 <?php print render($page['content_top_2']); ?>
	 <?php print render($page['content_top_3']); ?>
	</div>
	<div class="content-top-upper">
     <?php print render($page['content_top_upper_1']); ?>
	 <?php print render($page['content_top_upper_2']); ?>
	</div>
	<div class="content-top-lower">
     <?php print render($page['content_top_lower_1']); ?>
	 <?php print render($page['content_top_lower_2']); ?>
	</div>
	<div class="content-left-right">
     <?php print render($page['content_left']); ?>
	 <?php print render($page['content_right']); ?>
	</div>
	<?php print render($page['sidebar_first']); ?>
    <div id="main">
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
        <?php print $feed_icons; ?>
      </div>
    </div>
 	<?php print render($page['sidebar_second']); ?>
    <div class="content-bottom">
      <?php print render($page['content_bottom_1']); ?>
	  <?php print render($page['content_bottom_2']); ?>
	  <?php print render($page['content_bottom_3']); ?>
    </div>
    <div class="content-bottom-upper">
      <?php print render($page['content_bottom_upper_1']); ?>
	  <?php print render($page['content_bottom_upper_2']); ?>
    </div>
    <div class="content-bottom-lower">
      <?php print render($page['content_bottom_lower_1']); ?>
	  <?php print render($page['content_bottom_lower_2']); ?>
    </div>
	<div class="content-footer">
     <?php print render($page['content_footer_1']); ?>
	 <?php print render($page['content_footer_2']); ?>
	</div>
  	<footer class="l-footer" role="contentinfo">
    <?php print render($page['footer_1']); ?>
	<?php print render($page['footer_2']); ?>
	<?php print render($page['footer_3']); ?>
	<?php print render($page['footer_4']); ?>
  	</footer>
  </div>
  <?php print render($page['bottom']); ?>	
  <div id="toTop" style="display: none;"><a href="#top" class="top"></a></div>
</div>