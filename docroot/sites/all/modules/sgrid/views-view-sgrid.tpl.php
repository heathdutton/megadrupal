<?php
/**
 * @file views-view-list.tpl.php
 * Default simple view template to display a list of rows.
 *
 * - $title : The title of this group of rows.  May be empty.
 * - $options['type'] will either be ul or ol.
 * @ingroup views_templates
 */
 
$path = libraries_get_path('FirePHPCore');
if (file_exists($path . '/fb.php')) {
  include_once $path . '/fb.php';
  include_once $path . '/FirePHP.class.php';

  // output your array or object here
  fb($view);
}
// dpm($view->display[$view->current_display]);
//fb($view->display[$view->current_display]);

    drupal_add_js(array('sgrid' => array(
                                        'view_name' => $view->name,
                                        'display_name' => $view->current_display,
                                        'row_length' => $row_length,
                                        'sort_allowed' =>  user_access('sort Sortable Grid Views'),
                                    
                                    )
                            ), 
    'setting'); 
     
    drupal_add_js('misc/ui/jquery.ui.core.min.js');    
    drupal_add_css('misc/ui/jquery.ui.core.min.css');    
    drupal_add_js('misc/ui/jquery.ui.widget.min.js');    
    drupal_add_js('misc/ui/jquery.ui.mouse.min.js');    
    drupal_add_js('misc/ui/jquery.ui.sortable.min.js');    
    drupal_add_js(drupal_get_path('module', 'sgrid') . '/sgrid.js');   
    drupal_add_css(drupal_get_path('module', 'sgrid') . '/sgrid.css');   
?>
<?php print $wrapper_prefix; ?>
  <?php if (!empty($title)) : ?>
    <h3><?php print $title; ?></h3>
  <?php endif; ?>
  <?php print $list_type_prefix; ?>
    <?php foreach ($rows as $id => $row): ?>
<!-- Add the end of line class when necessary -->
    <?php  if (isset($sgrid_end_of_line[$id])) : $classes_array[$id] .= $sgrid_end_of_line[$id]; endif; ?>
      <li class="<?php print $classes_array[$id]  ?>"><?php print $row; ?></li>
    <?php endforeach; ?>
  <?php print $list_type_suffix; ?>
<?php print $wrapper_suffix; ?>