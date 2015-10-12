<div id="pagewidthHead">
	<div id="headerHead">
		<div id="logo">
			<?php if ($logo) : ?>
			<div class="logo">
				<a href="<?php print $front_page ?>" title="<?php print t('Home') ?>"><img src="<?php print($logo) ?>" alt="<?php print t('Home') ?>" border="0" /></a>
			</div>
			<?php endif; ?>
			<?php if ($site_name || $site_slogan) : ?>
				<div class="sitename">
					<?php if ($site_name) { ?><h1 class='site-name'><a href="<?php print $base_path ?>" title="<?php print $site_name ?>"><?php print $site_name ?></a></h1><?php } ?>
					<?php if ($site_slogan) { ?><h2 class='site-slogan'><?php print $site_slogan ?></h2><?php } ?>
				</div>
			<?php endif; ?>
		</div>
		<div id="HeaderSearch">
			<?php //print $search_box ?>
		</div>
	</div>
	<div id="wrapperHead" class="clearfixHead">
		<div class="headBoxTop">
			<?php print theme('links', array('links' => $main_menu, 'attributes' => array('id' => 'navlist', 'class' => array('linksTop', 'clearfix')), 'header' => array('text' => t('Main menu'), 'level' => 'h2', 'class' => array('element-invisible'))));  ?>
			</div>
		<div class="headerBg">
		<div id="maincolHead" class="banner<?php print rand(1,3); // set number of random banners ?>">
			</div>
		<div id="primaryHead">
			<?php print render($page['topleft']); ?>
		</div>
		</div>
		<div class="headBoxBottom">
		</div>
	</div>
</div>
<div id="pagewidth">
	<div id="wrapper" class="clearfix">
		<div class="bodyBoxTop">
		</div>
		<div id="maincolBody">
			<div class="mainColContent">
			<?php if($title) { ?><h1 class="pagetitle"><?php print $title ?></h1><?php } ?>
				<?php print $breadcrumb ?>
				
				<?php if ($tabs): ?><div class="tabs"><?php print render($tabs); ?></div><?php endif; ?>
				<div class="help"><?php print render($page['help']); ?></div>
				<?php print $messages ?>
				<?php print render($page['content']); ?>
				<?php print $feed_icons; ?>
			</div>
			<div class="mainColBottom">
			</div>
		</div>
		<div id="leftCol">
			<div class="leftColContent">
				<?php print render($page['left']); ?>
			</div>
			<div class="leftColBottom">
			</div>
		</div>
	</div>
	<div class="footer">
		<?php print render($page['footer']); ?> 
		<br/><a href="http://www.nigraphic.com"> Design by niGraphic</a><br />
	</div>
</div>