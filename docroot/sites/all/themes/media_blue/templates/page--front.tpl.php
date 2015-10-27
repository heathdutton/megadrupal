    <div id="wrapper">
    	<div class="container-12">
    		<div class="header">
    			<div class="logo grid-3">
    				<?php if ($site_name): ?>
                    <a href="<?php print check_url($front_page); ?>" title="<?php print t('Home'); ?>">
                    <img src="<?php print $logo; ?>" alt="<?php print $site_name; ?>" />
                    </a>
                    <?php endif; ?>
    			</div>
    			<div class="grid-9">
    			    <?php if ($page ['search']): ?>
                       <?print render($page['search']); ?>  
                    <?php endif; ?>
                    
                    <!--<?php if ($page ['primarynav']): ?>
    				  <?print render($page['primarynav']); ?>
    				<?php endif; ?>-->
    				          <div id="nav">
           
            <?php 
          if (module_exists('i18n')) { 
            $main_menu_tree = i18n_menu_translated_tree(variable_get('menu_main_links_source', 'main-menu'));
          } else {
            $main_menu_tree = menu_tree(variable_get('menu_main_links_source', 'main-menu'));
          }
          print drupal_render($main_menu_tree);
        ?>
          </div>
    			</div>
    		</div>
    		
    			<!--<?php if ($page ['slideshow']): ?>
           <div class=" container-12 slider"><div id="slider" class="grid-12">
            <?print render($page['slideshow']); ?>
           </div>
        <?php endif; ?>-->
        <?php if ($banner):  ?>                                        <!-- SLIDESHOW STARTS HERE -->
          <div class=" container-12 slider">
            <?php print $banner; ?>
          </div>
        <?php endif; ?>                                                <!-- SLIDESHOW ENDS HERE --> 
    		<div class="contentwrapper conatiner-12">
    			  <div class="<?php print ($page['sidebar_second'] && $page['sidebartwo']) ? 'grid-5' : (($page['sidebar_second'] || $page['sidebartwo']) ? 'grid-8' : 'grid-12') ?>">
    			  <?php if ($page ['content']): ?>
    		        <?print render($page['content']); ?>
    			  <?php endif; ?>		
    			</div>
    			<?php if ($page ['sidebar_second']): ?>
      			<div class="grid-4">
     		        <?print render($page['sidebar_second']); ?>
    		  	</div>
    			<?php endif; ?>
    			<?php if ($page['sidebartwo']): ?>
    			  <div class="contentright grid-3">
    		        <?print render($page['sidebartwo']); ?>
    			  </div>
    			<?php endif; ?>
    		</div>
    		<div class="footer container-16">
    			<div class="grid-5">
    			  <?php if ($page ['footernav']): ?>
    		        <?print render($page['footernav']); ?>
    			  <?php endif; ?>	
    			</div>
    			<div class="grid-6">
    			  <?php if ($page ['footercopyright']): ?>
    		        <?print render($page['footercopyright']); ?>
    			  <?php endif; ?>
    			</div>
    			<div class="grid-5">
                  <?php if ($page ['footericons']): ?>
    		        <?print render($page['footericons']); ?>
    			  <?php endif; ?>
    			</div>
    			<div class="footer_zyxware">Theme by <a href="http://www.zyxware.com/" title="Zyxware" target="_blank">Zyxware</a></div>
    		</div>
    		 
    	</div>
    </div>
