<?php 

/**
 * This class has been adapted from http://pommi.nethuis.nl/collectd-graph-panel-v0-3/
 * to fit into Drupal as the basis of graph generation.  All the data gathering logic
 * and graphing logic has not changed. The main changes are that the image writing
 * function has been delegated to the calling function inside the rrd_rest module.
 * 
 * This class also contains command calls for RRDTool fetch and xport.
 * 
 * @file
 */

class Type_Default {
  var $datadir;
  var $rrdtool;
  var $rrdtool_opts;
  var $cache;
  var $args;
  var $seconds;
  var $data_sources = array('value');
  var $order;
  var $ds_names;
  var $colors;
  var $rrd_title;
  var $rrd_vertical;
  var $rrd_format;
  var $scale;
  var $width;
  var $heigth;
  
  var $files;
  var $tinstances;
  var $identifiers;

  /**
   * Default constructor 
   */
  function __construct($config) {
    $this->datadir = $config['datadir'];
    $this->rrdtool = $config['rrdtool'];
    $this->rrdtool_opts = $config['rrdtool_opts'];
    if (isset($config['cache'])) {
      $this->cache = $config['cache'];
    }
    $this->args = $config['args'];
    $this->rrd_files();
    $this->identifiers = $this->file2identifier($this->files);
  }

  /**
   * Method is used to generate colors if no color definitions
   * are passed. 
   */
  function rainbow_colors() {
    $sources = count($this->rrd_get_sources());
    foreach ($this->rrd_get_sources() as $ds) {
      #hue (saturnation=1, value=1)
      $h = 360 - ($c * (330/($sources-1)));

      $h = ($h %= 360) / 60;
      $f = $h - floor($h);
      $q[0] = $q[1] = 0;
      $q[2] = 1*(1-1*(1-$f));
      $q[3] = $q[4] = 1;
      $q[5] = 1*(1-1*$f);
  
      $hex = '';
      foreach (array(4, 2, 0) as $j) {
        $hex .= sprintf('%02x', $q[(floor($h)+$j)%6] * 255);
      }
      $this->colors[$ds] = $hex;
      $c++;
    }
  }

  /**
   * Validation method 
   */
  function validate_color($color) {
    if (!preg_match('/^[0-9a-f]{6}$/', $color)) {
      return '000000';
    }
    else {
      return $color;
    }
  }

  /**
   * Faded color generation 
   */
  function get_faded_color($fgc, $bgc='ffffff', $percent=0.25) {
    $fgc = $this->validate_color($fgc);
    if (!is_numeric($percent)) {
      $percent=0.25;
    }

    $rgb = array('r', 'g', 'b');

    $fg['r'] = hexdec(substr($fgc, 0, 2));
    $fg['g'] = hexdec(substr($fgc, 2, 2));
    $fg['b'] = hexdec(substr($fgc, 4, 2));
    $bg['r'] = hexdec(substr($bgc, 0, 2));
    $bg['g'] = hexdec(substr($bgc, 2, 2));
    $bg['b'] = hexdec(substr($bgc, 4, 2));

    foreach ($rgb as $pri) {
      $c[$pri] = dechex(round($percent * $fg[$pri]) + ((1.0 - $percent) * $bg[$pri]));
      if ($c[$pri] == '0') {
        $c[$pri] = '00';
      }
    }

    return $c['r'] . $c['g'] . $c['b'];
  }

  /**
   * Locate the RRD files in the filesystem. 
   */
  function rrd_files() {
    $files = $this->get_filenames();

    foreach ($files as $filename) {
      $basename = basename($filename, '.rrd');
      $instance = strpos($basename, '-')
      ? substr($basename, strpos($basename, '-') + 1)
      : 'value';
  
      $this->tinstances[] = $instance;
      $this->files[$instance] = $filename;
    }

    sort($this->tinstances);
    ksort($this->files);
  }

  /**
   * Get the actual RRD filenames from the filesystem. 
   */
  function get_filenames() {
    $identifier = sprintf('%s/%s%s%s/%s%s%s', $this->args['host'],
    $this->args['plugin'], strlen($this->args['plugin_instance']) ? '-' : '', $this->args['plugin_instance'],
    $this->args['type'], strlen($this->args['type_instance']) ? '-' : '', $this->args['type_instance']);

    $wildcard = strlen($this->args['type_instance']) ? '.' : '[-.]*';

    $files = glob($this->datadir . '/' . $identifier . $wildcard . 'rrd');

    return $files;
  }

