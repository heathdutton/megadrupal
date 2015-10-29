<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
    <title><?php print $head_title ?></title>
    <?php print $head ?>
    <?php print $styles ?>
     <style type="text/css" media="all">@import "<?php print base_path() . path_to_theme() ?>/master.css";</style>  
    <?php print $scripts ?>    
</head>
<body class="<?php print $classes; ?>" <?php print $attributes;?>>
    <?php print $page_top; ?>
    <?php print $page; ?>
    <?php print $page_bottom; ?>
</body>
</html>
