/*
//////////////////////////////////////////////////////////////////////////////////
///////Copyright © 2009 Bird Wing Productions, http://www.birdwingfx.com//////////
//////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

/////////////////////////////////////////////////////////////////////
*/


(function($) {
    
    $.GoogleMapObjectDefaults = {        
        zoomLevel: 10,
	imagewidth: 50,
	imageheight: 50,
	center: '2000 Florida Ave NW Washington, DC 20009-1231',
	start: '#start',
        end: '#end',
	directions: 'directions',
        submit: '#getdirections',
	tooltip: 'false',
	image: 'false'
    };

    function GoogleMapObject(elementId, options) {
        /* private variables */
        this._inited = false;
        this._map = null;
        this._geocoder = null;
	
        /* Public properties */
        this.ElementId = elementId;
        this.Settings = $.extend({}, $.GoogleMapObjectDefaults, options || '');
    }

    $.extend(GoogleMapObject.prototype, {
        init: function() {
            if (!this._inited) {
                if (GBrowserIsCompatible()) {
                    this._map = new GMap2(document.getElementById(this.ElementId));
                    this._map.addControl(new GSmallMapControl());
                    this._geocoder = new GClientGeocoder();
                }
		
                this._inited = true;
            }
        },
        load: function() {
            //ensure existence
            this.init();
	    
            if (this._geocoder) {
                //"this" will be in the wrong context for the callback
                var zoom = this.Settings.zoomLevel;
                var center = this.Settings.center;
		var width = this.Settings.imagewidth;
		var height = this.Settings.imageheight;
                var map = this._map;
		
		if (this.Settings.tooltip != 'false') {
		    var customtooltip = true;
		    var tooltipinfo = this.Settings.tooltip;
		}
		
		if (this.Settings.image != 'false') {
		    var customimage = true;
		    var imageurl = this.Settings.image;
		}
		
                this._geocoder.getLatLng(center, function(point) {
                    if (!point) { alert(center + " not found"); }
                    else {
                        //set center on the map
                        map.setCenter(point, zoom);
			
			if (customimage == true) {
			    //add the marker
			    var customiconsize = new GSize(width, height);
			    var customicon = new GIcon(G_DEFAULT_ICON, imageurl);
			    customicon.iconSize = customiconsize;
			    var marker = new GMarker(point, { icon: customicon });
			    map.addOverlay(marker);
			} else {
			    var marker = new GMarker(point);
			    map.addOverlay(marker);
			}
			
			if(customtooltip == true) {
			    marker.openInfoWindowHtml(tooltipinfo);
			}
                    }
                });
            }
	    
	    
            //make this available to the click element
            $.data($(this.Settings.submit)[0], 'inst', this);
	    
            $(this.Settings.submit).click(function(e) {
              $("#directions").html("");
                e.preventDefault();
                var obj = $.data(this, 'inst');
		var outputto = obj.Settings.directions;
                var from = $(obj.Settings.start).val();
                var to = $(obj.Settings.end).val();
		map.clearOverlays();
		var gdir = new GDirections(map, document.getElementById(outputto));
		gdir.load("from: " + from + " to: " + to);
		
                //open the google window
                //window.open("http://maps.google.com/maps?saddr=" + from + "&daddr=" + to, "GoogleWin", "menubar=1,resizable=1,scrollbars=1,width=750,height=500,left=10,top=10");
            });
	    
            return this;
        }
    });

    $.extend($.fn, {
        googleMap: function(options) {
            // check if a map was already created
            var mapInst = $.data(this[0], 'googleMap');
            if (mapInst) {
                return mapInst;
            }
	    
            //create a new map instance
            mapInst = new GoogleMapObject($(this).attr('id'), options);
            $.data(this[0], 'googleMap', mapInst);
            return mapInst;
        }
    });
})(jQuery);
