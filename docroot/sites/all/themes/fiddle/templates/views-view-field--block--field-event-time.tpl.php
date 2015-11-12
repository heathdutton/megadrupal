<?php
// $Id: views-view-field--events--block-field-date-value.tpl.php,v 1.0.0 2010/01/06 09:00:32 SymphonyThemes Exp $
?>
<?php 
  $create_date = explode(' ',preg_replace('/<[^>]*>/i','', $output));

  echo date('d/m',strtotime($create_date[0]));
