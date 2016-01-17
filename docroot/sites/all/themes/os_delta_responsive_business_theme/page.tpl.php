<?php if($messages) print '<div class="messageLog">'.$messages.'</div>'; ?>

<div id="wrapper" class="default">

		<div id="staticPanel" class="row-fluid">
		<div class="container">
			<?php if($logo): ?>
				<div class="logoPanel">
					<a href="<?php print check_url($front_page); ?>" title="<?php print t('Home'); ?>">
						<img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
					</a>
				</div>
			<?php endif; ?>

			<?php if($main_menu): ?>
				<div class="navbar navButton">
					<a class="btn btn-navbar" data-toggle="collapse" data-target=".main-collapse">
						<i class="fa fa-bars"></i>
					</a>
				</div>
				<div class="mainMenu">
					<div class="main-collapse nav-collapse collapse">
						<?php print theme('links__system_main_menu', array('links' => $main_menu));?>
					</div>
				</div>
			<?php endif; ?>
		</div>
		</div>

		<?php if($site_slogan) print '<h1 class="siteSlogan">'.$site_slogan.'</h1>'; ?>
		
				
				<?php if($show_hide_video):?>
					<div id="videoBox" class="<?php print $video_id; ?>">
					<?php	if($page['video_text'])
					 print '<div id="videoText">'.render($page['video_text']).'</div>'; ?>
				</div>
	 		<?php
			
	 	endif; ?>

		<?php if($page['text_box']): ?>
			<div id="textBox" class="row-fluid">
				<div class="container">
					<?php print render($page['text_box']); ?>
				</div>
			</div>
		<?php endif; ?>

		<?php if($page['features']): ?>
			<div id="features" class="row-fluid">
				<div class="container-fluid">
					<?php print render($page['features']); ?>
				</div>
			</div>
		<?php endif; ?>

	 	<?php if($page['map_block']): ?>
		    <div class="row-fluid">
		    	<?php print render($page['map_block']); ?>
			</div>
		<?php endif; ?>

	<div class="<?php print (drupal_is_front_page()) ? 'frontBox' : 'container contBox'; ?>">
<div class="row-fluid">
		<div class="<?php print ($page['categories_blog'] || $page['recent_blog'] || $page['recent_comments']) ? 'globalContant span9' : 'gCont'; ?>">
			<?php print render($page['content']); ?>
			<?php if($page['contact_info']): ?>
				<div id="contactInfo">
					<?php print render($page['contact_info']); ?>
				</div>
	 		<?php endif; ?>
		</div>

		<?php if($page['categories_blog'] || $page['recent_blog'] || $page['recent_comments']): ?>
			<div class="leftSidebar span3">
				<?php 
					print render($page['categories_blog']);
					print render($page['recent_blog']);
					print render($page['recent_comments']);
				?>
			</div>
		<?php endif; ?>
		</div>
	</div>

<?php if($page['home_page_gallery']): ?>
	<div id="homeGallery" class="row-fluid">
		<div class="container-fluid">
			<?php print render($page['home_page_gallery']);	?>
		</div>
	</div>
<?php endif; ?>

<?php if($page['textarea']): ?>
	<div id="textTwo" class="row-fluid">
		<div class="container-fluid">
			<?php print render($page['textarea']); ?>
		</div>
	</div>
<?php endif; ?>

<?php if($page['our_team']): ?>
	<div id="ourTeam" class="row-fluid">
		<div class="container-fluid">
			<?php print render($page['our_team']);	?>
		</div>
	</div>
<?php endif; ?>


	<div id="footer" class="row-fluid">
		<?php if($site_name) print '<p class="siteName">'.$site_name.'</p>'; ?>

		<?php if($page['contact_form']): ?>
			<div id="contactForm" class="row-fluid">
				<div class="container-fluid">
					<?php print render($page['contact_form']);	?>
				</div>
			</div>
		<?php endif; ?>

			<?php if ($show_hide_icon): ?>
				<div class="socBox">
					<ul class="socIcons">
					<?php
						$soc = array(
							"fa-twitter" => $twitter,
							"fa-facebook" => $facebook,
							"fa-flickr" => $flickr,
							"fa-linkedin" => $linkedin,
							"fa-youtube-play" => $youtube,
							"fa-pinterest" => $pinterest,
							"fa-google-plus" => $google,
							"fa-dribbble" => $dribbble,
							"fa-vimeo-square" => $vimeo,
							"fa-instagram" => $instagram,
							"fa-vk" => $vk
						);
					foreach($soc as $key => $value) {
						if (trim($value) != "") { ?>
						<li><a href="<?php print $value ?>" target="_blank">
							<i class="fa <?php print $key ?>"></i>
						</a></li>
					<?php }
					} ?>
					</ul>
				</div>
			<?php endif; ?>

		<?php if ($show_hide_copyright): ?>
			<div class="copyright">
				<a href="<?php print ($copyright_url); ?> " target="_blank">
				    <?php print t('Copyright'). ' &copy; ' .date("Y"). ' ' .$copyright_developedby; ?>
				</a>
			</div>
		<?php endif; ?>

		<?php if ($secondary_menu): ?>
			<div class="footerMenu">
				<?php print theme('links__system_secondary_menu', array('links' => $secondary_menu));?>
			</div>
		<?php endif; ?>

	</div> <!--footer-->
</div>  <!--wrapper-->