<?php
?>

  <div id="page-wrapper">
    <div id="page">
  
      <div id="header"><div class="container section header clearfix">
  
        <?php if ($logo): ?>
          <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home" id="logo" class="logo">
            <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
          </a>
        <?php endif; ?>
        <div id="log-in-out">
        <?php if(!$logged_in) print l('log in', 'user/login'); ?>
        <?php print theme('links__system_secondary_menu', array('links' => $secondary_menu, 'attributes' => array('id' => 'secondary-menu', 'class' => array('links', 'inline', 'clearfix')), 'heading' => t('Secondary menu'))); ?>
        </div>
  
        <?php if ($site_name || $site_slogan): ?>
          <div id="name-and-slogan">
            <?php if ($site_name): ?>
              <?php if ($title): ?>
                <div id="site-name"><strong>
                  <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><span><?php print $site_name; ?></span></a>
                </strong></div>
              <?php else: /* Use h1 when the content title is empty */ ?>
                <h1 id="site-name">
                  <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><span><?php print $site_name; ?></span></a>
                </h1>
              <?php endif; ?>
            <?php endif; ?>
  
            <?php if ($site_slogan): ?>
              <div id="site-slogan"><?php print $site_slogan; ?></div>
            <?php endif; ?>
          </div> <!-- /#name-and-slogan -->
        <?php endif; ?>
          <?php print render($page['header']); ?>

      </div><!-- /.section .header -->
    </div> <!-- /#header -->
  
      <?php if($main_menu || $page['superfish_menu'] ): ?>
        <div id="navigation">
          <div class="container navigation section">
            <?php if($page['superfish_menu']) {
              print render($page['superfish_menu']);
            } else {
              print theme('links__system_main_menu', array('links' => $main_menu, 'attributes' => array('id' => 'main-menu', 'class' => array('links', 'clearfix'))));        
            }
            ?>
          </div><!-- /.section .navigation -->
        </div> <!-- /#navigation -->
      <?php endif; ?>
  
      <div id="banner-wrap" class="slider-content <?php if(!$page['banner']) print 'empty' ?>">
        <?php if ($page['banner']): ?>
             <div id="banner" class="clearfix">
               <div class="container region">
                 <?php print render ($page['banner']); ?>
               </div>
             </div>
        <?php endif; ?>
      </div>
  
  
      <div id="main-wrapper">
      <?php print $messages; ?>
    
        <?php if ($page['preface_one'] || $page['preface_two'] || $page['preface_three']): ?>
          <div id="preface" class="clearfix">
            <div class="container preface clearfix">
               <?php if ($page['preface_one']): ?>
              <div class="section preface-one<?php print' preface-'.  $preface; ?>">
                <div class="gutter">
                  <?php print render($page['preface_one']); ?>
                </div>
              </div>
              <?php endif; ?>
              <?php if ($page['preface_two']): ?>
              <div class="section preface-two<?php print' preface-'.  $preface; ?>">
                <div class="gutter">
                  <?php print render($page['preface_two']); ?>
                </div>
              </div>
              <?php endif; ?>
              <?php if ($page['preface_three']): ?>
              <div class="section preface-three<?php print' preface-'. $preface; ?>">
                <div class="gutter">
                  <?php print render($page['preface_three']); ?>
                </div>
              </div>
              <?php endif; ?>
            </div>
          </div>
        <?php endif; ?>
        
<div id="content-wrap" class="container content-wrap clearfix">
        
        <div id="main" class="main clearfix">

          <div id="content" class="column clear-fix">
          <?php if ($page['sidebar_first']): ?>
            <div id="first-sidebar" class="column sidebar first-sidebar">
              <div class="section">
                <div class="gutter">
                  <?php print render($page['sidebar_first']); ?>
                </div>
              </div><!-- /.section -->
            </div><!-- /#sidebar-first -->
          <?php endif; ?>
    
            <div class="page-content content-column section">
              <div class="gutter">
                <?php if ($breadcrumb): ?>
                  <div id="breadcrumb" class="container"><?php print $breadcrumb; ?></div>
                <?php endif; ?>
                <?php if ($page['highlighted']): ?><div id="highlighted"><?php print render($page['highlighted']); ?></div><?php endif; ?>
                <a id="main-content"></a>
                <?php print render($title_prefix); ?>
                <?php if ($title): ?><h1 class="title" id="page-title"><?php print $title; ?></h1><?php endif; ?>
                <?php print render($title_suffix); ?>
                <?php if ($tabs): ?><div class="tabs"><?php print render($tabs); ?></div><?php endif; ?>
                <?php print render($page['help']); ?>
                <?php if ($action_links): ?><ul class="action-links"><?php print render($action_links); ?></ul><?php endif; ?>
                <?php print render($page['content']); ?>
                <?php print $feed_icons; ?>
              </div>
            </div><!-- /.section .content .gutter -->
          </div> <!-- /#content -->
    
        </div><!-- /#main -->
        
        <?php if ($page['sidebar_second']): ?>
          <div id="second-sidebar" class="column sidebar second-sidebar">
            <div class="section">
              <div class="gutter">
                <?php print render($page['sidebar_second']); ?>
              </div><!-- /.gutter -->
            </div><!-- /.section -->
          </div> <!-- /#sidebar-second -->
        <?php endif; ?>
    
      </div> <!-- /#main-wrapper -->
  
</div><!-- /#content-main -->
  
    </div><!-- /#page -->
    
    <div id="footer">
      <?php if ($page['bottom_one'] || $page['bottom_two'] || $page['bottom_three'] || $page['bottom_four']): ?>
      <div id="bottom" class="container clearfix">
        <?php if ($page['bottom_one']): ?>
          <div class="region bottom bottom-one<?php print ' bottom-' .  $bottom; ?>">
            <div class="gutter">
              <?php print render($page['bottom_one']); ?>
            </div>
          </div>
        <?php endif; ?>
        <?php if ($page['bottom_two']): ?>
          <div class="region bottom bottom-two<?php print ' bottom-' .  $bottom; ?>">
            <div class="gutter">
              <?php print render($page['bottom_two']); ?>
            </div>
          </div>
        <?php endif; ?>
        <?php if ($page['bottom_three']): ?>
          <div class="region bottom bottom-three<?php print ' bottom-' . $bottom; ?>">
            <div class="gutter">
              <?php print render($page['bottom_three']); ?>
            </div>
          </div>
        <?php endif; ?>
        <?php if ($page['bottom_four']): ?>
          <div class="region bottom bottom-four<?php print ' bottom-'. $bottom; ?>">
            <div class="gutter">
              <?php print render($page['bottom_four']); ?>
            </div>
          </div>
        <?php endif; ?>
      </div>
      <?php endif; ?>
      <div class="container section footer">
        <?php print render($page['footer']); ?>
        <div id="levelten"><?php print l('Drupal Theme', 'http://www.leveltendesign.com/'); ?> by LevelTen Interactive</div>
      </div><!-- /.section -->
    </div> <!-- /#footer -->
    
  </div> <!-- /#page-wrapper -->
