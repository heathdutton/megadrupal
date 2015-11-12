// Create control and add some layers
// ----------------------------------
var fpControl = new OpenLayers.Control.FeaturePopups({
    boxSelectionOptions: {},
    layers: [
        [
        // Uses: Templates for hover & select and safe selection
        sundialsLayer, {
            popupOptions: {
                list:{
                    // Uses an existing div having an id 'divList'
                    popupClass: 'divList'
                },
                single: null // Show a list instead of single popup if the list
                             //     has only an item.
            },
            templates: {
                // hover: single & list
                hover: '${.name}',
                hoverList: '<b>${count}</b><br>${html}',
                list: '${html}',
                hoverItem: '${.name}<br>',
                // select: single & list
                single: '<div><h2>${.name}</h2>${.description}</div>',
                item: '<li><a href="#" ${showPopup()}>${.name}</a></li>'
            }
        }], [
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
    // poisLayer uses "fid", so by default SAFE_SELECTION uses "fid" (if it
    //     exists) instead of "id".
    poisLayer,
    {templates: {
        hover: '${.title}',
        single: '<h2>${.title}</h2>${.description}',
        item: '<li><a href="#" ${showPopup()}>${.title}</a></li>'
    }}
);

