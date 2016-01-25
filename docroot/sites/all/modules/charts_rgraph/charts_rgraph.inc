<?php
/**
 * @file
 * Callbacks and utility functions for rendering a rgraph Chart.
 */

/**
 * Chart render callback; Convert all chart-level data.
 *
 * This essentially is an additional #pre_render callback. It operates in the
 * same way and is simply called as part of the normal #pre_render process.
 *
 * @param array $chart
 *   The chart renderable.
 * @return
 *   The modified chart renderable, with necessary #attached, #theme, and
 *   similar properties prepared for rendering.
 */
function _charts_rgraph_render($chart) {

//dpm($chart);

  // Populate chart settings.
  $chart_definition = array();
  $chart_definition['chart type'] = _charts_rgraph_type($chart['#chart_type']);
  $chart_definition = _charts_rgraph_populate_chart_options($chart, $chart_definition);
  $chart_definition = _charts_rgraph_populate_chart_data($chart, $chart_definition);

  // Remove machine names from series. RGraph series must be an array.
  //**   $series = array_values($chart_definition['series']);
   unset($chart_definition['series']);

  // Trim out empty options (excluding "series" for efficiency).
  charts_trim_array($chart_definition);
  
  // Add configurations to options array
  foreach($chart_definition['config'] as $key => $value){
    $chart_definition['options'][] = array($key, $value);
  }
  unset($chart_definition['config']);

  if (!isset($chart['#id'])) {
    $chart['#id'] = drupal_html_id('rgraph-render');
  }
  $chart_definition['canvass']['id'] = 'canvass-'.$chart['#id'];
  if ($chart['#width']) $chart_definition['canvass']['width'] = $chart['#width'];
  if ($chart['#height']) $chart_definition['canvass']['height'] = $chart['#height'];

  $chart['#attached']['library'][] = array('charts_rgraph', 'charts_rgraph');
  $chart['#attributes']['class'][] = 'charts-rgraph';
  $chart['#chart_definition'] = $chart_definition;

  // add library depending on chart type
  drupal_add_js(libraries_get_path('rgraph') . '/libraries/RGraph.' . 
      strtolower($chart_definition['chart type']) . '.js');
  // add exCanvass to add canvass support to IE8
  $js = array(
    '#tag' => 'script',
    '#attributes' => array( // Set up an array of attributes inside the tag
      'src' => '/' . libraries_get_path('rgraph') . '/excanvas/excanvas.js',
    ),
    '#prefix' => '<!--[if (lte IE 9)]>',
    '#suffix' => '</script><![endif]-->',
  );
  drupal_add_html_head($js, 'excanvass');
//dpm($chart);

  return $chart;
}

/**
 * Utility to convert a Drupal renderable type to a RGraph visualization type.
 */
function _charts_rgraph_type($renderable_type) {
  $types = array(
    'area' => 'Line',
    'bar' => 'HBar',
    'column' => 'Bar',
    'line' => 'Line',
    'pie' => 'Pie',
    'scatter' => 'Scatter',
  );
  drupal_alter('charts_rgraph_types', $types);
  return isset($types[$renderable_type]) ? $types[$renderable_type] : FALSE;
}
/**
 * Utility to calculate lighter version of a color.
 */
function _charts_rgraph_lighten($hex) {
    // Format the hex color string
    $hex = str_replace('#', '', $hex);
    if (strlen($hex) == 3) {
        $hex = str_repeat(substr($hex,0,1), 2).str_repeat(substr($hex,1,1), 2).str_repeat(substr($hex,2,1), 2);
    }
    // Get decimal values
    $r = hexdec(substr($hex,0,2));
    $g = hexdec(substr($hex,2,2));
    $b = hexdec(substr($hex,4,2));

    $lr = dechex($r + floor((255-$r)/1.5));
    $lg = dechex($g + floor((255-$g)/1.5));
    $lb = dechex($b + floor((255-$b)/1.5));
    
  return '#'.$lr.$lg.$lb;
}

/**
 * Utility to populate chart options.
 */
