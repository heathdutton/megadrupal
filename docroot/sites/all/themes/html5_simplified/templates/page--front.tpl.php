<?php
/**
 * @file
 * Custom theme implementation to display a single Drupal page.
 */
?>
<!-- Header -->
<header id="header" class = "alt">
  <?php if ($logo): ?>
    <h1><a href="<?php print $front_page ?>">
        <img src="<?php print $logo ?>" alt="<?php print t('Home'); ?>" title="<?php print t('Home'); ?>" id="logo" />
      <?php endif; ?></a>
    </h1>
  <?php if ($main_menu): ?>
    <div class="menu_wrapper">
      <nav id="nav"  role="navigation">
        <div class="menu-navigation-container">
          <?php
          print theme('links__system_main_menu', array(
            'links' => $main_menu,
            'attributes' => array(
              'id' => 'main-menu-links',
              'class' => array('links', 'clearfix'),
            ),
            'heading' => array(
              'text' => t('Main menu'),
              'level' => 'h2',
              'class' => array('element-invisible'),
            ),
          ));
          ?>
        </div>
        <div class="clear"></div>
      </nav><!-- end main-menu -->
    </div>
  <?php endif ?>
</header>

<!-- Banner -->
<section id="banner">
  <?php if ($site_name): ?>
    <h2><?php print $site_name; ?></h2>
  <?php endif; ?>
  <?php if ($site_slogan): ?>
    <p id="site-slogan">
      <?php print $site_slogan; ?>
    </p>
  <?php endif; ?>
  <ul class="actions">
    <li><a href="/user/login" class="button special">Sign Up</a></li>
    <li><a href="#" class="button">Learn More</a></li>
  </ul>
  <?php if ($page['banner']): ?>
    <?php print render($page['banner']) ?>
  <?php endif ?>
</section>
<!-- Main -->
<section id="main" class="container">
  <?php if (theme_get_setting('demo_content') || $page['content-top']) : ?>
    <section class="box special">
      <?php if ($page['content-top']): ?>
        <?php print render($page['content-top']) ?>
      <?php elseif (theme_get_setting('demo_content')) : ?>
        <header class="major">
          <h2>Introducing the ultimate mobile app<br />
            for doing stuff with your phone</h2>
          <p>Blandit varius ut praesent nascetur eu penatibus nisi risus faucibus nunc ornare<br />
            adipiscing nunc adipiscing. Condimentum turpis massa.</p>
        </header>
        <span class="image featured"><img src="sites/all/themes/html5_simplified/images/pic01.jpg" alt="" /></span> 
      <?php endif; ?>
    </section>
  <?php endif; ?>

  <section>
    <?php if (theme_get_setting('demo_content') || $page['content-bottom']) : ?>
      <?php if ($page['content-bottom']): ?>
        <?php print render($page['content-bottom']) ?>
      <?php elseif (theme_get_setting('demo_content')) : ?>
        <section class="box special features">
          <div class="features-row">
            <section>
              <span class="icon major fa-bolt accent2"></span>
              <h3>Magna etiam</h3>
              <p>Integer volutpat ante et accumsan commophasellus sed aliquam feugiat lorem aliquet ut enim rutrum phasellus iaculis accumsan dolore magna aliquam veroeros.</p>
            </section>
            <section>
              <span class="icon major fa-area-chart accent3"></span>
              <h3>Ipsum dolor</h3>
              <p>Integer volutpat ante et accumsan commophasellus sed aliquam feugiat lorem aliquet ut enim rutrum phasellus iaculis accumsan dolore magna aliquam veroeros.</p>
            </section>
          </div>
          <div class="features-row">
            <section>
              <span class="icon major fa-cloud accent4"></span>
              <h3>Sed feugiat</h3>
              <p>Integer volutpat ante et accumsan commophasellus sed aliquam feugiat lorem aliquet ut enim rutrum phasellus iaculis accumsan dolore magna aliquam veroeros.</p>
            </section>
            <section>
              <span class="icon major fa-lock accent5"></span>
              <h3>Enim phasellus</h3>
              <p>Integer volutpat ante et accumsan commophasellus sed aliquam feugiat lorem aliquet ut enim rutrum phasellus iaculis accumsan dolore magna aliquam veroeros.</p>
            </section>
          </div>
        </section>
      <?php endif; ?>
    <?php endif; ?>
  </section>

  <div class="row">
    <div class="6u">
      <?php if (theme_get_setting('demo_content') || $page['content-left']) : ?>
        <section class="box special">
          <?php if ($page['content-left']): ?>
            <?php print render($page['content-left']) ?>
          <?php elseif (theme_get_setting('demo_content')): ?>
            <span class="image featured"><img src="sites/all/themes/html5_simplified/images/pic02.jpg" alt="" /></span>
            <h3>Sed lorem adipiscing</h3>
            <p>Integer volutpat ante et accumsan commophasellus sed aliquam feugiat lorem aliquet ut enim rutrum phasellus iaculis accumsan dolore magna aliquam veroeros.</p>
            <ul class="actions">
              <li><a href="#" class="button alt">Learn More</a></li>
            </ul>

          <?php endif ?>
        </section>
      <?php endif ?>
    </div>
    <div class="6u">
      <?php if (theme_get_setting('demo_content') || $page['content-right']) : ?>
        <section class="box special">
          <?php if ($page['content-right']): ?>
            <?php print render($page['content-right']) ?>
          <?php elseif (theme_get_setting('demo_content')): ?>
            <span class="image featured"><img src="sites/all/themes/html5_simplified/images/pic03.jpg" alt="" /></span>
            <h3>Accumsan integer</h3>
            <p>Integer volutpat ante et accumsan commophasellus sed aliquam feugiat lorem aliquet ut enim rutrum phasellus iaculis accumsan dolore magna aliquam veroeros.</p>
            <ul class="actions">
              <li><a href="#" class="button alt">Learn More</a></li>
            </ul>
          <?php endif ?>
        </section>
      <?php endif ?>
    </div>
  </div>
</section>

<!-- CTA -->
<section id="cta">

  <h2>Sign up for beta access</h2>
  <p>Blandit varius ut praesent nascetur eu penatibus nisi risus faucibus nunc.</p>
  <?php if ($page['signup']): ?>
    <?php print render($page['signup']) ?>
  <?php endif ?>

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
    <li>&copy; Untitled. All rights reserved.</li>
  </ul>
</footer>
