<?php
global $theme_key;
$dtheme=variable_get('theme_default','bartik');

if (variable_get('wrp_cssfile_completed','none')!=$theme_key&&$dtheme==$theme_key&&theme_get_setting('colors_form')>0) {
        $path = 'public://wscolor/';
        $stylesheet=theme_get_setting('color_file');
        $stylesheet=$stylesheet['default'];
        $tarr=explode('/',$stylesheet);
        $filename=end($tarr);
        $ocpickers=(array)wrappedsites_get_color_pickers();
        $source = drupal_get_path('theme', $theme_key) . '/';
        if ($stylesheet&&file_exists($source.$stylesheet)) {
        $style = drupal_load_stylesheet($source  . $stylesheet, FALSE);
        foreach ($ocpickers as $value) {
            $selector='color_sett'.$value->id;
            $rval='#'.$value->value;
            $style=str_replace('%'.$value->id.'%', $rval, $style);
        }

        $filepath = file_unmanaged_save_data($style, $path.$filename, FILE_EXISTS_REPLACE);

          // Set standard file permissions for webserver-generated files.
         drupal_chmod($path.$filename);

         variable_set($theme_key.'_wscolor_file', $path.$filename);
         drupal_set_message('css config file was detected :'.$path.$filename, 'status');
         variable_set('wrp_cssfile_completed',$theme_key);
         

        }
}