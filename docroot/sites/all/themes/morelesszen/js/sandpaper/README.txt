==== How to use ====
copy this php lines in your HOOK_preprocess_page

drupal_add_js(path_to_theme() . '/js/sandpaper/jcoglan.com/sylvester.js', array('every_page' => TRUE));
drupal_add_js(path_to_theme() . '/js/sandpaper/cssSandpaper.js', 	array('every_page' => TRUE));

add CSS files with <link … class="cssSandpaper" … /> to your page.

