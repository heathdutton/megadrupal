<?php

/**
 * @file
 * Implementation to display a single Drupal page while offline.
 *
 * All the available variables are mirrored in page.tpl.php.
 *
 * @see template_preprocess()
 * @see template_preprocess_maintenance_page()
 * @see portal_theme_process_maintenance_page()
 */
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language; ?>" lang="<?php print $language->language; ?>" dir="<?php print $language->dir; ?>">
<head>
  <?php print $head; ?>
  <title><?php print $head_title; ?></title>
  <?php print $styles; ?>
  <?php print $scripts; ?>
</head>
<body class="<?php print $classes; ?>" <?php print $attributes;?>>

  <div id="skip-link">
    <a href="#main-content" class="element-invisible element-focusable"><?php print t('Skip to main content'); ?></a>
  </div>

  <div id="header">
  <div class="section">
    <div id="site-logo-and-name">
      <?php if ($logo): ?>
        <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home" id="logo">
          <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
        </a>
      <?php endif; ?>
    </div> <!-- /#site-logo-and-name -->
  </div> <!-- /.section -->
</div> <!-- /#header -->

<div id="page-wrapper"><div id="page">
  <div id="main">
    <a id="main-content"></a>
    <?php print $messages; ?>
    <div id="content">
      <?php print render($page['help']); ?>
      <?php print $content; ?>
			</div> <!-- /#content -->
		</div> <!-- /#main -->
	</div> <!-- /#page -->
</div> <!-- /#page-wrapper -->
<div id="footer">
	<div class="section">
			Content &copy; 2013 <a href="http://clientdomain.here" title="Your Client's Company">Your Client's Company</a>
				<br />
			Design &amp; Customisation &copy; 2013 <a href="http://yourdomain.here" title="Your Design Company">Your Design Company</a>
				<br />
			Powered by <a href="http://drupal.org" title="Drupal">Drupal</a><?php if ($portal_themecivicrm): ?> &amp; <a href="http://civicrm.org" title="CiviCRM 4.2.7">CiviCRM 4.2.7</a><?php endif; ?>.		
	</div>

	<div id="footerleft">
		<a href="http://drupal.org" title="Powered By Drupal"><img class="poweredby" src="<?php print base_path() . drupal_get_path('theme', 'portal_theme'); ?>/images/poweredby.drupal.png" alt="Powered By Drupal">
		<br />
		<a href="http://mjco.me.uk" title="Based on Portal Theme created by MJCO"><img class="linkback" src="<?php print base_path() . drupal_get_path('theme', 'portal_theme'); ?>/images/linkback.logo.png" alt="Based on Portal Theme created by MJCO" /></a>
		<?php if ($portal_themecivicrm): ?>
		<br />
		<a href="http://civicrm.org" title="Powered By CiviCRM"><img class="poweredby" src="<?php print base_path() . drupal_get_path('theme', 'portal_theme'); ?>/images/poweredby.civicrm.png" alt="Powered By CiviCRM" /></a>
		<?php endif; ?>
	</div>
		
		
	</div>
</div>

</body>
</html>
