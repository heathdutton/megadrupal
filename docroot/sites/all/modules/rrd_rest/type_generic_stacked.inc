<?php

/**
 * @file
 * This class has been adapted from http://pommi.nethuis.nl/collectd-graph-panel-v0-3/
 * to fit into Drupal as the basis of graph generation.  All the data gathering logic
 * and graphing logic has not changed. The main changes are that the image writing
 * function has been delegated to the calling function inside the rrd_rest module
 *
 * This provides the basic foundation for generation graphs from RRDTool
 */
 
class Type_GenericStacked extends Type_Default {

  function rrd_gen_graph() {
    $rrdgraph = $this->rrd_options();

    $sources = $this->rrd_get_sources();

    $i=0;
    foreach ($this->tinstances as $tinstance) {
      foreach ($this->data_sources as $ds) {
        $rrdgraph[] = sprintf('DEF:min_%s=%s:%s:MIN', $this->crc32hex($sources[$i]), $this->files[$tinstance], $ds);
        $rrdgraph[] = sprintf('DEF:avg_%s=%s:%s:AVERAGE', $this->crc32hex($sources[$i]), $this->files[$tinstance], $ds);
        $rrdgraph[] = sprintf('DEF:max_%s=%s:%s:MAX', $this->crc32hex($sources[$i]), $this->files[$tinstance], $ds);
        $i++;
      }
    }

    for ($i=count($sources)-1 ; $i>=0 ; $i--) {
      if ($i == (count($sources)-1))
        $rrdgraph[] = sprintf('CDEF:area_%s=avg_%1$s', $this->crc32hex($sources[$i]));
      else
        $rrdgraph[] = sprintf('CDEF:area_%s=area_%s,avg_%1$s,+', $this->crc32hex($sources[$i]), $this->crc32hex($sources[$i+1]));
    }

    $c = 0;
    foreach ($sources as $source) {
      $color = is_array($this->colors) ? (isset($this->colors[$source])?$this->colors[$source]:$this->colors[$c++]) : $this->colors;
      $color = $this->get_faded_color($color);
      $rrdgraph[] = sprintf('AREA:area_%s#%s', $this->crc32hex($source), $color);
    }

    $c = 0;
    foreach ($sources as $source) {
      $dsname = $this->ds_names[$source] != '' ? $this->ds_names[$source] : $source;
      $color = is_array($this->colors) ? (isset($this->colors[$source])?$this->colors[$source]:$this->colors[$c++]) : $this->colors;
      $rrdgraph[] = sprintf('LINE1:area_%s#%s:\'%s\'', $this->crc32hex($source), $this->validate_color($color), $dsname);
      $rrdgraph[] = sprintf('GPRINT:min_%s:MIN:\'%s Min,\'', $this->crc32hex($source), $this->rrd_format);
      $rrdgraph[] = sprintf('GPRINT:avg_%s:AVERAGE:\'%s Avg,\'', $this->crc32hex($source), $this->rrd_format);
      $rrdgraph[] = sprintf('GPRINT:max_%s:MAX:\'%s Max,\'', $this->crc32hex($source), $this->rrd_format);
      $rrdgraph[] = sprintf('GPRINT:avg_%s:LAST:\'%s Last\\l\'', $this->crc32hex($source), $this->rrd_format);
    }

    return $rrdgraph;
  }
}