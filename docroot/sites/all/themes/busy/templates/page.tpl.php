<?php
?>
    <div id="container">
      <div id="header-wrapper">
        <div id="header-top">
          <div id="logo-floater">
            <?php if ($logo || $site_title): ?>
            <div id="branding" class="clearfix">
              <a href="<?php print $front_page ?>" title="<?php print $site_name_and_slogan ?>">
                <?php if ($logo): ?>
                <img src="<?php print $logo ?>" alt="<?php print $site_name_and_slogan ?>" id="logo" />
                <?php endif; ?>
                <span class="site-title"><?php print $site_name ?></span>
              </a>
            </div>
            <?php else: /* Use h1 when the content title is empty */ ?>
            <h1 id="branding">
              <a href="<?php print $front_page ?>" title="<?php print $site_name_and_slogan ?>">
                <?php if ($logo): ?>
                <img src="<?php print $logo ?>" alt="<?php print $site_name_and_slogan ?>" id="logo" />
                <?php endif; ?>
              </a>
            </h1>
            <?php endif; ?>
          </div>
          <?php if ($page['header_top_right']): ?>
          <div id="header-top-right" class="clearfix">
            <?php print render($page['header_top_right']); ?>
          </div>
          <?php endif; ?>
        </div>
        <div id="header" class="clearfix">
          <?php if ($page['header_left']): ?>
          <div id="header-left">
            <?php print render($page['header_left']); ?>
          </div>
          <?php endif; ?>
          <?php if ($page['header_right'] || $site_slogan): ?>
          <div id="header-right">
            <div id="site-slogan">
              <?php print $site_slogan ?>
            </div>
            <?php print render($page['header_right']); ?>
          </div>
          <?php endif; ?>
        </div>
      </div>
      <div id="main-wrapper">
        <div id="main" class="clearfix">
          <div id="content"<?php print ($main_menu ? ' class="has-main-menu"' : ''); ?>>
            <?php if ($main_menu): ?>
            <div id="navigation">
              <div class="section">
                <?php print theme('links__system_main_menu', array('links' => $main_menu, 'attributes' => array('id' => 'main-menu', 'class' => array('links', 'clearfix')))); ?>
              </div>
            </div>
            <?php endif; ?>
            <div id="content-area">
              <?php print $breadcrumb; ?>
              <?php if ($page['highlight']): ?>
              <div id="highlight"><?php print render($page['highlight']); ?></div>
              <?php endif; ?>
              <a id="main-content"></a>
              
              <div id="tabs-wrapper" class="clearfix">
              
                <?php print render($title_prefix); ?>
                <?php if ($title): ?>
                <h1 class="with-tabs"><?php print $title ?></h1>
                <?php endif; ?>
                <?php print render($title_suffix); ?>
              <?php if ($tabs): ?>
                <?php print render($tabs); ?>
              <?php endif; ?>
              </div>
             
              <?php print $messages; ?>
              <?php print render($page['help']); ?>
              <?php if ($action_links): ?><ul class="action-links"><?php print render($action_links); ?></ul><?php endif; ?>
              <div class="clearfix">
                <?php print render($page['content']); ?>
              </div>
              <?php print $feed_icons ?>
            </div>
          </div>
          <?php if ($page['sidebar_first']): ?>
          <div class="sidebar-first sidebar">
            <?php print render($page['sidebar_first']); ?>
            <?php print theme('links__system_secondary_menu', array('links' => $secondary_menu, 'attributes' => array('id' => 'secondary-menu', 'class' => array('links', 'clearfix')))); ?>
          </div>
          <?php endif; ?>
        </div>
      </div>
      <div id="page-footer" class="clearfix">
        <?php print render($page['footer']); ?>
      </div>
    </div>
