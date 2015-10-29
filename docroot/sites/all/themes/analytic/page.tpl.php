<!--  make-it-center --><div class="make-it-center">

<div class="top-menu clearfix">

    <?php if ($page['highlight']):?>
    <div class="mission">
        <?php echo render($page['highlight']);?>
    </div>
    <?php endif;?>
  
  <!-- >>>>>>>> REMOVE THIS IF YOU WANT TO GET RID OF TOP LINKS (RSS, LOGIN, REGISTER | PROFILE) start-->  
  
  <div id="top-links">
    <ul class="top-links-ul">
    <?php if (!$user->uid): ?>	
    	<li><?php echo l(t("Log in"), "user");?></li>
      <?php if (variable_get('user_register', 1)): ?>    	
    	<li><?php echo l(t("Create new account"), "user/register");?></li>
    	<?php endif; ?>
    <?php else: ?> 
       <li><?php echo t("You are logged in as <strong>!user</strong>", array('!user' => l($user->name, "user"))); ?>&nbsp;|&nbsp;<?php echo l(t("Edit"), "user/" . $user->uid . "/edit");?></li>
      	<li><?php echo l(t("Log out"), "user/logout"); ?></li>
    <?php endif; ?>
    
	<?php if ($feed_icons): ?>
	<li><a href="<?php echo url("rss.xml"); ?>"><img src="<?php echo base_path() . path_to_theme() ?>/images/rss.gif"  alt="RSS" /></a></li>
	<?php endif; ?>
   
	
	</ul>
  </div>
  
  <!-- <<<<<<<< REMOVE THIS IF YOU WANT TO GET RID OF TOP LINKS (RSS, LOGIN, REGISTER) end -->

</div>

<!-- logo-container -->
<div id="logo-container">
  <div id="money-bg" class="clearfix">
  <div id="logo<?php if ($logo && !$site_name && !$site_slogan): ?>-no-padding<?php endif; ?>">
  
			<?php if ($logo): ?> 
            <div id="logo-picture">
            <a href="<?php print $base_path ?>" title="<?php print t('Home') ?>"><img src="<?php print $logo ?>" alt="<?php print t('Home') ?>" /></a>
            </div>
            <?php endif; ?>


 <?php if ($site_name): ?>
 <!-- if logo picture is defined, text is aligned to left -->
 
    <h1 <?php if ($logo && !$site_slogan): ?>class="logo-picture-true-slogan-false"<?php endif; ?>  <?php if ($logo): ?>class="logo-picture-true"<?php endif; ?>><a href="<?php echo $front_page; ?>" title="<?php echo t('Home') ?>"><?php echo $site_name ?></a></h1>
  <?php endif; ?>
  
 <?php if ($site_slogan): ?>
 <!-- if logo defined, text is aligned to left -->
    <strong <?php if ($logo): ?>class="logo-picture-true"<?php endif; ?>><?php echo $site_slogan; ?></strong>
  <?php endif; ?> 
  </div>
  </div>
</div>
<!-- /logo-container -->


<!-- primary menu -->

<div class="rws-primary-menu clearfix">
  
<?php if ($page['searchbox_region']): ?><div class="searchbox-region"><?php echo render($page['searchbox_region']); ?></div><?php endif; ?>


<!-- render div that will make primary menu smaller because of search -->
<?php if ($page['searchbox_region']&&$page['primary_menu']): ?><div class="primary-with-search"><?php endif; ?>



<?php echo render($page['primary_menu']); ?>


<!-- render div that will make primary menu smaller because of search -->
<?php if ($page['searchbox_region']&&$page['primary_menu']): ?></div><?php endif; ?>



    <!-- admin panel 
   <?php if ($is_admin): ?>
    <ul id="rws-uni-tabs" class="clearfix">
      <li><?php echo l(t("Administer"), "admin"); ?></li>
      <li><?php echo l(t("Blocks"), "admin/structure/block"); ?></li>
      <li><?php echo l(t("Menus"), "admin/structure/menu"); ?></li>
      <li><?php echo l(t("Modules"), "admin/modules"); ?></li>
      <li><?php echo l(t("Translation"), "admin/config/regional/language"); ?></li>
      <li><?php echo l(t("Cashe"), "admin/config/development/performance"); ?></li>
    </ul>
  <?php endif; ?>
 / admin panel -->


</div>

<!--  /primary menu -->


<?php if ($page['left']): ?>
<!-- column-1 -->
<div class="column-1"><?php echo render($page['left']) ?></div>
<!-- / column-1 -->
<?php endif; ?>





