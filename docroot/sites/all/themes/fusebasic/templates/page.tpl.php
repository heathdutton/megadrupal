<div id="skip"><a href="#content">Skip to Content</a> <a href="#navigation">Skip to Navigation</a></div>
    <div id="page" class="<?php print $classes; ?>"<?php print $attributes; ?>>

    <!-- ______________________ HEADER _______________________ -->

    <div id="header">
      <div id="header-inner" class="inner">

      <?php if ($logo): ?>
        <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home" id="logo">
          <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>"/>
        </a>
      <?php endif; ?>

      <?php if ($site_name || $site_slogan): ?>
        <div id="name-and-slogan"<?php if ($hide_site_name && $hide_site_slogan) { print ' class="element-invisible"'; } ?>>

          <?php if ($site_name): ?>
            <?php if ($title): ?>
              <div id="site-name"<?php if ($hide_site_name) { print ' class="element-invisible"'; } ?>>
                <strong>
                  <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><span><?php print $site_name; ?></span></a>
                </strong>
              </div>
            <?php else: /* Use h1 when the content title is empty */ ?>
              <h1 id="site-name"<?php if ($hide_site_name) { print ' class="element-invisible"'; } ?>>
                <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><span><?php print $site_name; ?></span></a>
              </h1>
            <?php endif; ?>
          <?php endif; ?>

          <?php if ($site_slogan): ?>
            <div id="site-slogan"<?php if ($hide_site_slogan) { print ' class="element-invisible"'; } ?>>
              <?php print $site_slogan; ?>
            </div>
          <?php endif; ?>

        </div> <!-- /#name-and-slogan -->
      <?php endif; ?>


      <?php if ($page['header']): ?>
        <div id="header-region">
          <?php print render($page['header']); ?>
        </div>
      <?php endif; ?>

      </div> <!-- /header-inner -->
    </div> <!-- /header -->

    <!-- ______________________ MAIN _______________________ -->

    <div id="main" class="clearfix">

      <div id="content">
        <div id="content-inner" class="inner column center">

          <?php if ($breadcrumb || $title || $messages || $tabs || $action_links): ?>
            <div id="content-header">

              <?php print $breadcrumb; ?>

              <?php if ($page['highlight']): ?>
                <div id="highlight"><?php print render($page['highlight']) ?></div>
              <?php endif; ?>

              <?php if ($title): ?>
                <h1 class="title"><?php print $title; ?></h1>
              <?php endif; ?>

              <?php print $messages; ?>

              <?php print render($page['help']); ?>

              <?php print render($tabs); ?>

            </div> <!-- /#content-header -->
          <?php endif; ?>

          <div id="content-area">
            <?php print render($page['content']) ?>
          </div> <!-- /#content-area -->

          <?php print $feed_icons; ?>

          </div>
        </div> <!-- /content-inner /content -->

        <?php if ($main_menu || $secondary_menu): ?>
          <div id="navigation"><div class="section clearfix">
            <?php print theme('links__system_main_menu', array(
              'links' => $main_menu,
              'attributes' => array(
                'id' => 'main-menu',
                'class' => array('links', 'clearfix'),
              ),
              'heading' => array(
                'text' => t('Main menu'),
                'level' => 'h2',
                'class' => array('element-invisible'),
              ),
            )); ?>

            <?php print theme('links__system_secondary_menu', array(
              'links' => $secondary_menu,
              'attributes' => array(
                'id' => 'secondary-menu-links',
                'class' => array('links', 'clearfix'),
              ),
              'heading' => array(
                'text' => t('Secondary menu'),
                'level' => 'h2',
                'class' => array('element-invisible'),
              ),
            )); ?>

          </div></div> <!-- /.section, /#navigation -->
        <?php endif; ?>

        <?php if ($page['sidebar_first']): ?>
          <div id="sidebar-first" class="column sidebar first">
            <div id="sidebar-first-inner" class="inner">
              <?php print render($page['sidebar_first']); ?>
            </div>
          </div>
        <?php endif; ?> <!-- /sidebar-first -->

        <?php if ($page['sidebar_second']): ?>
          <div id="sidebar-second" class="column sidebar second">
            <div id="sidebar-second-inner" class="inner">
              <?php print render($page['sidebar_second']); ?>
            </div>
          </div>
        <?php endif; ?> <!-- /sidebar-second -->

    </div> <!-- /main -->

    <!-- ______________________ FOOTER _______________________ -->

    <?php if($page['footer']): ?>
      <div id="footer">

        <?php print render($page['footer']); ?>
      </div> <!-- /footer -->
    <?php endif; ?>

</div> <!-- /page -->