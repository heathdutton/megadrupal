

<div class="page">

 <header role="banner" class="clearfix">
    <section class="utility-bar">
	  <a href="#nav" aria-controls="menu-name-main-menu" class="nav-menu-toggle control" id="menu-toggle">Menu</a>
      <?php print render($page['utility_bar']); ?>
    </section>

  <section class="siteinfo">
    <?php if ($logo): ?>
      <figure class="logo">
      <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home" id="logo">
        <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
      </a>
      </figure>
    <?php endif; ?>

    <?php if($site_name OR $site_slogan ): ?>
    <hgroup>
      <?php if($site_name): ?>
        <h1><a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><?php print $site_name; ?></a></h1>
      <?php endif; ?>
      <?php if ($site_slogan): ?>
        <h2><?php print $site_slogan; ?></h2>
      <?php endif; ?>
    </hgroup>
    <?php endif; ?>
  </section>

  <?php if($page['header']): ?>
    <section class="header-region clearfix">
      <?php print render($page['header']); ?>
    </section>
  <?php endif; ?>
  
 </header>
  <?php if ($page['navigation']): ?>
  	<section class="clearfix">
      <?php print render($page['navigation']); ?>
    </section><!-- /#navigation -->
  <?php endif; ?>


  <div class="content-area clearfix">
	  <section role="main" class="main-content clearfix">
	    <?php if (isset($section_title)): ?>
	      <h2 class="section-title"><?php print render($section_title); ?></h2>
	    <?php endif; ?>
	    
	    <?php print render($title_prefix); ?>
	    <?php if ($title): ?>
	      <h1><?php print $title; ?></h1>
	    <?php endif; ?>
	    <?php print render($title_suffix); ?>
	
	    <?php //print $breadcrumb; ?>
	
	    <?php if ($action_links): ?>
	      <ul class="action-links"><?php print render($action_links); ?></ul>
	    <?php endif; ?>
	
	    <?php if (isset($tabs['#primary'][0]) || isset($tabs['#secondary'][0])): ?>
	      <nav class="tabs"><?php print render($tabs); ?></nav>
	    <?php endif; ?>
	
	    <?php if($page['highlighted'] OR $messages){ ?>
	      <div class="drupal-messages">
	      <?php print render($page['highlighted']); ?>
	      <?php print $messages; ?>
	      </div>
	    <?php } ?>
	
	
	    <?php print render($page['content_pre']); ?>
	
	    <?php print render($page['content']); ?>
	
	    <?php print render($page['content_post']); ?>
	
	  </section><!-- /main -->
	
	  <?php if ($page['sidebar_second']): ?>
	    <section class="sidebar sidebar-content-second clearfix">
	      <?php print render($page['sidebar_second']); ?>
	    </section>
	  <?php endif; ?>
	
	  <?php if ($page['sidebar_first']): ?>
	    <section class="sidebar sidebar-content-first clearfix">
	    <?php print render($page['sidebar_first']); ?>
	    </section>
	  <?php endif; ?>
  </div>

  <footer role="contentinfo">
	  <?php print render($page['footer']); ?>
  </footer>

</div><!-- /page -->
<?php if ($page['closure']): ?>
<footer id="closure">
	<?php print render($page['closure']); ?>
</footer>
<?php endif; ?>

