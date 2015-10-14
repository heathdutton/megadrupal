
/**
 * Build data object which will be send to MapFish Print.
 *
 * To override
 *
 * @param map
 *  OpenLayers map object.
 * @return
 *  Object
 */
Drupal.openlayers.getPrintData = function(map) {
    return data = {
        title: '',
        srs: map.projection.projCode,
        units: map.units,
        pages: [{
            bbox: map.getExtent().toArray()
        }]
    };
};

Drupal.openlayers.addBehavior('openlayers_behavior_print', function (data, options) {
    var map = data.openlayers;

    // Copied form extjs2: src/GeoExt/data/MapfishPrintProvider.js
    var getAbsoluteUrl = function(url) {
        var a;
        if(jQuery.browser === "msie") {
            a = document.createElement("<a href='" + url + "'/>");
            a.style.display = "none";
            document.body.appendChild(a);
            a.href = a.href;
            document.body.removeChild(a);
        } else {
            a = document.createElement("a");
            a.href = url;
        }
        return a.href;
    };

    /**
     * Encoders for all print content.
     * Copied form extjs2: src/GeoExt/data/MapfishPrintProvider.js
     *
     * @private
     * @property {Object} encoders
     */
    var encoders = {};
    encoders = {
        "layers": {
            "Layer": function(layer) {
                var enc = {};
                if (layer.options && layer.options.maxScale) {
                    enc.minScaleDenominator = layer.options.maxScale;
                }
                if (layer.options && layer.options.minScale) {
                    enc.maxScaleDenominator = layer.options.minScale;
                }
                return enc;
            },
            "WMS": function(layer) {
                var enc = encoders.layers.HTTPRequest.call(this, layer);
                jQuery.extend(enc, {
                    type: 'WMS',
                    layers: [layer.params.LAYERS].join(",").split(","),
                    format: layer.params.FORMAT,
                    styles: [layer.params.STYLES].join(",").split(",")
                });
                var param;
                for(var p in layer.params) {
                    param = p.toLowerCase();
                    if(!layer.DEFAULT_PARAMS[param] &&
                    "layers,styles,width,height,srs".indexOf(param) == -1) {
                        if(!enc.customParams) {
                            enc.customParams = {};
                        }
                        enc.customParams[p] = layer.params[p];
                    }
                }
                return enc;
            },
            "OSM": function(layer) {
                var enc = encoders.layers.TileCache.call(this, layer);
                return jQuery.extend(enc, {
                    type: 'OSM',
                    baseURL: enc.baseURL.substr(0, enc.baseURL.indexOf("$")),
                    extension: "png"
                });
            },
            "TMS": function(layer) {
                var enc = encoders.layers.TileCache.call(this, layer);
                return jQuery.extend(enc, {
                    type: 'TMS',
                    format: layer.type
                });
            },
            "TileCache": function(layer) {
                var enc = encoders.layers.HTTPRequest.call(this, layer);
                return jQuery.extend(enc, {
                    type: 'TileCache',
                    layer: layer.layername,
                    maxExtent: layer.maxExtent.toArray(),
                    tileSize: [layer.tileSize.w, layer.tileSize.h],
                    extension: layer.extension,
                    resolutions: layer.serverResolutions || layer.resolutions
                });
            },
            "WMTS": function(layer) {
                var enc = encoders.layers.HTTPRequest.call(this, layer);
                return jQuery.extend(enc, {
                    type: 'WMTS',
                    layer: layer.layer,
                    version: layer.version,
                    requestEncoding: layer.requestEncoding,
                    tileOrigin: [layer.tileOrigin.lon, layer.tileOrigin.lat],
                    tileSize: [layer.tileSize.w, layer.tileSize.h],
                    style: layer.style,
                    formatSuffix: layer.formatSuffix,
                    dimensions: layer.dimensions,
                    params: layer.params,
                    maxExtent: (layer.tileFullExtent != null) ? layer.tileFullExtent.toArray() : layer.maxExtent.toArray(),
                    matrixSet: layer.matrixSet,
                    zoomOffset: layer.zoomOffset,
                    resolutions: layer.serverResolutions || layer.resolutions
                });
            },
            "KaMapCache": function(layer) {
                var enc = encoders.layers.KaMap.call(this, layer);
                return jQuery.extend(enc, {
                    type: 'KaMapCache',
                    // group param is mandatory when using KaMapCache
                    group: layer.params['g'],
                    metaTileWidth: layer.params['metaTileSize']['w'],
                    metaTileHeight: layer.params['metaTileSize']['h']
                });
            },
            "KaMap": function(layer) {
                var enc = encoders.layers.HTTPRequest.call(this, layer);
                return jQuery.extend(enc, {
                    type: 'KaMap',
                    map: layer.params['map'],
                    extension: layer.params['i'],
                    // group param is optional when using KaMap
                    group: layer.params['g'] || "",
                    maxExtent: layer.maxExtent.toArray(),
                    tileSize: [layer.tileSize.w, layer.tileSize.h],
                    resolutions: layer.serverResolutions || layer.resolutions
                });
            },
            "HTTPRequest": function(layer) {
                var enc = encoders.layers.Layer.call(this, layer);
                return jQuery.extend(enc, {
                    baseURL: getAbsoluteUrl(layer.url instanceof Array ?
                        layer.url[0] : layer.url),
                    opacity: (layer.opacity != null) ? layer.opacity : 1.0,
                    singleTile: layer.singleTile
                });
            },
            "Image": function(layer) {
                var enc = encoders.layers.Layer.call(this, layer);
                return jQuery.extend(enc, {
                    type: 'Image',
                    baseURL: getAbsoluteUrl(layer.getURL(layer.extent)),
                    opacity: (layer.opacity != null) ? layer.opacity : 1.0,
                    extent: layer.extent.toArray(),
                    pixelSize: [layer.size.w, layer.size.h],
                    name: layer.name
                });
            },
            "Vector": function(layer) {
                if(!layer.features.length) {
                    if (console) {
                        console.log("empty vector layer. ignoring");
                    }
                    return;
                }

                var encFeatures = [];
                var encStyles = {};
                var features = layer.features;
                var featureFormat = new OpenLayers.Format.GeoJSON();
                var styleFormat = new OpenLayers.Format.JSON();
                var nextId = 1;
                var styleDict = {};
                var feature, style, dictKey, dictItem, styleName;
                for(var i=0, len=features.length; i<len; ++i) {
                    feature = features[i];
                    style = feature.style || layer.style ||
                    layer.styleMap.createSymbolizer(feature,
                        feature.renderIntent);
                    dictKey = styleFormat.write(style);
                    dictItem = styleDict[dictKey];
                    if(dictItem) {
                        //this style is already known
                        styleName = dictItem;
                    } else {
                        //new style
                        styleDict[dictKey] = styleName = nextId++;
                        if(style.externalGraphic) {
                            encStyles[styleName] = jQuery.extendIf({
                                externalGraphic: getAbsoluteUrl(
                                    style.externalGraphic)}, style);
                        } else {
                            encStyles[styleName] = style;
                        }
                    }
                    var featureGeoJson = featureFormat.extract.feature.call(
                        featureFormat, feature);

                    featureGeoJson.properties = OpenLayers.Util.extend({
                        _gx_style: styleName
                    }, featureGeoJson.properties);

                    encFeatures.push(featureGeoJson);
                }
                var enc = encoders.layers.Layer.call(this, layer);
                return jQuery.extend(enc, {
                    type: 'Vector',
                    styles: encStyles,
                    styleProperty: '_gx_style',
                    geoJson: {
                        type: "FeatureCollection",
                        features: encFeatures
                    },
                    name: layer.name,
                    opacity: (layer.opacity != null) ? layer.opacity : 1.0
                });
            },
            "Markers": function(layer) {
                var features = [];
                for (var i=0, len=layer.markers.length; i<len; i++) {
                    var marker = layer.markers[i];
                    var geometry = new OpenLayers.Geometry.Point(marker.lonlat.lon, marker.lonlat.lat);
                    var style = {externalGraphic: marker.icon.url,
                        graphicWidth: marker.icon.size.w, graphicHeight: marker.icon.size.h,
                        graphicXOffset: marker.icon.offset.x, graphicYOffset: marker.icon.offset.y};
                    var feature = new OpenLayers.Feature.Vector(geometry, {}, style);
                    features.push(feature);
            }
                var vector = new OpenLayers.Layer.Vector(layer.name);
                vector.addFeatures(features);
                var output = encoders.layers.Vector.call(this, vector);
                vector.destroy();
                return output;
            }
        }
    }

    // Copied form extjs2: src/GeoExt/data/MapfishPrintProvider.js
    var encodeLayer = function(layer) {
        var encLayer;
        for(var c in encoders.layers) {
            if(OpenLayers.Layer[c] && (layer instanceof OpenLayers.Layer[c])) {
                encLayer = encoders.layers[c](layer);
                break;
            }
        }
        // only return the encLayer object when we have a type. Prevents a
        // fallback on base encoders like HTTPRequest.
        if (!(encLayer && encLayer.type)) {
            encLayer = null;
            if (console) {
                console.log("Layertype "+layer.CLASS_NAME+" is not supported for print. Ignoring this layer.");
            }
        }
        return encLayer;
    }

    var printcontrol = new OpenLayers.Control.Button({
            displayClass: "olPrintButton",
            title: OpenLayers.i18n("Print this map"),
            trigger: function() {
                var createData = Drupal.openlayers.getPrintData(map);

                createData.layers = [];

                // ensure that the baseLayer is the first one in the encoded list
                var layers = map.layers.concat();
                var idxBaseLayer = layers.indexOf(map.baseLayer);
                if (idxBaseLayer != -1) {
                    layers.splice(idxBaseLayer, idxBaseLayer);
                }
                layers.splice(0, 0, map.baseLayer);

                var addPrintLayer = function(aLayer) {
                    if (aLayer) {
                        if (aLayer.getVisibility() === true) {
                            // for example OpenLayers.Layer.Vector.RootContainer has sublayers
                            if (aLayer.layers && jQuery.isArray(aLayer.layers)) {
                                for(var li=0; li<aLayer.layers.length; li++) {
                                    addPrintLayer(aLayer.layers[li]);
                                }
                            } else {
                                var enc = encodeLayer(aLayer);
                                enc && createData.layers.push(enc);
                            }
                        }
                    }
                };
                for(var li=0; li<layers.length; li++) {
                    addPrintLayer(layers[li]);
                }

                jQuery(printcontrol.panel_div).addClass('olPrintButtonLoading');

                // send the create request to the server
                jQuery.ajax({
                    /* note: without the appended "_" to the URL bellow, the request will not get routed to the
                     * correct function */
                    url: Drupal.settings.basePath + '?q=admin/structure/openlayers/print/callbacks/create/_',
                    contentType: "application/json",
                    dataType: "json",
                    data: JSON.stringify(createData),
                    type: "POST",
                    error: function(xhr, textstatus) {
                        var data = JSON.parse(xhr.responseText);
                        if (data && data.error) {
                                alert(data.error)
                        }
                        else {
                            alert(textstatus);
                        }
                    },
                    success:  function(data) {
                        // open the produced print in a new window
                        var fullUrl = Drupal.settings.basePath + '?q=admin/structure/openlayers/print/callbacks/fetch/' + data.getURL;
                        window.open(fullUrl, '_blank');
                    },
                    complete: function() {
                        jQuery(printcontrol.panel_div).removeClass('olPrintButtonLoading');
                    }
                });
            }
    });
    var vpanel = new OpenLayers.Control.Panel({
        defaultControl: printcontrol,
        displayClass: "olPrintButtonPanel"
    });

    vpanel.addControls([printcontrol]);
    data.openlayers.addControl(vpanel);
    printcontrol.activate();
    vpanel.activate();
    vpanel.redraw();
})
