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
 
class Type_GenericIO extends Type_Default {
  
  function rrd_gen_graph() {
    $raw = '';
    $rrdgraph = $this->rrd_options();

    $sources = $this->rrd_get_sources();

    if ($this->scale) {
      $raw = '_raw';
    }
    $i=0;
    foreach ($this->tinstances as $tinstance) {
      foreach ($this->data_sources as $ds) {
        $rrdgraph[] = sprintf('DEF:min_%s%s=%s:%s:MIN', $this->crc32hex($sources[$i]), $raw, $this->files[$tinstance], $ds);
        $rrdgraph[] = sprintf('DEF:avg_%s%s=%s:%s:AVERAGE', $this->crc32hex($sources[$i]), $raw, $this->files[$tinstance], $ds);
        $rrdgraph[] = sprintf('DEF:max_%s%s=%s:%s:MAX', $this->crc32hex($sources[$i]), $raw, $this->files[$tinstance], $ds);
        $rrdgraph[] = sprintf('VDEF:tot_%1$s=avg_%1$s%2$s,TOTAL', $this->crc32hex($sources[$i]), $raw);
        $i++;
      }
    }
    if ($this->scale) {
      $i=0;
      foreach ($this->tinstances as $tinstance) {
        foreach ($this->data_sources as $ds) {
          $rrdgraph[] = sprintf('CDEF:min_%s=min_%1$s_raw,%s,*', $this->crc32hex($sources[$i]), $this->scale);
          $rrdgraph[] = sprintf('CDEF:avg_%s=avg_%1$s_raw,%s,*', $this->crc32hex($sources[$i]), $this->scale);
          $rrdgraph[] = sprintf('CDEF:max_%s=max_%1$s_raw,%s,*', $this->crc32hex($sources[$i]), $this->scale);
//          $rrdgraph[] = sprintf('VDEF:tot_%1$s=avg_%1$s,TOTAL', $this->crc32hex($sources[$i]));
          $i++;
        }
      }
    }

    $rrdgraph[] = sprintf('CDEF:overlap=avg_%s,avg_%s,LT,avg_%1$s,avg_%2$s,IF',
            $this->crc32hex($sources[0]), $this->crc32hex($sources[1]));

    foreach ($sources as $source) {
      $rrdgraph[] = sprintf('AREA:avg_%s#%s', $this->crc32hex($source), $this->get_faded_color($this->colors[$source]));
    }

    $rrdgraph[] = sprintf('AREA:overlap#%s',
      $this->get_faded_color(
        $this->get_faded_color($this->colors[$sources[0]]),
        $this->get_faded_color($this->colors[$sources[1]])
      )
    );

    foreach ($sources as $source) {
      $dsname = $this->ds_names[$source] != '' ? $this->ds_names[$source] : $source;
      $rrdgraph[] = sprintf('LINE1:avg_%s#%s:\'%s\'', $this->crc32hex($source), $this->colors[$source], $dsname);
      $rrdgraph[] = sprintf('GPRINT:min_%s:MIN:\'%s Min,\'', $this->crc32hex($source), $this->rrd_format);
      $rrdgraph[] = sprintf('GPRINT:avg_%s:AVERAGE:\'%s Avg,\'', $this->crc32hex($source), $this->rrd_format);
      $rrdgraph[] = sprintf('GPRINT:max_%s:MAX:\'%s Max,\'', $this->crc32hex($source), $this->rrd_format);
      $rrdgraph[] = sprintf('GPRINT:avg_%s:LAST:\'%s Last\'', $this->crc32hex($source), $this->rrd_format);
      $rrdgraph[] = sprintf('GPRINT:tot_%s:\'%s Total\l\'', $this->crc32hex($source), $this->rrd_format);
    }
    
    return $rrdgraph;
  }
}