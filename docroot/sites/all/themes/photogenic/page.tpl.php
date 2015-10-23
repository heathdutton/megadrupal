  <div id="page-wrapper"><div id="page">

    <div id="header" class="clearfix">
      <div id="logo-title" class="clearfix">
        <?php if ($logo): ?>
          <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home" id="logo">
            <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
          </a>
        <?php endif; ?>

        <?php if ($site_name || $site_slogan): ?>
          <div id="name-and-slogan">
            <?php if ($site_name): ?>
              <div id="site-name">
                <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><?php print $site_name; ?></a>
              </div>
            <?php endif; ?>

            <?php if ($site_slogan): ?>
              <div id="site-slogan"><?php print $site_slogan; ?></div>
            <?php endif; ?>
          </div> <!-- /#name-and-slogan -->
        <?php endif; ?>
      </div> <!-- /#logo-title -->

      <?php print render($page['header']); ?>
    </div> <!-- /#header -->

    <?php if ($page['menu']): ?>
      <div id="menu" class="clearfix"><?php print render($page['menu']); ?></div>
    <?php endif; ?>

    <?php print $messages; ?>

    <div id="content" class="clearfix">

      <?php print render($title_prefix); ?>
      <?php if ($title): ?>
        <h1 id="page-title" class="title"><?php print $title; ?></h1>
      <?php endif; ?>
      <?php print render($title_suffix); ?>

      <?php if ($tabs): ?>
        <div class="tabs"><?php print render($tabs); ?></div>
      <?php endif; ?>

      <?php print render($page['help']); ?>

      <?php if ($action_links): ?>
        <ul class="action-links"><?php print render($action_links); ?></ul>
      <?php endif; ?>

      <?php print render($page['content']); ?>

      <?php print $feed_icons; ?>

    </div> <!-- /#content -->

    <div id="footer">
      <?php print render($page['footer']); ?>
    </div> <!-- /#footer -->

  </div></div> <!-- /#page, /#page-wrapper -->
