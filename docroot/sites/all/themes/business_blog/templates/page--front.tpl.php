    <div id="wrapper">
      <div id="mainbox" class="container-12">
        <div class="region-header">                                             <!-- HEADER STARTS HERE -->
          <?php if ($page ['login']): ?>
            <div class="grid-12 righttext">
              <?print render($page['login']); ?>
            </div>
          <?php endif; ?>
          <?php if ($page ['search']): ?>
            <div class="grid-12 clearfix righttext">
              <?print render($page['search']); ?>
            </div>
          <?php endif; ?>
            <div class="logonavouter grid-12 clearfix">
              <div class="logo grid-3 omega">
                <?php if ($site_name): ?>
                  <a href="<?php print check_url($front_page); ?>" title="<?php print t('Home'); ?>">
                    <img src="<?php print $logo; ?>" alt="<?php print $site_name; ?>" />
                  </a>
                <?php endif; ?>
              </div>
              <?php if ($page ['primarynav']): ?>
                <div class="primarynav prefix-1 grid-7">
                  <?print render($page['primarynav']); ?> 
                </div>
              <?php endif; ?>
            </div>
          </div> 
                                                                         <!-- HEADER ENDS HERE -->
          <?php if ($banner):  ?>                                        <!-- SLIDESHOW STARTS HERE -->
                <div id="slides" class="grid-12">
                  <?php print $banner; ?>
                </div>
              <?php endif; ?>                                            <!-- SLIDESHOW ENDS HERE -->          
          <div class="grid-12">
            <div class="<?php print ($page['rightarticle'] && $page['sidelistingbar']) ? 'region-leftartlcle content_readmore' : (($page['rightarticle'] || $page['sidelistingbar']) ? 'region-leftartlcle content_readmore' : 'region-leftartlcle-full content_readmore') ?>">
              <?php if ($page ['content']): ?>
                <?php print render($page['content']); ?>
              <?php endif; ?>               
            </div>
            <?php if ($page['rightarticle'] || $page['sidelistingbar']): ?>
            <div class="right_outer">            
              <div class="articletopcurve"></div>
              <div class="articlemiddlecurve">
                <?php if ($page ['rightarticle']): ?>
                  <?print render($page['rightarticle']); ?>         
                <?php endif; ?>
              </div> 
              <div class="articlebottomcurve"></div>
              <div class="grid-4 alpha region-sidelistingbar clearfix">
                <?php if ($page ['sidelistingbar']): ?>
                  <div class="grid-4 alpha">
                    <?print render($page['sidelistingbar']); ?>  
                  </div>
                <?php endif; ?>
              </div>
            </div>
            <?php endif; ?> 
        </div>      
      </div>
      
      <div class="footer">                                                            <!-- FOOTER STARTS HERE -->
        <div class="region-footer container-16 clearfix">
          <div class="grid-16">
          
            <?php if ($page ['bottomboxleft']): ?>
             
                <?print render($page['bottomboxleft']); ?>  
             
            <?php endif; ?>
           
            
            <?php if ($page ['bottomboxmiddle']): ?>
              
                <?print render($page['bottomboxmiddle']); ?>   
              
           <?php endif; ?>
           <?php if ($page ['bottomboxright']): ?>
           
              <?print render($page['bottomboxright']); ?>   
           
            <?php endif; ?>
         </div>
         <div class="grid-16 bottom_section">
          <div class="footer_divider1"></div>
          <div class="footer_divider2"></div>
            <?php if ($page ['footerleft']): ?>
              <div class="grid-5 omega">
                <?print render($page['footerleft']); ?>    
              </div>
            <?php endif; ?>
            <?php if ($page ['footermiddle']): ?>
              <div class="grid-5 alpha">
               <?print render($page['footermiddle']); ?>
              </div>
            <?php endif; ?>
            <?php if ($page ['footerright']): ?>
              <div class="grid-6">
                <?print render($page['footerright']); ?>
              </div>
            <?php endif; ?>
        </div>
        <div class="footer_zyxware">Theme by <a href="http://www.zyxware.com/" title="Zyxware" target="_blank">Zyxware</a></div> 
      </div> 
    </div>                                                                                <!-- FOOTER ENDS HERE --> 
  </div>
