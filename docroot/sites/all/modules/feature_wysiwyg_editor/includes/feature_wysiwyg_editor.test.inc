<?php


function feature_wysiwyg_editor_menu() {
  $items['ckeditor_wysiwyg/test'] = array(
    'page callback' => 'mymodule_abc_view',
    'access callback' => TRUE
  );
  return $items;
}

function mymodule_abc_view($ghi = 0, $jkl = '') {

  $video_urls = array(
    'https://www.youtube.com/watch?v=Pwe-pA6TaZk',
    'https://vimeo.com/1019038'
  );


  $output = '';
  $output .= '<h2>Video URLS</h2>';
  $output .= implode('<br/>',$video_urls);

  $output .= '<h2>Lists</h2>';
  $output .= '<ol><li>One</li><li>Two</li><li>Three<ol start="1" style="list-style-type: lower-roman;"><li>Three i</li><li>Three ii</li><li>Three iii<ol start="1" style="list-style-type: lower-alpha;"><li>Three &nbsp;iii &nbsp;a</li><li>Three iii b</li></ol></li></ol></li><li>Four</li><li>Five</li><li>Six</li></ol>';

  $output .= '<h2>Image Floats</h2>';

  $output .= '<img class="alignleft" src="https://placehold.it/350x150" />';
  $output .= '<img class="alignright" src="https://placehold.it/350x150" />';
  $output .= '<img class="aligncenter" src="https://placehold.it/350x150" />';

  return check_markup($output,'ckeditor_wysiwyg');
}