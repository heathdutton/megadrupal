	<!-- Front Page Layout
	================================================== -->
	<div id="page-wrapper">
	  <div id="page" class="container">
	    <header>
	      <div id="header" class="container">
		   <?php if ($page['header']): ?>
		     <?php if ($page['header_top']): ?>
		       <div id="header-top" class="sixteen columns">
			     <?php print render($page['header_top']); ?>
		       </div>
		     <?php endif; ?>
		     <div id="header-main" class="sixteen columns">
		       <?php print render($page['header']); ?>
		     </div>
		   <?php endif; ?>
		   <?php if ($logo): ?>
		     <div id="logo" class="three columns">
               <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home" id="logo">
                 <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
               </a>
		     </div>
            <?php endif; ?>
		    <?php if ($site_name || $site_slogan): ?>
              <div id="name-and-slogan">
                <?php if ($site_name): ?>
                  <?php if ($title): ?>
                    <div id="site-name" class="five columns"><strong>
                      <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><span><?php print $site_name; ?></span></a></strong>
			        </div>
                    <?php else: /* Use h1 when the content title is empty */ ?>
                    <h1 id="site-name" class="five columns">
                      <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><span><?php print $site_name; ?></span></a>
                    </h1>
                  <?php endif; ?>
                <?php endif; ?>
                <?php if ($site_slogan): ?>
                  <div id="site-slogan" class="eight columns">
				    <?php print $site_slogan; ?>
				  </div>
                <?php endif; ?>
              </div> <!-- /#name-and-slogan -->
            <?php endif; ?>
			<?php if ($page['promo_1'] || $page['promo_2']): ?>
			  <div id="promo-1" class="six columns">
			    <?php print render($page['promo_1']); ?>
			  </div><!-- /#promo-1 -->
			  <div id="promo-2" class="six columns">
			    <?php print render($page['promo_2']); ?>
			  </div><!-- /#promo-2 -->
			<?php endif; ?>
		    <?php if ($main_menu): ?>
	          <nav id="zone-menu">
	            <div id="top-navigation" class="region-menu">      
                  <div id="main-menu" class="nav">
				    <h3 class="rwd-menu">Menu</h3>
                    <?php print $main_menu_tree; ?>
                  </div><!-- /end #primary-menu -->
		        </div><!-- /end #top-navigation -->
	          </nav>
            <?php endif; ?>
		    <div class="clear"></div>
	      </div><!-- /#header -->
	    </header>
	    <a name="main-content" id="main-content"></a>
	    <?php if ($page['showcase']): ?>
	      <section id="showcase">
	        <div id="showcase">
	          <div class="sixteen columns">
	            <?php print render($page['showcase']); ?>
			    <div class="clear"></div>
	          </div>
	        </div><!-- /#showcase -->
		    <div class="clear"></div>
	      </section>
	    <?php endif; ?>
	    <?php if ($page['homepage_message']): ?>
	      <section id="homepage-message">
	        <div class="sixteen columns">
	          <div class="homepage-message">
	            <?php print render($page['homepage_message']); ?>
		      </div>
	        </div>
	      <section>
	    <?php endif; ?>
		<?php if ($page['top_thirds_1'] || $page['top_thirds_2'] || $page['top_thirds_3']): ?>
	      <section id="top-thirds-columns">
	        <div id="top-thirds-columns" class="container">
	          <div class="one-third column">
			    <?php if ($page['top_thirds_1']): ?>
	              <?php print render($page['top_thirds_1']); ?>
			    <?php endif; ?>
	          </div>
	          <div class="one-third column">
			    <?php if ($page['top_thirds_2']): ?>
	              <?php print render($page['top_thirds_2']); ?>
			    <?php endif; ?>
	          </div>
	          <div class="one-third column">
			    <?php if ($page['top_thirds_3']): ?>
	              <?php print render($page['top_thirds_3']); ?>
			    <?php endif; ?>
	          </div>
			  <div class="clear"></div>
	        </div><!-- /#thirds-columns -->
	      </section>
	    <?php endif; ?>
	    <section id="main-content">
	      <div id="main-content" class="container">
	          <?php if ($messages): ?>
                <div id="messages">
                  <?php print $messages; ?>
                </div><!-- /#messages -->
              <?php endif; ?>
              <?php if ($breadcrumb): ?>
                <div id="breadcrumb"><?php print $breadcrumb; ?></div>
              <?php endif; ?>			  
              <div id="main" class="twelve columns"> 
                <?php print render($title_prefix); ?>
                <?php if ($title): ?>
                  <h1 class="title" id="page-title"><?php print $title; ?></h1>
                <?php endif; ?>
                <?php print render($title_suffix); ?>
                <?php if ($tabs): ?>
                  <div class="tabs">
                    <?php print render($tabs); ?>
                  </div>
                <?php endif; ?>
                <?php print render($page['help']); ?>
                <?php if ($action_links): ?>
                  <ul class="action-links">
                    <?php print render($action_links); ?>
                  </ul>
                <?php endif; ?>
                <?php print render($page['content']); ?>
		      </div>
			  <?php if ($page['sidebar_first']): ?>
			    <div id="sidebar_first" class="four columns">
				  <?php print render($page['sidebar_first']); ?>
				</div>
              <?php endif; ?>
	      </div>
	    </section>
	    <?php if ($page['quarters_1'] || $page['quarters_2'] || $page['quarters_3'] || $page['quarters_4'] || $page['quarter_columns_top']): ?>
	      <section id="quarter-columns">
	        <?php if ($page['quarter_columns_top']): ?>
	          <div id="quarter-columns-top" class="container">
		        <?php print render($page['quarter_columns_top']); ?>
		      </div>
		    <?php endif; ?>
		    <?php if ($page['quarters_1'] || $page['quarters_2'] || $page['quarters_3'] || $page['quarters_4']): ?>
	          <div id="quarter-columns">
	            <div class="four columns">
	              <?php print render($page['quarters_1']); ?>
	            </div>
	            <div class="four columns">
	              <?php print render($page['quarters_2']); ?>
	            </div>
	            <div class="four columns">
	              <?php print render($page['quarters_3']); ?>
	            </div>
	            <div class="four columns">
	              <?php print render($page['quarters_4']); ?>
	            </div>
			    <div class="clear"></div>
	          </div><!-- /#quarter-columns -->
		    <?php endif; ?>
	      </section>
	    <?php endif; ?>
	    <?php if ($page['mid_section_1']): ?>
	      <section id="mid-section-1">
		    <div id="mid-section-1" class="container">
		      <?php print render($page['mid_section_1']); ?>
		    <div class="clear"></div>
		    </div><!-- /#mid-section-1 -->
		  </section>
	    <?php endif; ?>
	    <?php if ($page['btm_thirds_1'] || $page['btm_thirds_2'] || $page['btm_thirds_3']): ?>
	      <section id="btm-thirds-columns">
	        <div id="btm-thirds-columns" class="container">
	          <div class="one-third column">
	            <?php print render($page['btm_thirds_1']); ?>
	          </div>
	          <div class="one-third column">
	            <?php print render($page['btm_thirds_2']); ?>
	          </div>
	          <div class="one-third column">
	            <?php print render($page['btm_thirds_3']); ?>
	          </div>
			  <div class="clear"></div>
	        </div><!-- /#thirds-columns -->
	      </section>
	    <?php endif; ?>
	    <?php if (($page['footer']) || ($page['footer_message'])): ?>
	      <footer id="footer">
		    <div id="footer" class="container">
		      <div id="footer-left" class="eight columns">
		        <?php print render($page['footer']); ?>
		      </div><!-- /#footer-left -->
		      <div id="footer-right" class="eight columns">
		        <?php print render($page['footer_right']); ?>
			  </div><!-- /#footer-right -->
			  <div id="footer-message" class="eight columns">
			    <?php print render($page['footer_message']); ?>
			  </div>
			  <div id="credits" class="eight columns">
			    <a href="http://themeroot.com" title="Responsive Drupal Themes">Responsive Drupal Themes</a> by <a href="http://arborwebdev.com" title="Arbor Web Development">Arbor Web Development</a>
			  </div>
			  <div class="clear"></div>
		    </div><!-- /#footer -->
		  </footer>
	    <?php endif; ?>
	  </div><!-- /#page -->
	</div><!-- /#page-wrapper -->
<!-- End Document
================================================== -->