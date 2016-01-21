
<div id="page-wrapper">
  
  
  <div id="wrapper" class="clear-block">

    <?php if ($page['header']): ?>
      <div id="header-region">
        <?php print render($page['header']); ?>
      </div>
    <?php endif; ?>

    <header id="header" class="clearfix">

      <div id="logo-floater">
        <?php if ($logo || $site_title): ?>
          <?php if ($title): ?>
            <div id="branding"><strong><a href="<?php print $front_page ?>">
                  <?php if ($logo): ?>
                    <img src="<?php print $logo ?>" alt="<?php print $site_name_and_slogan ?>" title="<?php print $site_name_and_slogan ?>" id="logo" />
                  <?php endif; ?>
                  <?php print $site_html ?>
                </a></strong></div>
          <?php else: /* Use h1 when the content title is empty */ ?>
            <h1 id="branding"><a href="<?php print $front_page ?>">
                <?php if ($logo): ?>
                  <img src="<?php print $logo ?>" alt="<?php print $site_name_and_slogan ?>" title="<?php print $site_name_and_slogan ?>" id="logo" />
                <?php endif; ?>
                <?php print $site_html ?>
              </a></h1>
          <?php endif; ?>
        <?php endif; ?>
      </div>

      <nav id="primary-menu">
        <?php print theme('links__system_main_menu', array('links' => $main_menu, 'attributes' => array('id' => 'main-menu', 'class' => array('links', 'inline')))); ?>
      </nav>

    </header>

    <section id="container" class="clear-block">

      <?php if ($breadcrumb || $secondary_menu): ?>
        <div id="sub-header" class="clearfix">
          <?php if ($secondary_menu): ?>
            <nav id="secondary-menu">
              <?php print theme('links__system_secondary_menu', array('links' => $secondary_menu, 'attributes' => array('id' => 'user-menu', 'class' => array('links', 'inline')))); ?>
            </nav>
          <?php endif; ?>

          <?php if ($breadcrumb && $secondary_menu): ?>
            <div class="divider"></div>
          <?php endif; ?>

          <?php if ($breadcrumb): ?>
            <div id="breadcrumb"><?php print $breadcrumb; ?></div>
          <?php endif; ?>
        </div>
      <?php endif; ?>

      <div id="center">

        <?php if ($messages): ?>
          <div id="notice">
            <?php print $messages; ?>
          </div>
        <?php endif; ?>

        <?php if ($page['highlighted']): ?><div id="highlighted"><?php print render($page['highlighted']); ?></div><?php endif; ?>

        <div id="squeeze" class="content">
          <?php if ($tabs): ?>
            <div id="tabs-wrapper" class="clearfix">
              <?php print render($tabs); ?>
            <?php endif; ?>

            <?php if ($page['help']): ?>
              <div id="help">
                <?php print render($page['help']); ?>
              </div>
            <?php endif; ?>

            <?php print render($title_prefix); ?>

            <?php if ($title): ?>
              <h1<?php print $tabs ? ' class="with-tabs"' : ''  ?>><?php print $title ?></h1>
            <?php endif; ?>

            <?php print render($title_suffix); ?>


            <?php if ($tabs): ?>
            </div>
          <?php endif; ?>

          <?php print render($tabs2); ?>

          <?php if ($action_links): ?><ul class="action-links"><?php print render($action_links); ?></ul><?php endif; ?>

          <div class="clearfix">
            <?php print render($page['content']); ?>
          </div>
          <?php print $feed_icons ?>

        </div>

      </div>

      <?php if ($page['sidebar_first']): ?>
        <div id="left" class="sidebar">
          <div class="content">
            <?php print render($page['sidebar_first']); ?>
          </div>
        </div>
      <?php endif; ?>

      <?php if ($page['sidebar_second']): ?>
        <div id="right" class="sidebar">

          <div class="content">
            <?php print render($page['sidebar_second']); ?>
          </div>

        </div>
      <?php endif; ?>

    </section>  <!-- /#container -->

    <footer id="footer" class="clearfix">

      <?php print render($page['footer']); ?>

    </footer>

  </div>  <!-- /#wrapper -->

</div> <!-- /#page-wrapper -->
