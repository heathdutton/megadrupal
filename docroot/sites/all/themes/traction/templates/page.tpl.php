<div class="skip-content"><a href="#content">Skip to content</a></div>
	<div id="pg-nav-bg" >
		<div class="wrapper container-12 clearfix">
			<!--<div id="pg-nav" class="menu-top-menu-container" >
				<?php if ($primary_nav): print $primary_nav; endif; ?>
       </div>-->
       <div id="nav" >
              <?php 
          if (module_exists('i18n')) { 
            $main_menu_tree = i18n_menu_translated_tree(variable_get('menu_main_links_source', 'main-menu'));
          } else {
            $main_menu_tree = menu_tree(variable_get('menu_main_links_source', 'main-menu'));
          }
          print drupal_render($main_menu_tree);
        ?>
              <?php //if ($page ['primarynav']): ?>
                <!--<div class="primarynav prefix-1 grid-7">
                  <?print render($page['primarynav']); ?> 
                </div>-->
              <?php //endif; ?>
              </div>
     </div><!--end wrapper-->
	</div><!--end page-navigation-bg-->
	<div class="wrapper big">
		<div id="header" class="clear">
							<div class="logo logo-img grid-12">
					 <?php if ($logo): ?>
          <a href="<?php print check_url($front_page); ?>" title="<?php print t('Home'); ?>">
            <img src="<?php print $logo; ?>" alt="<?php print $site_name; ?>" />
          </a>
        <?php endif; ?>
									</div><!--end logo-->
									<div id="cat-nav" class="grid-12">
										<?php if ($secondary_nav): print $secondary_nav; endif; ?>
									</div>		</div><!--end header-->
<div id="main-top" class="container-16 clear">
		<div class="grid-11 clearfix">
		<?php if ($node_created): ?><h4><?php print $node_created; ?></h4><?php endif; ?>
		<div class="single-comments">
					<a href="#comments"><?php print $node_comment_count; ?></a>
		</div>
		</div>
		<div class="grid-5 omega">
			<?php print render($page['subscribe']); ?>	
	</div>		</div>

	<div id="main" class="container-16 clear">
			<div id="content" class="grid-11 alpha omega">
        <?php print $messages; ?>
       	<?php if ($tabs): ?>
          <?php print render($tabs); ?>
        <?php endif; ?>
        <?php if ($page['content']): ?>
  			  <?php print render($page['content']); ?>
				<?php endif; ?> 				
	</div>
		
   <?php if ($page['ads'] || $page['sidebar_second']): ?>
		  <div id="sidebar" class="grid-5 alpha omega">
		    <?php if ($page['ads']): ?>
			    <?php print render($page['ads']); ?>		
			  <?php endif; ?> 
			  <?php if ($page['sidebar_second']): ?>
  			  <?php print render($page['sidebar_second']); ?>				
  			<?php endif; ?>	
	  	</div><!--end sidebar-->	
		<?php endif; ?>	
				
				</div><!--end main-->
	<div id="main-bottom"></div>
</div><!--end wrapper-->



<div id="footer">
	<div class="wrapper container-12 clearfix">
		<div  class="footer-column  grid-4">
        	
			  <?php print render($page['footer_first']); ?>  
			  
		</div>
		<div class="footer-column  grid-4">
     	 <?php print render($page['footer_second']); ?>
		</div>
		<div class="footer-column  grid-4 omega">
			  <?php print render($page['footer_third']); ?>		

	
		</div>
		
	</div><!--end wrapper-->
</div><!--end footer-->
<div id="copyright" class="wrapper container-12 clearfix">
	 <div class="grid-4">
	  <?php print render($page['footer_fourth']); ?>	
	</div>
	<div class="grid-3">
	  <?php print render($page['footer_fifth']); ?>
	</div>
	<div class="grid-5">
		Designed by <a href="http://thethemefoundry.com/traction/" title="Traction" target="_blank">The Theme Foundry</a> . &nbsp&nbsp
				Developed by <a href="http://www.zyxware.com/" title="Zyxware" target="_blank">Zyxware</a> 	</div>
</div>

