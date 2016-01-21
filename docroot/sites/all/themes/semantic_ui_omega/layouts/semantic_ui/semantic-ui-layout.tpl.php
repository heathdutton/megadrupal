<div<?php print $attributes; ?>>
  <header class="l-header header" role="banner">
    <?php if($page['fixed_nav'] || $primary_nav):?>
    <div class="ui fixed transparent inverted main menu fixed-nav-wrapper fixed">
      <div class="fixed-nav container l-constrained">
        <?php if ($logo): ?>
          <a href="<?php print $front_page; ?>" title="<?php print t($site_name . ' Home'); ?>" rel="home" class="ui mini image fl-left logo site-branding__logo"><img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" class="site-logo-small" /></a>
        <?php endif; ?>
          <?php if ($primary_nav): print render($primary_nav); endif; ?>
        <?php if ($secondary_nav): print render($secondary_nav); endif; ?>
        <?php print render($page['fixed_nav']); ?>
      </div>
    </div>
    <?php endif;?>
    <div class="nws-site-info nws-header <?php if($page['navigation'])print "with-nav"; ?> <?php if($page['header'])print "with-header"; ?>">
      <div class="l-constrained site-branding">
        <?php if ($logo): ?>
          <a href="<?php print $front_page; ?>" title="<?php print t($site_name . ' Home'); ?>" rel="home" class="site-branding__logo fl-left"><img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" class="site-logo ui small image" /></a>
        <?php endif; ?>
        <?php if ($site_name): ?>
          <h1 class="fl-left site-name"><a href="<?php print $front_page; ?>" class="site-branding__name" title="<?php print t('Home'); ?>" rel="home"><span><?php print $site_name; ?></span></a></h1>
        <?php endif; ?>
        <?php if ($site_slogan): ?>
          <h2 class="site-branding__slogan"><?php print $site_slogan; ?></h2>
        <?php endif; ?>
        <?php print render($page['branding']); ?>
      </div>
      <div class="clear"></div>
      <?php print render($page['navigation']); ?>
      <?php if ($page['header']): ?>
        <div class="l-constrained header l-constrained">
            <?php print render($page['header']); ?>
        </div>
      <?php endif; ?>
    </div>
  </header>

  <?php print render($page['hero']); ?>

  <?php if (!empty($page['highlighted'])): ?>
    <div class="l-highlighted-wrapper region-highlighted highlighted-wrapper">
      <?php print render($page['highlighted']); ?>
    </div>
  <?php endif; ?>

  <div class="l-main l-constrained" id="main">
    <?php print render($page['content_top']); ?>
    <a id="main-content"></a>
    <?php print render($tabs); ?>
    <?php print $breadcrumb; ?>
    <?php print $messages; ?>
    <?php print render($page['help']); ?>
    <div class="l-content ui segment main" role="main">
      <?php print render($title_prefix); ?>
      <?php if ($title): ?>
        <h1 class="title"><?php print $title; ?></h1>
      <?php endif; ?>
      <?php print render($title_suffix); ?>
      <?php if ($action_links): ?>
        <ul class="action-links"><?php print render($action_links); ?></ul>
      <?php endif; ?>
        <?php print render($page['content']); ?>
      <?php print $feed_icons; ?>
    </div>

    <?php print render($page['sidebar_first']); ?>
    <?php print render($page['sidebar_second']); ?>
  </div>

  <footer role="contentinfo">
    <?php if($page['footer_top'] || $page['footer']):?>
    <div class="footer-inner l-constrained">
    <div class="l-footer-wrapper footer-top">
      <?php print render($page['footer_top']); ?>
    </div>
    <div class="l-footer-wrapper footer-bottom">
      <?php print render($page['footer']); ?>
    </div>
    <?php endif;?>
    </div>
  </footer>
</div>
