<?php

	/**
	* @file
	* Black Lagoon Theme
	* Created by Zyxware Technologies
	*/
?>

<div id="wrapper">
	<div id="header">
		<div class="logo">
			<?php if ($logo): ?>
			
				<a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home" id="logo">
				<img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
			</a>
			<?php endif; ?>
		</div>
		<?php if($page['mainmenu']) { ?>
				<div id="main-menu">
					<?php print render($page['mainmenu']); ?>
				</div>
		<?php }	?>
	</div>
	<?php if ($messages): ?>
    <div id="messages">
      <?php print $messages; ?>
    </div>
  <?php endif; ?>
	
		<div id="content">
		  <?php if ($banner): ?>
		    <div class="slider">
			    <?php print $banner;?>  
			  </div>
			 <?php endif;?>
		</div>
	<?php if ($page['footer_first'] || $page['footer_second'] || $page['footer_third']) { ?>
		<div id="footercontainer">
			<?php if ($page['footer_first']): ?>
				<div class="footerfirst">
					<?php print render($page['footer_first']); ?>
				</div>
			<?php endif; ?>
			<?php if ($page['footer_second']): ?>
			<div class="footersecond">
				<?php print render($page['footer_second']); ?>
			</div>
			<?php endif; ?>
			<?php if ($page['footer_third']): ?>
			<div class="footerthird">
				<?php print render($page['footer_third']); ?>
			</div>
			<?php endif; ?>
		</div>
	<?php }	?>
	<?php if ($page['footer']) { ?>
		<div id="footer">
			<?php print render($page['footer']); ?>
			<div class="footer_zyxware">Theme by <a href="http://www.zyxware.com/" title="Zyxware" target="_blank">Zyxware</a></div>
		</div>
	<?php } ?>
	
</div> <!-- /#wrapper -->
