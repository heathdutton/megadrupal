<div id="container" class="clearfix container_24">

  <div id="skip-link">
    <a href="#main-content" class="element-invisible element-focusable"><?php print t('Skip to main content'); ?></a>
    <?php if ($main_menu): ?>
      <a href="#navigation" class="element-invisible element-focusable"><?php print t('Skip to navigation'); ?></a>
    <?php endif; ?>
  </div>

  <?php if ($page['header_top']): ?>
    <div id="header-top" class="grid_24 clearfix">
      <?php print render($page['header_top']); ?>
    </div>  <!-- /#header-top -->
  <?php endif; ?>

  <header id="header" role="banner" class="grid_24 clearfix">
    <div id="header-first" class="gradient-bg">
    <?php if ($logo): ?>
        <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" id="logo" class="grid_2 alpha">
          <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
        </a>
      <?php endif; ?>
      <?php if ($site_name || $site_slogan): ?>
        <hgroup id="site-name-slogan" class="grid_14">
          <?php if ($site_name): ?>
            <h1 id="site-name">
              <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>"><span><?php print $site_name; ?></span></a>
            </h1>
          <?php endif; ?>
          <?php if ($site_slogan): ?>
            <h2 id="site-slogan"><?php print $site_slogan; ?></h2>
          <?php endif; ?>
        </hgroup>
      <?php endif; ?>

      <?php if ($secondary_menu): ?>
        <nav id="navigation-secondary" role="navigation" class="grid_4 push_4 omega">
        <?php print theme('links__system_secondary_menu', array(
              'links' => $secondary_menu,
              'attributes' => array(
                'id' => 'secondary-menu',
                'class' => array('links', ''),
              ),
              'heading' => array(
                'text' => t('Secondary menu'),
                'level' => 'h2',
                'class' => array('element-invisible'),
              ),
            )); ?>
        </nav>
      <?php endif; ?>

    </div>

    <?php if($page['header']): ?>
      <div id="header-region" class="grid_24 alpha omega clearfix">
        <?php print render($page['header']); ?>
      </div>
    <?php endif; ?>

    <?php if ($main_menu || !empty($page['navigation'])): ?>
      <nav id="navigation" role="navigation" class="clearfix gradient-bg">
        <?php if (!empty($page['navigation'])): ?> <!--if block in navigation region, override $main_menu-->
          <?php print render($page['navigation']); ?>
        <?php endif; ?>
        <?php // if (empty($page['navigation'])): ?>
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
        <?php // endif; ?>
      </nav> <!-- /#navigation -->
    <?php endif; ?>
  </header> <!-- /#header -->

  <?php if($page['preface_one'] || $page['preface_two'] || $page['preface_three'] || $page['preface_four']): ?>
    <div id="preface" class="grid_24 equal-height-container clearfix">

      <?php if ($page['preface_one']): ?>
        <div id="preface-one" class="preface <?php print $grid_preface_one; ?> equal-height-element gradient-bg clearfix">
          <div class="preface-content">
            <?php print render($page['preface_one']); ?>
          </div>
        </div>  <!-- /#preface-one -->
      <?php endif; ?>

      <?php if ($page['preface_two']): ?>
        <div id="preface-two" class="preface <?php print $grid_preface_two; ?> equal-height-element gradient-bg clearfix">
          <div class="preface-content">
            <?php print render($page['preface_two']); ?>
          </div>
        </div>  <!-- /#preface-two -->
      <?php endif; ?>

      <?php if ($page['preface_three']): ?>
        <div id="preface-three" class="preface <?php print $grid_preface_three; ?> equal-height-element gradient-bg clearfix">
          <div class="preface-content">
            <?php print render($page['preface_three']); ?>
          </div>
        </div>  <!-- /#preface-three -->
      <?php endif; ?>

      <?php if ($page['preface_four']): ?>
        <div id="preface-four" class="preface <?php print $grid_preface_four; ?> equal-height-element gradient-bg clearfix">
          <div class="preface-content">
            <?php print render($page['preface_four']); ?>
          </div>
        </div>  <!-- /#preface-four -->
      <?php endif; ?>

    </div> <!-- /#preface -->
  <?php endif; ?>

  <?php if ($breadcrumb): print $breadcrumb; endif;?>


  <?php if ($messages): ?>
    <div id="messages" class="grid_24 clearfix">
      <?php print $messages; ?>
    </div>
  <?php endif; ?>

  <section id="main" role="main" class="<?php print $grid_main; ?> clearfix">
    <a id="main-content"></a>
    <?php if ($page['highlighted']): ?><div id="highlighted"><?php print render($page['highlighted']); ?></div><?php endif; ?>
    <?php print render($title_prefix); ?>
    <?php if ($title): ?><h1 class="title" id="page-title"><?php print $title; ?></h1><?php endif; ?>
    <?php print render($title_suffix); ?>
    <?php if (!empty($tabs['#primary'])): ?><div class="tabs-wrapper clearfix"><?php print render($tabs); ?></div><?php endif; ?>
    <?php print render($page['help']); ?>
    <?php if ($action_links): ?><ul class="action-links"><?php print render($action_links); ?></ul><?php endif; ?>
    <div id="content"><?php print render($page['content']); ?></div>
    <?php print render($page['postscript']); ?>
  </section> <!-- /#main -->

  <?php if ($page['sidebar_first']): ?>
    <aside id="sidebar-first" role="complementary" class="<?php print $grid_first; ?> sidebar clearfix">
      <?php print render($page['sidebar_first']); ?>
    </aside>  <!-- /#sidebar-first -->
  <?php endif; ?>

  <?php if ($page['sidebar_second']): ?>
    <aside id="sidebar-second" role="complementary" class="<?php print $grid_second; ?> sidebar clearfix">
      <?php print render($page['sidebar_second']); ?>
    </aside>  <!-- /#sidebar-second -->
  <?php endif; ?>

  <footer id="footer" role="contentinfo" class="grid_24 gradient-bg clearfix">
    <?php print render($page['footer']) ?>
    <?php print $feed_icons ?>
  </footer> <!-- /#footer -->

  <div id="footer-bottom" class="grid_24 clearfix">
    <?php print render($page['footer_bottom']).async_credits();  ?>
  </div>  <!-- /#footer-bottom -->

</div> <!-- /#container -->