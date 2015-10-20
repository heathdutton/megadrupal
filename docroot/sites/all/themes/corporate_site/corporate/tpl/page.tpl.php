<div id="page" class="page-default">
<div id="page-inner1">
<div id="page-inner2">
  <a name="Top" id="Top"></a>
  <?php if(isset($page['show_skins_menu']) && $page['show_skins_menu']):?>
    <?php print $page['show_skins_menu'];?>
  <?php endif;?>

  <!-- HEADER -->
  <div id="header-wrapper" class="wrapper">
    <div class="container">
	  <div class="row">
	    <div class="span12 clearfix">
	      <div id="header" class="clearfix">
	
	        <?php if ($logo): ?>
	          <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" id="logo">
	            <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
	          </a>
	        <?php endif; ?>
	
	        <?php if ($site_name || $site_slogan): ?>
	          <div id="name-and-slogan" class="hgroup">
	            <?php if ($site_name): ?>
	              <h1 class="site-name">
	                <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>">
	                  <?php print $site_name; ?>
	                </a>
	              </h1>
	            <?php endif; ?>
	            <?php if ($site_slogan): ?>
	              <p class="site-slogan"><?php print $site_slogan; ?></p>
	            <?php endif; ?>
	          </div>
	        <?php endif; ?>
	
	        <?php if ($header = render($page['header'])): print $header; endif; ?>
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
	    </div>
	  </div>
    </div>
  </div>
  <!-- //HEADER -->

  <?php if($main_menu = render($page['main_menu'])): ?>
    <div id="main-menu-wrapper" class="wrapper">
      <div class="container">
        <div class="row">
          <div class="span12 clearfix">
            <div class="block-menu-icon">
				<a class="home-button" href="index.php" title="Home">Home</a>
              <a class="responsive-menu-button" href="javascript:void(0);" title="Navigation Icon" style="display:none;">Menu</a>
            </div>
            <?php print $main_menu; ?>
          </div>
        </div>
      </div>
    </div>
  <?php endif;?>

  <?php if($slideshow = render($page['slideshow'])): ?>
    <div id="slideshow-wrapper" class="wrapper">
      <div class="container">
        <div class="row">
          <div class="span12 clearfix">
            <?php print $slideshow; ?>
          </div>
        </div>
      </div>
    </div>
  <?php endif;?>

  <?php if($messages || $page['help']): ?>
    <div id="system-messages-wrapper" class="wrapper">
      <div class="container">
        <div class="row">
          <div class="grid-inner span12 clearfix">
            <?php print $messages . render($page['help']); ?>
          </div>
        </div>
      </div>
    </div>
  <?php endif; ?>
  
  

  <div id="main-wrapper" class="wrapper">
    <div class="container">
	    <div class="row clearfix">
        <?php if ($title): ?>
        <div id="main-title" class="span12">
          <?php print render($title_prefix); ?>
          <h1 id="page-title"><span><?php print $title; ?></span></h1>
          <?php print render($title_suffix); ?>
        </div>
        <?php endif; ?>
	      <!-- MAIN CONTENT -->
	      <div id="main-content" class="span<?php print $regions_width['content']?>">
	        <div class="grid-inner clearfix">
	          <?php if ($tabs = render($tabs)): ?>
	            <div class="tabs"><?php print $tabs; ?></div>
	          <?php endif; ?>
	
	          <?php if ($highlighted = render($page['highlighted'])): print $highlighted; endif; ?>
	
	          <?php if ($action_links = render($action_links)): ?>
	            <ul class="action-links"><?php print $action_links; ?></ul>
	          <?php endif; ?>
	
	          <?php if ($content = render($page['content'])): print $content; endif; ?>
	        </div>
	      </div>
	      <!-- //MAIN CONTENT -->
	
	      <?php if ($regions_width['sidebar_first']): ?>
	        <div id="sidebar-first-wrapper" class="span<?php print $regions_width['sidebar_first'];?>">
	          <div class="grid-inner clearfix">
	            <?php print render($page['sidebar_first']); ?>
	          </div>
	        </div>
	      <?php endif; ?>
	
	      <?php if ($regions_width['sidebar_second']): ?>
	        <div id="sidebar-second-wrapper" class="span<?php print $regions_width['sidebar_second'];?>">
	          <div class="grid-inner clearfix">
	            <?php print $sidebar_second; ?>
	          </div>
	        </div>
	      <?php endif; ?>
	    </div>
    </div>
  </div>
  
  <?php if ($map_area = render($page['map_area'])): ?>
  <div id="map-area-wrapper" class="wrapper map-area">
    <div class="container">
      <div class="row clearfix">
        <div class="span12 clearfix">
          <?php print $map_area; ?>
        </div>
      </div>
    </div>
  </div>
  <?php endif; ?>

  <?php if($panel_first): ?>
    <!-- PANEL FIRST -->
    <div id="panel-first-wrapper" class="wrapper panel panel-first">
      <div class="container">
        <div class="row clearfix">
          <?php print $panel_first;?>
        </div>
      </div>
    </div>
    <!-- //PANEL FIRST -->
  <?php endif; ?>

  <?php if($panel_second): ?>
    <!-- PANEL SECOND -->
    <div id="panel-second-wrapper" class="wrapper panel panel-second">
      <div class="container">
        <div class="row clearfix">
          <?php print $panel_second;?>
        </div>
      </div>
    </div>
    <!-- //PANEL SECOND -->
  <?php endif; ?>

  <?php if($panel_third): ?>
    <!-- PANEL THIRD -->
    <div id="panel-third-wrapper" class="wrapper panel panel-third">
      <div class="container">
        <div class="row clearfix">
          <?php print $panel_third;?>
        </div>
      </div>
    </div>
    <!-- //PANEL THIRD -->
  <?php endif; ?>
  
  <?php if ($breadcrumb): ?>
	<!-- BREADCRUMB -->
	<div id="breadcrumb-wrapper" class="wrapper">
	  <div class="container">
        <div class="row clearfix">
			<div class="span12 clearfix">
			  <?php if ($breadcrumb):?>
				<?php print $breadcrumb; ?>
			  <?php endif; ?>
		  </div>
	  </div>
	</div>
	</div>
	<!-- //BREADCRUMB -->
  <?php endif; ?>

  <?php if($panel_forth): ?>
    <!-- PANEL FORTH -->
    <div id="panel-forth-wrapper" class="wrapper panel panel-forth">
      <div class="container">
        <div class="row clearfix">
          <?php print $panel_forth;?>
        </div>
      </div>
    </div>
    <!-- //PANEL FORTH -->
  <?php endif; ?>

  <?php if ($footer = render($page['footer'])): ?>
    <!-- FOOTER -->
    <div id="footer-wrapper" class="wrapper">
      <div class="container">
        <div class="row">
          <div class="span12 clearfix">
            <div id="footer" class="clearfix">
              <?php print $footer; ?>
              <a title="<?php print t('Top')?>" class="btn-btt" href="#Top"><?php print t('Top')?></a>
            </div>
          </div>
        </div>
      </div>
    </div>
    <!-- //FOOTER -->
  <?php endif; ?>
 </div> 
 </div> 
  <div id="credits">Corporate Site - This is a contributing Drupal Theme<br/>Design by <a href="http://www.weebpal.com/" target="_blank">WeebPal</a>.</div>    
</div>
