  <div id="page-wrapper"><div id="page">
    <?php if ($secondary_menu): ?>
      <div id="top-navigation"><div class="section clearfix">      
        <?php print theme('links__system_secondary_menu', array('links' => $secondary_menu, 'attributes' => array('id' => 'secondary-menu', 'class' => array('links', 'inline', 'clearfix')))); ?>
      </div></div> <!-- /.section, /#top-navigation -->
    <?php endif; ?>

    <div id="header"><div class="section clearfix">

      <div id="logo-title">
        <?php if ($logo): ?>
          <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home" id="logo">
            <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
          </a>
        <?php endif; ?>

        <?php if ($site_name || $site_slogan): ?>
          <div id="name-and-slogan">
            <?php if ($site_name): ?>
              <?php if ($title): ?>
                <div id="site-name">
                  <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><span><?php print $site_name; ?></span></a>
                </div>
              <?php else: /* Use h1 when the content title is empty */ ?>
                <h1 id="site-name">
                  <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><span><?php print $site_name; ?></span></a>
                </h1>
              <?php endif; ?>
            <?php endif; ?>

            <?php if ($site_slogan): ?>
              <div id="site-slogan"><?php print $site_slogan; ?></div>
            <?php endif; ?>
          </div> <!-- /#name-and-slogan -->
        <?php endif; ?>
      </div> <!-- /#logo-title -->

      <?php if ($page['header']): ?>
        <div id="header-region">
          <?php print render($page['header']); ?>
        </div> <!-- /#header-region -->
      <?php endif; ?>
    </div></div> <!-- /.section, /#header -->

    <?php if ($main_sf_menu || $feed_icons): ?>
      <div id="navigation"><div class="section clearfix">
        <?php print $main_sf_menu;  ?>
        <?php print $feed_icons; ?>
      </div></div> <!-- /.section, /#navigation -->
    <?php endif; ?>

    <?php print $messages; ?>
    <?php if ($breadcrumb): ?>
      <div id="breadcrumb"><?php print $breadcrumb; ?></div>
    <?php endif; ?>

    <div id="main-wrapper"><div id="main-wrapper-1"><div id="main-wrapper-2"><div id="main-wrapper-3"><div id="main-wrapper-4"><div id="main-wrapper-5"><div id="main" class="clearfix">

      <?php if ($page['highlighted']): ?>
        <div id="highlighted">
          <?php print render($page['highlighted']); ?>
        </div>
      <?php endif; ?>
        
      <div id="content" class="column"><div class="section">
        <a id="main-content"></a>
        <?php print render($title_prefix); ?>
        <?php if ($title): ?><h1 class="title" id="page-title"><?php print $title; ?></h1><?php endif; ?>
        <?php print render($title_suffix); ?>
        <?php if ($tabs): ?><div class="tabs"><?php print render($tabs); ?></div><?php endif; ?>
        <?php print render($page['help']); ?>
        <?php if ($action_links): ?><ul class="action-links"><?php print render($action_links); ?></ul><?php endif; ?>
        <?php print render($page['content']); ?>
      </div></div> <!-- /.section, /#content -->

      <?php if ($page['sidebar_first'] || $page['sidebar_second']): ?>
        <div id="sidebar-wrapper">
          <?php if ($page['sidebar_first']): ?>
            <div id="sidebar-first" class="column sidebar"><div class="section">
              <?php print render($page['sidebar_first']); ?>
            </div></div> <!-- /.section, /#sidebar-first -->
          <?php endif; ?>

          <?php if ($page['sidebar_second']): ?>
            <div id="sidebar-second" class="column sidebar"><div class="section">
              <?php print render($page['sidebar_second']); ?>
            </div></div> <!-- /.section, /#sidebar-second -->
          <?php endif; ?>
        </div> <!-- /#sidebar-wrapper -->
      <?php endif; ?>

    </div></div></div></div></div></div></div> <!-- /#main, /#main-wrapper-5, /#main-wrapper-4, /#main-wrapper-3, /#main-wrapper-2, /#main-wrapper-1, /#main-wrapper -->

    <div id="footer"><div class="section">
      <?php print render($page['footer']); ?>
    </div></div> <!-- /.section, /#footer -->

  </div></div> <!-- /#page, /#page-wrapper -->
