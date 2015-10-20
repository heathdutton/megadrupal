<?php
$sidebar_first = $page['sidebar_first'];
$sidebar_last = $page['sidebar_last'];
$right_dark = $page['right_dark'];
$content_top = $page['content_top'];
$content_bottom = $page['content_bottom'];
$main_banner = $page['main_banner'];
$above_content_ad = $page['above_content_ad'];
$right_ad = $page['right_ad'];
$footer = $page['footer'];
$help = $page['help'];
$highlighted = $page['highlighted'];
$content = $page['content'];
$footer = $page['footer'];
$search_box = $page['search_box'];

?>

<!-- Preload Images (More important) -->
<div class="preload">
	<img class="preload" src="<?php print base_path() . path_to_theme() ?>/images/bg2.jpg" alt=""/>
	<img class="preload" src="<?php print base_path() . path_to_theme() ?>/images/bg2.jpg" alt="" />
	<img class="preload" src="<?php print base_path() . path_to_theme() ?>/images/primary.jpg"  alt="" />
	<img class="preload" src="<?php print base_path() . path_to_theme() ?>/images/search_box.jpg"  alt="" />
	<img class="preload" src="<?php print base_path() . path_to_theme() ?>/images/top_bar.jpg" alt="" />
	<img class="preload" src="<?php print base_path() . path_to_theme() ?>/images/footer_wood.jpg"  alt="" />
</div>


<div id="top_bar">
	<div class="container">
		<div id="top_menu">
<?php if (isset($secondary_menu)) { ?>        
	<?php print theme('links__system_secondary_menu', array(
          'links' => $secondary_menu,
          'attributes' => array(
            'id' => 'secondary-menu-links',
            'class' => array('links', 'inline', 'clearfix'),
          ),
          'heading' => array(
            'text' => t('Secondary menu'),
            'level' => 'h2',
            'class' => array('element-invisible'),
          ),
        )); ?>
        <?php } ?>
		</div>
		
		<div id="topbar_right">
		
			
			
			<div id="feed_icon_grunge">
			<a href="<?php if(!theme_get_setting('admire_grunge_feed_url')){ print $base_path . drupal_get_path_alias('rss.xml'); } else  print theme_get_setting('admire_grunge_feed_url'); ?>"><img src="<?php print base_path() . path_to_theme() ?>/images/rss.png" alt="Rss Feed" title="Subscribe to updates via RSS" /></a>
			</div>
		
			<div id="user_links">
			<?php if (!($user->uid)) { print login_register_links(); } else { echo  admire_grunge_welcome_user(); } ?>
			</div>
			
		</div>
	</div>
</div>


<div class="container">

<div id="head">
<?php if ($logo) { ?><div id="logocontainer"><a href="<?php print $base_path ?>" title="<?php print t('Home') ?>"><img src="<?php print $logo ?>" alt="<?php print t('Home') ?>" /></a></div><?php } ?>
<?php if($site_name || $site_slogan) { ?>
	<div id="texttitles">
	  <?php if ($site_name) { ?><h1 class='site-name'><a href="<?php print $base_path ?>" title="<?php print t('Home') ?>"><?php print $site_name ?></a></h1><?php } ?>
      <?php if ($site_slogan) { ?><div class='site-slogan'><?php print $site_slogan ?></div><?php } ?>
	</div>
<?php } ?>	

<?php if ($search_box){ ?>
<div id="head_right">
	<div id="grunge_search">
		<?php  print render($search_box); ?>
	</div>
</div>
<?php } ?>
</div>

<?php if (isset($main_menu)) : ?>
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
<?php endif; ?>

