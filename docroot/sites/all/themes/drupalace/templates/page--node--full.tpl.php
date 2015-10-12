<div id="wrapper">

		<div id="header">
      <div id="header-inner">

        <?php if ($main_menu): ?>
          <div id="top-menu" class="navigation">
            <div id="top-menu-inner">
              <?php print theme('links__system_main_menu', array('links' => $main_menu)); ?>
            </div>
          </div>
        <?php endif; ?>

        <a href="<?php print $front_page ?>" id="logo" rel = "home">
          <?php if ($logo): ?>
            <img src="<?php print $logo; ?>" alt="<?php print $site_name; ?>" title="<?php print $site_name; ?>" />
          <?php endif; ?>
        </a>

        <?php if ($site_name): ?>
          <a href="<?php print $front_page; ?>" id="site-name" title="<?php print $site_name; ?>" rel = "home">
            <?php echo $site_name; ?>
          </a>
        <?php endif; ?>

        <?php if ($site_slogan): ?>
          <div id = "site-slogan">
            <?php echo $site_slogan; ?>
          </div>
        <?php endif; ?>

        <?php if (isset($login_button)): ?>
          <?php print $login_button; ?>
        <?php endif; ?>

        <?php print render($page['header']); ?>

			</div> <!-- /#header-inner -->
		</div> <!-- /#header -->

    <div id="container-wrapper" class="clearfix">
      <div id="container" class="clearfix">

        <?php if ($page['content_top']): ?>
          <div id="page-top" class="clearfix">
            <?php print render($page['content_top']); ?>
          </div>
        <?php endif; ?>

        <div id="main">
          <div id = "main-inner" <?php if ($page['sidebar']) print 'class = "with-sidebar"'; ?>>

            <?php if ($title): ?>
              <?php print render($title_prefix); ?>
              <h1<?php print $title_attributes; ?>><?php if (isset($title_additional)) print $title_additional; ?><?php print $title; ?></h1>
              <?php print render($title_suffix); ?>
            <?php endif; ?>

            <?php print render($tabs); ?>
            <?php print render($tabs2); ?>
            <?php print $messages; ?>

            <?php if ($action_links): ?>
              <ul class="action-links">
                <?php print render($action_links); ?>
              </ul>
            <?php endif; ?>

            <?php print render($page['content']); ?>

          </div> <!-- /#main-inner -->
        </div> <!-- /#main -->

        <?php if ($page['sidebar']): ?>
           <div id="sidebar">
             <?php print render($page['sidebar']); ?>
           </div>
         <?php endif; ?>

         <div class = "clearfix"></div>

         <?php if ($page['content_bottom']): ?>
           <div id="page-bottom" class="clearfix">
             <?php print render($page['content_bottom']); ?>
           </div>
         <?php endif; ?>

       </div> <!-- /#container -->
     </div> <!-- /#container-wrapper -->

     <?php if ($secondary_menu): ?>
       <div id = "footer-menu">
         <div id = "footer-menu-inner">
             <?php print theme('links__system_second_menu', array('links' => $secondary_menu)); ?>
         </div>
       </div>
     <?php endif; ?>

   <div id = "footer">
     <div id = "footer-inner">

       <?php if ($page['footer_second']): ?>
         <div id = "footer-region-second">
           <div id = "footer-region-second-inner" class="<?php if ($page['footer_first']) print 'left-region'; ?> <?php if ($page['footer_third']) print 'right-region'; ?>">
             <?php print render($page['footer_second']); ?>
           </div>
         </div>
       <?php endif; ?>

       <?php if ($page['footer_first']): ?>
         <div id = "footer-region-first">
           <?php print render($page['footer_first']); ?>
         </div>
       <?php endif; ?>

       <?php if ($page['footer_third']): ?>
         <div id = "footer-region-third">
           <?php print render($page['footer_third']); ?>
         </div>
       <?php endif; ?>

       <div class = "clear"></div>

       <?php print render($page['footer']); ?>

       <?php if (isset($developers)): ?>
         <div id = "developer"><?php print $developers; ?></div>
       <?php endif; ?>

     </div> <!-- /#footer-inner -->
   </div> <!-- /#footer -->

 </div> <!-- /#wrapper -->