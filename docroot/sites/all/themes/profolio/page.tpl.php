<?php 
/**
 * @file
 * theme implementation to display a single Drupal page.
 */

?>
<div id="wrap">
  <div id="main0">
<div id="header">
  <div id="headcontainer">
    <div id="logowrap">
		<?php if ($logo): ?>
          <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><img src="<?php print $logo ?>" alt="<?php print "logo " . $site_name ?>" title="<?php print $site_name ?>" id="logo" /></a>
        <?php endif; ?>
    
        <?php if ($site_name): ?>
            <h1 id="site-name"<?php if ($hide_site_name) : print ' class="element-invisible"'; endif; ?>>
              <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><span><?php print $site_name; ?></span></a>
            </h1>
          <?php endif; ?>
       
        <?php if ($site_slogan): ?>
          <div id="site-slogan"<?php if ($hide_site_slogan) : print ' class="element-invisible"'; endif; ?>>
            <?php print $site_slogan; ?>
          </div>
            <?php endif; ?>
      </div> <!-- /#logowrap -->
  
   <div id="menu">
     <?php print $primary_nav; ?>
   </div>

  </div>
</div><!--Header-->


<div id="maincontainer">
    <?php if ($page['slider']): ?>
     <div class="scanlines">
       <div class ="innerborder">
         <div class="slider">
           <?php print render($page['slider']); ?>
         </div>
       </div>
     </div>
     <?php endif;?>

    <?php if (($page['top1']) && ($page['top2']) && ($page['top3']) &&  ($page['top4'])): ?>
    <div id="topcontent">
      <div id="top1" class="small"><?php print render($page['top1']); ?></div>
      <div id="top2" class="small"><?php print render($page['top2']); ?></div>
      <div id="top3" class="small"><?php print render($page['top3']); ?></div>
      <div id="top4" class="small"><?php print render($page['top4']); ?></div>
    </div>
  <?php endif;?>
 
    <?php if (($page['top5']) && ($page['top6'])): ?>
    <div id="topcontent2">
      <div id="top5" class="bigger"><?php print render($page['top5']); ?></div>
      <div id="top6" class="bigger"><?php print render($page['top6']); ?></div>
    </div>
    <?php endif;?>
    
    <div id="main">
        <?php if($page['sidebar_first']): ?>
          <div id="sidebar_first" class="sidebar">
            <?php print render($page['sidebar_first']); ?>
          </div>
        <?php endif; ?>
  
      <div id="content">
       <?php print $messages; ?>
	   <?php print render($title_prefix); ?>
         <?php if (!$page): ?>
         <h2 class="title"<?php print $title_attributes; ?>><a href="<?php print $node_url; ?>"><?php print $title; ?></a></h2>
         <?php endif; ?>
	   <?php print render($title_suffix); ?>
            <?php if ($tabs): ?>
              <div class="tabs"><?php print render($tabs); ?></div>
            <?php endif; ?>
            <?php print render($page['help']); ?>
            <?php if ($action_links): ?>
              <ul class="action-links"><?php print render($action_links); ?></ul>
            <?php endif; ?>
            <?php print render($page['content']); ?>
            <div class="feedicons">
              <?php echo $feed_icons ?>
         </div>

    </div><!--content-->
          <?php if($page['sidebar_second']): ?>
             <div id="sidebar_last" class="sidebar">
               <?php print render($page['sidebar_second']); ?>
             </div>
           <?php endif; ?>
  </div><!--main-->
 </div><!--maincontainer-->
  </div> <!--main0-->
</div> <!--maincontainer-->

<div id="footer0">
<div id="footer">
  <div id="footcontainer">
     <div id="foot1"><?php print render($page['foot1']); ?></div>
     <div id="foot2"><?php print render($page['foot2']); ?></div>
     <div id="foot3"><?php print render($page['foot3']); ?></div>
  </div>
</div>
<div id="footer2">
  <div id="footlegal">
    <?php print render($page['footlegal']); ?>   
  </div>
  <div id="ThemeAuthor"><a href="http://www.evad.be">Theme by Evad.be</a></div>
</div>
</div>
