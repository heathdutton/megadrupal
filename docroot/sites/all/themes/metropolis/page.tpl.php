<?php 
?>
<div id="page-wrapper">
		<div id="header-wrapper">
			<div id="header"> 
					<div id="branding-wrapper">
						<div class="branding">
							<?php if ($logo): ?>
								<div class="logo">
									<a href="<?php print $base_path ?>" title="<?php print t('Home') ?>"><img src="<?php print $logo ?>" alt="<?php print t('Home') ?>" /></a>
								</div> <!-- end logo -->
							<?php endif; ?>
							<div class="name-slogan-wrapper">
							<?php if ($site_name) : ?>
								<?php if ($is_front) : ?>
									<h1 class="site-name"><a href="<?php print $base_path ?>" title="<?php print $site_name ?>"><?php print $site_name ?></a></h1>
								<?php endif; ?>
								<?php if (!$is_front) : ?>
									<h2 class="site-name"><a href="<?php print $base_path ?>" title="<?php print $site_name ?>"><?php print $site_name ?></a></h2>
								<?php endif; ?>
							<?php endif; ?>
							<?php if ($site_slogan) : ?>
								<span class='site-slogan'><?php print $site_slogan; ?></span>
							<?php endif; ?>
							</div> <!-- end site-name + site-slogan wrapper -->
						</div>
					</div> <!-- end branding wrapper -->
				<?php if ($feed_icons): ?>
					<div class="feed-wrapper">
						<?php print $feed_icons; ?>
					</div> <!-- end feed wrapper -->
				<?php endif; ?>
				<div id="authorize">
					<ul>
						<?php global $user; 
							if ($user->uid != 0) {
								print '<li class="first">' .t('Logged in as '). '<a href="' .url('user/'.$user->uid). '">' .$user->name. '</a></li>';
								print '<li class="last"><a href="' .url('user/logout'). '">' .t('Logout'). '</a></li>'; } 
							else {
								print '<li class="first"><a href="' .url('user'). '">' .t('Login'). '</a></li>';
								print '<li class="last"><a href="' .url('user/register'). '">' .t('Register'). '</a></li>'; }
								?>
					</ul>
				</div> <!-- end authorize -->
			</div> <!-- end header -->
		</div> <!-- end header wrapper -->
		
		<div id="container-wrapper">
			<div id="container-outer">
				<div id="menu-wrapper">
					<div class="menu-outer">
						<div class="menu-inner">
							<div class="menu-left"></div> 
							<?php if ($main_menu || $page['superfish_menu']): ?>
										<div id="<?php print $main_menu ? 'menu' : 'superfish' ; ?>">
											<?php if ($main_menu) {
												print theme('links__system_main_menu', array('links' => $main_menu));}
											elseif (!empty($page['superfish_menu'])) {
												print render ($page['superfish_menu']);} ?>
										</div> <!-- end menu / superfish -->
							<?php endif; ?>
							<div class="menu-right"></div>
						</div>
					</div>
					<?php if (!$is_front) print $breadcrumb; ?>
				</div> <!-- end menu wrapper -->
				
				<?php if (!$page['slideshow']): ?>
					<?php if (!$is_front): ?>
						<div class="breadcrumb-shadow"></div> <!-- end breadcrumb shadow -->
					<?php endif; ?>
				<?php endif; ?>
				
				<?php if ($page['slideshow']): ?>
					<div id="slideshow-wrapper">
						<div class="slideshow">
							<?php print render ($page['slideshow']);?>
						</div> <!-- end slideshow -->
					</div> <!-- end slideshow wrapper -->
				<?php endif; ?>
				
				<?php if (!$page['slideshow']): ?>
					<?php if ($is_front): ?>
						<div id="slideshow-wrapper">
							<?php if ($page['highlighted']): ?><div class="mission"><?php print render($page['highlighted']); ?></div><?php endif; ?>
							<div class="slideshow">
								<img src="<?php print $base_path . $directory; ?>/images/slides/metropolis-1.jpg" width="804" height="375" alt="Metropolis 1"/>
								<img src="<?php print $base_path . $directory; ?>/images/slides/metropolis-2.jpg" width="804" height="375" alt="Metropolis 2"/>
								<img src="<?php print $base_path . $directory; ?>/images/slides/metropolis-3.jpg" width="804" height="375" alt="Metropolis 3"/>
							</div> <!-- end slideshow -->
						</div> <!-- end slideshow wrapper -->
					<?php endif; ?>
				<?php endif; ?>
			
				<div id="container-inner">
					<div id="content-wrapper" class="clearfix">
						<div id="main-content">
							<?php if($page['content_top']):?><div id="content-top"><?php print render ($page['$content_top']); ?></div><?php endif; ?>
							<?php if ($show_messages) { print $messages; }; ?>
							<?php print render($title_prefix); ?>
								<?php if ($title): ?>
									<h1 class="title" class="page-title">
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
						</div>
						<?php if ($page['sidebar_second']): ?>
							<div id="sidebar" class="column sidebar">
								<div class="section">
									<?php print render($page['sidebar_second']); ?>
								</div>
							</div> <!-- end sidebar-second -->
							<div class="clearfix"></div>
						<?php endif; ?>
					</div> <!-- end content wrapper-->
				</div> <!-- end container inner -->
				
				<?php if($page['bottom_1'] || $page['bottom_2'] || $page['bottom_3'] || $page['bottom_4']) : ?>
					<div id="bottom-wrapper" class="in<?php print (bool) $page['bottom_1'] + (bool) $page['bottom_2'] + (bool) $page['bottom_3'] + (bool) $page['bottom_4']; ?> clearfix">
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
						<div class="clearfix"></div>
					</div><!-- end bottom wrapper-->
				<?php endif; ?>

			</div> <!-- end container outer -->
			<div id="shadow-bottom"></div>
		</div> <!-- end container wrapper -->
			
		<div id="footer">
			<?php print render ($page['footer']); ?>
			<?php if($secondary_menu) : ?>
				<div id="subnav">
					<?php print theme('links__system_secondary_menu', array('links' => $secondary_menu, 'attributes' => array('class' => array('links', 'clearfix')))); ?>
				</div>
			<?php endif; ?>
			<?php print $notice; ?>
		</div> <!-- end footer -->
			
</div> <!-- end page wrapper -->