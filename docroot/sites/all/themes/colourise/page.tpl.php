<?php

/**
 * @file
 * Colourise's theme implementation to display a page.
 */
?>

<div id="page" class="<?php print $page_class ?>">

 <?php print $nav_access ?>

  <div id="header" class="clearfix">

  <?php if ($site_name): ?>
    <h1 id="site-name">
      <a href="<?php print check_url($front_page) ?>" title="<?php print t('Home') ?>">
        <?php print $site_name ?>
      </a>
    </h1>
  <?php endif; ?>

  <?php if ($site_slogan): ?>
    <p id="slogan"><?php print $site_slogan ?></p>
  <?php endif; ?>

  <?php if ($primary_nav): ?>
    <div id="main-menu">
      <h3 class="hidden">Main Menu</h3>
    <?php print render($primary_nav);?>
    </div>
  <?php endif; ?>

      <?php if ($breadcrumb): ?>
    <?php print $breadcrumb; ?>
  <?php endif; ?>

  </div> <!-- /header -->

  <div id="content-wrapper" class="clearfix">

    <div id="main-content" class="column center">
    <div id="content-inner" class="inner">

     <?php if ($page['highlighted']): ?>
      <?php print render($page['highlighted']); ?>
     <?php endif; ?>

    <?php if ($page['content_top']): ?>
      <div id="top-content-block" class="content-block">
   <?php print render($page['content_top']); ?>
      </div>
    <?php endif; ?>

    <?php if ($title): ?>
      <h1 class="title"><?php print $title ?></h1>
    <?php endif; ?>

    <?php if (!empty($messages)): ?><?php print $messages ?><?php endif; ?>

    <?php if ($page['help']): ?><?php print render($page['help']); ?><?php endif; ?>

    <?php if (!empty($tabs)): ?><?php print render($tabs); ?><?php endif; ?>

    <?php print render($page['content']); ?>

    <?php if ($page['content_bottom']): ?>
      <div id="bottom-content-block" class="content-block">
   <?php print render($page['content_bottom']); ?>
      </div>
    <?php endif; ?>
    <?php if ($breadcrumb): ?>
      <?php print $breadcrumb; ?>
    <?php endif; ?>

    <?php print $feed_icons ?>

    </div> <!-- /content-inner -->
    </div> <!-- /main-content -->

    <?php if ($page['sidebar_first']): ?>
      <div id="sidebar-first" class="column sidebar">
      <div id="sidebar-first-inner" class="inner">
   <?php print render($page['sidebar_first']); ?>
      </div>
      </div> <!-- /sidebar-first -->
    <?php endif; ?>

<?php if ($page['sidebar_second']): ?>
      <div id="sidebar-second" class="column sidebar">
      <div id="sidebar-second-inner" class="inner">
   <?php print render($page['sidebar_second']); ?>
      </div>
      </div> <!-- /sidebar-second -->
    <?php endif; ?>

  </div> <!-- /content-wrapper -->

  <?php if ($page['footer_1'] ||$page['footer_2'] ||$page['footer_3']): ?>
    <div id="footer-column-wrap" class="clearfix">

 <?php if ($page['footer_1']): ?>
        <div class="footer-column">
     <?php print render($page['footer_1']); ?>
        </div>
      <?php endif; ?>

      <?php if ($page['footer_2']): ?>
        <div class="footer-column">
     <?php print render($page['footer_2']); ?>
        </div>
      <?php endif; ?>

      <?php if ($page['footer_3']): ?>
        <div id="last" class="footer-column">
     <?php print render($page['footer_3']); ?>
        </div>
      <?php endif; ?>

    </div> <!-- /footer-column-wrap -->
  <?php endif; ?>

  <div id="footer" class="clearfix">

    <?php print $to_top ?>

  </div> <!-- /footer -->

</div> <!-- /page -->
