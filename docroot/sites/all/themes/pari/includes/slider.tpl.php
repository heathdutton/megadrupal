<div class="container">
	<div id="slider">
		<ul id="js-rotating">
		<?php if (theme_get_setting('slider_code') !== '') {
			echo theme_get_setting('slider_code');
		} else { ?>
			<li>
				<h1>We are Pari, Multipurpose Drupal theme</h1>
				<p>Pari Theme is packed full of all the amazing features and options for you to create a successful website</p>
				<a class="button" href="#">Get Started</a>
			</li>
			<li>
				<h1>Welcome To Drupar Design Studio</h1>
				<p>We present you material design. We put our hearts and soul into making every project.</p>
				<a class="button" href="#">Get Started</a>
			</li>
			<li>
				<h1>We Create Awesome Drupal Themes!</h1>
				<p>Our themes are of high quality, flexible and beautifully crafted that stand out of crowd.</p>
				<a class="button" href="#">Get Started</a>
			</li>
		<?php } ?>
		</ul>
	</div><!--/#slider -->
</div><!--/.container -->