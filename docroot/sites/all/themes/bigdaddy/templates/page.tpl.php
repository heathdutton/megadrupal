<!-- begin HEADER -->
<header class="site-header" role="banner">
  <div class="container">

    <?php if ($logo): ?>
      <a href="<?php print check_url($front_page); ?>" class="logo"><img src="<?php print check_url($logo); ?>" alt="<?php print $site_name; ?>" /></a>
    <?php endif; ?>
    <?php if ($site_name): ?>
      <h2 class="site-name"><a href="<?php print check_url($front_page); ?>"><?php print check_plain($site_name); ?></a></h2>
    <?php endif; ?>
    <?php if ($site_slogan): ?>
      <h3 class="site-slogan"><?php print check_plain($site_slogan); ?></h3>
    <?php endif; ?>

    <?php print render($page['header']); ?>

  </div>
</header>
<!-- end HEADER -->

<!-- begin MIDDLE CONTENT -->
<div id="#main-content" class="middle-content" role="main">
  <div class="container">
    <?php print $messages; ?>
    <?php if (isset($tabs['#primary'][0]) || isset($tabs['#secondary'][0])): ?>
      <nav class="tabs"><?php print render($tabs); ?></nav>
    <?php endif; ?>
    <?php if ($action_links): ?>
      <ul class="action-links"><?php print render($action_links); ?></ul>
    <?php endif; ?>
    <?php print render($page['content_top']); ?>
    <?php print render($page['content']); ?>
    <?php print render($page['content_bottom']); ?>
  </div>
</div>
<!-- end MIDDLE CONTENT -->

<?php if ($page['sidebar_second']): ?>
<!-- begin SIDEBAR SECOND -->
<aside class="sidebar-second" role="complementary">
  <?php print render($page['sidebar_second']); ?>
</aside>
<!-- end SIDEBAR SECOND -->
<?php endif; ?>

<?php if ($page['footer']): ?>
<!-- begin FOOTER -->
<footer class="site-footer" role="contentinfo" class="clearfix">
  <div class="container">
    <?php print render($page['footer']); ?>
  </div>
</footer>
<!-- end FOOTER -->
<?php endif; ?>