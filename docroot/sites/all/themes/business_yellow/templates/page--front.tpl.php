<?php

	/**
	* @file
	* Business Yellow Theme
	* Created by Zyxware Technologies
	*/

?>
  <div id="mainwrapper" class="container-12">
    <div id="header" class="grid-12">
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
        <div id="nav" class="grid-12 clearfix alpha omega">
          <?php // if ($page ['primarynav']): ?>
            <div class="grid-7 alpha">
            <?php //print render($page['primarynav']); ?> 
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
      <div id="steps3_outer" class="grid-12">
          <div class="arrow1"></div>
          <div class="arrow2"></div>
         
         <?php if ($page ['stepone']): ?> 
          <div id="stepone" class="grid-4 alpha">
          <?print render($page['stepone']); ?>
          </div>
        <? endif; ?>
       
        <?php if ($page ['steptwo']): ?>
          <div id="steptwo" class="grid-4 alpha">
          <?print render($page['steptwo']); ?>
          </div>
        <? endif; ?>
        <?php if ($page ['stepthree']): ?>
        <div id="stepthree" class="grid-4 alpha">
          <?print render($page['stepthree']); ?>
        </div> 
        <? endif; ?>      
      </div>
    </div>
    <div id="middlesection_outer" class="grid-12 clearfix">
            <div id="middle_inner2" class="grid-12 alpha omega">
              <?php if ($page ['content']): ?>
              <?print render($page['content']); ?>
              <?php endif; ?>         
            </div>
          
      <div id="middle_lower" class="grid-11">
        <?php if ($page ['sidebar_first']): ?>
        <div id="leftbar_home" class="grid-3"style="width:200px;">
        <?print render($page['sidebar_first']); ?> 
        </div>
        <? endif; ?>
        <?php if ($page ['sidebar_second']): ?>
        <div id="rightbar_home" class="grid-8 alpha">
        <?print render($page['sidebar_second']); ?> 
        </div>
        <? endif; ?>
        <?php if ($page ['rightblock1']): ?>
        <div id="rightblock1 " class="grid-4">
        <?print render($page['rightblock1']); ?> 
        </div>
        <? endif; ?>
        <?php if ($page ['rightblock2']): ?>
        <div id="rightblock2" class="grid-4 omega">
        <?print render($page['rightblock2']); ?> 
        </div>
        <? endif; ?>
        <?php if ($page ['rightblock3']): ?>
        <div id="rightblock3" class="grid-4 omega">
        <?print render($page['rightblock3']); ?> 
        </div>
        <? endif; ?>
        <?php if ($page ['rightblock4']): ?>
        <div id="rightblock4" class="grid-4 omega">
        <?print render($page['rightblock4']); ?> 
        </div>
        <? endif; ?>
      </div>          
    </div>
    <div id="footer" class="grid-12 clearfix">
      <div class="yellowborder_wrap">
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
