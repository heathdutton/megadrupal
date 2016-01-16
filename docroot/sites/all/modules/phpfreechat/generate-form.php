<?php
/*
 * This is a command line script that generates a settings form definition
 * for the phpfreechat module by parsing parameters.txt, a file I created 
 * from the phpFreeChat code file pfcglobalconfig.class.php. You should never 
 * need to use this. -permutations.
 */

$install_path = dirname(__FILE__);
if (file_exists($install_path . '/parameters.txt')) {
  $file = file_get_contents($install_path . '/parameters.txt');
  $settings_start = strpos($file, 'full parameters list:');
  $settings_finish = strpos($file, '2008 phpFreeChat', $settings_start) - 4;
  $settings_section = substr($file, $settings_start, $settings_finish - $settings_start);
  $matches = array();
  preg_match_all('/\s{4}(\S.*)((?:\n\s{9}.*)*)/', $settings_section, $matches, PREG_SET_ORDER);
  $settings = array();
  foreach ($matches as $match) {
    $setting_names = explode(' and ', $match['1']); // For x and y lines
    foreach ($setting_names as $setting_name) {
      $settings[$setting_name]['help'] = $match['2'];
      // Find some of the common defaults
      if (strstr($match['2'], '(true value by default')) {
        $settings[$setting_name]['default'] = 'true';
      }
      if (strstr($match['2'], '(false value by default')) {
        $settings[$setting_name]['default'] = 'false';
      }
    }
  }
  //  print_r($settings);
  $settings_list = "'";
  foreach ($settings as $setting => $value) {
    $settings_list .= $setting . "','";
    $setting_title = ucwords(str_replace('_', ' ', $setting));
    if ($value['default'] == 'true' || $value['default'] == 'false') {
      print '$form[\'phpfreechat_' . $setting . '\'] = array(' . "\n<br />";
      print '  \'#type\' => \'select\',' . "\n<br />";
      print '  \'#title\' => t(\'' . str_replace("'", "\'", $setting_title) . '\'),' . "\n<br />";
      print '  \'#default_value\' => variable_get(\'phpfreechat_' . $setting . '\', $value[\'default\']),' . "\n<br />";
      print '  \'#options\' => array(\'\' => \'default\', \'true\' => \'true\', \'false\' => \'false\'),' . "\n<br />";
      print '  \'#description\' => t(\'' . str_replace("'", "\'", $value['help']) . '\'),' . "\n<br />";
      print ');' . "\n<br />\n<br />";
    }
    else {
      print '$form[\'phpfreechat_' . $setting . '\'] = array(' . "\n<br />";
      print '  \'#type\' => \'textfield\',' . "\n<br />";
      print '  \'#title\' => t(\'' . str_replace("'", "\'", $setting_title) . '\'),' . "\n<br />";
      print '  \'#default_value\' => variable_get(\'phpfreechat_' . $setting . '\', \'\'),' . "\n<br />";
      print '  \'#description\' => t(\'' . str_replace("'", "\'", $value['help']) . '\'),' . "\n<br />";
      print ');' . "\n<br />\n<br />";
    }
  }
  $settings_list .= "'";
  print "\n<br />\n<br />Array of settings:\n<br />" . $settings_list . "\n<br />\n<br />";
}


?>
