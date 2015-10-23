<div id="wrapper">
  		<div class="headerwrap">
  			<div class="header container-12">
  				<div class="grid-12 login">
  					<?php if ($page ['login']): ?>
                    <?print render($page['login']); ?>  
                    <?php endif; ?>
  				</div>
  				<div class="grid-12">
  					<div class="logo grid-4 alpha">
  						<?php if ($site_name): ?>
                        <a href="<?php print check_url($front_page); ?>" title="<?php print t('Home'); ?>">
                        <img src="<?php print $logo; ?>" alt="<?php print $site_name; ?>" />
                        </a>
                        <?php endif; ?>
  					</div>
  					<div class="grid-4 alpha omega colorschemes">
  					  
					<?php if ($page ['styleswitcher']): ?>
                      <?print render($page['styleswitcher']); ?>  
                      <?php endif; ?>
  					</div>
  					<div class="grid-4 socialmediaicons omega">
	  				  <?php if ($page ['mediaicons']): ?>
                      <?print render($page['mediaicons']); ?>  
                      <?php endif; ?>
  					</div>
  				</div>
  				<div class="container-12 primarynavwrapper">
	  				<div class="grid-8 omega primarynav">
	  				<?php if ($page ['primarynav']): ?>
					<div class="navbar">
						<? print render($page['primarynav']); ?> 
					</div>
					<?php endif; ?>
	  				</div>
	  				<div class="grid-4 alpha search">
	  				  <?php if ($page ['search']): ?>
                      <?print render($page['search']); ?>  
                      <?php endif; ?>
	  				</div>
  				</div>
  			</div>
  		</div>
  		<div class="middleline"></div>
  		<div class="contentwrapper">
  			<div class="container-12">
  			  <div class="<?php print ($page['articleright'] && $page['articlerighttwo']) ? 'grid-8 mainarticle' : (($page['articleright'] || $page['articlerighttwo']) ? 'grid-8 mainarticle' : 'grid-12 mainarticle') ?>">
  			    <?php print render($title_prefix); ?>
            <?php print render($title_suffix); ?>
            <?php print $messages; ?>
            <?php if ($tabs): ?><?php print render($tabs); ?><?php endif; ?>
              <?php if ($title): ?>
                <div class="articleheading">
                  <h1><?php //print $title ?></h1>
                </div>
              <?php endif; ?>             
              <?php if ($page['content']): ?>
                <?php print render($page['content']); ?>
              <?php endif; ?>
              <?php if ($feed_icons): ?><?php print $feed_icons; ?><?php endif; ?>
  			  </div>
  			  <?php if ($page['articleright'] || $page['articlerighttwo']): ?>
  			     <div class="grid-4 alpha omega">
  			       <div class="grid-4">
     	           <?php if ($page ['articleright']): ?>
                   <?print render($page['articleright']); ?>  
                 <?php endif; ?>
               </div>	
               <div class="grid-4">
     	           <?php if ($page ['articlerighttwo']): ?>
                   <?print render($page['articlerighttwo']); ?>  
                 <?php endif; ?>
               </div>	
             </div>
          <?php endif; ?>
  			</div>
  				
  			</div>
  			<div class="container-12 bottomblocks">
  			   <div class="grid-4">
     	        <?php if ($page ['articlebottomone']): ?>
                <?print render($page['articlebottomone']); ?>  
                <?php endif; ?>
                  </div>
                  		   <div class="grid-4">
     	        <?php if ($page ['articlebottomtwo']): ?>
                <?print render($page['articlebottomtwo']); ?>  
                <?php endif; ?>
                  </div>
                  		   <div class="grid-4">
     	        <?php if ($page ['articlebottomthree']): ?>
                <?print render($page['articlebottomthree']); ?>  
                <?php endif; ?>
                  </div>
  			</div> 		
  			
  		</div>
<div class="footerwrapper">
  			<div class="footer container-12">
  				<?php if ($page ['footernav']): ?>
  				      <div class="grid-12">
                <?print render($page['footernav']); ?>  
                </div>
                <?php endif; ?>
  				<?php if ($page ['footericons']): ?>
  				      <div class="grid-12">
                <?print render($page['footericons']); ?> 
                </div> 
                <?php endif; ?>
  				<?php if ($page ['footercopyright']): ?>
  				      <div class="grid-12">
                <?print render($page['footercopyright']); ?>  
                </div>
                <?php endif; ?>
                <div class="footer_zyxware">Theme by <a href="http://www.zyxware.com/" title="Zyxware" target="_blank">Zyxware</a></div>
  			</div>
  		</div>
  	</div>
