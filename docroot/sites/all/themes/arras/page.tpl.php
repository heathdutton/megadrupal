  <div id="page-wrapper"><div id="page">

    <?php if ($secondary_sf_menu): ?>
      <div id="top-navigation"><div class="section clearfix">      
        <?php print $secondary_sf_menu;  ?>
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

    <div id="main-wrapper"><div id="main" class="clearfix">    
      <div id="middle">
        <?php if ($page['highlighted']): ?>
          <div id="highlighted"><div class="section">
            <?php print render($page['highlighted']); ?>
          </div></div> <!-- /.section, /#highlighted -->
        <?php endif; ?>

        <div id="content" class="column"><div class="section">
          <?php if ($breadcrumb): ?><div id="breadcrumb"><?php print $breadcrumb; ?></div><?php endif; ?>
          <?php print $messages; ?>

          <a id="main-content"></a>
          <?php print render($title_prefix); ?>
          <?php if ($title): ?><h1 class="title" id="page-title"><?php print $title; ?></h1><?php endif; ?>
          <?php print render($title_suffix); ?>
          <?php if ($tabs): ?><div class="tabs"><?php print render($tabs); ?></div><?php endif; ?>
          <?php print render($page['help']); ?>
          <?php if ($action_links): ?><ul class="action-links"><?php print render($action_links); ?></ul><?php endif; ?>
          <?php print render($page['content']); ?>        
        </div></div> <!-- /.section, /#content -->

        <?php if ($page['postscript']): ?>
          <div id="postscript" class="column"><div class="section">
            <?php print render($page['postscript']); ?>
          </div></div> <!-- /.section, /#postscript -->
        <?php endif; ?>
      </div> <!-- /#middle -->

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

    </div></div> <!-- /#main, /#main-wrapper -->

    <?php if ($page['footer']): ?>
      <div id="footer"><div class="section">
        <?php print render($page['footer']); ?>
      </div></div> <!-- /.section, /#footer -->
    <?php endif; ?>

    <!-- Linkback: Please do not remove this as a courtesy to the effort we have put into this theme. -->
    <div id="postamble"><?php print $postamble; ?></div>

  </div></div> <!-- /#page, /#page-wrapper -->
