  <div id="wrap">
    <div id="header">
      <div id="headertop">
        <div id="headertopwrap">
        <div id="headertopleft">
        <div id="headertopleft-left">
          <?php if ($logo): ?>
            <a href="<?php print check_url($front_page); ?>" title="<?php print bella_brandinginfo(); ?>"><img src="<?php print check_url($logo) ?>" alt="<?php print bella_brandinginfo(); ?>" /></a>
          <?php endif; ?>
        </div><!--/#headertopleft-left-->
        <div id="headertopleft-right">
          <?php if ($site_name || $site_slogan): ?>
          <?php if ($site_name): ?>
            <div class="bella-sitename"><?php print $site_name; ?></div>
          <?php endif; ?>
          <?php if ($site_slogan): ?>
            <div class="bella-slogan<?php print !$site_name ? '-nositename' : ''; ?>"><?php print $site_slogan; ?></div>
          <?php endif; ?>
          <?php endif; ?>
        </div><!--/#headertopleft-right-->
        </div><!--/#headertopleft-->
        <div id="headertopright">
          <?php if ($page['headertopright']): ?>
            <?php print render($page['headertopright']); ?>
          <?php else: ?>
            <?php print bella_searchbox(); ?>
          <?php endif; ?>
        <div id="toplinks"><div class="toplinkswrap"><?php print bella_social_links(); ?></div></div>
        </div><!--/#headertopright-->
      </div><!--/#headertopwrap-->
      </div><!--/#headertop-->
      <div id="menurow">
        <div id="menu">
          <?php if ($main_menu): ?>
            <?php print bella_nav_menu($main_menu); ?>
          <?php endif; ?>  
        </div><!--/#menu-->
      </div><!--/#menurow-->
    </div><!--/#header-->
    <?php if ($page['subheader']): ?>
    <div id="subheader">
      <div id="subheadertr">
        <div id="subheadertd">
            <?php print render($page['subheader']); ?>
        </div><!--/#subheadertd-->
      </div><!--/#subheadertr-->
    </div><!--/#subheader-->
    <?php endif; ?>
    <div id="content">
    <div id="pageLoading"></div>
        <?php if ($page['sidebar_first']): ?>
          <div id="sidebar-left">
            <?php print render($page['sidebar_first']); ?>
          </div><!--/#sidebar-left-->
        <?php endif; ?>
        <div id="main">
          <div id="main-inner-wrap">
          <?php if ($page['contenttop']): ?>
            <div id="content-top">
              <?php print render($page['contenttop']); ?>
            </div><!--/#content-top-->
            <div class="clearfix"></div>
          <?php endif; ?>
          <?php if ($breadcrumb && theme_get_setting('breadcrumb_display')): ?>
            <?php print $breadcrumb ?>
          <?php endif; ?>
          <?php if ($messages): ?>
            <div id="msgs" class="bellamsgs-messages">
              <?php print $messages ?>
            </div>
          <?php endif; ?>
          <?php if ($title && bella_show_title()): ?><h1 class="bella-main-title"><?php print $title ?></h1><?php endif; ?>
          <?php print render($page['help']); ?>
          <?php if ($action_links): ?><ul class="action-links"><?php print render($action_links); ?></ul><?php endif; ?>
          <?php if ($tabs): ?>
            <div class="tabs bella-main-tabs"><?php print render($tabs); ?></div>
          <?php endif; ?>
          <?php bella_toggle_frontpagenocontent_message($page); ?>
          <?php print render($page['content']); ?>
          <?php if (theme_get_setting('feed_icons_display_icons')): print $feed_icons; endif; ?>
        </div><!--/#main-inner-wrap-->
        </div><!--/#main-->
        <?php if ($page['sidebar_second']): ?>
          <div id="sidebar-right">
            <?php print render($page['sidebar_second']); ?>
          </div><!--/#sidebar-right-->
        <?php endif; ?>
    </div><!--/#content-->
  <?php if ($page['bottom1'] || $page['bottom2'] || $page['bottom3'] || $page['bottom4']): ?>
    <div id="bottom">
      <div id="bottom-row">
      <?php if ($page['bottom1']): ?>
        <div id="bottom1"><?php print render($page['bottom1']); ?></div><!--/#bottom1-->
      <?php endif; ?>
      <?php if ($page['bottom2']): ?>
        <div id="bottom2"><?php print render($page['bottom2']); ?></div><!--/#bottom2-->
      <?php endif; ?>
      <?php if ($page['bottom3']): ?>
        <div id="bottom3"><?php print render($page['bottom3']); ?></div><!--/#bottom3-->
      <?php endif; ?>
      <?php if ($page['bottom4']): ?>
        <div id="bottom4"><?php print render($page['bottom4']); ?></div><!--/#bottom4-->
      <?php endif; ?>
      </div><!--/#bottom-row-->
    </div><!--/#bottom-->
  <?php endif; ?>
  <div class="clearfix"></div>
  <div id="footer">
    <div id="footertr">
      <div id="footertd">
        <?php if ($page['footer']): print render($page['footer']); endif; ?>
        <?php if ($secondary_menu): ?>
          <div id="footerlinks">
            <?php print bella_nav_menu($secondary_menu, 'secondary-menu', 'subnavlist'); ?>
          </div><!--/#footerlinks-->
        <?php endif; ?>
        <?php if (theme_get_setting('footer_message_auto_message')): ?><div id="footermsg"><?php print bella_override_footer_message(); endif; ?></div><!--/#footermsg-->
      </div><!--/#footertd-->
    </div><!--/#footertr-->
  </div><!--/#footer-->
  </div><!--/#wrap-->
