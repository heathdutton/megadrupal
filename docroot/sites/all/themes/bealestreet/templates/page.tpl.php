<?php

/**
 * @file
 * Beale Street page.tpl.php
 *
 * for Default theme implementation see module/system/page.tpl.php
 *
 */
?>
<div class="page">
  <div id="loginWrapper">
    <div id="login-box">
      <?php
      global $user;
      if ($logged_in) {
        print t('You are logged in as ') . l(t('@name', array('@name' => $user->name)), 'user', array('attributes' => array('title' => t('@name', array('@name' => $user->name)), 'class' => array('loginwraper', 'user-name'))));
        print t(': ');
        print (l(t('Logout'), 'user/logout', array('attributes' => array('title' => t('Logout'), 'class' => array('loginwrapper logout')))));
      }
      else {
        print t('Welcome Visitor: ') . (l(t('Login/Register'), 'user/login', array('attributes' => array('title' => t('Login/Register'), 'class' => array('loginwrapper', 'login')))));
      }
      ?>
    </div>
  </div><!-- /login -->
  <div class="topWrapper">
    <div class="topBlock">
      <div id="masthead">
        <div id="header" class="clearfix">
          <div class="mastheadRight">
            <div class="mastheadLeft">
              <?php if ($site_name || $site_slogan || $logo || $page['search_box']): ?>
                <table id="logo-name-slogan" border="0">
                  <tr>
                    <?php if ($logo): ?>
                      <td id="logo-title">
                        <a href="<?php print $front_page ?>" title="<?php print t('Home'); ?>"><img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" id="logo" /></a>
                        <!-- /Logo -->
                      </td>
                    <?php endif; ?>
                    <?php if ($site_name || $site_slogan): ?>
                      <td id="name-and-slogan">
                        <div>
                          <?php if ($site_name): ?>
                            <div id="site-name">
                              <h1><a href="<?php print $front_page ?>" title="<?php print t('Home'); ?>"><?php print $site_name; ?></a></h1>
                            </div><!-- /site-name -->
                          <?php endif; ?>
                          <?php if ($site_slogan): ?>
                            <div id="site-slogan">
                              <?php print $site_slogan; ?>
                            </div><!-- /site-slogan -->
                          <?php endif; ?>
                        </div>
                      </td>
                    <?php endif; ?>
                    <?php if ($page['search_box']): ?>
                      <td id="searchbox">
                        <div id="search">
                          <?php print render($page['search_box']); ?>
                        </div><!-- /searchbox -->
                      </td>
                    <?php endif; ?>
                  </tr>
                </table>
              <?php endif; ?>
              <?php if ($page['header']): ?> 
                <div class="clearfix">
                  <?php print render($page['header']); ?>
                </div><!-- /Header -->
              <?php endif; ?>
            </div> <!-- /mastheadLeft -->
          </div> <!-- /mastheadRight -->
        </div> <!-- /header -->
      </div> <!-- /masthead -->
    </div> <!-- /topBlock -->
  </div>  <!-- /topWrapper -->
  <div class="middleWrapper">
    <div class="middleBlock">
      <?php if ($page['banner']): ?>
        <div id='banner'>
          <?php print render($page['banner']); ?>
        </div>
      <?php endif; ?>
      <div id="navigation" class="menu <?php if ($main_menu) { print "withprimary"; } if ($secondary_menu) { print " withsecondary"; } ?> ">
        <?php if ($main_menu): ?>
          <div id="primarymenu" class="clearfix links">
            <?php print theme('links__system_main_menu', array('links' => $main_menu, 'attributes' => array('id' => array('primary'), 'class' => array('clearfix')))); ?>
          </div>
        <?php endif; ?>
        
        <?php if ($secondary_menu): ?>
          <div id="secondarymenu" class="clearfix links">
            <?php print theme('links__system_secondary_menu', array('links' => $secondary_menu, 'attributes' => array('id' => array('secondary'), 'class' => array('clearfix')))); ?>
          </div>
        <?php endif; ?>
      </div> <!-- /navigation -->
      <?php if ($page['suckerfish_menu']): ?>
        <div id="suckerfishmenu" class="clearfix">
          <?php print render($page['suckerfish_menu']); ?>
        </div>
      <?php endif; ?>
      <?php
      $section1count = 0;
        if ($page['user1']) {
      $section1count++;
      }
      if ($page['user2']) {
        $section1count++;
      }
      if ($page['user3']) {
        $section1count++;
      }
      ?>
      <?php if ($section1count): ?>
        <?php $section1width = 'width' . floor(99 / $section1count); ?>
        <div class="clr" id="section1">
          <table class="sections" cellspacing="0" cellpadding="0">
            <tr>
              <td class="topleft1"></td>
              <td class="tophorizontal1" colspan="<?php echo $section1count ?>"></td>
              <td class="topright1"></td>
            </tr>
            <tr valign="top">
            <td  class="leftvertical1">&nbsp;</td>
            <?php if ($page['user1']): ?>
              <td class="section colorsection <?php echo $section1width ?>">
                <?php print render($page['user1']); ?>
              </td>
            <?php endif; ?>
            <?php if ($page['user2']): ?>
              <td class="section colorsection <?php echo $section1width ?>">
                <?php print render($page['user2']); ?>
              </td>
            <?php endif; ?>  
            <?php if ($page['user3']): ?>
              <td class="section colorsection <?php echo $section1width ?>">
                <?php print render($page['user3']); ?>
              </td>
            <?php endif; ?>  
            <td class="rightvertical1">&nbsp;</td>
            </tr>
            <tr>
              <td class="bottomleft1"></td>
              <td class="bottomhorizontal1" colspan="<?php echo $section1count ?>"></td>
              <td class="bottomright1"></td>
            </tr>
          </table>
        </div>  <!-- /section1 -->
      <?php endif; ?>
      <?php
     $section2count = 0;
     if ($page['user4']) {
        $section2count++;
     }
     if ($page['user5']) {
        $section2count++;
     }
     if ($page['user6']) {
        $section2count++;
     }
      ?>
      <?php if ($section2count): ?>
        <?php $section2width = 'width' . floor(99 / $section2count); ?>
        <div class="clr" id="section2">
          <table class="sections" cellspacing="0" cellpadding="0">
            <tr>
              <td class="topleft2"></td>
              <td class="tophorizontal2" colspan="<?php echo $section2count ?>"></td>
              <td class="topright2"></td>
            </tr>
            <tr valign="top">
              <td  class="leftvertical2">&nbsp;</td>
              <?php if ($page['user4']): ?>
                <td class="section blacksection <?php echo $section2width ?>">
                  <?php print render($page['user4']); ?>
                </td>
              <?php endif; ?>  
              <?php if ($page['user5']): ?>
                <td class="section blacksection <?php echo $section2width ?>">
                  <?php print render($page['user5']); ?>
                </td>
              <?php endif; ?>
                <?php if ($page['user6']): ?>
                <td class="section blacksection <?php echo $section2width ?>">
                  <?php print render($page['user6']); ?>
                </td>
              <?php endif; ?>
              <td class="rightvertical2">&nbsp;</td>
            </tr>
            <tr>
              <td class="bottomleft2"></td>
              <td class="bottomhorizontal2" colspan="<?php echo $section2count ?>"></td>
              <td class="bottomright2"></td>
            </tr>
          </table>
        </div>  <!-- /section2 -->
      <?php endif; ?>
      <div id="middlecontainer">
        <table border="0" cellpadding="0" cellspacing="0" id="content">
          <tr>
            <?php if ($page['sidebar_first']) { ?><td id="sidebar-left">
              <?php print render($page['sidebar_first']) ?>
            </td><?php } ?>
            <td valign="top">
              <?php if ($page['mission']) { ?><div id="mission"><?php print render($page['mission']) ?></div><?php } ?>
              <div id="main">
                <?php if (theme_get_setting('bealestreet_breadcrumb')): ?>
                  <?php if ($breadcrumb): ?>
                     <div id="breadcrumb">
                       <?php print $breadcrumb; ?>
                     </div>
                  <?php endif; ?>
                  <?php endif; ?>
                  <?php if ($page['content_top']):?><div id="content-top"><?php print render($page['content_top']); ?></div><?php endif; ?>
                  <?php if ($title): ?>
                    <div id="branding" class="clearfix">
                      <?php print render($title_prefix); ?>
                      <h1 class="title"><?php print render($title); ?></h1>
                      <?php print render($title_suffix); ?>
                    </div>
                  <?php endif; ?>
                  <div class="tabs"><?php print render($tabs) ?></div>
                  <?php if ($action_links): ?>
                    <!-- action links -->
                    <ul class="action-links">
                      <?php print render($action_links); ?>
                    </ul>
                    <!-- /action links -->
                  <?php endif; ?>
                  <?php if ($page['help']): ?>
                    <!-- Help region -->
                    <?php print render($page['help']); ?>
                    <!-- /Help region -->
                  <?php endif; ?>
                  <?php if ($show_messages) { print $messages; } ?>
                  <?php print render($page['content']); ?>
                  <?php // print $feed_icons; ?>
                  <?php if ($page['content_bottom']): ?><div id="content-bottom"><?php print render($page['content_bottom']); ?></div><?php endif; ?>
                </div>
              </td>
              <?php if ($page['sidebar_second']) { ?>
                <td id="sidebar-right">
                  <?php print render($page['sidebar_second']) ?>
                </td>
              <?php } ?>
          </tr>
        </table>
      </div>
      <?php
      $section3count = 0;
      if ($page['user7']) {
        $section3count++;
      }
      if ($page['user8']) {
        $section3count++;
      }
      if ($page['user9']) {
        $section3count++;
      }
      ?>

      <?php if ($section3count): ?>
        <?php $section3width = 'width' . floor(99 / $section3count); ?>
        <div class="clr" id="section3">
          <table class="sections" cellspacing="0" cellpadding="0">
            <tr>
              <td class="topleft1"></td>
              <td class="tophorizontal1" colspan="<?php echo $section3count ?>"></td>
              <td class="topright1"></td>
            </tr>
            <tr valign="top">
              <td  class="leftvertical1">&nbsp;</td>
              <?php if ($page['user7']): ?>
                <td class="section colorsection <?php echo $section3width ?>">
                  <?php print render($page['user7']); ?>
                </td>
              <?php endif; ?>
              <?php if ($page['user8']): ?>
                <td class="section colorsection <?php echo $section3width ?>">
                  <?php print render($page['user8']); ?>
                </td>
              <?php endif; ?>  
              <?php if ($page['user9']): ?>
                <td class="section colorsection <?php echo $section3width ?>">
                  <?php print render($page['user9']); ?>
                </td>
              <?php endif; ?>  
              <td class="rightvertical1">&nbsp;</td>
            </tr>
            <tr>
              <td class="bottomleft1"></td>
              <td class="bottomhorizontal1" colspan="<?php echo $section3count ?>"></td>
              <td class="bottomright1"></td>
            </tr>
          </table>
        </div>  <!-- /section3 -->
      <?php endif; ?>
      <?php
       $section4count = 0;
       if ($page['user10']) {
        $section4count++;
       }
       if ($page['user11']) {
        $section4count++;
       }
       if ($page['user12']) {
        $section4count++;
       }
      ?>
      <?php if ($section4count): ?>
        <?php $section4width = 'width' . floor(99 / $section4count); ?>
        <div class="clr" id="section4">
          <table class="sections" cellspacing="0" cellpadding="0">
            <tr>
              <td class="topleft2"></td>
              <td class="tophorizontal2" colspan="<?php echo $section4count ?>"></td>
              <td class="topright2"></td>
            </tr>
            <tr valign="top">
              <td  class="leftvertical2">&nbsp;</td>
                <?php if ($page['user10']): ?>
                  <td class="section blacksection <?php echo $section4width ?>">
                <?php print render($page['user10']); ?>
              </td>
              <?php endif; ?>
              <?php if ($page['user11']): ?>
                <td class="section blacksection <?php echo $section4width ?>">
                  <?php print render($page['user11']); ?>
                </td>
              <?php endif; ?>
              <?php if ($page['user12']): ?>
                <td class="section blacksection <?php echo $section4width ?>">
                  <?php print render($page['user12']); ?>
                </td>
              <?php endif; ?>
              <td class="rightvertical2">&nbsp;</td>
            </tr>
            <tr>
              <td class="bottomleft2"></td>
              <td class="bottomhorizontal2" colspan="<?php echo $section4count ?>"></td>
              <td class="bottomright2"></td>
            </tr>
          </table>
        </div>  <!-- /section4 -->
      <?php endif; ?>
    </div>	 <!-- /middleBlock -->
  </div>	 <!-- /middleWrapper -->
  <div class="bottomWrapper">
    <div class="bottomBlock">
      <div id="footer-wrapper" class="clearfix">
        <div class="footer-right">
          <div class="footer-left">
            <?php if ($page['footer'] || $page['footer_message'] || theme_get_setting('bealestreet_footerlogo')): ?>
              <div id="footer">
                <?php if ($page['footer']): ?>
                  <?php print render($page['footer']) ?>
                <?php endif; ?>
                <?php if ($page['footer_message']): ?>
                  <?php print render($page['footer_message']) ?>
                <?php endif; ?>
                <?php if (theme_get_setting('bealestreet_footerlogo')): ?>
                  <div class="rooplelogo"><a href="http://www.roopletheme.com" title="RoopleTheme!" target="_blank"><img src="<?php print base_path() . path_to_theme() . "/roopletheme.png"; ?>" alt="RoopleTheme"/></a></div>
                <?php endif; ?>
              </div>
            <?php endif; ?>	
          </div> <!-- /footer-left -->
        </div> <!-- /footer-right -->
      </div> <!-- /footer-wrapper -->
    </div> <!-- /bottomBlock -->
  </div> <!-- /bottomWrapper -->
  <?php // print $closure ?>
</div> <!-- /page -->
