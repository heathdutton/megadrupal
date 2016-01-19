<!-- Wrapper / Start -->
<div id="wrapper">
  <!-- Header
  ================================================== -->
  <div id="top-line"></div>

  <?php if (($page['main_menu'])): ?>
    <!-- Navigation
    ================================================== -->
<div class="ten text_center">
  <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home" id="logo">
            <img class="logo" src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
          </a>
<h2 class="display_inline">
    <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><span><?php print $site_name; ?></span></a>
</h2>
<div class="clear"></div>

<?php if ($site_slogan): ?>
      <h5 class="site_slogan">
          <?php print $site_slogan; ?>
      </h5>
      <?php endif; ?>
</div>

    <nav id="navigation" class="navbar replaceme">
       <div class="container">

      <div class="left-corner"></div>
      <div class="right-corner"></div>

      <?php print render($page['main_menu']); ?>
       </div>
    </div>
    </nav>
    <div class="clearfix"></div>
  <?php endif; ?>



  <!-- Content
  ================================================== -->
  <div id="content">

    <?php if ($page['highlighted']): ?>
      <div class="container">
        <div class="sixteen columns">
          <div id="highlighted">
            <?php print render($page['highlighted']); ?>
          </div>
        </div>
      </div>
    <?php endif; ?>


    <?php if ($title): ?>
      <?php if (!drupal_is_front_page()): ?>
        <!-- 960 Container -->
        <div class="container floated">
          <div class="sixteen floated page-title">

            <h2 class="title_page"><?php print $title; ?></h2>


            <?php if ($breadcrumb): ?>
              <nav id="breadcrumbs">

                <?php print $breadcrumb; ?>
              </nav>
            <?php endif; ?>

          </div>
        </div>
        <!-- 960 Container / End -->
      <?php endif; ?>
    <?php endif; ?>

    <?php if (drupal_is_front_page() && !theme_get_setting('homepage_title', 'light_skeleton')): ?>
      <div class="container floated">
        <div class="sixteen floated">

          <h2 class="title_page"><?php print $title; ?></h2>


          <?php if ($breadcrumb): ?>
            <nav id="breadcrumbs">

              <?php print $breadcrumb; ?>
            </nav>
          <?php endif; ?>

        </div>
      </div>
    <?php endif; ?>



    <!-- 960 Container -->
    <div class="container floated <?php print $containner_class; ?>">

      <!-- Page Content -->
      <div class="<?php print $content_class; ?>">
        <section class="page-content">
          <?php print $messages; ?>


          <a id="main-content"></a>
          <?php print render($title_prefix); ?>
          <?php print render($title_suffix); ?>
          <?php if ($tabs): ?><div class="tabs"><?php print render($tabs); ?></div><?php endif; ?>
          <?php print render($page['help']); ?>
          <?php if ($action_links): ?><ul class="action-links"><?php print render($action_links); ?></ul><?php endif; ?>
          <?php print render($page['content']); ?>
          <?php print $feed_icons; ?>
        </section>
      </div>
      <!-- Page Content / End -->

      <div class="clearfix"></div>

    </div>
    <!-- 960 Container / End -->

  </div>
  <!-- Content / End -->

</div>
<!-- Wrapper / End -->


<!-- Footer
================================================== -->

<?php if ($page['footer_first'] || $page['footer_second'] || $page['footer_third']): ?>
  <!-- Footer / Start -->
  <footer id="footer">
    <!-- 960 Container -->
    <div class="container">

      <?php if ($page['footer_first']): ?>
        <div class="four columns left">
          <?php print render($page['footer_first']); ?>
        </div>
      <?php endif; ?>

      <?php if ($page['footer_second']): ?>
        <div class="four columns">
          <?php print render($page['footer_second']); ?>
        </div>
      <?php endif; ?>

      <?php if ($page['footer_third']): ?>
        <div class="four columns right">
          <?php print render($page['footer_third']); ?>
        </div>
      <?php endif; ?>

    </div>
    <!-- 960 Container / End -->

  </footer>
  <!-- Footer / End -->
<?php endif; ?>


<!-- Footer Bottom / Start  -->
<footer id="footer-bottom">

  <!-- 960 Container -->
  <div class="container">
    <div class="sixteen columns">
      <?php print render($page['footer']); ?>
    </div>
  </div>
  <!-- 960 Container / End -->

</footer>
<!-- Footer Bottom / End -->
