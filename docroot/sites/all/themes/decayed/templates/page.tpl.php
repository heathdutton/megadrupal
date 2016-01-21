    <!-- page wrapper -->
    <div id="wrap" class="clearfix">
      <!-- header -->
      <div id="header" class="clearfix">
        <?php if ($linked_site_name): ?>
          <?php if ($is_front): ?>
            <h1 id="site-name"><?php print $linked_site_name; ?></h1>
          <?php else: ?>
            <div id="site-name"><?php print $linked_site_name; ?></div>
          <?php endif; ?>
        <?php endif; ?>
        <?php if ($site_slogan): ?>
          <p id="slogan"><?php print $site_slogan ?></p>
        <?php endif; ?>
          
        <?php if ($main_menu_links): ?>
          <nav class="navigation-wrapper">
            <?php print $main_menu_links; ?>
          </nav>
        <?php endif; ?>

        <?php $breadcrumb ? print $breadcrumb : ''; ?>

      </div>
      <!-- end #header-->

      <!-- content wrapper  -->
      <div id="content-wrap" class="clearfix">
        <div id="main" class="clearfix">
          <?php if ($page['search']): ?>
            <?php print render($page['search']); ?>
          <?php endif; ?>
          <?php if ($page['highlighted']): ?>
            <p id="mission"><?php print render($page['highlighted']); ?></p>
          <?php endif; ?>

          <?php if ($page['content_top']): ?>
          <div id="top-content-block" class="content-block">
            <?php print render($page['content_top']); ?>
          </div>
          <?php endif; ?>
          <a id="main-content"></a>
          <?php if ($title): ?>
            <?php print render($title_prefix); ?>
            <h1 class="title"><?php print $title ?></h1>
            <?php print render($title_suffix); ?>
          <?php endif; ?>
          <?php if (!empty($tabs)): ?>
            <?php print render($tabs); ?>
            <?php if (!empty($tabs2)): print render($tabs2); endif; ?>
          <?php endif; ?>
          <?php if ($action_links): ?>
            <ul class="action-links">
              <?php print render($action_links); ?>
            </ul>
          <?php endif; ?>
          <?php if ($messages): print $messages; endif; ?>
          <?php if (!empty($page['help'])): print render($page['help']); endif; ?>

          <?php print render($page['content']); ?>

          <?php if ($page['content_bottom']): ?>
          <div id="bottom-content-block" class="content-block">
            <?php print render($page['content_bottom']); ?>
          </div>
          <?php endif; ?>

          <?php print $feed_icons ?>

        </div>
        <!-- end #main -->

        <!-- left sidebar -->
        <div id="sidebar" class="clearfix">
          <?php if ($page['sidebar_first']): ?>
            <?php print render($page['sidebar_first']); ?>
          <?php endif; ?>

        </div>
        <!-- end #sidebar -->

      </div>
      <!-- end #content-wrap-->

      <!-- footer -->
      <div id="footer" class="clearfix">

        <div id="column-wrap" class="clearfix">

          <?php if ($page['footer_1']): ?>
          <div class="column">
            <?php print render($page['footer_1']); ?>
          </div>
          <?php endif; ?>

          <?php if ($page['footer_2']): ?>
          <div class="column">
            <?php print render($page['footer_2']); ?>
          </div>
          <?php endif; ?>

          <?php if ($page['footer_3']): ?>
          <div id="last" class="column">
            <?php print render($page['footer_3']); ?>
          </div>
          <?php endif; ?>

        </div>
        <div id="site-credit" class="clearfix">
          <?php if ($page['footer']): ?>
            <p><?php print render($page['footer']); ?></p>
          <?php endif; ?>
        </div>

      </div>
      <!-- end #footer -->
 
    </div>
    <!-- end #wrap -->