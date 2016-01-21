<?php
/**
 * @file
 * Theme implementation to display a single Drupal page.
 *
 * @see template_preprocess()
 * @see template_preprocess_page()
 * @see portal_theme_preprocess_page()
 */
?>
<div id="header">
  <div class="section">
    <div id="site-logo-and-name">
      <?php if ($logo): ?>
        <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home" id="logo">
          <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
        </a>
      <?php endif; ?>
    </div> <!-- /#site-logo-and-name -->
		<?php if ($page['header']) : ?>
     <?php print drupal_render($page['header']); ?>
		<?php endif; ?>
  </div> <!-- /.section -->
</div> <!-- /#header -->

<div id="page-wrapper">
	<div id="page">
		<?php if($page['highlight']) : ?>
		<div id="highlight">
			<?php print drupal_render($page['highlight']); ?>
		</div>
		<?php endif; ?>
		<div id="main">
			<a id="main-content"></a>
			<?php print $messages; ?>
			<div id="content">
				<?php print render($title_prefix); ?>
				<?php if ($title): ?>
				<h1 class="title" id="page-title">
					<?php print $title; ?>
				</h1>
				<?php endif; ?>
				<?php print render($title_suffix); ?>
				<?php if ($action_links): ?>
				<ul class="action-links">
					<?php print render($action_links); ?>
				</ul>
				<?php endif; ?>
				<?php if ($tabs): ?>
				<div class="tabs">
					<?php print render($tabs); ?>
				</div>
				<?php endif; ?>
				<?php print render($page['help']); ?>
				<?php print render($page['content']); ?>
				<?php print $feed_icons; ?>
			</div> <!-- /#content -->
		</div> <!-- /#main -->
	</div> <!-- /#page -->
</div> <!-- /#page-wrapper -->
<div id="footer">
	<div class="section">
			Content &copy; 2013 <a href="<?php print $portal_themeclienturl; ?>" title="<?php print $portal_themeclientname; ?>"><?php print $portal_themeclientname; ?></a>
				<br />
			Design &amp; Customisation &copy; 2013 <a href="<?php print $portal_themedesignerurl; ?>" title="<?php print $portal_themedesignername; ?>"><?php print $portal_themedesignername; ?></a>
			<?php if ($portal_themeattribution): ?>
				<br />
			Powered by <a href="http://drupal.org" title="Drupal">Drupal</a><?php if ($portal_themecivicrm): ?> &amp; <a href="http://civicrm.org" title="CiviCRM 4.2.7">CiviCRM 4.2.7</a><?php endif; ?>.
			<?php endif; ?>
	</div>
	<?php if ($portal_themeattribution): ?>
		<div id="footerleft">
			<a href="http://drupal.org" title="Powered By Drupal"><img class="poweredby" src="<?php print base_path() . drupal_get_path('theme', 'portal_theme'); ?>/images/poweredby.drupal.png" alt="Powered By Drupal">
        <br />
			<a href="http://mjco.me.uk" title="Based on Portal Theme created by MJCO"><img class="linkback" src="<?php print base_path() . drupal_get_path('theme', 'portal_theme'); ?>/images/linkback.logo.png" alt="Based on Portal Theme created by MJCO" /></a>
			<?php if ($portal_themecivicrm): ?>
        <br />
			<a href="http://civicrm.org" title="Powered By CiviCRM">
				<img class="poweredby" src="<?php print base_path() . drupal_get_path('theme', 'portal_theme'); ?>/images/poweredby.civicrm.png" alt="Powered By CiviCRM" />
			</a>
			<?php endif; ?>
		</div>
	<?php endif; ?>
	</div>
</div>
