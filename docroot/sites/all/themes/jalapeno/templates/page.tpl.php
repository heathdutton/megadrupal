<div id="leaderboard" class="leaderboard-container">
<!-- Leaderboard Area -->
  <div class="container">
<?php print render($page['leaderboard']); ?>
  </div>
<!-- End Leaderboard Area --> 
</div>
<div id="header-container" class="header-container" >
<!-- Header Area -->
<div class="container">
  <div class="full-width last" id="header">
    <div class="span-8 append-8 last" id="branding">
    <?php if ($logo): ?>
      <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home" id="logo">
        <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
      </a>
    <?php endif; ?>
    <?php if ($site_name || $site_slogan): ?>
      <?php if ($site_name): ?>
       <h1>
         <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home">
            <?php print $site_name; ?>
         </a>
       </h1>
      <?php endif; ?>
      <?php if ($site_slogan): ?>
        <h2><?php print $site_slogan; ?></h2>
       <?php endif; ?>
    <?php endif; ?>
    </div>
  </div>
</div>
<!-- End Header Area -->  
</div>
<div class="nav-container">
<!-- Nav Area --> 
<div class="container">   
  <div class="clear full-width last" id="nav">
    <div class="full-width last" id="primary-nav">
    <?php print render($page['menu_bar']); ?>
    </div>
  </div>
</div>  
<!-- End Nav Area -->  
</div>
    <?php if ($page['featured_cta']): ?>
<div class="feature-container">
<div class="feature-container-inner">
<!-- Feature Area -->
<div class="container">
  <div class="clear full-width last" id="feature-area">
    <div class="span-10" id="featured-content">
      <?php print render($page['featured_content']); ?>
    </div>
    <div class="span-6 last" id="feature-call-to-action">
      <?php print render($page['featured_cta']); ?>
    </div>
  </div>
</div>
<!-- End Feature Area -->
</div>
</div>

    <?php elseif ($page['featured_content']): ?>
<div class="feature-container">
<!-- Feature Area -->
<div class="container">
  <div class="clear full-width last" id="feature-area">
    <div class="full-width last" id="featured-content">
      <?php print render($page['featured_content']); ?>
    </div>
  </div>
</div>
<!-- End Feature Area -->  
</div>
    <?php endif; ?>
    
<div class="columns-container">
<div class="columns-container-inner">
<!-- Columns -->
<div class="container">
  <div class="clear full-width last" id="columns">
  	<div class="clear main-content">
      <?php print render($title_prefix); ?>
      <?php if ($title): ?>
        <h1 id="page-title"><span></span><?php print $title; ?></h1>
      <?php endif; ?>
      <?php print render($title_suffix); ?>

 			<?php print $messages; ?>
      <?php if ($tabs = render($tabs)): ?>
        <div class="tabs"><?php print $tabs; ?></div>
      <?php endif; ?>
      <?php print render($page['help']); ?>
      <?php if ($action_links): ?>
        <ul class="action-links"><?php print render($action_links); ?></ul>
      <?php endif; ?>

      <?php print render($page['content']); ?>
      <?php print $feed_icons; ?>
    </div>
    
    <?php if ($page['sidebar_first']): ?>  
    <div class="sidebar-first">
      <?php print render($page['sidebar_first']); ?>
    </div>
    <?php endif; ?>
    
    <?php if ($page['sidebar_second']): ?> 
    <div class="sidebar-second last">
    	<?php print render($page['sidebar_second']); ?>
    </div>
	  <?php endif; ?>

  <div class="clear full-width last">
  	<div class="span-8" id="content-bottom-left"><?php print render($page['content_bottom_left']); ?></div>
  	<div class="span-8 last" id="content-bottom-right"><?php print render($page['content_bottom_right']); ?></div>
  </div>
  
  </div>
</div>
<!-- End Columns -->  
</div> <!-- end container-inner --> 
</div> <!-- end container --> 

<div class="footer-container">
<!-- Footer Area -->
<div class="container">
  <div class="clear full-width last" id="footer">
    <div class="span-4" id="col4-hori-first">
      <?php print render($page['col4-hori-first']); ?>
    </div>
    <div class="span-4" id="col4-hori-second">
      <?php print render($page['col4-hori-second']); ?>
    </div>
    <div class="span-4" id="col4-hori-third">
      <?php print render($page['col4-hori-third']); ?>
    </div>
    <div class="span-4 last" id="col4-hori-fourth">
      <?php print render($page['col4-hori-fourth']); ?>
    </div>
  </div>
</div>  
<!-- End Footer Area -->
</div>
<div id="back-top"> 
  <a>
  <span class="totopimage">Top</span>
  </a> 
</div> 