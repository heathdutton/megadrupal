<?php
// $Id: page.tpl.php $
?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language ?>" lang="<?php print $language->language ?>" dir="<?php print $language->dir ?>">
<head>
  <?php print $head ?>
  <title><?php print $head_title ?></title>
  <?php print $styles ?>
  <?php print $scripts ?>
</head>
<body id="mainBody">
	<div id="pagewidth" class="clearfix">
		<div id="header">
			<div class="logo">
				<?php if ($logo) : ?>
				  <a class='site-logo' href="<?php print $front_page ?>" title="<?php print t('Home') ?>"><img src="<?php print($logo) ?>" alt="<?php print t('Home') ?>" border="0" /></a>
        <?php endif; ?>
        <?php if ($site_name) : ?>
          <div class='site-name'><h1><a href="<?php print $front_page ?>" title="<?php print t('Home') ?>"><?php print($site_name) ?></a></h1></div>
        <?php endif;?>
        <?php if ($site_slogan) : ?>
          <div class='site-slogan'><h2 class='site-slogan'><?php print($site_slogan) ?></h2></div>
        <?php endif;?>
			</div>
			<?php if ($header): ?>
		    <div class="ad"><?php print ($header) ?></div>
		  <?php endif; ?>
			<ul id="navigation">
			<?php if ($primary_links): ?>
        <div id="primary-menu">
          <?php print $primary_links_tree; ?>
        </div><!-- /primary links menu -->
      <?php endif; ?>
			  <li class="rss"><a href="rss.xml"></a></li>
			</ul>
		</div>
		<div id="content">
			<div id="left">
				<div class="node-post">
			      <?php print $breadcrumb; ?>
                  <?php if ($mission): print '<div id="mission">'. $mission .'</div>'; endif; ?>
                  <?php if ($tabs): print '<div id="tabs-wrapper" class="clear-block">'; endif; ?>
                  <?php if ($title): print '<h2'. ($tabs ? ' class="with-tabs"' : '') .'>'. $title .'</h2>'; endif; ?>
                  <?php if ($tabs): print '<ul class="tabs primary">'. $tabs .'</ul></div>'; endif; ?>
                  <?php if ($tabs2): print '<ul class="tabs secondary">'. $tabs2 .'</ul>'; endif; ?>
                  <?php if ($show_messages && $messages): print $messages; endif; ?>
                  <?php print $help; ?>
                  <div class="clear">
                  <?php print $content ?>
                  </div>
				</div>
			</div>
			<div id ="right">
			<div id="sidebar">
			  <div id="searchform" class="search_widget">
	          <?php print $search_box ?>
			  </div>
			  <?php print $sidebar ?>
			  <div id="creditsfix"></div>
			</div>
			<ul id="credits">
			</ul>
			</div>
		</div>
		<div id="footer">
		  <div class="left"><?php print $footer_message . $footer ?>
	      </div>
	      <div class="right"><a href="http://drupalservers.net" title="Drupal Themes">Drupal Themes</a> by <a href="http://arborwebdevelopment.com" title="Arbor Web Development">Arbor Web Development</a>
	      </div>
		</div>
	</div>
	
	<?php print $closure ?>
</body>
</html>