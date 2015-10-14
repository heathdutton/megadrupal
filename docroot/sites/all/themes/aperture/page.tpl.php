  <div id="page-wrapper"><?php if ($site_slogan): ?><div id="site-slogan"><?php print $site_slogan; ?></div><?php endif; ?><div id="page">
    <div id="header"><div class="section clearfix">
      <?php print render($page['header']); ?>

      <div id="logo-title">
        <?php if ($logo): ?>
          <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home" id="logo">
            <img src="<?php print $logo; ?>" alt="<?php print $logo_alt; ?>" />
          </a>
        <?php endif; ?>

        <?php if ($site_name): ?>
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

          </div> <!-- /#name-and-slogan -->
        <?php endif; ?>
      </div> <!-- /#logo-title -->

      <?php if (!empty($primary) || $main_menu || $secondary_menu): ?>
        <div id="navigation"><div class="section">
          <?php if (!empty($primary)): ?>
            <?php print $primary; ?>
          <?php else: ?>
            <?php print theme('links__system_main_menu', array('links' => $main_menu, 'attributes' => array('id' => 'main-menu', 'class' => array('links', 'inline', 'clearfix')), /* 'heading' => t('Main menu')*/ )); ?>
          <?php endif; ?>          
          <?php print theme('links__system_secondary_menu', array('links' => $secondary_menu, 'attributes' => array('id' => 'secondary-menu', 'class' => array('links', 'inline', 'clearfix')), 'heading' => t('Secondary menu'))); ?>
        </div></div> <!-- /.section, /#navigation -->
      <?php endif; ?>

    </div></div> <!-- /.section, /#header -->

    <?php if ($page['highlighted']): ?><div id="highlighted"><?php print render($page['highlighted']); ?></div><?php endif; ?>

    <div id="container" class="clearfix">    

      <?php if ($page['preface_one']): ?>
        <div id="preface-one"><div class="section">
          <?php print render($page['preface_one']); ?>
        </div></div> <!-- /.section /preface-one -->
      <?php endif; ?>

      <div id="main"><div id="main-inner">

        <?php if ($page['preface_two']): ?>
          <div id="preface-two"><div class="section">
            <?php print render($page['preface_two']); ?>
          </div></div> <!-- /.section /preface-two -->
        <?php endif; ?>

        <div id="content" class="column"><div class="section">
          <?php if ($breadcrumb): ?><div id="breadcrumb"><?php print $breadcrumb; ?></div><?php endif; ?>
          <a id="main-content"></a>
          <?php print render($title_prefix); ?>
          <?php if ($title): ?><h1 class="title" id="page-title"><?php print $title; ?></h1><?php endif; ?>
          <?php print render($title_suffix); ?>
          <?php if ($tabs): ?><div class="tabs"><?php print render($tabs); ?></div><?php endif; ?>
          <?php print $messages; ?>
          <?php print render($page['help']); ?>
          <?php if ($action_links): ?><ul class="action-links"><?php print render($action_links); ?></ul><?php endif; ?>
          <?php print render($page['content']); ?>
          <?php print $feed_icons; ?>
        </div></div> <!-- /.section, /#content -->

        <?php if ($page['sidebar_second']): ?>
          <div id="sidebar-second" class="column sidebar"><div class="section">
            <?php print render($page['sidebar_second']); ?>
          </div></div> <!-- /.section, /#sidebar-second -->
        <?php endif; ?>

      </div></div> <!-- /#main, /#main-inner -->

      <?php if ($page['sidebar_first']): ?>
        <div id="sidebar-first" class="column sidebar"><div class="section">
          <?php print render($page['sidebar_first']); ?>
        </div></div> <!-- /.section, /#sidebar-first -->
      <?php endif; ?>

      <?php if ($page['postscript_one']): ?>
        <div id="postscript-one"><div class="section">
          <?php print render($page['postscript_one']); ?>
        </div></div> <!-- /.section /postscript-one -->
      <?php endif; ?>

    </div> <!-- /#container -->

    <?php if ($page['postscript_two']): ?>
      <div id="postscript-two"><div class="section">
        <?php print render($page['postscript_two']); ?>
      </div></div> <!-- /.section /postscript-two -->
    <?php endif; ?>

    <div id="footer"><div class="section">
      <?php print render($page['footer']); ?>
    </div></div> <!-- /.section, /#footer -->

  </div></div> <!-- /#page, /#page-wrapper -->
