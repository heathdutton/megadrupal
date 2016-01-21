<div id="pagewidthHead">
<div id="headerHead">
		<?php if ($logo) : ?>
				<div class="logo"><a href="<?php print $front_page ?>" title="<?php print t('Home') ?>"><img src="<?php print($logo) ?>" alt="<?php print t('Home') ?>" border="0" /></a></div>
		<?php endif; ?>
		<div class="site-name">
			<?php if ($site_name) { ?><h1><a href="<?php print $base_path ?>" title="<?php print $site_name ?>"><?php print $site_name ?></a></h1><?php } ?>
		</div>
		<div class="site-slogan">
			<?php if ($site_slogan) { ?><h2><?php print $site_slogan ?></h2><?php } ?>
		</div>
		<div id="HeaderSearch">
			<?php //print $search_box ?>
		</div>
	</div>
	<div id="wrapperHead" class="clearfixHead">
		<div class="headBoxTop">
			<?php print theme('links', array('links' => $main_menu, 'attributes' => array('id' => 'primary-links', 'class' => array('linksTop', 'clearfix')), 'header' => array('text' => t('Main menu'), 'level' => 'h2', 'class' => array('element-invisible'))));  ?>
		</div>
		<div id="maincolHead">
			<!-- <div class="header-left-block">
					<?php //print $header_left ?>
		    </div> -->
		</div>
		<!--		
		<div class="headBoxBottom">
		</div> 
		<div id="header-blocks">
			<?php //print $header ?>
		</div> 
		-->
	</div>
</div>
<div id="pagewidth">
	<div id="wrapper" class="clearfix">
		<div id="rightcol">
			<div class="mainColContent">
				<?php print render($page['sidebar']); ?>
			</div>
		</div>
		<div id="leftCol">
			<div class="leftColContent">
				<?php print $breadcrumb ?>
				<?php if ($title): ?><h1 class="pagetitle" id="page-title"><?php print $title; ?></h1><?php endif; ?>
				<?php if ($tabs): ?><div class="tabs"><?php print render($tabs); ?></div><?php endif; ?>
				<div class="help"><?php print render($page['help']); ?></div>
				<?php print $messages ?>
				<?php print render($page['content']); ?>
				<?php print $feed_icons; ?>
			</div>
		</div>
	</div>
</div>
<div class="footer">
	 <?php print render($page['footer']); ?> 
		<br/>
		 <a href="http://www.nigraphic.com" title="Web Design &amp; Photography">Design by niGraphic</a>
</div>