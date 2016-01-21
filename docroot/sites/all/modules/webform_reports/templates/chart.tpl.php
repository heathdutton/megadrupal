<?php
$xseries = array2dateSeries($variables['xseries'], 'custom', 'd M y');
$yseries = array2series($variables['yseries']);
$options = $variables['options'];

dsm($yseries);
print 'xyz';
?>

<script type="text/javascript">
  jQuery(document).ready(function(){    
    
    jQuery.jqplot.config.enablePlugins = true;
    
    var plot2 = jQuery.jqplot('chart', <?php print $yseries; ?>, {
      title: 'Some Plot',
      seriesDefaults:{                        
        pointLabels: { show: true }
      },
      axes: {
        xaxis: {                            
          renderer:  jQuery.jqplot.CategoryAxisRenderer,
          ticks: <?php print $xseries; ?>
        },
        yaxis: {
          tickOptions: {
            //formatString: '$%.2f'
          }
        }
      },
      highlighter: {
        sizeAdjust: 7.5
      },
      cursor: {
        show: true
      }
    });

  });
</script>
hello
<div id="chart">hi</div>