<?php
/**
 * @file
 * Custom theme implementation to display a single Drupal page.
 */
?>
<!-- Header -->
<header id="header">

  <?php if ($logo): ?>
    <h1><a href="<?php print $front_page ?>">
        <img src="<?php print $logo ?>" alt="<?php print $site_name_and_slogan ?>" title="<?php print $site_name_and_slogan ?>" id="logo" />
      <?php endif; ?>
    </a></h1>
  <?php if ($navbar_menu): ?>
    <nav id="nav">
      <?php print $navbar_menu ?>
    </nav>
  <?php endif ?>
</header>
<!-- Main -->
<section id="main" class="container">
  <?php print render($page['content']) ?>
  <?php print render($page['content-top']) ?>
  <?php print render($page['content-bottom']) ?>
  <div class="row">
    <div class="6u">
      <?php if ($page['content-left']): ?>
        <?php print render($page['content-left']) ?>
      <?php endif ?>
    </div>
    <div class="6u">
      <?php if ($page['content-right']): ?>
        <?php print render($page['content-right']) ?>
      <?php endif ?>
    </div>
  </div>
</section>

<!-- Footer -->
<footer id="footer">
  <?php if ($page['footer']): ?>
    <?php print render($page['footer']) ?>
  <?php endif ?>
  <ul class="fa-icons">
    <li><a href="<?php print $front_page; ?>/rss.xml"><img src="<?php print base_path() . drupal_get_path('theme', 'html5_simplified') . '/images/rss.png'; ?>" alt="RSS Feed"/></a></li>
    <li><a href="http://www.facebook.com/<?php echo theme_get_setting('facebook_username', 'html5_simplified'); ?>" target="_blank" rel="me"><img src="<?php print base_path() . drupal_get_path('theme', 'html5_simplified') . '/images/facebook.png'; ?>" alt="Facebook"/></a></li>
    <li><a href="http://www.twitter.com/<?php echo theme_get_setting('twitter_username', 'html5_simplified'); ?>" target="_blank" rel="me"><img src="<?php print base_path() . drupal_get_path('theme', 'html5_simplified') . '/images/twitter.png'; ?>" alt="Twitter"/></a></li>
  </ul>
  <ul class="copyright">
    <li>&copy; Untitled. All rights reserved.</li><li>Design: <a href="https://www.drupal.org/u/ameymudras">Amey</a></li>
  </ul>
</footer>
