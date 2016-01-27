<div<?php print $attributes; ?>>
  <header class="l-header-wrapper" role="banner">
    <div class="l-top-wrapper">
      <div class="l-constrained">
        <?php print render($page['top']); ?>
      </div>
    </div>
    <div class="l-constrained">
      <div class="l-branding site-branding">
        <?php if ($logo): ?>
          <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home" class="site-branding__logo"><img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" /></a>
        <?php endif; ?>
        <?php if ($site_name): ?>
          <a href="<?php print $front_page; ?>" class="site-branding__name" title="<?php print t('Home'); ?>" rel="home"><span><?php print $site_name; ?></span></a>
        <?php endif; ?>
        <?php if ($site_slogan): ?>
          <h2 class="site-branding__slogan"><?php print $site_slogan; ?></h2>
        <?php endif; ?>
      </div>
      <?php print render($page['navigation']); ?>
    </div>
  </header>

  <?php if (!empty($page['highlighted'])): ?>
    <div class="l-highlighted-wrapper">
      <?php print render($page['highlighted']); ?>
    </div>
  <?php endif; ?>

  <div class="l-main-wrapper l-constrained">
    <a id="main-content"></a>
    <?php print $breadcrumb; ?>
    <?php print $messages; ?>
    <?php print render($page['help']); ?>

    <div class="l-content" role="main">
      <?php print render($tabs); ?>
      <?php print render($title_prefix); ?>
      <?php if ($title): ?>
        <h1><?php print $title; ?></h1>
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

  <?php if (!empty($page['test'])): ?>
    <div class="l-test-wrapper">
      <div class="l-constrained">
        <?php print render($page['test']); ?>
      </div>
    </div>
  <?php endif; ?>

  <footer class="l-footer-wrapper" role="contentinfo">
    <?php if ($page['footer_firstcolumn'] || $page['footer_secondcolumn'] || $page['footer_thirdcolumn'] || $page['footer_fourthcolumn']): ?>
      <div class="l-footer-column">
        <div class="l-constrained">
          <?php print render($page['footer_firstcolumn']); ?>
          <?php print render($page['footer_secondcolumn']); ?>
          <?php print render($page['footer_thirdcolumn']); ?>
          <?php print render($page['footer_fourthcolumn']); ?>
        </div>
      </div>
    <?php endif; ?>
    <div class="l-footer">
      <?php print render($page['footer']); ?>
    </div>
  </footer>
</div>
