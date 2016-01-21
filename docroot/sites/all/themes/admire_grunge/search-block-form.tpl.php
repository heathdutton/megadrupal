<div class="container-inline">
<?php
$replace_button = 'type="image" src="'. base_path (). path_to_theme() . '/images/search_button.jpg" alt="Submit"';
$search["search_block_form"]=str_replace('value=""', 'value="Enter your search term..." onblur="setTimeout(\'closeResults()\',2000); if (this.value == \'\') {this.value = \'\';}"  onfocus="if (this.value == \'Enter your search term...\') {this.value = \'\';}" ', $search["search_block_form"]);
$search["actions"] = str_replace('type="submit"', $replace_button, $search["actions"]);
  print $search["search_block_form"];
  print $search['actions'];
  print $search["hidden"];  
?>
</div>

