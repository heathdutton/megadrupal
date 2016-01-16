<?php
// $Id: page.tpl.php,v 1.20 2010/11/14 01:41:18 danprobo Exp $
?>
<div <?php print simpler_page_class($page['sidebar_first'], $page['sidebar_second']); ?>>
<div id="page">
	<div id="header-wrapper">
	<?php if($secondary_menu): ?>
		<div id="sub-menu">
			<?php print theme('links__system_secondary_menu', array('links' => $secondary_menu, 'attributes' => array('class' => array('menu', 'clearfix')))); ?>
		</div> <!-- end sub menu -->
	<?php endif; ?>
	<?php if($page['search_box']): ?>
		<div id="search-wrapper">
			<div id="search-box">
			<?php print render ($page['search_box']); ?>
			</div>
		</div> <!-- end search wrapper -->
	<?php endif; ?>
	<div id="header">
		<?php if ($logo): ?>
			<div class="logo">
				<a href="<?php print $base_path ?>"><img src="<?php print $logo ?>" alt="<?php print t('Logo') ?>" title="<?php print t('Home') ?>"/></a>
			</div> <!-- end logo -->
		<?php endif; ?>
		<?php if($site_name || $site_slogan): ?>
		<div class="branding">
		<?php if ($site_name): ?>
			<?php if ($is_front) : ?>
			<h1><a href="<?php print $base_path ?>" title="<?php print $site_name ?>"><?php print $site_name ?></a></h1>
			<?php endif; ?>
			<?php if (!$is_front) : ?>
			<h2><a href="<?php print $base_path ?>" title="<?php print $site_name ?>"><?php print $site_name ?></a></h2>
			<?php endif; ?>
		<?php endif; ?>
		<?php if ($site_slogan): ?>
			<span><?php print $site_slogan ?></span>
		<?php endif; ?>
		</div> <!-- end branding -->
		<?php endif; ?>
	</div> <!-- end header -->
	</div> <!-- end header wrapper-->

	<div style="clear:both"></div>

	<div id="menu-wrapper">
	<div id="menu-inner">
	<div id="main-menu">
		<?php if (isset($main_menu)) : ?>
			<?php print theme('links__system_main_menu', array('links' => $main_menu, 'attributes' => array('class' => array('menu', 'clearfix')))); ?>
		<?php endif; ?>
	</div> <!-- end main menu -->
	</div>
	</div> <!-- end menu wrapper -->

	<div style="clear:both"></div>

	<div id="container">
	<div id="container-outer">
	<div id="container-inner">
		<div id="content-wrapper">
			<?php if ($page['sidebar_first']): ?>
      			<div id="sidebar-left" class="column sidebar"><div class="section">
       			 <?php print render($page['sidebar_first']); ?>
      			</div></div> <!-- end sidebar-first -->
    			<?php endif; ?>

			<div id="main-content">
				<?php if ($page['highlighted']) : ?><div class="mission"><?php print render ($page['highlighted']); ?></div><?php endif; ?>
				<?php if ($page['content_top']) : ?><div class="content-top"><?php print render ($page['content_top']); ?></div><?php endif; ?>
				<?php if (!$is_front) print $breadcrumb; ?>
				<?php if ($show_messages) { print $messages; }; ?>
      			<?php print render($title_prefix); ?>
      				<?php if ($title): ?>
        					<h1 class="title" id="page-title">
         			 			<?php print $title; ?>
        					</h1>
     				 	<?php endif; ?>
      			<?php print render($title_suffix); ?>
      			<?php if ($tabs): ?>
        				<div class="tabs">
          					<?php print render($tabs); ?>
        				</div>
      			<?php endif; ?>
      			<?php print render($page['help']); ?>
      			<?php if ($action_links): ?>
        				<ul class="action-links">
          					<?php print render($action_links); ?>
        				</ul>
      			<?php endif; ?>
		      	<?php if ($page['content']) : ?><?php print render ($page['content']); ?><?php endif; ?>
			</div> <!-- end main content -->
    			<?php if ($page['sidebar_second']): ?>
      			<div id="sidebar-right" class="column sidebar"><div class="section">
        				<?php print render($page['sidebar_second']); ?>
      			</div></div> <!-- end sidebar-second -->
    			<?php endif; ?>
		</div> <!-- end content wrapper -->
	</div>
	</div>
	</div> <!-- end container -->

	<div style="clear:both"></div>

	<div id="bottom-wrapper">
		<div id="bottom-inner">
		<?php if($page['bottom_1'] || $page['bottom_2'] || $page['bottom_3'] || $page['bottom_4']) : ?>
    		<div style="clear:both"></div><!-- Do not touch -->
   		<div id="bottom" class="in<?php print (bool) $page['bottom_1'] + (bool) $page['bottom_2'] + (bool) $page['bottom_3'] + (bool) $page['bottom_4']; ?>">
          		<?php if($page['bottom_1']) : ?>
          			<div class="column A">
            			<?php print render ($page['bottom_1']); ?>
          			</div>
          		<?php endif; ?>
          		<?php if($page['bottom_2']) : ?>
          			<div class="column B">
            			<?php print render ($page['bottom_2']); ?>
         			</div>
          		<?php endif; ?>
          		<?php if($page['bottom_3']) : ?>
          			<div class="column C">
            			<?php print render ($page['bottom_3']); ?>
          			</div>
          		<?php endif; ?>
          		<?php if($page['bottom_4']) : ?>
          			<div class="column D">
           				<?php print render ($page['bottom_4']); ?>
          			</div>
          		<?php endif; ?>
      		<div style="clear:both"></div>
		</div> <!-- end bottom -->
    		<?php endif; ?>
		<div style="clear:both"></div>
	<?php if ($page['footer']): ?>
		<div id="footer-wrapper">
		<div id="footer">
			<?php print render ($page['footer']); ?>
		</div> <!-- end footer -->
		</div> <!-- end footer wrapper -->
	<?php endif; ?>
		</div> <!-- end bottom inner -->
		<div id="notice">		
		Theme provided by <a href="http://www.danetsoft.com">Danetsoft</a> under GPL license from <a href="http://www.danpros.com">Danang Probo Sayekti</a>
		</div> <!-- end notice -->
    		</div> <!-- end bottom wrapper -->

<div style="clear:both"></div>

</div> <!-- end page -->
</div>