function _charts_rgraph_populate_chart_options($chart, $chart_definition) {

  $chart_definition['config']['gutter.top'] = ($chart["#chart_type"] == 'pie'?25:10);
  $chart_definition['config']['gutter.bottom'] = ($chart["#chart_type"] == 'pie'?25:10);
  $chart_definition['config']['gutter.left'] = ($chart["#chart_type"] == 'pie'?25:10);
  $chart_definition['config']['gutter.right'] = ($chart["#chart_type"] == 'pie'?25:10);
  // title
  if ($chart['#title']) {
    $chart_definition['config']["title"] =  $chart['#title'];
    $chart_definition['config']["title.color"] = $chart['#title_color'];
    $chart_definition['config']["title.bold"] = ($chart['#title_font_weight'] == 'bold');
    $chart_definition['config']["title.size"] = $chart['#title_font_size'];
    //TODO: set #title_position (in,out) through gutter.top)
    if ($chart['#title_position'] == 'out') {
      $chart_definition['config']['title.y'] = 25;
      $chart_definition['config']['gutter.top'] += 30;
    } else {
      $chart_definition['config']['title.y'] = 40;
      $chart_definition['config']['title.background'] = 'rgba(255,255,255,0.8)';
    }
  }
  // style
  $chart_definition['config']["colors"] = $chart['#colors'];
  $chart_definition['config']["strokestyle"] = 'white';
  $chart_definition['config']["linewidth"] = 2;
  // $chart_definition['options'][] = array("colors,sequential" , false);
  $chart_definition['config']["text.font"] = $chart['#font'];
  $chart_definition['config']["text.size"] = $chart['#font_size'] - 1;
  $chart_definition['config']['background.color'] = $chart['#background'];
  // shadows
  $chart_definition['config']["shadow"] = true;
  // grid lines
  $chart_definition['config']['background.grid.color'] = 'lightgrey';
  // stacking
  if ($chart['#stacking']) $chart_definition['config']['grouping'] = ($chart['#stacking']?'grouped':'stacked');

  return $chart_definition;
}

/**
 * Utility to populate chart data.
 */
