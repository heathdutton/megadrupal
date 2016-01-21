    <header>
	  <?php if ($page['header_top']): ?>
	    <div id="header-top">
		  <?php print render($page['header_top']); ?>
	    </div>
	  <?php endif; ?>
	  <div id="header">
	    <?php if ($page['header']): ?>
		  <div class="g3">
		    <?php print render($page['header']); ?>
	      </div>
		  <div class="cf"></div>
	    <?php endif; ?>
	    <div class="g3">
          <?php if ($site_name): ?>
            <?php if ($is_front): /* Use h1 when on front page */ ?>
			  <h1 id="site-name">
                <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><span><?php print $site_name; ?></span></a>
              </h1>
            <?php else:  ?>
              <div id="site-name">
			    <strong><a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><span><?php print $site_name; ?></span></a></strong>
		      </div>
            <?php endif; ?>
          <?php endif; ?>
        </div>
	    <?php if ($site_slogan): ?>
	      <div class="g3">
		    <h2 id="site_slogan"><?php print $site_slogan ?></h2>
		  </div>
	    <?php endif; ?>
	  </div>
	  <div class="cf"></div>
	</header>
	<?php if ($main_menu): ?>
	  <nav>
        <div id="primary-menu" class="g3 nav">
          <?php print theme('links__system_main_menu', array('links' => $main_menu, 'attributes' => array('id' => 'main-menu', 'class' => array('menu', 'links', 'clearfix')))); ?>
        </div><!-- /end #primary-menu -->
	  </nav>
	  <div class="cf"></div>
    <?php endif; ?>
	<section id="section-content" class="section section-content">
	  <div id="page">
	    <?php if ($page['highlighted']): ?>
	    <div class="g2">
	      <?php print render($page['highlighted']); ?>
	    </div>
        <?php endif; ?>
	    <?php if ($page['branding']): ?>
	      <div class="g1">
	        <?php print render($page['branding']); ?>
	      </div>
	    <?php endif; ?>
	    <div class="cf"></div>
	    <?php if ($page['tryptych_top1']): ?>
	      <div class="g1">
	        <?php print render($page['tryptych_top1']); ?>
	      </div>
	    <?php endif; ?>
	    <?php if ($page['tryptych_top2']): ?>
	      <div class="g1">
	        <?php print render($page['tryptych_top2']); ?>
	      </div>
	    <?php endif; ?>
	    <?php if ($page['tryptych_top3']): ?>
	      <div class="g1">
	  	    <?php print render($page['tryptych_top3']); ?>
	      </div>
	    <?php endif; ?>
	    <div class="cf"></div>
	    <?php if ($page['highlighted_2']): ?>
	      <div class="g2">
		    <?php print render($page['highlighted_2']); ?>
	      </div>
	    <?php endif; ?>
	    <?php if ($page['branding2']): ?>
	      <div class="g1">
	        <?php print render($page['branding2']); ?>
	      </div>
	    <?php endif; ?>
	    <?php if ($page['sidebar_first']): ?>
	      <aside id="region-sidebar-first" class="region region-sidebar-first">
		    <div id="sidebar1" class="g1">
	          <?php print render($page['sidebar_first']); ?>
	        </div>
		  </aside><!-- /#region-sidebar-first -->
	    <?php endif; ?>
	    <div id="main" <?php if ($page['sidebar_first'] xor $page['sidebar_second']): ?>class="g2"<?php endif; ?> <?php if ($page['sidebar_first'] && $page['sidebar_second']): ?>class="g1"<?php endif; ?>>
	      <?php if ($page['content_top']): ?>
		    <div class="g3">
		      <?php print render($page['content_top']); ?>
		    </div>
	      <?php endif; ?>
		    <div class="g3">
              <?php if ($breadcrumb): ?>
                <div id="breadcrumb"><?php print $breadcrumb; ?></div>
              <?php endif; ?>
			  <?php if ($page['mission']): ?>
                <div id="mission"><?php print render($page['breadcrumb']); ?></div>
              <?php endif; ?>
			  <?php print $messages; ?>
              <?php print render($page['help']); ?>
              <a id="main-content"></a>
              <?php print render($title_prefix); ?>
              <?php if ($title): ?><h1 class="title" id="page-title"><?php print $title; ?></h1><?php endif; ?>
              <?php print render($title_suffix); ?>
              <?php if ($tabs): ?><div class="tabs"><?php print render($tabs); ?></div><?php endif; ?>
              <?php if ($action_links): ?><ul class="action-links"><?php print render($action_links); ?></ul><?php endif; ?>
              <div id="content" class="column">
                <?php print render($page['content']); ?>
              </div>
              <?php print render($page['$feed_icons']); ?>		
		    </div>
	      <?php if ($page['content_bottom']): ?>
		    <div class="g3">
		      <?php print render($page['content_bottom']); ?>
		    </div>
	      <?php endif; ?>  
	    </div><!-- /#content -->
	    <?php if ($page['sidebar_second']): ?>
	      <aside id="region-sidebar-second" class="region region-sidebar-second">
		    <div id="sidebar2" class="g1">
	          <?php print render($page['sidebar_second']); ?>
	        </div>
		  </aside><!-- /#region-sidebar-second -->
	    <?php endif; ?>
	    <div class="cf"></div>
	    <?php if ($page['tryptych_bottom1']): ?>
	      <div class="g1">
	        <?php print render($page['tryptych_bottom1']); ?>
	      </div>
	    <?php endif; ?>
	    <?php if ($page['tryptych_bottom2']): ?>
	      <div class="g1">
	        <?php print render($page['tryptych_bottom2']); ?>
	      </div>
	    <?php endif; ?>
	    <?php if ($page['tryptych_bottom3']): ?>
	      <div class="g1">
		    <?php print render($page['tryptych_bottom3']); ?>
	      </div>
	    <?php endif; ?>
	  </div><!-- /#page -->
	</section><!-- /section -->
	<?php if (($page['footer']) || ($page['footer_message'])): ?>
	  <footer>
	    <div id="footer" class="g3">
	      <?php if ($page['footer']): ?>
	        <?php print render($page['footer']); ?>
		  <?php endif; ?>
	      <?php if ($page['footer_message']): ?>
	        <div id="footer-message">
	          <?php print render($page['footer_message']); ?>
	        </div>
		  <?php endif; ?>
		</div>
	  </footer>
	<?php endif; ?>