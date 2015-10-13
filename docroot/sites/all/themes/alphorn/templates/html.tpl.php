<?php print $doctype; ?>
<html lang="<?php print $language->language; ?>" dir="<?php print $language->dir; ?>"<?php print $rdf->version . $rdf->namespaces; ?>>
<head<?php print $rdf->profile; ?>>
  <?php print $head; ?>
  <title><?php print $head_title; ?></title>  
  <?php print $styles; ?>
  <?php print $scripts; ?>
  <!--[if lt IE 9]><script src="http://html5shiv.googlecode.com/svn/trunk/html5.js"></script><![endif]-->
  
   <?php 
    if (! isset($_SESSION['color']) ) {
	  $_SESSION['color'] = 'blue';
    }
    $flag = 0;

    switch(arg(1)) {
      // Red color
      case '13':
        $_SESSION['color'] = 'blue';
        $flag = 1;
        break;

      // Dark Red color
      case '14':
        $_SESSION['color'] = 'green';
        $flag = 1;
        break;
		
      // Teal color
      case '15':
        $_SESSION['color'] = 'red';
        $flag = 1;
        break;
		
      default:
        break;
    }
    
    if($flag) header('Location:'.base_path());   
    
    if($_SESSION['color'] != 'green' and $_SESSION['color'] != 'red' and $_SESSION['color'] != 'blue') $_SESSION['color'] = 'blue';
	
   
    print '<link href="'.base_path().$directory .'/css/' . $_SESSION['color'] . '-style.css" media="all" rel="stylesheet" type="text/css"/>';
	
  ?>
  
</head>
<body<?php print $attributes;?>>
  <div id="skip-link">
    <a href="#main-content" class="element-invisible element-focusable"><?php print t('Skip to main content'); ?></a>
  </div>
  <?php print $page_top; ?>
  <?php print $page; ?>
  <?php print $page_bottom; ?>
</body>
</html>