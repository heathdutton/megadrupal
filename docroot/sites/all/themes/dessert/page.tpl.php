<?php
?>

<!-- Body Content -->
<!-- Header -->
<div id="header-wrapper">
	<div id="header" class="clearfix">	
	<div id="header-region" class="clearfix">
		<?php print render($page['header']); ?>
	</div>

	<!-- Logo -->
	<span id="sitename">
	<?php if($logo) : ?>
		<a id="logo" href="<?php print $front_page; ?>" title="<?php print $site_name; ?>" rel="home"><img src="<?php print $logo; ?>" alt="<?php print $site_name; ?>" /></a>
	<?php endif; ?>
	<a href="<?php print $front_page; ?>" title="<?php print $site_name; ?>" rel="home">
	  <?php print $site_name; ?>
		<!-- Site Slogan -->
			<span id="slogan"id="site-slogan"><?php print $site_slogan; ?></span>
		<!-- /Site Slogan -->
	</a>
	</span>
	<!-- /Logo -->
	</div>
</div>
<!-- /Header -->
<!-- Primary + Secondary Navigation -->


<div id="navigation" class="menu<?php if ($main_menu) { print " withprimary"; } if ($secondary_menu) { print " withsecondary"; }; ?>">
<?php if ($main_menu): ?>
	<div id="primary" class="clearfix">
		<?php print theme('links__system_main_menu', array('links' => $main_menu, 'attributes' => array('id' => 'main-menu', 'class' => array('links', 'primary-links', 'clearfix')), 
					'heading' => array(
            'text' => t('Main Menu'),
            'level' => 'h2',
            'class' => array('element-invisible'),
          ),
)); ?>
	</div>
<?php endif; ?>
<?php if ($secondary_menu): ?>
	<div id="secondary" class="clearfix">
		<?php print theme('links__system_secondary_menu', array('links' => $secondary_menu, 'attributes' => array('id' => 'secondary-menu', 'class' => array('links', 'secondary-links', 'clearfix')), 'heading' => array(
            'text' => t('Secondary Menu'),
            'level' => 'h2',
            'class' => array('element-invisible'),
          ),
				 )); ?>
	</div>
<?php endif; ?>
</div>


<!-- /Primary + Secondary Navigation -->
<!-- Content + Sidebars -->
<div id="maincontent">
	<?php if($tabs || $title) : ?>
	<div class="node-title<?php if(!empty($tabs)): print " withtabs"; endif; ?> clearfix">
		<?php if($tabs): print render($tabs); endif; ?>
		<?php if(!empty($title)) : ?>		
			<?php print render($title_prefix); ?>
			<h1><?php print $title; ?></h1>
			<?php print render($title_suffix); ?>
		<?php endif; ?>
	</div>
	<?php endif; ?>
	<?php if (!empty($messages)): print $messages; endif; ?>
	<?php if (!empty($page['help'])): print $page['help']; endif; ?>
	<?php //Outputs Number of Sidebars Class for Theming
		if (!empty($page['sidebar_first']) && (!empty($page['sidebar_second']))):
			$sidebar_state = "two-sidebar"; 
		elseif((!empty($page['sidebar_first']) && (empty($page['sidebar_second']))) | (!empty($page['sidebar_second']) && (empty($page['sidebar_first'])))):
			$sidebar_state = "one-sidebar";
		elseif(empty($page['sidebar_first']) && (empty($page['sidebar_second']))):
			$sidebar_state = "no-sidebar";
		endif;
	?>
	<div id="content" class="<?php print $sidebar_state; ?> clearfix">
		<!-- sidebar-left -->
		<?php if (!empty($page['sidebar_first'])): ?>
			<div id="sidebar-left">			
				  <?php print render($page['sidebar_first']); ?>
			</div> 
		<?php endif; ?>
		<!-- /sidebar-left -->
		<!-- main -->
		<div id="main">
			<?php if (!empty($highlighted)): ?>
				<div id="mission"><?php print render($page['highlighted']); ?></div>
			<?php endif; ?>
			<?php print render($page['content']); ?>
			<?php print $feed_icons; ?>
		</div>
		<!-- /main -->
		<!-- sidebar-right -->
		<?php if (!empty($page['sidebar_second'])): ?>
			<div id="sidebar-right">
				<?php print render($page['sidebar_second']); ?>
			</div>
		<?php endif; ?>
		<!-- /sidebar-right -->
			<!-- Breadcrumbs -->
			<?php if (!empty($breadcrumb)): ?>
				<div id="breadcrumb"><?php print $breadcrumb; ?></div>
			<?php endif; ?>
		<!-- Breadcrumbs -->
	</div>
</div>
<!-- /Content + Sidebars -->
<!-- Footer -->
<div id="footer-wrapper">
	<div id="footer">
		<?php if($page['footer']): ?>
			<?php print render($page['footer']); ?>
		<?php endif; ?>
		<div id="credit">
			<?php print t("Drupal Theme by ") . '<a href="' . t('http://www.outcome3.com" title="Vancouver Internet Marketing"') . '>' . t("www.outcome3.com") . '</a>'; ?>
	</div>
</div>
</div>
<!-- /Footer -->
<!-- /Body Content -->