  /**
   * Helper method 
   */
  function file2identifier($files) {
    foreach ($files as $key => $file) {
      if (is_file($file)) {
        $files[$key] = preg_replace("#^$this->datadir/#u", '', $files[$key]);
        $files[$key] = preg_replace('#\.rrd$#', '', $files[$key]);
      }
    }
    return $files;
  }

  /**
   * Main entry point into this class
   * This class will generate the command required to
   * call RRDTool and generate the graph
   */
  function rrd_graph() {
    if (!$this->colors) {
      $this->rainbow_colors();
    }

    $graphdata = $this->rrd_gen_graph();
    return implode(' ', $graphdata);
  }
    
  function rrd_options() {
    $rrdgraph[] = $this->rrdtool;
    $rrdgraph[] = 'graph - -a PNG';
    if ($this->rrdtool_opts != '') {
      $rrdgraph[] = $this->rrdtool_opts;
    }

    $rrdgraph[] = sprintf('-w %d', is_numeric($this->args['width']) ? $this->args['width'] : 400);
    $rrdgraph[] = sprintf('-h %d', is_numeric($this->args['height']) ? $this->args['height'] : 175);
    $rrdgraph[] = '-l 0';
    $rrdgraph[] = sprintf('-t "%s on %s"', $this->rrd_title, $this->args['host']);
    $rrdgraph[] = sprintf('-v "%s"', $this->rrd_vertical);
    $rrdgraph[] = sprintf('-s -%d', is_numeric($this->args['seconds']) ? $this->args['seconds'] : 86400);

    return $rrdgraph;
  }

  /**
   * Gather the files given the source. 
   */
  function rrd_get_sources() {
    # is the source spread over multiple files?
    if (is_array($this->files) && count($this->files)>1) {
      # and must it be ordered?
      if (is_array($this->order)) {
        $this->tinstances = array_merge(array_intersect($this->order, $this->tinstances));
      }
      # use tinstances as sources
      if (is_array($this->data_sources) && count($this->data_sources) > 1) {
        $sources = array();
        foreach ($this->tinstances as $f) {
          foreach ($this->data_sources as $s) {
            $sources[] = $f . '-' . $s;
          }
        }
      }
      else {
        $sources = $this->tinstances;
      }
    }
    # or one file with multiple data_sources
    else {
      if (is_array($this->data_sources) && count($this->data_sources)==1 && in_array('value', $this->data_sources)) {
        # use tinstances as sources
        $sources = $this->tinstances;
      } 
      else {
        # use data_sources as sources
        $sources = $this->data_sources;
      }
    }
    $this->parse_ds_names($sources);
    return $sources;
  }

  function parse_ds_names($sources) {
    # fill ds_names if not defined by plugin
    if (!is_array($this->ds_names)) {
      $this->ds_names = array_combine($sources, $sources);
    }

    # detect length of longest ds_name
    $max = 0;
    foreach ($this->ds_names as $ds_name) {
      if (strlen((string)$ds_name) > $max) {
        $max = strlen((string)$ds_name);
      }
    }

    # make all ds_names equal in lenght
    $format = sprintf("%%-%ds", $max);
    foreach ($this->ds_names as $index => $value) {
      $this->ds_names[$index] = sprintf($format, $value);
    }
  }

