(function($) {
  Drupal.openlayers_geosearch = {};

  Drupal.behaviors.openlayers_behavior_geosearch = {
      'attach': function(context, settings) {
        $('#openlayersgeosearchresults').once('openlayersgeosearchresults', function() {
          // Take all the maps on the page
          Drupal.openlayers_geosearch.data = $('.openlayers-map').data('openlayers');
          if (!Drupal.openlayers_geosearch.data.map.displayProjection) {
            Drupal.openlayers_geosearch.data.map.displayProjection = 4326;
          }
          /*
           * Register functions that listen to the map zooming and panning
           * and then recalculate what should be shown in the search results table
           */
          Drupal.openlayers_geosearch.data.openlayers.events.register("zoomend", Drupal.openlayers_geosearch.data.map, Drupal.behaviors.openlayers_behavior_OLrecalc);
          Drupal.openlayers_geosearch.data.openlayers.events.register("moveend", Drupal.openlayers_geosearch.data.map, Drupal.behaviors.openlayers_behavior_OLrecalc);
          
          Drupal.openlayers_geosearch.displayProjection = new OpenLayers.Projection(Drupal.openlayers_geosearch.data.map.displayProjection);
          Drupal.openlayers_geosearch.projection = new OpenLayers.Projection(Drupal.openlayers_geosearch.data.map.projection);
          var searchLayers = Drupal.openlayers_geosearch.data.openlayers.getLayersBy('drupalID', "openlayers_searchresult_layer");
          // If the searchlayer is not selected, we just create one on the fly
          if (searchLayers.length == 0) {
            var searchLayer = new OpenLayers.Layer.Vector(
                Drupal.t("Search Layer"),
                {
                  projection: new OpenLayers.Projection('EPSG:4326'),
                  drupalID: 'openlayers_searchresult_layer'
                }
            );
            // We add the default styles to the layer, so we can use them when the table is clicked
            var styleMap = Drupal.openlayers.getStyleMap(Drupal.openlayers_geosearch.data.map, 'openlayers_searchresult_layer');
            searchLayer.StyleMap = styleMap;
            Drupal.openlayers_geosearch.data.openlayers.addLayer(searchLayer);
            searchLayers.push(searchLayer);
          }
          Drupal.openlayers_geosearch.vectorLayer = Drupal.openlayers_geosearch.data.openlayers.getLayersBy('drupalID', "openlayers_searchresult_layer");

          var options = {
            clickout: true, 
            toggle: true,
            multiple: false,
            hover: Drupal.settings.openlayers_geosearch.hoover,
          };
          /*
           * Create an Openlayers Control that keeps the selection in the table and the map in sync
           */
          var popupSelect = new OpenLayers.Control.GeoSearchSelectFeature(searchLayers[0],options);

          Drupal.openlayers_geosearch.popupSelect = popupSelect;

          Drupal.openlayers_geosearch.data.openlayers.addControl(popupSelect);
          popupSelect.activate();
        });

        /*
         *  This event is only when the results are reset, after a fresh search
         */

        $("#openlayersgeosearchresults").once(function() {
          // here we unselect any dot and remove all dots from the map (only before adding the first result
          Drupal.openlayers_geosearch.popupSelect.unselectAll();
          Drupal.openlayers_geosearch.vectorLayer[0].removeAllFeatures();
        });

        /*
         * This only happens upon (re)loading the full set of results
         */
        $('.openlayers-geosearch-result-struct').once('openlayers-geosearch-result-struct', function() {
          /*
           * Now we loop through all the links within the table or panel, the links hold the lat & lon for each point to be plotted
           */
          $('.openlayers-geosearch-result-struct a').once('openlayers-geosearch-result-a', function() {
            // and here we add the dot to the map
            var point = Drupal.openlayers_geosearch.getpoint($(this)[0].href);
            var geometry = new OpenLayers.Geometry.Point(point.lat, point.lon).transform(Drupal.openlayers_geosearch.displayProjection, Drupal.openlayers_geosearch.projection);

            var pointfeature = new OpenLayers.Feature.Vector(geometry);
            // lets get the styles of this layer
            var styleMap = Drupal.openlayers.getStyleMap(Drupal.openlayers_geosearch.data.map, 'openlayers_searchresult_layer');
            pointfeature.style = styleMap.styles['default'].defaultStyle;
            // we store the id of the feature in our <a id="id"> tag, so we can do things when we click the link
            var id = pointfeature.id + ".list";
            // the . does not go well with css
            id = id.replace(/\./g, '-');
            $(this)[0].id = id;
            pointfeature.attributes.name = $(this)[0].innerHTML;
            var popupid = pointfeature.id + ".popup";
            // the . does not go well with css
            popupid = popupid.replace(/\./g, '-');
            pointfeature.attributes.description = "";

            Drupal.openlayers_geosearch.vectorLayer[0].addFeatures([pointfeature], styleMap.styles['default'].defaultStyle);
            $(this).click(Drupal.openlayers_geosearch.blockclick);
          });
          
          if ($(".form-item-openlayers-geosearch-search-by-boundingbox .form-checkbox")[0].checked) {
            Drupal.openlayers_geosearch.recalcResultTable(this);
          }
          else {
            Drupal.openlayers_geosearch.zoomtoresults();
          }
        });
      }
  };

  Drupal.behaviors.openlayers_behavior_tabs = {
      'attach': function(context, settings) {
        var tabsIds = $("#openlayersgeosearchtabs"); 
        if (tabsIds.length > 0) {
          tabsIds.tabs();
        }
      }
  };

  Drupal.behaviors.openlayers_behavior_panels = {
      'attach': function(context, settings) {
        var tabsIds = $("#openlayersgeosearchpanels");
        if (tabsIds.length > 0) {
          tabsIds.tabs({ collapsible: true });
        }
      }
  };

  /**
   * The click of the checkbox. Determine here wether we need to filter the results or not
   */
  Drupal.behaviors.openlayers_behavior_checkbox = {
      'attach': function(context, settings) {
          $(".form-item-openlayers-geosearch-search-by-boundingbox .form-checkbox").click(function () {
            $('.openlayers-geosearch-result-struct').each(function() {
              var checked = $(".form-item-openlayers-geosearch-search-by-boundingbox  .form-checkbox")[0].checked;  
              if (checked) {
                Drupal.openlayers_geosearch.recalcResultTable(this);
              }
              else {
                $(this).find('a').each(function() {
                  $(this).parent().removeClass('outsidebox');
                  $(this).parent().addClass('insidebox');
                });
                Drupal.openlayers_geosearch.noOfVisibleElements(this);
              }
           });
        });
     }
  };

  /**
   * This function is fired when the map has just zoomed or panned
   */
  Drupal.behaviors.openlayers_behavior_OLrecalc = function() {
    $('.openlayers-geosearch-result-struct').each(function() {
        var checked = $(".form-item-openlayers-geosearch-search-by-boundingbox .form-checkbox")[0].checked;  
        if (checked) {
          Drupal.openlayers_geosearch.recalcResultTable(this);
          Drupal.openlayers_geosearch.noOfVisibleElements(this);
        }
    });  
  }

  /**
   * Performs a search on the a links in the Results Block
   */
  Drupal.openlayers_geosearch.blockclick = function () {
    // the id is passed as OpenLayers.Features.id.list (so we remove the .list from the string to get the id)
    var id = this.id.substring(0, this.id.length - 5);
    // find the results layer
    var vectorLayer = Drupal.openlayers_geosearch.data.openlayers.getLayersBy('drupalID', "openlayers_searchresult_layer");
    // css does not like dots
    id = id.replace(/-/g, '.');
    // find the feature
    var feature = vectorLayer[0].getFeatureById(id);
    if (Drupal.settings.openlayers_geosearch.hoover) {
      Drupal.openlayers_geosearch.popupSelect.unselectAll();
      Drupal.openlayers_geosearch.popupSelect.overFeature(feature);
    }
    else {
      Drupal.openlayers_geosearch.popupSelect.unselectAll();
      Drupal.openlayers_geosearch.popupSelect.clickFeature(feature);        
    }
    return false;
  };

  /*
   *  Returns a Point from the href we have crafted
   */
  Drupal.openlayers_geosearch.getpoint = function(href) {
    var mainparts = href.split("?");
    var parts = mainparts[1].split("&");
    var point = {};
    for (var i in parts) {
      part = parts[i].split("=");
      point[part[0]] = part[1];
    }
    return point;
  };

  Drupal.openlayers_geosearch.zoomtoresults = function() {
    var layerextent = Drupal.openlayers_geosearch.vectorLayer[0].getDataExtent();

    // Check for valid layer extent
    if (layerextent != null) {
      Drupal.openlayers_geosearch.data.openlayers.zoomToExtent(layerextent);

      // If unable to find width due to single point,
      // zoom in with point_zoom_level option.
      // Lets try to change this to the Bounding box of the Point.
      if (layerextent.getWidth() == 0.0) {
        Drupal.openlayers_geosearch.data.openlayers.zoomTo(Drupal.settings.openlayers_geosearch.zoomlevel);
      }
    }
  };

  /**
   * Javascript Drupal Theming function for inside of Popups
   *
   * To override
   *
   * @param feature
   *  OpenLayers feature object
   * @return
   *  Formatted HTML
   */
  Drupal.theme.prototype.openlayers_geosearchPopup = function(feature) {
    var output = '';
    if (typeof Drupal.theme.prototype.openlayers_geosearchPopupCustom != 'function') {
      output =
        '<div class="openlayers-geosearch-popup openlayers-geosearch-popup-name">' +
        feature.attributes.name +
        '</div>' +
        '<div class="openlayers-geosearch-popup openlayers-geosearch-popup-description">' +
        feature.attributes.description +
        '</div>';
    } else {
      output = Drupal.theme.prototype.openlayers_geosearchPopupCustom(feature);
    }
    return output;
  };

  /**
   * function to calculate the extent of the current display 
   */
  Drupal.openlayers_geosearch.isPointWithinBounds = function(point) {
    //gets the real points to be plotted on the map
    var geometry = new OpenLayers.Geometry.Point(point.lat, point.lon).transform(Drupal.openlayers_geosearch.displayProjection, Drupal.openlayers_geosearch.projection);
    var xx = geometry.x;
    var yy = geometry.y;        
    //gets the bounds. Note that left==right, and top==bottom
    var extentValues = Drupal.openlayers_geosearch.data.openlayers.getExtent();
    var extentleft = extentValues.left;
    var extenttop = extentValues.top;
    var extentbottom = extentValues.bottom;
    var extentright = extentValues.right;
    var result = "false";
    
    if ((xx > extentleft && xx < extentright)) {
      if ((yy > extentbottom && yy < extenttop)) {
        result = "true";
      }
      else {
        result = "false";
      }
    }
    else {
      result = "false"
    }
    return result;
  };
  
  /**
   * This function goes through all items in the table and marks wether they are
   * within the current viewport or not
   */
  Drupal.openlayers_geosearch.recalcResultTable = function(me) {
    $(me).find('a').each(function() {
      // and here we add the dot to the map
      var point = Drupal.openlayers_geosearch.getpoint(this.href);
      var isWithinBounds = Drupal.openlayers_geosearch.isPointWithinBounds(point);
      if (isWithinBounds == 'false' ) {
        $(this).parent().addClass("outsidebox");
        $(this).parent().removeClass("insidebox");
      }
      else {
        $(this).parent().addClass("insidebox");                
        $(this).parent().removeClass("outsidebox");                
      }
    });
    if ($(".form-item-openlayers-geosearch-search-by-boundingbox .form-checkbox")[0].checked) {
      Drupal.openlayers_geosearch.noOfVisibleElements(me);
    }
  }
  
  /** This function calculates the number of visible results in the results table 
   *  For now, we calcualte for google, yandex, yahoo and mapquest
   */
  Drupal.openlayers_geosearch.noOfVisibleElements = function(me) {
      var counthidden = $(me).find('.outsidebox').length;
      var counttotal = $(me).children().length;
      var countvisible = counttotal - counthidden;
      
      $(me).parent().parent().find('h2 .count')[0].innerHTML = countvisible;
     
  };
  /**
   * This is additional stuff we do when items are clicked.
   * Like zooming in
   */
  Drupal.openlayers_geosearch.extraclick = function (feature) {
      /*
       * This is added for the totally exceptional scenario where the Drupal 
       * WMS GetFeatureInfo control is also active. 
       * In that scenario, clicking a search result should also trigger the WMS
       * search, so items behind a search result can be queried.
       */
    wmscontrol = Drupal.openlayers_geosearch.data.openlayers.getControlsByClass('OpenLayers.Control.DrupalGetFeatureInfo');
    if ((wmscontrol) && (wmscontrol[0])) {
      wmscontrol[0].onClick(feature);
    }
    /*
     * Now zoom the map
     * somehow it looks like the center will not be the center, but it 
     * takes the extend of the search results layer into account
     * this makes the results show up in the corner of the map
     * a beer for who solves this.
     */
    if (Drupal.settings.openlayers_geosearch.zoomonselect && (this.map.zoom < Drupal.settings.openlayers_geosearch.zoomlevel)) {
   //   var point = Drupal.openlayers_geosearch.getpoint(jQuery(that)[0].href);
   //   var geometry = new OpenLayers.Geometry.Point(point.lat, point.lon).transform(Drupal.openlayers_geosearch.displayProjection, Drupal.openlayers_geosearch.projection);
      if (this.feature) {
      if (this.map.zoom < Drupal.settings.openlayers_geosearch.zoomlevel) {
        this.map.setCenter(new OpenLayers.LonLat(this.feature.geometry.x, this.feature.geometry.y), Drupal.settings.openlayers_geosearch.zoomlevel);
      }
      else {
        this.map.setCenter(new OpenLayers.LonLat(this.feature.geometry.x, this.feature.geometry.y));
      }
      }
    }
  }
})(jQuery);


