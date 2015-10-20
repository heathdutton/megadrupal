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
    		<div class="contentwrapper conatiner-12">
    			<div class="articlewrap <?php print (($page['sidebar_second'] ) ? 'grid-8' : 'grid-12')?>">
    			  <?php if ($tabs): ?>
                <?php print render($tabs); ?>
            <?php endif; ?>
            <?php if ($page ['content']): ?>
    		        <?php print render($page['content']); ?>
    			  <?php endif; ?>		
    			</div>
    			<?php if ($page['sidebar_second']) : ?>
    			  <div class="grid-4">
    		        <?php print render($page['sidebar_second']); ?>
    			  </div>
    			<?php endif; ?>	
    			<div class="articlebottom grid-12">
    			  
    			  <div class="grid-3 alpha">
    			  <?php if ($page ['bottomarticle1']): ?>
    		        <?php print render($page['bottomarticle1']); ?>
    			  <?php endif; ?>
    			  </div>
    			  <div class="grid-3">
    			  <?php if ($page ['bottomarticle2']): ?>
    		        <?php print render($page['bottomarticle2']); ?>
    			  <?php endif; ?>
    			  </div>
    			  <div class="grid-3">
    			  <?php if ($page ['bottomarticle3']): ?>
    		        <?php print render($page['bottomarticle3']); ?>
    			  <?php endif; ?>
    			  </div>
    			  <div class="grid-3 omega">
    			  <?php if ($page ['bottomarticle4']): ?>
    		        <?php print render($page['bottomarticle4']); ?>
    			  <?php endif; ?>
    			  
    			</div>
    		</div>
    		<div class="footer container-16">
    			<div class="grid-5">
    			  <?php if ($page ['footernav']): ?>
    		        <?php print render($page['footernav']); ?>
    			  <?php endif; ?>	
    			</div>
    			<div class="grid-6">
    			  <?php if ($page ['footercopyright']): ?>
    		        <?php print render($page['footercopyright']); ?>
    			  <?php endif; ?>
    			</div>
    			<div class="grid-5">
                  <?php if ($page ['footericons']): ?>
    		        <?php print render($page['footericons']); ?>
    			  <?php endif; ?>
    			</div>
    			<div class="footer_zyxware">Theme by <a href="http://www.zyxware.com/" title="Zyxware" target="_blank">Zyxware</a></div>
    		</div>
    	</div>
    </div>