  function rrd_gen_graph() {
    $rrdgraph = $this->rrd_options();

    $sources = $this->rrd_get_sources();
    
    $i=0;
    foreach ($this->tinstances as $tinstance) {
      foreach ($this->data_sources as $ds) {
        $rrdgraph[] = sprintf('DEF:min_%s="%s":%s:MIN', $this->crc32hex($sources[$i]), $this->files[$tinstance], $ds);
        $rrdgraph[] = sprintf('DEF:avg_%s="%s":%s:AVERAGE', $this->crc32hex($sources[$i]), $this->files[$tinstance], $ds);
        $rrdgraph[] = sprintf('DEF:max_%s="%s":%s:MAX', $this->crc32hex($sources[$i]), $this->files[$tinstance], $ds);
        $i++;
      }
    }

    if (count($this->files) <= 1) {
      $c = 0;
      foreach ($sources as $source) {
        $color = is_array($this->colors) ? (isset($this->colors[$source])?$this->colors[$source]:$this->colors[$c++]): $this->colors;
        $rrdgraph[] = sprintf('AREA:max_%s#%s', $this->crc32hex($source), $this->get_faded_color($color));
        $rrdgraph[] = sprintf('AREA:min_%s#%s', $this->crc32hex($source), 'ffffff');
        break; # only 1 area to draw
      }
    }

    $c = 0;
    foreach ($sources as $source) {
      $dsname = $this->ds_names[$source] != '' ? $this->ds_names[$source] : $source;
      $color = is_array($this->colors) ? (isset($this->colors[$source])?$this->colors[$source]:$this->colors[$c++]): $this->colors;
      $rrdgraph[] = sprintf('LINE1:avg_%s#%s:\'%s\'', $this->crc32hex($source), $this->validate_color($color), $dsname);
      $rrdgraph[] = sprintf('GPRINT:min_%s:MIN:\'%s Min,\'', $this->crc32hex($source), $this->rrd_format);
      $rrdgraph[] = sprintf('GPRINT:avg_%s:AVERAGE:\'%s Avg,\'', $this->crc32hex($source), $this->rrd_format);
      $rrdgraph[] = sprintf('GPRINT:max_%s:MAX:\'%s Max,\'', $this->crc32hex($source), $this->rrd_format);
      $rrdgraph[] = sprintf('GPRINT:avg_%s:LAST:\'%s Last\\l\'', $this->crc32hex($source), $this->rrd_format);
    }

    return $rrdgraph;
  }
  
  /**
   * Issue the xport data command to return a set of data
   */
  function rrd_export_data() {
    $rrd_export = array(
      "{$this->rrdtool} xport -s -{$this->args['seconds']}",
    );
    
    $sources = $this->rrd_get_sources();
    $i=0;
    
    foreach ($this->tinstances as $tinstance) {
      foreach ($this->data_sources as $ds) {
        $rrd_export[] = sprintf('DEF:min_%s="%s":%s:MIN', $this->crc32hex($sources[$i]), $this->files[$tinstance], $ds);
        $rrd_export[] = sprintf('DEF:avg_%s="%s":%s:AVERAGE', $this->crc32hex($sources[$i]), $this->files[$tinstance], $ds);
        $rrd_export[] = sprintf('DEF:max_%s="%s":%s:MAX', $this->crc32hex($sources[$i]), $this->files[$tinstance], $ds);
        $i++;
      }
    }

    foreach ($sources as $source) {
      $rrd_export[] = sprintf('XPORT:min_%s:"%s min"', $this->crc32hex($source), $source);
      $rrd_export[] = sprintf('XPORT:avg_%s:"%s average"', $this->crc32hex($source), $source);
      $rrd_export[] = sprintf('XPORT:max_%s:"%s max"', $this->crc32hex($source), $source);
    }

    return $rrd_export;
  }
  
  
  /**
   * This method returns data via the RRDTool Fetch command.  The data
   * can be returned in computed form, or raw list form.
   */
  function rrd_gen_data() {
    $data = array();
    $multi_files = count($this->files) > 1 ? TRUE: FALSE;
    
    switch ($this->args['data_set']) {
      case 'computed':
        if ($multi_files == TRUE) {
          foreach ($this->files as $source => $file) {
            $data[$source] = $this->compute_values($file);
          }
        }
        else {
          $file = array_shift($this->files);
          $data = $this->compute_values($file);
        }
        break;
      case 'full':
        $cmd = implode(' ', $this->rrd_export_data());
        $output = `$cmd`;
        $data = simplexml_load_string($output);
                
        if (!is_null($this->args['format']) && $this->args['format'] == 'csv') {
          $data = $this->xml_to_csv($data);
        }
        break;
    }
    return $data;
  }
  
