
    <div id="header-section" class="clearfix">
        <div class="wrapper clearfix">
            <div id="logo">
            	<h1><a href="<?php print $base_path ?>" title="Homepage"><?php print $site_name ?></a></h1>
            	<?php if($site_slogan) { ?>
            	    <div id="slogan">
            	        <?php print $site_slogan ?>
            	    </div>
            	<?php } ?>
            </div>  <!-- END LOGO -->
            
    <?php if ($main_menu): ?>
      <div id="nav" class="navigation">
        <?php print theme('links__system_main_menu', array(
          'links' => $main_menu,
          'attributes' => array(
            'id' => 'main-menu-links',
            'class' => array('links', 'clearfix'),
          ),
          'heading' => array(
            'text' => t('Main menu'),
            'level' => 'h2',
            'class' => array('element-invisible'),
          ),
        )); ?>
      </div> <!-- /#nav -->
    <?php endif; ?>
        </div> <!-- END WRAPPER -->
    </div> <!--END HEADER SECTION-->
	

  <?php if ($secondary_menu): ?>
      <div id="subnav" class="navigation">
      	<div class="wrapper">
        	<?php print theme('links__system_secondary_menu', array(
         	 'links' => $secondary_menu,
          	'attributes' => array(
          	  'id' => 'secondary-menu-links',
         	   'class' => array('links', 'inline', 'clearfix'),
        	  ),
         	 'heading' => array(
        	    'text' => t('Secondary menu'),
        	    'level' => 'h2',
         	   'class' => array('element-invisible'),
        	  ),
       	 )); ?>
     	 </div><!--/wrapper-->
      </div> <!-- /subnav -->
    <?php endif; ?>



    <div id="content-region" class="wrapper clearfix">
		
        <?php if ($page['sidebar_first'] OR $logo) { ?>
            <div id="left-sidebar">
							<?php if($logo) {print '<img src="'. check_url($logo) .'" alt="'. $site_name .'" class="sitelogo" />'; } ?>
							<?php print render($page['sidebar_first']); ?>
            </div> <!--END LEFT SIDEBAR-->
        <?php } ?>
        
        <div id="body-section<?php if(!$page['sidebar_first'] AND !$logo) {print '-wide';} ?>">
					<?php print render($page['help']) ?>
					<?php if ($tabs): ?>
						<ul class="tabs primary">
							<?php print render($tabs) ?>
						</ul>
					<?php endif; ?>
					<?php if ($action_links): ?>
						<ul class="action-links">
							<?php print render($action_links); ?>
						</ul>
					<?php endif; ?>
					
					<?php if($page['content_top']) { ?>
                <div id="content-top">
                	<?php print render($page['content_top']); ?>
								</div>
            <?php } ?>
						<?php print render($title_prefix); ?>
            	<h1 id="page-title"><?php print $title ?></h1>
						<?php print render($title_suffix); ?>
            <?php print render($page['content']); ?>
        </div><!--END BODY-SECTION-->
    </div> <!-- END CONTENT-REGION -->
    
    <div id="footer" class="wrapper clearfix">
			<?php if($feed_icons) { ?><div id="feeds"><?php print $feed_icons ?></div><?php } ?>
 			<?php print render($page['footer']) ?>
    </div> <!--END FOOTER-->