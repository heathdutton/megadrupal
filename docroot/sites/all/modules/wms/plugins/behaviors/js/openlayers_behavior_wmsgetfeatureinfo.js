// $Id$

/**
 * @file
 * JS Implementation of OpenLayers behavior.
 */

/**
 * WMSGetFeatureinfo Behavior
 * http://dev.openlayers.org/releases/OpenLayers-2.9/doc/apidocs/files/OpenLayers/Control/WMSGetFeatureInfo-js.html
 */

(function ($) {
  //Initialize settings array.
  Drupal.openlayers.openlayers_behavior_wmsgetfeatureinfo = Drupal.openlayers.openlayers_behavior_wmsgetfeatureinfo || {};

  //Add the wmsgetfeatureinfo behavior
  Drupal.openlayers.addBehavior('openlayers_behavior_wmsgetfeatureinfo', function (data, options) {
    var map = data.openlayers;
    var layer;
    var layers = [];

    if (data.map.behaviors['openlayers_behavior_wmsgetfeatureinfo']) {
      if (options.getfeatureinfo_usevisiblelayers == false) { 
        layers = data.openlayers.getLayersBy('drupalID', 
          options.getfeatureinfo_layers);  // TODO Make this multiple select! 
      } else {
        for (layer in data.openlayers.layers) {
          if ((data.openlayers.layers[layer].CLASS_NAME == "OpenLayers.Layer.WMS") &&
              (data.openlayers.layers[layer].isBaseLayer == false)){
            layers.push(data.openlayers.layers[layer]);
          }
        }
      }

      Drupal.openlayers.openlayers_behavior_wmsgetfeatureinfo.getfeatureinfo_htmlelement = 
        options.getfeatureinfo_htmlelement;
      Drupal.openlayers.openlayers_behavior_wmsgetfeatureinfo.getfeatureinfo_usevisiblelayers = 
        options.getfeatureinfo_usevisiblelayers;
      Drupal.openlayers.openlayers_behavior_wmsgetfeatureinfo.getfeatureinfo_info_format = 
        options.getfeatureinfo_info_format;
      Drupal.openlayers.openlayers_behavior_wmsgetfeatureinfo.layers = layers;

      OpenLayers.Control.Click = OpenLayers.Class(OpenLayers.Control, 
        {
          defaultHandlerOptions: {
            'single': true,
            'double': false,
            'pixelTolerance': 0,
            'stopSingle': false,
            'stopDouble': false
          },

          initialize: function(options) {
            this.handlerOptions = OpenLayers.Util.extend(
              {}, this.defaultHandlerOptions
            );
            OpenLayers.Control.prototype.initialize.apply(
              this, arguments
            ); 
            this.handler = new OpenLayers.Handler.Click(
              this, {
                'click': this.onClick,
              }, this.handlerOptions
            );
          }, 

          onClick: function(evt) {
            //map = data.openlayers;
            var layernames = "";
            var wmslayers = "";
            var projection = "";
            // TODO check if all layers have same projection
            // TODO check if all layers are from same server
            for (layer in Drupal.openlayers.openlayers_behavior_wmsgetfeatureinfo.layers) {
              if (layers[layer].visibility 
                  && (layers[layer].CLASS_NAME == "OpenLayers.Layer.WMS") &&
                  layers[layer].isBaseLayer == false ) {
                wmslayers = wmslayers + layers[layer].params.LAYERS + ",";
                layernames = layernames + " " + layers[layer].name;
                projection = layers[layer].projection['projCode']; 
              }
            }
            if (wmslayers.length > 0) { 
              wmslayers = wmslayers.substring(0, wmslayers.length -1);
              Drupal.openlayers.openlayers_behavior_wmsgetfeatureinfo.beforegetfeatureinfo(layernames);
              var params = {
                    REQUEST: "GetFeatureInfo",
                    EXCEPTIONS: "application/vnd.ogc.se_xml",
                    BBOX: layers[0].getExtent().toBBOX(),
                    X: Math.round(evt.xy.x),
                    Y: Math.round(evt.xy.y),
                    INFO_FORMAT: Drupal.openlayers.openlayers_behavior_wmsgetfeatureinfo.getfeatureinfo_info_format, 
                    QUERY_LAYERS: wmslayers,
                    FEATURE_COUNT: 50,
                    Srs: projection,
                    LAYERS: wmslayers, 
                    Styles: '',
                    WIDTH: data.openlayers.size.w,
                    HEIGHT: data.openlayers.size.h,
                    format: 'image/png'}; // TODO parameterize this, like ...?
               var url = layers[0].getFullRequestString(params);
               $.ajax({ 
                 type: 'POST', 
                 url: Drupal.settings.basePath + 'openlayers/wms/wmsgetfeatureinfo',
                 data: { 
                   ajax : true, 
                   url : url 
                 },
                 success: Drupal.openlayers.openlayers_behavior_wmsgetfeatureinfo.fillHTML,
                 fail: Drupal.openlayers.openlayers_behavior_wmsgetfeatureinfo.fillHTMLerror
               });
            }
            else {
              alert ("No Layer to Query is visible.");
            }
          }
        }
      );
      GetFeatureControl = new OpenLayers.Control.Click(); 

      data.openlayers.addControl(GetFeatureControl);
      GetFeatureControl.activate();
    
      // This is to update the OpenLayers Plus block switcher feature
      // Drupal.OpenLayersPlusBlockswitcher.redraw();  // TODO Fix this for D7
    }
  });

  Drupal.openlayers.openlayers_behavior_wmsgetfeatureinfo.beforegetfeatureinfo = function(layernames) {
    if((typeof $.colorbox == 'function') && ($("#popup").length == 0)) {
      $.colorbox({
        Title: "Results.",
        height: "80%",
        width: "80%",
        opacity: ".25",
        inline: true,
        href:"#" + Drupal.openlayers.openlayers_behavior_wmsgetfeatureinfo.getfeatureinfo_htmlelement}
      );
    } else {
      $("#" + Drupal.openlayers.openlayers_behavior_wmsgetfeatureinfo.getfeatureinfo_htmlelement).parent().css("display", "block");
    }
    if (Drupal.openlayers.openlayers_behavior_wmsgetfeatureinfo.getfeatureinfo_usevisiblelayers == false) { 
      document.getElementById(Drupal.openlayers.openlayers_behavior_wmsgetfeatureinfo.getfeatureinfo_htmlelement).innerHTML = Drupal.t('Searching...');
      return; 
    }
  
    if (layernames.length == 0 ) {
      document.getElementById(Drupal.openlayers.openlayers_behavior_wmsgetfeatureinfo.getfeatureinfo_htmlelement).innerHTML = Drupal.t('No layer selected');
      return;
    } else {
      document.getElementById(Drupal.openlayers.openlayers_behavior_wmsgetfeatureinfo.getfeatureinfo_htmlelement).innerHTML = Drupal.t('Searching in ' + layernames);
    }
    return;
  };
 
  Drupal.openlayers.openlayers_behavior_wmsgetfeatureinfo.fillHTML = function(result) {
    $('#' + Drupal.openlayers.openlayers_behavior_wmsgetfeatureinfo.getfeatureinfo_htmlelement).html(result);
    Drupal.attachBehaviors($('#' + Drupal.openlayers.openlayers_behavior_wmsgetfeatureinfo.getfeatureinfo_htmlelement));
  };

  Drupal.openlayers.openlayers_behavior_wmsgetfeatureinfo.fillHTMLerror = function(result) {
    $('#' + Drupal.openlayers.openlayers_behavior_wmsgetfeatureinfo.getfeatureinfo_htmlelement).html(result);
    Drupal.attachBehaviors($('#' + Drupal.openlayers.openlayers_behavior_wmsgetfeatureinfo.getfeatureinfo_htmlelement));
  };
})(jQuery);
