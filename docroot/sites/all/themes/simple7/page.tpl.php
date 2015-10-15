<div id="wrapper">
  
 <header>
	
		<?php print render($page['header']) ?>
	
		<h1><a href="<?php print $front_page ?>"><?php print $site_name ?></a></h1>
		
		<h2><?php print $site_slogan ?></h2>
		
		<div id="block-bar">
		
		  <nav>
		
  			<?php if ($main_menu): ?>
    		  <?php print theme('links__system_main_menu', array('links' => $main_menu )); ?>
  		  <?php endif; ?>
		
		  </nav>
		  
		  <div id="search-form-wrap"></div>		
		
		</div>
		
		<div class="clear">&nbsp;</div>
	
  </header>
	
	<section id="main-stuff">
	
		<?php if($title): ?><h2><?php print $title ?></h2><?php endif ?>
		
    <?php if($tabs): ?><?php print render($tabs) ?><?php endif ?>
		
		<?php print render($page['help']) ?>
		
		<?php print $messages ?>
				
		<?php print render($page['content']) ?>

	</section>
	
	<asside>
	
		<?php print render($page['sidebar_first']) ?>
		
	</asside>
		
	<footer>
	
		<?php print render($page['footer']) ?>
		
		<nav>
		
			<?php if ($main_menu): ?>
			
		        <?php print theme('links__system_main_menu', array('links' => $main_menu )); ?>
	
		    <?php endif; ?>
		
		</nav>
	
	</footer>
	
</div>