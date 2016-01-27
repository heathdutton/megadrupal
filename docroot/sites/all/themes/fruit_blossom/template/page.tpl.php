<?php
?>
  <?php print render($page['header']); ?>

  <div id="page">
    <div id="header">
      <div id="header-logo">
        <?php if (!empty($logo)): ?>
          <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home" id="logo">
            <img src="<?php print $logo; ?>" alt="" />
          </a>
        <?php endif; ?>
      </div> <!-- /header-logo -->
      <div id="header-left">
        <?php if ($page['header-left']): print render($page['header-left']); endif; ?>
      </div> <!-- /header-left -->
      <div id="header-center">
        <div id="name-and-slogan">
          <?php if (!empty($site_name)): ?>
            <div id="site-name">
              <a href="<?php print $front_page ?>" title="<?php print t('Home'); ?>" rel="home"><?php print $site_name; ?></a>
            </div>
          <?php endif; ?>

          <?php if (!empty($site_slogan)): ?>
            <div id="site-slogan"><?php print $site_slogan; ?></div>
          <?php endif; ?>
        </div> <!-- /name-and-slogan -->
      </div> <!-- /header-center -->
      <div id="header-right">
        <?php if ($page['header-right']): print render($page['header-right']); endif; ?>
      </div> <!-- /header-right -->
      <div id="header-bottom">
        <?php if ($page['header-bottom']): print render($page['header-bottom']); endif; ?>
      </div> <!-- /header-bottom -->
    </div> <!-- /header -->

    <div id="container" class="clearfix">

      <?php if ($page['sidebar_first']): ?>
        <div id="sidebar-first" class="column sidebar">
          <?php print render($page['sidebar_first']); ?>
        </div> <!-- /sidebar-first -->
      <?php endif; ?>
      
      <div id="main" class="column">
        <?php if ($breadcrumb): ?><div id="breadcrumb"><?php print $breadcrumb; ?></div><?php endif; ?>
        <?php if ($page['highlighted']): ?><div id="highlighted"><?php print render($page['highlighted']); ?></div><?php endif; ?>

        <div id="content">
          <?php if ($tabs): print '<div id="tabs-wrapper">'; endif; ?>
          <?php print render($title_prefix); ?>
          <?php if ($title): print '<h2' . ($tabs ? ' class="with-tabs"' : '') . '>' . $title . '</h2>'; endif; ?>
          <?php print render($title_suffix); ?>
          <?php if ($tabs): print '<ul class="tabs primary">' . render($tabs) . '</ul></div>'; endif; ?>
          <?php if ($messages): print $messages; endif; ?>
          <?php if ($page['help']): print render($page['help']); endif; ?>
          <?php if ($action_links): ?><ul class="action-links"><?php print render($action_links); ?></ul><?php endif; ?>
          <?php print render($page['content']); ?>
          <?php print $feed_icons; ?>
        </div> <!-- /content -->

      </div> <!-- /main -->

    </div> <!-- /container -->

      <div id="footer" class="clearfix">
        <?php if ($page['footer']): print render($page['footer']); endif; ?>
      </div> <!-- /footer -->
  </div> <!-- /page -->
