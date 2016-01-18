<div id="wrapper">
  <div id="mobile-header">

    <?php if ($logo): ?>
      <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home" id="logo">
        <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
      </a>
    <?php endif; ?>
  
    <?php if ($site_name): ?>
      <?php if ($title): ?>
        <div id="site-name"><strong>
          <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><span><?php print $site_name; ?></span></a>
        </strong></div>
      <?php else: /* Use h1 when the content title is empty */ ?>
        <h1 id="site-name">
          <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><span><?php print $site_name; ?></span></a>
        </h1>
      <?php endif; ?>
    <?php endif; ?>

    <?php if ($site_slogan): ?>
      <div id="site-slogan"><?php print $site_slogan; ?></div>
    <?php endif; ?>
  
  </div><!--/#mobile-header-->

  <?php print $messages; ?>

  <div id="content-wrapper"> 
    <div id="content">
    
      <?php if ($page['highlighted']): ?><div id="highlighted"><?php print render($page['highlighted']); ?></div><?php endif; ?>
  
      <a id="main-content"></a>
  
      <?php print render($title_prefix); ?>
      <?php if ($title): ?><h1 class="title" id="page-title"><?php print $title; ?></h1><?php endif; ?>
      <?php print render($title_suffix); ?>
  
      <?php if ($tabs): ?><div class="tabs"><?php print render($tabs); ?></div><?php endif; ?>
  
      <?php print render($page['help']); ?>
  
      <?php if ($action_links): ?><ul class="action-links"><?php print render($action_links); ?></ul><?php endif; ?>
  
      <?php print render($page['content']); ?>
  
      <?php if ($breadcrumb): ?>
        <div id="breadcrumb"><?php print $breadcrumb; ?></div>
      <?php endif; ?>
    
    </div><!--/#content-->
  
    <?php if ($page['supplementary']): ?>
    <div id="supplementary" class="section">
      <?php print render($page['supplementary']); ?>
    </div><!--/#supplementary-->
    <?php endif; ?>
  
    <?php if ($main_menu || $secondary_menu): ?>
      <div id="navigation" class="section">
  
        <?php print theme('links__system_main_menu', array('links' => $main_menu, 'attributes' => array('id' => 'main-menu', 'class' => array('links', 'inline', 'clearfix')), 'heading' => t('Main menu'))); ?>
  
        <?php print theme('links__system_secondary_menu', array('links' => $secondary_menu, 'attributes' => array('id' => 'secondary-menu', 'class' => array('links', 'inline', 'clearfix')), 'heading' => t('Secondary menu'))); ?>
  
      </div><!-- /.section, /#navigation -->
    <?php endif; ?>
  </div><!--/#content-wrapper -->

  <div id="footer" class="section">
    <?php print render($page['footer']); ?>
  </div><!-- /.section, /#footer -->

</div><!--/#wrapper-->