  /**
   * Take the RRD Export XML and convert it to
   * a comma delimited file. 
   */
  function xml_to_csv($xml) {
    $data_array = array();
    $header = array('timestamp');
    foreach ($xml->meta->legend->entry as $entry) {
      $header[] = (string)$entry;
    }
    $data_array[] = implode(",", $header);
    
    //loop the data row and convert to row with commas
    foreach ($xml->data->row as $row) {
      $row_array = array($row->t);    
      foreach ($row->v as $v) {
        $row_array[] = floatval($v);
      }
      $data_array[] = implode(',', $row_array);
    }
    return $data_array;
  }

  /**
   * This method will take an RRD file and then compute the 
   * average, min, max values
   */
  function compute_values($file) {
    $data = array();
    
    $functions = array('AVERAGE', 'MIN', 'MAX');
    foreach ($functions as $function) {
      $output = $this->data_cmd($file, $function);
      $data_array = $this->format_raw_data($output);
      
      foreach ($data_array as $key => $value) {
        if ($function == 'AVERAGE') {
          $data[$key]['average'] = $this->scale_format(array_sum($value) / count($value));
        }
        elseif ($function == 'MAX') {
          $data[$key]['max'] = $this->scale_format(max($value));
        }
        elseif ($function == 'MIN') {
          $data[$key]['min'] = $this->scale_format(min($value));
        }
      }
    }
    
    return $data;
  }
  
  /**
   * Builds the RRD fetch command
   */
  function data_cmd($file, $function) {
    $cmd = "{$this->rrdtool} fetch $file $function -s -{$this->args['seconds']}";
    $output = `$cmd`;
    return $output;
  }

  /**
   * Format the returned raw text into
   * an associative array of data.
   */
  function format_raw_data($rrd_data) {
    $data_array = array();
    $values = explode("\n", $rrd_data);

    $ds = array_shift($values);
    $ds = trim($ds);
    $columns = preg_split("/\s+/", $ds);

    if (count($columns) == count($this->data_sources)) {
      foreach ($values as $row) {
        $row = trim($row);
        if (!empty($row)) {
          $row_array = explode(':', $row);
          $data_values = preg_split("/\s+/", trim($row_array[1]));
          //now take the values and toss them into an array for 
          foreach ($this->data_sources as $key => $value) {
            if ($data_values[$key] != 'nan') {
              $data_array[$value][] = floatval(trim($data_values[$key]));
            }
          }
        }
      }
    }
    return $data_array;    
  }
  
  
  /**
   * Helper function
   */
  function scale_symbols($key) {
    $symbol = '';
    $symbols = array(
      -18 => 'a',
      -15 => 'f',
      -12 => 'p',
      -9 => 'n',
      -6 => 'u',
      -3 => 'm',
      3 => 'k',
      6 => 'M',
      9 => 'G',
      12 => 'T',
      15 => 'P',
      18 => 'E',
    );
    if (isset($symbols[$key])) {
      $symbol = $symbols[$key];
    }
    
    return $symbol;
  }
  
  /**
   * This method will auto-scale the value
   * apply the unit to it and return the
   * raw and formatted string version of the 
   * number.  The return will be an associative 
   * array with a 2 decimal place rounded two decimal
   * number and a formatted string auto-scaled and 
   * formatted with units of measurement
   */
  function scale_format($val) {
    $data = array();
    $floor = $this->rrd_floor($val);
    $mag = intval($floor/3);
    $index = $mag * 3;
    
    $new_value = $val / pow(10, $index);
    $symbol = $this->scale_symbols($index);

    $data['raw'] = $val;
    $data['autoscaled'] = number_format($new_value, 2) . $this->scale_symbols($index);
    $data['symbol'] = $this->scale_symbols($index);
    return $data;
  }
  
  /**
   * Helper function
   */
  function rrd_floor($val) {
   $i = 0;
   if ($val > 1.0) {
     while ($val > 10.00) {
       $i++;
       $val = $val/10.0;
     }
   }
   elseif ($val != 0.0) {
     while ($val < 10.00) {
       $i--;
       $val = $val * 10.0;
     }
   }

   return $i;
  }
  
  /**
   * Helper function to generate a hex value 
   */
  function crc32hex($str) {
    return sprintf("%x", crc32($str));
  }
}