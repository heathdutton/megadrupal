
        <!-- Page -->
        <div id="page">
            <!-- Header -->
            <div id="header">
                <!-- Logo -->
                <div id="logo">
                     <?php
                        global $base_path;
                        print '<h1 id="logo"><a href="'. $base_path .'">';
                        print $site_name;
                        print '</a></h1>';
                        if ($site_slogan) {
                            print '<div class="description">'. $site_slogan .'</div>';
                        }
                        ?>
                </div>
                <!-- END Logo -->
                <!-- Main Navigation -->
                <?php if ($main_menu): ?>
                <div id="nav">
                <?/*php print theme('links__system_main_menu', array(
		          'links' => $main_menu,
		          'attributes' => array(
		          'id' => 'front_menu',
			  'class' => array('links', 'clearfix'),
		          ),
		          'heading' => array(
		            'text' => t('Main menu'),
		            'level' => 'h2',
		            'class' => array('element-invisible'),
		          ),
		        )); */?>

		              <?php if ($page ['primary_nav']): ?>
		                <?php // print $main_menu; ?>
		                <?php 
                      if (module_exists('i18n')) {
                        $main_menu_tree = i18n_menu_translated_tree(variable_get('menu_main_links_source', 'main-menu'));
                      } 
                      else {
                        $main_menu_tree = menu_tree(variable_get('menu_main_links_source', 'main-menu'));
                      }
                      print drupal_render($main_menu_tree);
                    ?>
                  <?php endif; ?>

                </div>
                 <?php endif; ?>
                <!-- END Main Navigation -->
                <div class="cl">&nbsp;</div>
            </div>
            <!-- END Header -->
            <!-- Main Block -->
            <div id="main">
                <?php
                print '<a href="'. url('rss.xml') .'"  id="rss-link" >RSS </a>';
                ?>
                <div id="main-top">
                    <div id="main-bot">
                        <div class="cl">&nbsp;</div>
                        <!-- Content -->
                        <div id="content">
                          <!--<div class="post ">-->
                           <?php if ($tabs): print '<div id="tabs-wrapper" class="clear-block">'; endif; ?>
                           <?php if ($title): print '<h2'. ($tabs ? ' class="with-tabs"' : '') .'>'. $title .'</h2>'; endif; ?>
                           <?php if ($tabs): print render ($tabs) . '</div>'; endif; ?>
                           <?php print $messages; ?>
                           <?php print render($page['content']); ?>
                          <!--</div>-->                    
                           <div class="navigation">
                             <div class="alignleft"></div>
                             <div class="alignright"></div>
                           </div>
                        </div>
                        <!-- END Content -->
                        <!-- Sidebar -->
                        
                        <div id="sidebar">
                            <?php if ($page['sidebar_first']): 
                             print render($page['sidebar_first']);
                            endif; ?>
                        </div>
                        <!-- END Sidebar -->
                        <div class="cl">&nbsp;</div>
                    </div>
                </div>
            </div>
            <!-- END Main Block -->
			<!-- Footer -->
			<div id="footer">
                <p>
                  
                </p>
                <p class="rss">
                    <?php print '<a href="'. url('rss.xml') .'"   >Entries (RSS) </a>and'; ?>
                    <?php print '<a href="'. url('rss.xml') .'"  >Comments (RSS)</a>'; ?>
                </p>
                <div class="attribution">
                    Design by
                    <a href="http://cssmayo.com">
                        .css{mayo}
                    </a>
                    | Theme by
                    <a href="http://www.zyxware.com">
                        Zyxware
                    </a>
                </div>
            </div>
            <!-- END Footer -->
        </div>
        <!-- END Page -->
   
