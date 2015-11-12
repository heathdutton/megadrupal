/**
 * @file
 * JavaScript integration between RGraph and Drupal.
 */
function chart_rgraph_option(option){
    if (typeof(option) === 'string'){
       option = "'"+ option+"'";
    } else if (typeof(option) === 'object'){
       option = JSON.stringify(option);
    }
  return option;
}

(function ($) {

  Drupal.behaviors.chartsRGraph = {};
  Drupal.behaviors.chartsRGraph.attach = function(context, settings) {
    $('.charts-rgraph').once('charts-rgraph', function() {
      if ($(this).attr('data-chart')) {
         var dcc = $(this).attr('data-chart');
         // fix background in ie8
         var ua = window.navigator.userAgent;
         var msie = ua.indexOf("MSIE ");
         if (msie > 0 && parseInt(ua.substring(msie + 5, ua.indexOf(".", msie))) == 8)   {   // If Internet Explorer, return version number
           dcc = dcc.replace("transparent","rgba(0,0,0,0)");
         }
         // end fix
         var config = $.parseJSON(dcc);
         var c = "";
         for (i = 0; i < config['options'].length; ++i) {
           c = c+".Set(" + chart_rgraph_option(config['options'][i][0]) + ","
               + chart_rgraph_option(config['options'][i][1]) + ")";
         }
         c = "var bar = new RGraph." + config['chart type'] + "('"
             + config['canvass']['id'] +"', "  + JSON.stringify(config['data']) 
             + ")" + c + ".Draw()";
         var canv = document.createElement('canvas');
         canv.id = config['canvass']['id'];
         if (config['canvass']['width']) {canv.width = config['canvass']['width'];}
         if (config['canvass']['height']) {canv.height = config['canvass']['height'];}
         canv.className = "charts-rgraph-canvass";
         var el = this;
         // quick fix to allow excanvass to load
         if (!document.createElement('canvas').getContext) {
           setTimeout(function(){
              el.appendChild(canv);
              G_vmlCanvasManager.initElement(canv);
              eval(c);
           },750);
         } else {
           el.appendChild(canv);
           eval(c);
         }
       }
    });
  };

})(jQuery);
