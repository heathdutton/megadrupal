/* 
 * This file is based on :  
 * jQuery googleMap Copyright Dylan Verheul <dylan@dyve.net>
 * Licensed like jQuery, see http://docs.jquery.com/License
 * 
 * Some initial modifications made by Peter Brownell for Code Positive and
 * School of Everything. Additional modifications made by Jeremy Chinquist
 * for Google Maps Api v3
 *
 * We have made some modifications to the original file, but not that many. 
 * There is a good amount of work to do to tie this into Dupal and make
 * it configurable and themeable. 
 * 
 * For now, unless you want to patch the source, there is not all that
 * much to can do to change things. 
 * 
 * Ideas and plans welcome. 
 */
(function($){
  Drupal.behaviors.geomap = {
    attach: function (context, settings) {
      $("#map").googleMap(null,null, 13, { markers: $(".geo"),controls: ["GSmallZoomControl"]});
    }
  }

  $.googleMap = {
    maps: {},
    marker: function(m) {
      if (!m) {
        return null;
      } 
      else if (m.lat == null && m.lng == null) {
        return $.googleMap.marker($.googleMap.readFromGeo(m));
      } 
      else {
    	var marker = null;
    	var infoWindowOptions = null;
        var tinyIcon = new google.maps.MarkerImage(); 
        var tinyIconShadow = null; 
        var point = null; 

        // use custom icon if set
        if (m.image != 'null') {
          tinyIcon.url = m.image;
          
          if(m.iconwidth && m.iconheight){
            tinyIcon.size = new google.maps.Size(m.iconwidth, m.iconheight);
            
            if(m.iconanchor_x && m.iconanchor_y){
  	        tinyIcon.anchor = new google.maps.Point(m.iconanchor_x, m.iconanchor_y);
            }
          }
        
          //repeat for shadow
          if(m.shadow != 'null'){
            tinyIconShadow = new google.maps.MarkerImage(); 
            tinyIconShadow.url = m.shadow;
          
            if(m.iconwidth && m.iconheight){
              tinyIconShadow.size = new google.maps.Size(m.iconwidth, m.iconheight);
              
              if(m.iconanchor_x && m.iconanchor_y){
              	tinyIconShadow.anchor = new google.maps.Point(m.iconanchor_x, m.iconanchor_y);
              }
            }
          }
        }
      
        markerOptions = { icon:tinyIcon, draggable:false, clickable:true, icon:tinyIcon, shadow:tinyIconShadow };
      
        marker = new google.maps.Marker(markerOptions);
        marker.setPosition(new google.maps.LatLng(m.lat, m.lng));
        if (m.bind) {
          marker.infowindow = new google.maps.InfoWindow({content: '<div class="geomap-infowindow">'+$(m.bind).html()+'</div>'});
        } 
        else if (m.txt) {
        	marker.infowindow = new google.maps.InfoWindow({content: m.txt});
        }
        if (m.link) {
        	marker.infowindow = new google.maps.InfoWindow({content: m.link});
        }
        // set plain title for clusters
        marker.titleplain=m.titleplain;
        return marker;
      }
    },
    readFromGeo: function(elem) {
	
      /*
       * The following section maps the geo microformat to JS variables to use in
       * this object's googleMap function
       */
      var latElem = $(".latitude", elem)[0];
      var lngElem = $(".longitude", elem)[0];
      var markerElem = null;
      var infowindowElem = null;
      var infowindowtextElem = null;
      var iconElem = null;
      var iconoptions = new Array();
      var nodeElem = $(".node", elem)[0];
      var titlehtml = null;
      var titleplain = null;
      var windowlink = null;
    
      if($(".marker", elem)[0]){
	    markerElem = $(".marker", elem)[0];
      }
     
      if (latElem && lngElem) {
        if(nodeElem){
	      if ( $(nodeElem).attr("nid") > 0 ) {
  	        windowlink = $(nodeElem).attr("link");
          }
        }
      
        if(markerElem){
          // marker elements have 2 parts - infowindow and icon
	      if($(".icon", markerElem)[0]){
            iconElem = $(".icon", markerElem)[0];
		  
            // read all options: we can only support the following
            $(".option", iconElem).each(function(index) {
              //  there must be a more dynamic way to do this!
	          iconoptions[$(this).attr("name")] = $(this).attr("value");
	        });
	      }
	      
	      if($(".infowindow", markerElem)[0]){
            infowindowElem = $(".infowindow", markerElem)[0];
		    if($(".infowindow-text", infowindowElem)[0]){
              infowindowtextElem = $(".infowindow-text", infowindowElem)[0];
              if(infowindowtextElem){
                titleplain = $(infowindowtextElem).html();
                titlehtml = titleplain;  //@todo
                if(windowlink){ 
                  titlehtml = '<a href="'+windowlink+'">'+titlehtml+'</a>'; 
                }
              }
            }
          }
        }
        
        console.log('Text: '+ titlehtml);
      
        return { lat:parseFloat($(latElem).attr("title")), lng:parseFloat($(lngElem).attr("title")), 
               txt:titlehtml, link: windowlink, /* bind: $(elem).attr('bind'), */ 
               image: $(iconElem).attr('src'), iconwidth: iconoptions["iconwidth"], iconheight: iconoptions["iconheight"], iconanchor_x: iconoptions["iconanchor_x"], iconanchor_y: iconoptions["iconanchor_y"], shadow: iconoptions["shadow"],
               cckfield: $(elem).attr("cckfield"), cckfieldindex: $(elem).attr("cckfieldindex"), nid: $(nodeElem).attr("nid"),
               titleplain: titleplain
             }
    } else {
      return null;
    }
  },
  mapNum: 1
};

 
$.fn.googleMap = function(lat, lng, zoom, options) {
  // Default values make for easy debugging
  if (lat == null) lat = 51.52177;
  if (lng == null) lng = -0.20101;
  if (!zoom) zoom = 1;
  // Sanitize options
  if (!options || typeof options != 'object') options = {};
  options.mapOptions = options.mapOptions || {mapTypeId: google.maps.MapTypeId.ROADMAP};
  options.markers = options.markers || [];
  options.controls = options.controls || {};

  // Map all our elements
  return this.each(function() {
	$('#map-loader-image').fadeIn('slow');
	
    // Make sure we have a valid id
    if (!this.id) {
      this.id = "gMap" + $.googleMap.mapNum++;
    }
    // find our markers
    var marker = new Array(); //an array serves the purpose to receive both marker and window objects
    var markers = new Array();
    var bounds = null;
    for (var i = 0; i < options.markers.length; i++) {
      if (marker = $.googleMap.marker(options.markers[i])) {
        markers[i] = marker;
        console.log(markers[i]);
        if (!bounds) { 
          bounds = new google.maps.LatLngBounds(markers[i].getPosition(), markers[i].getPosition());
        } 
        else {
          // extend bounds
          bounds.extend(markers[i].getPosition());
        }
      }
    }
    // we only want a map if we have markers
    if (markers.length) {
      // Create a map and a shortcut to it at the same time
	  $(this).addClass('googlemap-display');
      var map = $.googleMap.maps[this.id] = new google.maps.Map(this, options.mapOptions); 
      
      // Center and zoom the map
      map.setCenter(new google.maps.LatLng(lat, lng))
      map.setZoom(zoom);
      
      // Add controls to our map
      map.controls = new google.maps.MVCArray(options.controls); 
      
      for (var i = 0; i < markers.length; i++) {
    	// Add markers to our map
    	markers[i].setMap(map);
    	
    	// Add info windows to our markers
    	google.maps.event.addListener(markers[i], "click", function(){
          this.infowindow.open(this.map, this);
    	});
      }

      // time to zoom the map
      var distance = 0.015;
      if ( markers.length == 1 ) {
        // if we only have one marker, we move out a bit more
        distance = 0.5;
      }
      
      // Moving the map to show our markers
      // We have to set the centre to get the bounds properly
      map.setCenter(bounds.getCenter());
      map.fitBounds(bounds);
      var southWest=bounds.getSouthWest();
      var northEast=bounds.getNorthEast();
      bounds.extend(new google.maps.LatLng(southWest.lat() - distance, southWest.lng() - distance ));
      bounds.extend(new google.maps.LatLng(northEast.lat() + distance, northEast.lng() + distance ));                 
      map.setCenter(bounds.getCenter());  
      map.fitBounds(bounds);  
    }
    
	$('#map-loader-image').fadeOut('slow');
  });
};
})(jQuery);