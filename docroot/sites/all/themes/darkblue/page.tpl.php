<div id="wrap">
  <div id="top">
    <?php if (isset($secondary_menu)): ?>
      <div id="secondary-menu">
        <?php print theme('links__system_secondary_menu', array('links' => $secondary_menu, 'attributes' => array('id' => 'secondary-menu', 'class' => array('links', 'inline', 'clearfix')))); ?>
      </div>
      <div class="clear"></div>
    <?php endif; ?>
  </div>

  <?php if (isset($main_menu)): ?>
    <div id="menu">
      <?php print theme('links__system_main_menu', array('links' => $main_menu, 'attributes' => array('id' => 'main-menu', 'class' => array('links', 'inline', 'clearfix')))); ?>
    </div>
  <?php endif ?>

  <div id="header" class="clearfix">
    <?php if ($logo): ?>
      <div class="logo">
        <a id="logo" href="<?php print url(); ?>">
          <img src="<?php print $logo ?>" alt="<?php print t('Home') ?>" />
        </a>
      </div>
    <?php endif ?>

    <?php if ( $site_slogan || $site_name) : ?>
      <div class="site-info">
        <?php if ($site_name): ?>
          <div class="site-name">
            <h1>
              <a href="<?php print url(); ?>">
                <?php print $site_name ?>
              </a>
            </h1>
          </div>
        <?php endif ?>

        <?php if ($site_slogan): ?>
          <div class="slogan">
            <h2><?php print $site_slogan ?></h2>
          </div>
        <?php endif ?>
      </div>
    <?php endif ?>
    <?php if (!empty($page['header_blocks'])) { print render($page['header_blocks']); } ?>
  </div>  <!-- END div#header -->

  <div id="content">
     <?php if (!empty($page['content_top'])): ?>
      <div id="content-top">
        <div id="content-top-inner">
          <?php print render($page['content_top']); ?>
          <div class="clear"></div>
        </div>
      </div><!-- END div#content-top -->
    <?php endif ?>

    <?php print $breadcrumb; ?>
    <?php print $messages; ?>
    <?php // dpm(get_defined_vars()); ?>
    <div id="main-content"> <!-- main content -->
      <?php print render($primary_local_tasks); ?>
      <?php print render($secondary_local_tasks); ?>

      <?php print render($title_prefix); ?>
      <?php if ($title): ?>
        <h1 class="page-title"><?php print $title ?></h1>
      <?php endif ?>
      <?php print render($title_suffix); ?>

      <?php print render($page['help']); ?>
      <?php print render($page['content']); ?>
      <?php print $feed_icons; ?>
    </div>

    <?php if (!empty($page['sidebar_first'])): ?> <!-- right sidebar region -->
      <div id="sidebar-first" class="sidebar">
       <?php print render($page['sidebar_first']); ?>
      </div>
    <?php endif ?>
    <div class="clear"></div>
  </div><!-- END div#content -->

  <div id="footer">
    <?php if (!empty($page['footer'])): ?> <!-- footer region -->
      <?php print render($page['footer']); ?>
      <div class="clear"></div>
    <?php endif ?>
  </div>
</div>
