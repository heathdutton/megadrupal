// Advanced use of FeaturePopups
// =============================
// This code shows the customization possibilities of the control, creating the
//      control and adding layers with a lot of options.

// ** Allows pan the map and zoom box when the mouse is around a popup and zoom
//      with the scroll wheel when the mouse is in popup active area (see
//      `spopupSingleOptions`, `popupListItemOptions` and `popupListOptions`)
var framedCloudScrollable = OpenLayers.Class(OpenLayers.Popup.FramedCloud, {
    displayClass: 'olScrollable olPopup', // scroll wheel
    registerEvents:function() { // 
        this.events = new OpenLayers.Events(this, this.contentDiv, null, true);
        this.events.on({
            "mousedown": this.onmousedown,
            "mousemove": this.onmousemove,
            "mouseup": this.onmouseup,
            "click": this.onclick,
            "mouseout": this.onmouseout,
            "dblclick": this.ondblclick,
            "touchstart": function(evt) {
                OpenLayers.Event.stop(evt, true);
            },
            scope: this
        });
    }
});
// ** Alert instead of popup on `poisLayer` (see `spopupSingleOptions` and
//    `popupListItemOptions`)
var singleEventListeners = {
    'beforepopupdisplayed': function(e) {
        var sel = e.selection;
        // Use alert instead of a popup for poisLayer
        if (sel.layer === poisLayer) {
            alert(sel.feature.attributes.title);
            return false;
        }
    }
};

// Creating the control... with some layers...
var fpControl = new OpenLayers.Control.FeaturePopups({
    boxSelectionOptions: {},
    // ** Options for the SelectFeature control to select **
    selectOptions: {clickout: true},
    // ** Don't use close box on popups
    mode: OpenLayers.Control.FeaturePopups.DEFAULT &
          ~OpenLayers.Control.FeaturePopups.CLOSE_BOX,
    // ** Allow pan or zoom the map and to zoom with the scroll wheel when the
    //    mouse is in popup active area (see `framedCloudScrollable`)
    popupSingleOptions: {
        popupClass: framedCloudScrollable,
        // ** Alert instead of popup on `poisLayer`, see `singleEventListeners`
        eventListeners: singleEventListeners
    },
    popupListItemOptions: {
        popupClass: framedCloudScrollable,
        // ** Close the window when displaying the detail of an item **
        relatedToClear: ['list', 'single'],
        // ** Alert instead of popup on `poisLayer`, see `singleEventListeners`
        eventListeners: singleEventListeners
    },
    // ** Overwrites html of list popups, to show only first 10 items **
    popupListOptions: {
        popupClass: framedCloudScrollable, 
        eventListeners: {
            'beforepopupdisplayed': function(e) {
                var html = [];
                for (var i = 0, iLen = e.selection.length; i < iLen; i++) {
                    var htmlAux = [],
                        sel = e.selection[i],
                        layerObj = sel.layerObj;
                    for (var ii = 0, iiLen = sel.features.length; ii < iiLen;
                                                                             ii++) {
                        if (ii >= 10) { // only first 10 items.
                            htmlAux.push('<li>...and more</li>');
                            break;
                        }
                        htmlAux.push(
                            layerObj.applyTemplate.item(sel.features[ii])
                        );
                    }
                    html.push(layerObj.applyTemplate.list({
                            layer: sel.layer,
                            count: sel.features.length,
                            html: htmlAux.join('\n')
                        })
                    );
                }
                e.html = html.join('\n');
            }
        }
    },
    // ** Overwrites html of hover list popups to alternate color text **
    popupHoverListOptions: {eventListeners: {
        'beforepopupdisplayed': function(e) {
            var htmlAux = [],
                sel = e.selection,
                layerObj = sel.layerObj;
            for (var ii = 0, iiLen = sel.features.length; ii < iiLen; ii++) {
                if (ii % 2) {
                    htmlAux.push(
                        '<span style="color:#777">' +
                        layerObj.applyTemplate.hoverItem(sel.features[ii]) +
                        '</span>'
                    );
                } else {
                    htmlAux.push(
                        layerObj.applyTemplate.hoverItem(sel.features[ii])
                    );
                }
            }
            e.html = layerObj.applyTemplate.hoverList({
                layer: sel.layer,
                count: sel.features.length,
                html: htmlAux.join('\n')
            });
        }
    }},
    layers: [
        [
        // Uses: Templates for hover & select and safe selection
        sundialsLayer, {templates: {
            // hover: single & list
            hover: '${.name}',
            hoverList: '<b>${count}</b><br>${html}',
            hoverItem: '${.name}<br>',
            // select: single & list
            single: '<div><h2>${.name}</h2>${.description}</div>',
            item: '<li><a href="#" ${showPopup()}>${.name}</a></li>'
        }}], [
        // Uses: Internationalized templates.
        sprintersLayer, {templates: {
            hover: '${.Name}',
            single: '${i18n("Name")}: ${.Name}<br>' +
                 '${i18n("Country")}: ${.Country}<br>' +
                 '${i18n("City")}: ${.City}<br>',
            item: '<li><a href="#" ${showPopup()}>${.Name}</a></li>'
        }}], [
        // Uses: Templates as functions (only from hover-single and select-list)
        tasmaniaRoadsLayer, {templates: {
            hover: function(feature) {
                return 'Length: ' +
                    Math.round(feature.geometry.getLength() / 10) / 100 +
                    ' km';
            },
            item: function(feature) {
                return '<li>' +
                    Math.round(feature.geometry.getLength() / 10) / 100 +
                    ' km</li>';
            }
        }}]
    ]
});
map.addControl(fpControl);

// Add a layer to the control using addLayer
// -----------------------------------------
fpControl.addLayer(
    // poisLayer uses "fid", so SAFE_SELECTION uses "fid" instead of "id"
    poisLayer,
    {templates: {
        hover: '${.title}',
        single: ' ', // Need to declare a template to use `singleEventListeners`.
        item: '<li><a href="#" ${showPopup()}>${.title}</a></li>'
    }}
);