<div id="wrap" class="clearfix">
	
	<?php if($main_banner){ ?>
	<div id="main_adbanner" class="clearfix">
		<?php print render($main_banner); ?>
	</div>
	<?php } ?>
	
	<?php if($sidebar_first) { ?>
	<div id="wrap_left" class="clearfix">
		<div id="left_top"><img src="<?php print base_path() . path_to_theme() ?>/images/left_top.jpg" alt=""/></div>
		<div id="left_inner">
		<div id="left_inner_inner">
		<?php  print render($sidebar_first) ;?>
		</div>
		<div id="left_bottom"><img src="<?php print base_path() . path_to_theme() ?>/images/left_bottom.png" alt=""/></div>
		</div>
	</div>
	<?php } ?>
	

	<div id="wrap_center" class="clearfix">
		<div id="wrap_inner">
		
		<?php if($above_content_ad){ ?>
		<div id="above_content_ad" class="clearfix">
		<?php print render($above_content_ad); ?>
		</div>
		<?php } ?>
		
		<?php if($content_top) {?>
			<div id="content_block_cover1">
			<div id="content_block_cover2">
			<div id="content_block_cover3">
			<div id="content_top">
			<?php print render($content_top); ?>
			</div></div></div></div><?php } ?>
			<?php if ($highlighted): print '<div id="mission">'. render($highlighted) .'</div>'; endif; ?>
			
			<?php if($breadcrumb){ ?><?php print $breadcrumb ?><?php } ?>
            <?php print render($title_prefix); ?>
            <?php if($title){ ?><h1 class="title"><?php print $title ?></h1><?php } ?>
            <?php print render($title_suffix); ?>
	        <?php if($tabs){ ?><div class="tabs"><?php print render($tabs) ?></div> <?php } ?>
	        <?php if ($show_messages) { print $messages; } ?>
            <?php if($help){ ?><?php print render($help); ?><?php } ?>
	        <?php print render($content); ?>
	        <?php print $feed_icons ?>
	        
	        <?php if($content_bottom) {?>
			<div id="content_bottom_block_cover1">
			<div id="content_bottom_block_cover2">
			<div id="content_bottom_block_cover3">
			<div id="content_bottom">
			<?php print render($content_bottom); ?>
			</div></div></div></div><?php } ?>
		</div>
	</div>

	
	<?php if($right_dark || $sidebar_last) { ?>
	<div id="wrap_right" class="clearfix">
		<?php if($right_dark){?>
		<div id="right_dark" class="clearfix">
			<?php  print render($right_dark) ;?>
		</div>
		<?php } ?>
		
		<?php if($right_ad){ ?>
		<div id="right_ad" class="clearfix">
		<?php print render($right_ad); ?>
		</div>
		<?php } ?>
		
		
		<?php if($sidebar_last){?>
		<div id="rightside" class="clearfix">
		<div id="right_block_inner" class="clearfix">
		<div id="right_bottom" class="clearfix">
		<?php  print render($sidebar_last) ;?>
		</div>
		</div>
		</div>
		<?php } ?>
	</div>
	<?php } ?>
	
	<br clear="all" class="clearfix"/>
	<div id="footer" class="clearfix">
	<div id="footer_message"><br />
	<!-- Please do not remove this credit line. This encourage us to update, contribute more themes to the community. --><span class="credit">Powered by <a href="http://drupal.org/">Drupal</a>, <a href="http://www.worthapost.com/" title="Drupal themes">Drupal Themes</a>. </span>
	</div>
	<div id="footer_region"><?php print render($footer ); ?></div>
	</div>
	
</div>

</div><!-- container -->

<!-- Preload Images (Less Important) -->
<div class="preload">
	<img class="preload" src="<?php print base_path() . path_to_theme() ?>/images/comment_form_mat.gif" alt="" />
	<img class="preload" src="<?php print base_path() . path_to_theme() ?>/images/comment_hanger.gif" alt=""/>
	<img class="preload" src="<?php print base_path() . path_to_theme() ?>/images/comment_paper.jpg" alt="" />
	<img class="preload" src="<?php print base_path() . path_to_theme() ?>/images/comment_paper_bottom.jpg" alt="" />
	<img class="preload" src="<?php print base_path() . path_to_theme() ?>/images/comment_paper_top.jpg" alt="" />
	<img class="preload" src="<?php print base_path() . path_to_theme() ?>/images/content_block_bg.jpg" alt="" />
	<img class="preload" src="<?php print base_path() . path_to_theme() ?>/images/content_block_top.jpg" alt="" />
	<img class="preload" src="<?php print base_path() . path_to_theme() ?>/images/left.jpg" alt="" />
	<img class="preload" src="<?php print base_path() . path_to_theme() ?>/images/right_top.jpg" alt="" />
</div>

</body>
</html>
