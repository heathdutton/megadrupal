<div id="page" class="container-12 clearfix">
  <div id="banner" class="grid-12 clearfix">
    <a href="<?php print $front_page; ?>" title="Back to <?php print $site_name; ?> home page">      <img src="<?php print $logo; ?>" alt="Back to <?php print $site_name; ?> home page." /></a>
  </div>
  <div id="main" class="grid-12 clearfix">
    <div id="nav" class="grid-2 alpha">
      <a href="<?php print $front_page; ?>" title="Back to <?php print $site_name; ?> home page">
      <h1 id="site-name"><img src="<?php print $base_path . $directory; ?>/site-name.jpg" alt="At Home" /></h1></a>
      <?php print render($page['nav_left']); ?>
    </div>
    <div id="feature" class="grid-4">
      <div class="decoration"><img src="<?php print $base_path . $directory; ?>/feature-decoration-top.jpg" alt="Furniture, Linen, Dishes" /></div>
      <?php print render($page['feature_middle']); ?>
      <div class="decoration"><img src="<?php print $base_path . $directory; ?>/feature-decoration-bottom.jpg" alt=" " /></div>
    </div>
    <div id="content" class="grid-6 omega">
      <?php print render($title_prefix); ?>
      <?php if ($title): ?>
        <h1 class="title" id="page-title"><?php print $title; ?></h1>
      <?php endif; ?>
      <?php print render($title_suffix); ?>      
      <?php if ($tabs): ?>
        <div class="tabs"><?php print render($tabs); ?></div>
      <?php endif; ?>
      <?php print $messages; ?>
      <?php print render($page['help']); ?>
      <?php print render($page['content']);?>
      <?php print $feed_icons; ?>
</div>
  </div>

  <div id="copyright" class="grid-12 clearfix"><?php print render($page['copyright_footer']); ?>
  <div><p>Graphic Design by <a href="http://design-house.ca">Design House</a>.</p></div>
  </div>
</div>
