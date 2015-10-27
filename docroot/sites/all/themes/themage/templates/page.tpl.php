<?php
/**
 * @file
 * Themage's implemenation to display a single Drupal page.
 */
?>
<div id="page">

  <header id="page-header" role="banner" class="clearfix">

    <a href="<?php print $front_page; ?>" rel="home" id="home-link">
      <?php if ($logo): ?>
        <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
      <?php endif; ?>

			<?php if ($site_name || $site_slogan): ?>
				<div id="name-and-slogan">
					<?php if ($site_name): ?>
						<?php if ($title): ?>
							<div id="site-name"><?php print $site_name; ?></div>
						<?php else: /* Use h1 when the content title is empty */ ?>
							<h1 id="site-name"><?php print $site_name; ?></h1>
						<?php endif; ?>
					<?php endif; ?>
					<?php if ($site_slogan): ?>
						<div id="site-slogan"><?php print $site_slogan; ?></div>
					<?php endif; ?>
				</div>
			<?php endif; ?>
    </a>

    <?php if ($page['header']): ?>
      <?php print render($page['header']); ?>
    <?php endif; ?>
  </header>

  <?php if ($main_menu): ?>
    <nav id="page-menu" role="navigation">
      <?php print theme('links__system_main_menu', array('links' => $main_menu, 'attributes' => array('id' => 'main-menu'))); ?>
    </nav>
  <?php endif; ?>

  <?php if ($breadcrumb): ?>
    <nav id="page-breadcrumb" role="navigation"><?php print $breadcrumb; ?></nav>
  <?php endif; ?>

  <?php print $messages; ?>

  <main id="page-main" role="main">

    <header id="main-header">
      <?php if ($page['highlighted']): ?><div id="highlighted"><?php print render($page['highlighted']); ?></div><?php endif; ?>
      <?php print render($title_prefix); ?>
      <?php if ($title): ?><h1 class="title" id="main-title"><?php print $title; ?></h1><?php endif; ?>
      <?php print render($title_suffix); ?>
      <?php if ($tabs): ?><nav class="tabs"><?php print render($tabs); ?></nav><?php endif; ?>
      <?php print render($page['help']); ?>
      <?php if ($action_links): ?><ul class="action-links"><?php print render($action_links); ?></ul><?php endif; ?>
    </header>

    <section id="main-content">
      <?php print render($page['content']); ?>
      <?php print $feed_icons; ?>
    </section> <!-- #main-content -->
  </main>

  <?php if ($page['sidebar_first']): ?>
    <?php print render($page['sidebar_first']); ?>
  <?php endif; ?>

  <?php if ($page['sidebar_second']): ?>
    <?php print render($page['sidebar_second']); ?>
  <?php endif; ?>

  <footer id="page-footer">
    <?php if ($secondary_menu): ?>
      <nav id="footer-menu" role="navigation" class="clearfix">
        <?php print theme('links__system_secondary_menu', array('links' => $secondary_menu, 'attributes' => array('id' => 'secondary-menu'))); ?>
      </nav>
    <?php endif; ?>

    <?php if ($page['footer']): ?>
      <?php print render($page['footer']); ?>
    <?php endif; ?>
  </footer>

</div> <!-- #page -->
