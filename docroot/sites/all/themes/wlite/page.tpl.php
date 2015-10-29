
<div id="primary_menu_bar">
<?php
if ($main_menu) {
$pid = variable_get('menu_main_links_source', 'main-menu');
$tree = menu_tree($pid);
$main_menu = drupal_render($tree);
}else{
$main_menu = FALSE;
}
?>
<?php
if ($main_menu): print $main_menu; endif;
?>

</div>



<div id="container">
	<div id="header" class="clearfix">
	    
		<?php if ($logo) { ?><div id="logocontainer"><a href="<?php print $base_path ?>" title="<?php print t('Home') ?>"><img src="<?php print $logo ?>" alt="<?php print t('Home') ?>" /></a></div><?php } ?>
		<?php if($site_name || $site_slogan) { ?>
			<div id="texttitles">
			  <?php if ($site_name) { ?><h1 class='site-name'><a href="<?php print $base_path ?>" title="<?php print t('Home') ?>"><?php print $site_name ?></a></h1><?php } ?>
		      <?php if ($site_slogan) { ?><div class='site-slogan'><?php print $site_slogan ?></div><?php } ?>
			</div>
		<?php } ?>
		
		

	</div>
	
	<div id="wrap" class="clearfix <?php if(!$page['left_sidebar'] && !$page['right_sidebar']) print ' no-sidebar '; elseif($page['left_sidebar'] && !$page['right_sidebar']) print ' only-left '; elseif(!$page['left_sidebar'] && $page['right_sidebar']) print ' only-right '?>" >
	
		<?php if($page['left_sidebar']) { ?>
		<div id="left_sidebar"><?php print render($page['left_sidebar'])?></div>
		<?php } ?>
		
		<div id="contentarea">
			<?php if ($page['highlighted']): print '<div id="mission">'. render($page['highlighted']) .'</div>'; endif; ?>
			
			<?php if($page['content_top']) {?>
	        	<div id="content_top"><?php print render($page['content_top']); ?></div>
	        <?php } ?>
			
			<?php if($breadcrumb){ ?><?php print $breadcrumb ?><?php } ?>
			<?php print render($title_suffix); ?>
			<?php if($title){ ?><h1 class="title"><?php print $title ?></h1><?php } ?>
			<?php print render($title_prefix); ?>
	        <?php if($tabs){ ?><div class="tabs"><?php print render($tabs) ?></div> <?php } ?>
	        <?php if ($show_messages) { print $messages; } ?>
	        <?php if($page['help']){ ?><?php print render($page['help']) ?><?php } ?>
	        <?php print render($page['content']); ?>
	        <?php print $feed_icons ?>
	        
	        <?php if($page['under_content']) {?>
	        	<div id="under_content"><?php print $page['under_content']; ?></div>
	        <?php } ?>
		</div>
		
		<?php if($page['right_sidebar']) { ?>
		<div id="right_sidebar"><?php print render($page['right_sidebar']) ?></div>
		<?php } ?>
		
	</div>
	
</div>

<div id="container_bottom"></div>

<div id="footer"><?php print render($page['footer']) ?><!-- Please do not remove this credit line. This encourage us to update, contribute more themes to the community. --><span class="credit">Powered by <a href="http://www.worthapost.com/" title="Drupal themes">Drupal Themes</a>, <a href="http://drupal.org/">Drupal</a>. </span></div>

<?php print render($page['page_bottom']) ?>

