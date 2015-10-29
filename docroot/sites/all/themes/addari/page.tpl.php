<?php
$header = $page['header'];
$content_top = $page['content_top'];
$content_bottom = $page['content_bottom'];
$big_right = $page['big_right'];
$right = $page['right'];
$left = $page['left'];
$footer = $page['footer'];


$footer = $page['footer'];
$help = $page['help'];
$highlighted = $page['highlighted'];
$content = $page['content'];
$footer = $page['footer'];
$search_box = $page['search_box'];
?>


<div id="container">

<div id="head">

<div id="hleft">

<div id="titles">
  <?php if ($logo) { ?><div id="logocontainer"><a href="<?php print $base_path ?>" title="<?php print t('Home') ?>"><img src="<?php print $logo ?>" alt="<?php print t('Home') ?>" /></a></div><?php } ?>
 <div id="textcontainer">
  <?php if ($site_name) { ?><h1 class='site-name'><a href="<?php print $base_path ?>" title="<?php print t('Home') ?>"><?php print $site_name ?></a></h1><?php } ?>
      <?php if ($site_slogan) { ?><div class='site-slogan'><?php print $site_slogan ?></div><?php } ?></div>
</div>

</div>

<div id="hright">

    <div id="secondary-links">
<?php if (isset($secondary_menu)) { ?>        
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
        <?php } ?>
</div>

<?php if($header){ ?>
<div id="header_block">
<?php print render($header) ?>
</div>
<?php } ?>

</div>

</div><!--head-->

<div id="mast">

<!--<div id="primary_menu_bar">-->
	        <div id="primary_menu_bar">
				<?php
		            if ($main_menu) {
		            $pid = variable_get('menu_main_links_source', 'main-menu');
		            $tree = menu_tree($pid);
		            $main_menu = drupal_render($tree);
		            }else{
		            $main_menu = FALSE;
		            }
		            ?>
		            <?php
		            if ($main_menu): print $main_menu; endif;
		        ?> 
			</div>

<div id="wrap" >

<div id="postarea">
<?php if ($content_top): print render($content_top); endif; ?>
<div id="postareainner">
 <?php if ($breadcrumb): print $breadcrumb; endif; ?>
          <?php if ($tabs): print '<div id="tabs-wrapper" class="clear-block">'; endif; ?>
          <?php if ($title): print '<div id="h2title"><h1'. ($tabs ? ' class="with-tabs"' : '') .'>'. render($title_prefix) . $title . render($title_suffix) . '</h1></div>'; endif; ?>
		  
          <?php if ($tabs): print  render($tabs) .'</div>'; endif; ?>
          <?php if (isset($tabs2)): print render($tabs2); endif; ?>
          <?php if ($help): print render($help); endif; ?>
          <?php if ($messages): print $messages; endif; ?>
          <?php print render($content); ?>
</div>
<?php if ($content_bottom): print render($content_bottom); endif; ?>
</div>

<?php if($highlighted || $search_box || $left || $right || $big_right) { ?>
<div id="sidearea">
<?php if ($highlighted): ?>
<div id="mission-block">

<?php print render($highlighted); ?>
</div>
<?php endif; ?>

<?php if ($search_box): ?><div id="search-block-main"><?php print render($search_box) ?></div><?php endif; ?>

<div id="sidebars" class="clear-block">

<?php if ($big_right): ?>
<div id="big_right" class="clear-block">
<?php print render($big_right) ?>
</div>
<?php endif;?>

<?php if ($left): ?>
<div id="left" class="clear-block">
<?php print render($left) ; ?>
</div>
<?php endif;?>

<?php if ($right): ?>
<div id="right" class="clear-block">
<?php print render($right); ?>
</div>
<?php endif;?>
</div><!--sidebars-->

</div>
<?php } ?>
<br style="clear:both;" />
</div>

</div>

<div id="footer"><?php if ($footer): ?> <div id="foot-block"><?php print render($footer) ?> </div><?php endif; ?><br/> Powered by <a href="http://drupal.org/">Drupal</a>, <a href="http://www.worthapost.com/" title="Worthapost Drupal themes">Drupal Themes</a> </div>

</div> <!--container-->