<!-- column-2 --><div class="column-2 
<?php if (!$page['right'] && !$page['left']): ?>no-right-and-left-columns
<?php elseif (!$page['left']): ?>
no-left-column
<?php elseif (!$page['right']): ?>
no-right-column
<?php endif; ?>
">


		<?php if ($messages): ?>
		<?php echo ($messages); ?>
		<?php endif; ?>
        


<!-- PRINTING BLOCKS BEFORE THE CONTENT (with RED headers) -->
<?php if ($page['top_content_block_left'] || $page['top_content_block_right']): ?>
  <!-- column-2-blocks -->
  <div id="block-top" class="column-2-blocks clearfix
  <?php if (!$page['right']&&!$page['left']): ?>column-2-blocks-no-right-and-left-columns
  <?php elseif (!$page['left']): ?>
  column-2-blocks-no-left-column
  <?php elseif (!$page['right']): ?>
  column-2-blocks-no-right-column
  <?php endif; ?>
  ">
  <!-- /column-2-blocks-left --><div class="column-2-blocks-left">
  <?php if ($page['top_content_block_left']): ?><?php echo render($page['top_content_block_left']) ?><?php endif; ?>
  <?php if (!$page['top_content_block_left']): ?>&nbsp;<?php endif; ?>
  <!-- /column-2-blocks-left --></div>
  <!-- /column-2-blocks-right --><div class="column-2-blocks-right">
  <?php if ($page['top_content_block_right']): ?><?php echo render($page['top_content_block_right']) ?><?php endif; ?>
  <!-- /column-2-blocks-right --></div>
  <!-- /column-2-blocks --></div>
<?php endif; ?>
<!-- PRINTING BLOCKS BEFORE THE CONTENT (with RED headers) -->

<?php if ($page['content_top']): ?><div id="content-top"><?php echo render($page['content_top']) ?></div><?php endif; ?>



		<?php echo $breadcrumb;?>
		
		<?php echo render($help) ?>



		
		

                <?php if ($tabs): ?><div id="tabs-wrapper" class="clearfix"><?php endif; ?>
          <?php print render($title_prefix); ?>
          <?php if ($title): ?>
            <h1<?php print $tabs ? ' class="with-tabs"' : '' ?>><?php print $title ?></h1>
          <?php endif; ?>
          <?php print render($title_suffix); ?>
          <?php if ($tabs): ?><?php print render($tabs) ?></div><?php endif; ?>
         



		<!-- main-content-block --><div class="main-content-block"> 
		<?php echo render($page['content']); ?>
		<!-- /main-content-block --></div>
		
<?php if ($page['content_bottom']): ?><?php echo render($page['content_bottom']) ?><?php endif; ?>
        



<!-- PRINTING BLOCK AFTER THE CONTENT -->

<?php if ($page['content_block_left'] || $page['content_block_right']): ?>

  <!-- column-2-blocks -->
  <div class="column-2-blocks clearfix
  <?php if (!$page['right'] && !$page['left']): ?>column-2-blocks-no-right-and-left-columns
  <?php elseif (!$page['left']): ?>
  column-2-blocks-no-left-column
  <?php elseif (!$page['right']): ?>
  column-2-blocks-no-right-column
  <?php endif; ?>
  ">
  <!-- /column-2-blocks-left --><div class="column-2-blocks-left">
  <?php if ($page['content_block_left']): ?><?php echo render($page['content_block_left']) ?><?php endif; ?>
  <?php if (!$page['content_block_left']): ?>&nbsp;<?php endif; ?>
  <!-- /column-2-blocks-left --></div>
  
  
  
  <!-- /column-2-blocks-right --><div class="column-2-blocks-right">
  <?php if ($page['content_block_right']): ?><?php print render($page['content_block_right']) ?><?php endif; ?>
  <!-- /column-2-blocks-right --></div>
  <!-- /column-2-blocks --></div>

<?php endif; ?>


<?php if ($page['content_after_blocks']): ?><div class="content_after_blocks"><?php if ($page['content_after_blocks']): ?><?php echo render($page['content_after_blocks']) ?><?php endif; ?></div><?php endif; ?>



<!-- / column-2 --></div>



<?php if ($page['right']): ?>
<!-- column-3 -->
<div class="column-3"><?php echo render($page['right']) ?></div>
<!-- / column-3 -->
<?php endif; ?>



<!-- footer -->
<div id="footer" class="clearfix">
 
  <?php echo render($page['footer']) ?>


</div>
<!-- /footer -->



</div>
<!-- / make-it-center -->