function _charts_rgraph_populate_chart_data(&$chart, $chart_definition) {
  $chart_definition['data'] = array();
  $max_value = 0;
  foreach (element_children($chart) as $key ) {
    switch($chart[$key]['#type']){
      case 'chart_xaxis':
      case 'chart_yaxis':
        if (empty($chart[$key]['#defaults_loaded'])) {
          $chart[$key] += element_info($chart[$key]['#type']);
        }
        // Populate the chart data.
        // title
        if ($chart[$key]['#title']) {
          $chart_definition['config']["title.".$key] =  $chart[$key]['#title'];
          $chart_definition['config']["title.".$key.".color"] = $chart[$key]['#title_color'];
          $chart_definition['config']["title.".$key.".bold"] = ($chart[$key]['#title_font_weight'] == 'bold');
          $chart_definition['config']["title.".$key.".size"] = $chart[$key]['#title_font_size'];
          if ($key == 'yaxis') {
            $chart_definition['config']["title.".$key.".x"] = 10;
            $chart_definition['config']['gutter.left'] += 15;
          } else {
            $chart_definition['config']["title.".$key.".y"] = ($chart['#height']? $chart['#height'] - 10: 140);
            $chart_definition['config']['gutter.bottom'] += 20;
          }
        }
        // labels
        $label_lenght = 0;
        if ($chart[$key]['#labels']){
          $chart_definition['config']['labels'] = $chart[$key]['#labels'];
          $chart_definition['config']['text.angle'] = $chart[$key]['#labels_rotation'];
          foreach($chart[$key]['#labels'] as $label){
            $label_lenght = max(strlen($label),$label_lenght);
          }
        }
        // style
        switch ($chart["#chart_type"]) {
          case 'bar':
            $chart_definition['config']['background.grid'] = true;
            $chart_definition['config']['background.grid.hlines'] = false;
            $chart_definition['config']['background.grid.vlines'] = true;
            $chart_definition['config']['background.grid.autofit'] = true;
            $chart_definition['config']['background.grid.autofit.numvlines'] = 8;
            $chart_definition['config']['gutter.left'] += 15 + $label_lenght * 6;
            $chart_definition['config']['gutter.bottom'] += 15;
            $chart_definition['config']['vmargin'] = 5;
            $chart_definition['config']['xlabels.count'] = 8;
            $chart_definition['config']['noxtickmarks'] = true;
            $chart_definition['config']['noytickmarks'] = true;
            $chart_definition['config']['xmax'] = $chart[$key]['#max'];
            $chart_definition['config']['xmin'] = $chart[$key]['#min'];
          break;
          case 'area':
            $chart_definition['config']['filled'] = true;
          case 'line':
            $chart_definition['config']['tickmarks'] = 'circle';
          case 'column':
            $chart_definition['config']['background.grid'] = true;
            $chart_definition['config']['background.grid.hlines'] = true;
            $chart_definition['config']['background.grid.vlines'] = false;
            $chart_definition['config']['background.grid.autofit'] = true;
            $chart_definition['config']['background.grid.autofit.numvlines'] = 5;
            $chart_definition['config']['numxticks'] = 0;
            $chart_definition['config']['numyticks'] = 5;
            $chart_definition['config']['gutter.left'] += 35;
            $chart_definition['config']['gutter.bottom'] += 5 + ($label_lenght * 7 * sin(deg2rad($chart['xaxis']['#labels_rotation'])));
            $chart_definition['config']['ymax'] = $chart[$key]['#max'];
            $chart_definition['config']['ymin'] = $chart[$key]['#min'];
          break;
          // $chart_definition['config']['labels.above'] = true; // value on the bar
          // $chart_definition['config']['labels.above.angle'] = 66; // 
        }

      break;
      
      case 'chart_data':

        if (empty($chart[$key]['#defaults_loaded'])) {
          $chart[$key] += element_info($chart[$key]['#type']);
        }
        $chart_definition['config']['units.pre'] = $chart[$key]['#prefix'];
        $chart_definition['config']['units.post'] = $chart[$key]['#suffix'];

        switch($chart["#chart_type"]){
          case 'pie':
            if (isset($chart[$key]["#labels"])){
              $keys = $chart[$key]["#labels"];
              $data = $chart[$key]["#data"];
            } else {
              foreach($chart[$key]["#data"] as $k => $v){
                 $keys[] = $v[0];
                 $data[] = $v[1];
              }
           }
          break;

          case 'line':
          case 'area':
            $data[] = $chart[$key]["#data"];
            if (isset($chart[$key]['#color'])) $colors[] = $chart[$key]['#color'];
            if (isset($chart[$key]['#title'])) $keys[] = $chart[$key]['#title'];
          break;

          default:
            foreach($chart[$key]["#data"] as $k => $v){
               $data[$k][] = $v;
               $max_value = max($v, $max_value);
            }
            if (isset($chart[$key]['#color'])) $colors[] = 'Gradient(' . _charts_rgraph_lighten($chart[$key]['#color']) .  
              ':' . $chart[$key]['#color'] . ':' .$chart[$key]['#color'] . ')';
            if (isset($chart[$key]['#title'])) $keys[] = $chart[$key]['#title'];
          break;
        }
      break;
    }

  }

  //data
  $chart_definition['data'] = $data;
  // tooltips
  if ($chart['#tooltips']) {
    // $chart['#tooltips_use_html'] -- always use html?
    if ($chart['#chart_type'] == 'line' || $chart['#chart_type'] == 'area') {
      $d = count($data[0]);
      $k = count($keys);
      for ($i2 = 0; $i2 < $k; $i2++){
        for ($i1 = 0; $i1 < $d; $i1++){
          $tooltips[] = '<b>' . $chart_definition['config']['labels'][$i1] . '</b><br>' . $keys[$i2] . ': ' . $data[$i2][$i1];
        }
      }
    } elseif ($chart['#chart_type'] == 'pie') {
      foreach ($data as $i => $item) {
          $tooltips[] = '<b>' . $keys[$i] . '</b><br>' .  $item ;
      }
    } elseif ($chart['#chart_type'] == 'scatter') { // temporary fix
     $tooltips = array();
    } else {
      foreach ($data as $i1 => $set) {
        foreach($set as $i2 => $item){
          $tooltips[] = '<b>' . $chart_definition['config']['labels'][$i1] . '</b><br>' . $keys[$i2] . ': ' . $item;
        }
      }
    }
    $chart_definition['config']["tooltips"] = $tooltips;
    $chart_definition['config']["tooltips.event"] = 'onmousemove'; // onclick
    drupal_add_js(libraries_get_path('rgraph') . '/libraries/RGraph.common.tooltips.js');
  }
  // legend
  if ($chart['#legend']){
    drupal_add_js(libraries_get_path('rgraph') . '/libraries/RGraph.common.key.js');
    drupal_add_js(libraries_get_path('rgraph') . '/libraries/RGraph.common.dynamic.js');
    drupal_add_js(libraries_get_path('rgraph') . '/libraries/RGraph.drawing.rect.js');
    if (isset($keys)) $chart_definition['config']["key"] = $keys;
    //  $chart_definition['options'][] = array(,$chart['#legend_font_weight']);
    //  $chart_definition['options'][] = array(,$chart['#legend_font_style']);
    $chart_definition['config']['key.size'] = $chart['#legend_font_size'];
    $chart_definition['config']['key.interactive'] = true;
    switch($chart['#legend_position']){
      case 'top':
        $chart_definition['config']['gutter.top'] += 15;
        $chart_definition['config']['key.position'] = 'gutter';
        if ($chart['#title'] && ($chart['#title_position'] == 'in')) $chart_definition['config']['title.y'] += 15; 
      break;
      case 'left':
       $chart_definition['config']['key.position'] = 'graph';
       $chart_definition['config']['key.halign'] = 'left';
      break;
      case 'right':
       $chart_definition['config']['key.position'] = 'graph';
       $chart_definition['config']['key.halign'] = 'right';
       if ($chart['#chart_type'] == 'pie') {
         $chart_definition['config']['gutter.right'] += 35;
         $chart_definition['config']['key.position.x'] = ($chart['#width']? $chart['#width'] - 100: 205);
       } 
      break;
      case 'bottom':
        $chart_definition['config']['gutter.bottom'] += 20;
        $chart_definition['config']['key.position'] = 'gutter';
        $chart_definition['config']['key.position.gutter.boxed'] = true;
        $chart_definition['config']['key.position.y'] = ($chart['#height']? $chart['#height'] - 20: 130);
  //      $chart_definition['config']["title.xaxis.y"] += -30; 
      break;
      default:
     
      break; 
    }
  }
  if (isset($colors)) $chart_definition['config']['colors'] = $colors;
  if ($chart['#chart_type'] == 'area'){
    foreach($colors as $hex){
       $light_colors[] = _charts_rgraph_lighten($hex);
    }
    $chart_definition['config']['fillstyle'] = $light_colors;
  }
  if ($chart['#chart_type'] == 'line' || $chart['#chart_type'] == 'area'){
    $chart_definition['config']['hmargin'] = 25;
  } else {
    $chart_definition['config']['hmargin'] = 10;
  }
  // nicer yaxis ticks if ymax not set
  if ($chart['#chart_type'] == 'column' && $chart_definition['config']['ymax'] == null) {
    $s = strlen(floor($max_value));
    $nt = $max_value / pow(10,$s-2);
    if ($nt <= 15) {
     $b = 2.5;
    } elseif ($nt <= 30) {
     $b = 5;
    } elseif ($nt <= 60) {
     $b = 10;
    } else {
     $b = 20;
    }
    $t = ceil($nt / $b);
    $chart_definition['config']['numyticks'] = $t;
    $chart_definition['config']['ylabels.count'] = $t;
    $chart_definition['config']['background.grid.autofit.numhlines'] = $t;
    $chart_definition['config']['ymax'] = $t*$b * pow(10,$s-2);
  }
  // nicer xaxis ticks if xmax not set
  if ($chart['#chart_type'] == 'bar' && $chart_definition['config']['xmax'] == null) {
    $s = strlen(floor($max_value));
    $nt = $max_value / pow(10,$s-2);
    if ($nt <= 15) {
     $b = 2.5;
    } elseif ($nt <= 30) {
     $b = 5;
    } elseif ($nt <= 60) {
     $b = 10;
    } else {
     $b = 20;
    }
    $t = ceil($nt / $b);
    $chart_definition['config']['numxticks'] = $t;
    $chart_definition['config']['xlabels.count'] = $t;
    $chart_definition['config']['background.grid.autofit.numvlines'] = $t;
    $chart_definition['config']['xmax'] = $t*$b * pow(10,$s-2);
    $chart_definition['config']['gutter.right'] += 10;
  }
  return $chart_definition;
}

