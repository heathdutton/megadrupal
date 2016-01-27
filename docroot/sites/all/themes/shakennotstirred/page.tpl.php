<?php
?>
<div id="authorize">
	<div class="mholder">
		<ul><?php global $user; if ($user->uid != 0) { print '<li class="first">' .t('Logged in as&nbsp;'). '<a href="' .url('user/'.$user->uid). '">' .$user->name. '</a></li>'; print '<li><a href="' .url('logout'). '">' .t('Logout'). '</a></li>'; } else { print '<li class="first"><a href="' .url('user'). '">' .t('Login'). '</a></li>'; print '<li><a href="' .url('user/register'). '">' .t('Register'). '</a></li>'; } ?></ul>
	<?php print $feed_icons; ?>
	</div>
</div><!-- /#authorize -->
<div id="header">
	<?php if ($logo) { ?>
	<div class="mholder">
		<div id="logo"><a href="<?php print $front_page ?>" title="<?php print t('Home') ?>"><img src="<?php print $logo ?>" alt="<?php print t('Home') ?>" /></a></div>
	<?php } else { ?>
		<div class="mholder default">
	<?php } ?>
		<?php if ($site_name) { ?><h1 class='site-name'><a href="<?php print $front_page ?>" title="<?php print t('Home') ?>"><?php print $site_name ?></a></h1><?php } ?>
			<?php if ($site_slogan) { ?><div class='site-slogan'><?php print $site_slogan ?></div><?php } ?>
		</div>
	</div> 
	<?php print render($page['header']); ?>
</div><!-- /#header -->


  <div id="content">
    <div class="mholder">
      <div id="center" valign="top">
          <a id="main-content"></a>
          <?php print $breadcrumb; ?>
					
					<div class="precontent">
						<?php print render($page['precontent']); ?>
						</div>
							
          <?php if ($title): ?>
            <h1><?php print $title ?></h1>
          <?php endif; ?>
					<?php if ($tabs): ?>
            <div id="tabs-wrapper">
              <?php print render($tabs); ?>
            </div>
          <?php endif; ?>		
          <?php print render($tabs_secondary); ?>
          <?php print $messages; ?>
          <?php if ($action_links): ?>
            <ul class="action-links">
              <?php print render($action_links); ?>
            </ul>
          <?php endif; ?>
          <div class="content-wrapper">
            <?php print render($page['content']); ?>
          </div>              
        </div> <!--#center -->

      <?php if ($page['sidebar_second']): ?>
        <div id="sidebar-second" class="sidebar" valign="top">
          <?php print render($page['sidebar_second']); ?>
        </div>
      <?php endif; ?>
		</div>
  </div> <!--#content -->
    
  <div id="footer">
		<div class="mholder">
			<div class="footer-blocks">
				<div class="footer-block">
					<?php print render($page['footer1']); ?>
				</div>
				<div class="footer-block">
					<?php print render($page['footer2']); ?>
				</div>
				<div class="footer-block">
					<?php print render($page['footer3']); ?>
				</div>
				<div class="footer-block">
					<?php print render($page['footer4']); ?>
				</div>
			</div>
			<div class="footer-info"><div class="footer-message">&copy; 2010 Shaken, not stirred theme | <a href="http://drupal.org" target="_blank">Drupal</a></div><div class="copyright"><a href="http://www.adciserver.com" title="Go to adciserver.com">Drupal theme</a> by <a href="http://www.adciserver.com" title="Go to adciserver.com">www.adciserver.com</a></div></div>
		</div>
  </div>