OpenLayers.Control.GeoSearchSelectFeature = OpenLayers.Class(OpenLayers.Control.SelectFeature, {

        onSelect: function (feature) {
          var map = Drupal.openlayers_geosearch.data.openlayers;

          /*
           * The popup code is copied (oh horror) from the popup_behaviour
           */
          // Create FramedCloud popup.
          popup = new OpenLayers.Popup.FramedCloud(
              'geosearchpopup',
              feature.geometry.getBounds().getCenterLonLat(),
              null,
              Drupal.theme('openlayers_geosearchPopup', feature),
              null,
              true,
              function (evt) {
                jQuery("#geosearchpopup").remove();
                var that = jQuery('#' + feature.id.replace(/\./g, '-') + '-list');
                jQuery(that[0]).removeClass("openlayers-geosearch-selected")
              }
          );
          // Redraw the feature as being selected.

          var styleMap = Drupal.openlayers.getStyleMap(Drupal.openlayers_geosearch.data.map, 'openlayers_searchresult_layer');
          feature.style = styleMap.styles['select'].defaultStyle;
          var vectorLayer = Drupal.openlayers_geosearch.data.openlayers.getLayersBy('drupalID', "openlayers_searchresult_layer");
          vectorLayer[0].drawFeature(feature, styleMap.styles['select'].defaultStyle);

          /*
           * Add a selected class to the html element with the corresponding id as this feature
           */
          id = feature.id;
          id = id.replace(/\./g, '-');
          var that = jQuery('#' + id + '-list');
          jQuery(that[0]).addClass("openlayers-geosearch-selected");
          jQuery(that[0]).parent().removeClass("outsidebox");
          jQuery(that[0]).parent().addClass("insidebox");
          // Assign popup to feature and map.
          feature.popup = popup;
          feature.layer.map.addPopup(popup);

          /*
           * This event is added for the totally exceptional scenario where the Drupal 
           * WMS GetFeatureInfo control is also active. 
           * In that scenario, clicking a search result should also trigger the WMS
           * search, so items behind a search result can be queried.
           */
          Drupal.openlayers_geosearch.data.openlayers.events.register('click', this.handlers.feature, Drupal.openlayers_geosearch.extraclick, true);

        },
        onUnselect: function (feature) {

          // redraw the feature as default
          var styleMap = Drupal.openlayers.getStyleMap(Drupal.openlayers_geosearch.data.map, 'openlayers_searchresult_layer');
          feature.style = styleMap.styles['default'].defaultStyle;
          var vectorLayer = Drupal.openlayers_geosearch.data.openlayers.getLayersBy('drupalID', "openlayers_searchresult_layer");
          vectorLayer[0].drawFeature(feature, styleMap.styles['default'].defaultStyle);

          var that = jQuery('#' + feature.id.replace(/\./g, '-') + '-list');
          jQuery(that[0]).removeClass("openlayers-geosearch-selected");

          // Remove popup if feature is unselected.
          if (feature.popup) {
            feature.layer.map.removePopup(feature.popup);
            feature.popup.destroy();
            feature.popup = null;
          }
        },
        CLASS_NAME: "OpenLayers.Control.DrupalGeoSearch"
      }
    );
