<div id="wrapper"> <!--width independent from body-->


  <div id="header">

    <?php if ($logo): ?>
	  <div id="logo">
		<a href="<?php print $base_path; ?>" title="<?php print t('Home'); ?>" rel="home">
		  <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" id="logo-image" />
		</a>
	  </div>
	<?php endif; ?>
	
    <?php if ($site_name): ?>
	  <h1 id="site-name">
	    <a href="<?php print $base_path; ?>" title="<?php print t('Home'); ?>" rel="home">
	      <?php print $site_name; ?> 
	    </a>
      </h1>
	<?php endif; ?>

    <?php if ($main_menu): ?>
	  <div id="navigation">
	    <?php print theme('links__system_main_menu', array(
          'links' => $main_menu,
          'attributes' => array(
            'id' => 'main-menu-links',
            'class' => array('links', 'primary-links', 'clearfix'),
          ),
          'heading' => array(
            'text' => t('Main menu'),
            'level' => 'h2',
            'class' => array('element-invisible'),
          ),
        )); ?>
	  </div>
	<?php endif; ?>


	<?php if ($page['header']): ?>
    <?php print render($page['header']); ?>
  <?php endif; ?>
	      
	      
	  
  </div> <!-- end header -->
  
  <div id="sub-navigation">
    <?php print theme('links__system_secondary_menu', array(
      'links' => $secondary_menu,
      'attributes' => array(
        'id' => 'secondary-links',
        'class' => array('links', 'inline', 'clearfix'),
      ),
      'heading' => array(
        'text' => t('Secondary menu'),
        'level' => 'h2',
        'class' => array('element-invisible'),
      ),
    )); ?>
  </div>
	
	

	<div id="main">
		
		 <?php print render($page['front']); ?>
		

	    <div id="sidebar-left">
		  <div id="blocks-wrapper">
		     <?php print render($page['sidebar_first']); ?>
		  </div>
	    </div>
	
	
	  <div id="main-content">
		
		    <?php print $messages ?>
		
		
		   	<?php if ($tabs): ?>
		        <div class="tabs">
		          <?php print render($tabs); ?>
		        </div>
		      <?php endif; ?>
		
			<?php print render($page['content']); ?>
			
	  </div>		
	  


	</div>

	
    <?php if ($page['footer']): ?>
	    <div id="footer">
		  <div id="footer-right">
			<div id="footer-left">
              <?php print render($page['footer']); ?>
			</div>
		  </div>
	    </div>
    <?php endif; ?>
	
	
</div>