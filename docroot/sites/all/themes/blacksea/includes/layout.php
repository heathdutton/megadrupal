<?php 
$maincontent_two_sidebars = $fixedwidth - ($leftwidth + $rightwidth); 
$maincontent_left_sidebar = $fixedwidth - ($leftwidth); 
$maincontent_right_sidebar = $fixedwidth - ($rightwidth); 
?>

<style type="text/css">
  
  <?php if ($layout == 0) { ?>
    .width-wrapper { width: <?php print $fixedwidth ?>px; }
		#slideshow  .views-field-title { max-width: <?php print $fixedwidth ?>px; }
		#slideshow  .views-field-body { max-width: <?php print $fixedwidth ?>px; }
		
  <?php } else { ?>
    .width-wrapper { width: 95%; }
  <?php } ?>
  
/*-- No Sidebars --*/  
  
  body.no-sidebars #main-content { 
    width: <?php echo $fixedwidth ?>px; 
  }
  
/*-- Two Sidebars --*/  
  
  body.two-sidebars #main-content { 
    width: <?php echo $maincontent_two_sidebars ?>px; 
	  margin: 0 -<?php echo $leftwidth + $maincontent_two_sidebars ?>px 0 <?php echo $leftwidth ?>px; 
  }

  body.two-sidebars #main-content-inner { 
    padding-left: 0px; 
  }

  body.two-sidebars .sidebar-first { 
    margin-left: 0;
    margin-right: -<?php echo $leftwidth ?>px;
    width: <?php echo $leftwidth ?>px; 
  }

  body.two-sidebars .sidebar-second { 
    margin-left: <?php echo $leftwidth + $maincontent_two_sidebars ?>px;
    margin-right: -<?php echo $fixedwidth ?>px;
    width: <?php echo $rightwidth ?>px; 
  }

/*-- Left Sidebar --*/  

  body.sidebar-first #main-content { 
    width: <?php echo $maincontent_left_sidebar ?>px;
    margin-left: <?php echo $leftwidth ?>px; 
    margin-right: -<?php echo $fixedwidth ?>px; 
  }
  
  body.sidebar-first .sidebar-first { 
    margin-left: 0;
    margin-right: -<?php echo $leftwidth ?>px;
    width: <?php echo $leftwidth ?>px; 
  }
  
/*-- Right Sidebar --*/  
  
  body.sidebar-second #main-content { 
    width: <?php echo $maincontent_right_sidebar ?>px;
	margin-left: 0;
    margin-right: -<?php echo $maincontent_right_sidebar ?>px; 
  }
  
  body.sidebar-second .sidebar-second { 
    margin-left: <?php echo $maincontent_right_sidebar ?>px;
    margin-right: -<?php echo $fixedwidth ?>px;
    width: <?php echo $rightwidth ?>px; 
  }

</style>

