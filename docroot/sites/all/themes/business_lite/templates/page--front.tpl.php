<div id="wrapper">
<!--<script type="text/javascript">
	var menu=new menu.dd("menu");
	menu.init("menu","menuhover");
</script>-->
  <div id="header">                                         <!--START OF Header!-->
    <div class="container-12">
      <div class="grid-4">
        <?php if ($logo): ?>
          <div id="logo">
            <a href="<?php print check_url($front_page); ?>" title="<?php print t('Home'); ?>">          
            <img src="<?php print $logo;?>" alt="<?php print t('Home'); ?>" />
            </a>
          </div>
        <?php endif; ?>
      </div>
      <?php if ($page ['primarynav']): ?>
        <div class="grid-8 clearfix omega">
          <div id="nav">
            <?php //print render ($page ['primarynav']); ?>
            <?php 
          if (module_exists('i18n')) { 
            $main_menu_tree = i18n_menu_translated_tree(variable_get('menu_main_links_source', 'main-menu'));
          } else {
            $main_menu_tree = menu_tree(variable_get('menu_main_links_source', 'main-menu'));
          }
          print drupal_render($main_menu_tree);
        ?>
          </div>                                          <!--END of nav!-->
        </div>
      <?php endif; ?>
     </div>
    </div>                                                <!--END of Header!-->
    
    <div id="slideshow">                                  <!--START of SLIDESHOW!-->
      <div class="container-12 clearfix">
        <?php if ($banner): ?>
           <?php print $banner;?>
        <?php endif;?>   
      </div>
    </div>                                                <!--END of SLIDESHOW!-->
                                                  
    <div  id="topcontent">                                <!--START of TOPCONTENT!-->
      <div class="container-12 clearfix">
        <div class="grid-12 clearfix contentfont">
          <?php print render($page ['topcontent']); ?>
        </div>	
      </div>
    </div>                                                <!--END of TOPCONTENT!-->
    
    <div class="container-12 clearfix" style="margin-top:20px;">    <!--START of BLOCKS!-->
      <div class="grid-12 clearfix">
        <?php if ($page['blockone']): ?>
          <div class="grid-3 clearfix alpha article_title">        
            <?php print render($page ['blockone']); ?>
          </div>
        <?php endif; ?>
        <?php if ($page['blocktwo']): ?>
          <div class="grid-3 clearfix article_title">
            <?php print render($page ['blocktwo']); ?>
          </div>
        <?php endif; ?>
        <?php if ($page['blockthree']): ?>
          <div class="grid-3 clearfix article_title">
            <?php print render($page ['blockthree']); ?>
          </div>
        <?php endif; ?>
        <?php if ($page['blockfour']): ?>
          <div class="grid-3 clearfix omega article_title">
        <?php print render($page ['blockfour']); ?>
          </div>
        <?php endif; ?>
      </div>
    </div>                                                          <!--END of BLOCKS!-->
    
    <div id="footer">                                               <!--START of FOOTER!-->                   
      <div class="container-12 clearfix">
        <div class="grid-12 clearfix">
          <?php if ($page['footer_firstcolumn']): ?>
            <div class="grid-3 clearfix alpha footer-widget-title">
              <?php print render($page ['footer_firstcolumn']); ?>
            </div>
          <?php endif; ?>
          <?php if ($page['footer_secondcolumn']): ?>
            <div class="grid-3 clearfix footer-widget-title">
              <?php print render($page ['footer_secondcolumn']); ?>
            </div>
          <?php endif; ?>
          <?php if ($page['footer_thirdcolumn']): ?>
            <div class="grid-3 clearfix footer-widget-title">
              <?php print render($page ['footer_thirdcolumn']); ?>
            </div>
          <?php endif; ?>
          <?php if ($page['footer_fourthcolumn']): ?>
            <div class="grid-3 clearfix omega footer-widget-title">
              <?php print render($page ['footer_fourthcolumn']); ?>
            </div>
          <?php endif; ?>
        </div>
       </div>
    </div>                                                            <!--END of FOOTER!-->  
    
    <div id="afterfooter">
      <div  class="container-12 clearfix">
        <div class="grid-12 clearfix">      
          <div id="credit" class="grid-6 clearfix">         
          </div>       
          <div class="grid-5 clearfix" style="text-align:right; margin-top:20px;">
             Powered by Drupal<br/>
             Developed by <a href="http://www.zyxware.com/">Zyxware</a>
          </div>
        </div>
     </div>
   </div>                                                               <!--END of AFTERFOOTER!-->
</div>
