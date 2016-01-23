<?php

	/**
	* @file
	* Business Yellow Theme
	* Created by Zyxware Technologies
	*/

?>
  <div id="mainwrapper" class="container-12">
    <div id="header_inner" class="grid-12">                                             <!-- Header STARTS HERE -->
      <div id="logo_menu_wrapper">
        <div id="logo_section" class="grid-3 alpha clearfix">
           <?php if ($site_name): ?>
            <a href="<?php print check_url($front_page); ?>" title="<?php print t('Home'); ?>">
              <img src="<?php print $logo; ?>" alt="<?php print $site_name; ?>" />
            </a>
           <?php endif; ?>
        </div>
        <div id="icons_outer" class="grid-4">
          <?php if ($page ['icons']): ?>
            <div class="socialmedia_icons">
              <?print render($page['icons']); ?> 
            </div>          
          <?endif; ?>
        </div> 
        <div id="nav" class="grid-12 clearfix">
          <?php //if ($page ['primarynav']): ?>
            <div class="grid-7 alpha">
            <? //print render($page['primarynav']); ?> 
                        <?php 
          if (module_exists('i18n')) { 
            $main_menu_tree = i18n_menu_translated_tree(variable_get('menu_main_links_source', 'main-menu'));
          } else {
            $main_menu_tree = menu_tree(variable_get('menu_main_links_source', 'main-menu'));
          }
          print drupal_render($main_menu_tree);
        ?>
            </div>
          <? //endif; ?>
          <?php if ($page ['search']): ?>
            <div id="search" class="grid-5 omega">
            <?print render($page['search']); ?> 
            </div>
          <? endif; ?>
        </div>        
      </div>
    </div>                                                                            <!-- Header ENDS HERE -->
    <div id="article_inner" class="grid-12"> 
			<div id="fullline">                                        <!-- Article_section STARTS HERE -->
				<div id="article_leftbar" class="<?php print (($page['article_rightbar']) ? 'grid-8' : 'grid-12') ?>">
		      <?php print render($title_prefix); ?>
		      <?php if ($title): ?>
		        <h2 class="title"><?php print $title ?></h2>
		      <?php endif; ?>
		      <?php print render($title_suffix); ?>
		      <?php print $messages; ?>
		      <?php if ($tabs): ?><?php print render($tabs); ?><?php endif; ?>
		      <?php if ($page ['content']): ?>
		        <?php print render($page['content']); ?>
		      <? endif; ?>
		      <?php if ($feed_icons): ?><?php print $feed_icons; ?><?php endif; ?>
		    </div>
		    <?php if ($page ['article_rightbar']): ?>                              
		      <div id="article_rightbar" class="grid-3 clearfix">
		        <div class="uparrow"><a href="#"><?php print $up_arrow; ?></a>
		        </div>
		        <?print render($page['article_rightbar']); ?>
		        <div class="downarrow"><a href="#"><?php print $down_arrow; ?></a>
		        </div>
		      </div>
		    <? endif; ?>
				<br/>                                                                  
			</div>
		</div>                                                                          <!-- Article_section ENDS HERE -->
    <div id="footer" class="grid-12 clearfix">
      <div class="yellowborder_wrap">                                      <!-- Footer STARTS HERE -->
      <?php if ($page ['footerleft']): ?>
      <div id="footer_left" class="grid-4 omega">
      <?print render($page['footerleft']); ?> 
      </div>
      <? endif; ?>
      <div id="footer_middle" class="grid-4 omega">
      <?php if ($page ['footermiddle']): ?>
      
      <?print render($page['footermiddle']); ?> 
      
      <? endif; ?>
      </div>
       <?php if ($page ['footerright']): ?>
      <div id="footer_right" class="grid-4 omega">
      <?print render($page['footerright']); ?> 
      </div>
      <? endif; ?>
    </div>
    </div>
    <div class="footer_zyxware">
        Theme by <a href="http://www.zyxware.com/">Zyxware Technologies</a> 
   </div>                                                                             
  </div>

