<?php
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
				<div id="authorize">
					<ul>
						<?php global $user; 
							if ($user->uid != 0) {
								print '<li class="first">' .t('Logged in as '). '<a href="' .url('user/'.$user->uid). '">' .$user->name. '</a></li>';
								print '<li class="last"><a href="' .url('logout'). '">' .t('Logout'). '</a></li>'; } 
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
							<div class="menu-right"></div>
						</div>
					</div>
				</div> <!-- end menu wrapper -->
								
				<div id="slideshow-wrapper">
					<?php if ($content): ?><div class="mission"><?php print $content; ?></div><?php endif; ?>
					<div class="slideshow">
						<img src="<?php print $base_path . $directory; ?>/images/slides/metropolis-1.jpg" width="804" height="375" alt="Metropolis 1"/>
						<img src="<?php print $base_path . $directory; ?>/images/slides/metropolis-2.jpg" width="804" height="375" alt="Metropolis 2"/>
						<img src="<?php print $base_path . $directory; ?>/images/slides/metropolis-3.jpg" width="804" height="375" alt="Metropolis 3"/>
					</div> <!-- end slideshow -->
				</div> <!-- end slideshow wrapper -->
			
				<div id="container-inner">
					<div id="content-wrapper" class="clearfix">
						<div id="main-content">
							<?php if ($show_messages) { print $messages; }; ?>
						</div>
					</div> <!-- end content wrapper-->
				</div> <!-- end container inner -->

			</div> <!-- end container outer -->
			<div id="shadow-bottom"></div>
		</div> <!-- end container wrapper -->
			
		<div id="footer">
			<div style="clear:both; font-size:11px; text-align:center;"><em>Theme provided by <a href="http://drupalbycity.com">DrupalByCity</a> under GPL license from <a href="http://comprendo.dk">Comprendo</a></em></div>
		</div> <!-- end footer -->
</div> <!-- end page wrapper -->

</body>
</html>
