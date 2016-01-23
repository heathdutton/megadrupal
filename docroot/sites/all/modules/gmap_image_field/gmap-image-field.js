/**
 * Implements Drupal.behaviors
 */
Drupal.behaviors.gmap_image_field = {
  attach: function (context, settings) {
    if (settings.gmapImageField) {
      for (map_id in settings.gmapImageField) {
        if (settings.gmapImageField[map_id] && !settings.gmapImageField[map_id]['o']) {
          settings.gmapImageField[map_id].o = gmapImageField_createMap(settings.gmapImageField[map_id]);
        }
      }
    }
  }
};

/**
 * Modal Info Box class
 * An replacement for Google's infowindows
 * 
 * @param options
 */
function ModalInfoBox(options) {

  this.options = options;

  if (!options['boxClass']) {
    this.options.boxClass = 'infoWindow';
  }

  if (!options['closeBoxURL']) {
    this.options.closeBoxURL = 'http://www.google.com/intl/en_us/mapfiles/close.gif';
  }

  this._w = 0;
  this._h = 0;
  this._timer = null;

  this.resize = function(map) {
    if (!(jQuery(this.box).width() == this._w && jQuery(this.box).height() == this._h)) {
      this._w = jQuery(this.box).width();
      this._h = jQuery(this.box).height();
      var left = parseInt((jQuery(map.getDiv()).width() - jQuery(this.box).width()) / 2);
      if (jQuery(this.box).height() < jQuery(map.getDiv()).height()) {
        var top = parseInt((jQuery(map.getDiv()).height() - jQuery(this.box).height()) / 2);
      }
      else {
        var top = 0;
      }
      jQuery(this.box).css({ left: left, top: top });
    }
  };

  this.open = function(map, marker) {
    if (!this.options['disableAutoPan']) {
      map.setCenter(marker.getPosition());
    }
    var self = this;
    this._timer = setInterval(function() { self.resize(map); }, 150);
    jQuery(this.overlay)
      .appendTo(map.getDiv())
      .show(0, function() {
        self.resize(map);
        google.maps.event.trigger(self, 'domready');
      })
      .click(function(event) {
        if (self.options['overlayClickClose'] && jQuery(event.target).hasClass('infoWindow-overlay')) {
          self.close();
        }
      });
  };

  this.close = function() {
    clearInterval(this._timer);
    jQuery(this.overlay).hide();
  };

  this.setContent = function(content) {
    var closeButton = jQuery('<img src="' + this.options.closeBoxURL + '" align="right" style="cursor:pointer;" />');

    var self = this;
    jQuery(closeButton)
      .click(function() {
        self.close();
      });

    jQuery(this.box).empty().append(closeButton).append(content);
    jQuery(closeButton).wrap('<p>');
  };

  this.overlay = jQuery('<div class="' + this.options.boxClass + '-overlay"></div>')
    .css({ position: 'absolute', display: 'none', top: '0', left: '0', width: '100%', height: '100%' });

  this.box = jQuery('<div class="' + this.options.boxClass + '"></div>')
    .css({ position: 'absolute' }).appendTo(this.overlay);

  if (!this.options['content']) {
    this.setContent(this.options['content']);
  }

}

/**
 * Custom overlays
 */
gmapImageField_customOverlay.prototype = new google.maps.OverlayView();
function gmapImageField_customOverlay(pos, content, map) {
  this.pos_ = pos;
  this.content_ = content;
  this.map_ = map;
  this.div_ = document.createElement('div');
  this.div_.style.position = 'absolute';
  this.div_.className = 'gmap-image-field-custom-overlay';
  this.setMap(map);
}
gmapImageField_customOverlay.prototype.onAdd = function() {
  this.div_.innerHTML = this.content_;
  this.getPanes().overlayMouseTarget.appendChild(this.div_);
};
gmapImageField_customOverlay.prototype.onRemove = function() {
  console.log(this.div_);
  this.div_.parentNode.removeChild(this.div_);
};
gmapImageField_customOverlay.prototype.draw = function() {
  c = this.getProjection().fromLatLngToDivPixel(this.pos_);
  this.div_.style.left = c.x + 'px';
  this.div_.style.top = c.y + 'px';
};

/**
 * GallPeters project calculation class.
 * 
 * @param px
 * @param py
 */
function GallPetersProjection(px, py) {

  this.px = px;
  this.py = py;

  this.worldOrigin_ = new google.maps.Point(this.px * 400 / 800, this.py / 2);
  this.worldCoordinatePerLonDegree_ = this.px / 360;
  this.worldCoordinateLatRange = this.py / 2;

  this.fromLatLngToPoint = function(latLng) {
    var origin = this.worldOrigin_;
    var x = origin.x + this.worldCoordinatePerLonDegree_ * latLng.lng();
    var latRadians = latLng.lat() * (Math.PI / 180);
    var y = origin.y - this.worldCoordinateLatRange * Math.sin(latRadians);
    return new google.maps.Point(x, y);
  };

  this.fromPointToLatLng = function(point, noWrap) {
    var y = point.y;
    var x = point.x;
    if (y < 0) {
      y = 0;
    }
    if (y >= this.py) {
      y = this.py;
    }
    var origin = this.worldOrigin_;
    var lng = (x - origin.x) / this.worldCoordinatePerLonDegree_;
    var latRadians = Math.asin((origin.y - y) / this.worldCoordinateLatRange);
    var lat = latRadians / (Math.PI / 180);
    return new google.maps.LatLng(lat, lng, noWrap);
  };
}

/**
 * Function that contain FavSpot class instructions.
 *
 * @param settings
 *
 * @return object
 */
function gmapImageField_createMap(settings) {

  if (settings && settings['id']) {
    var container = document.getElementById(settings.id);
    if (container) {
      jQuery(container)
        .addClass('gmap-image-field-map-placeholder')
        .width(settings.map_width)
        .height(settings.map_height);
    }
  }
  else {
    return null;
  }

  this._markThePlace_place = null;

  /**
   * Calculation to move map inbounds.
   */
  this._bndLockedRepan = function() {
    var nx = parseFloat(this.getCenter().lng().toFixed(1));
    var ny = parseFloat(this.getCenter().lat().toFixed(1));
    if (nx == 0 && ny == 0) {
      return;
    }
    var q3x = parseFloat(this.getBounds().getSouthWest().lng().toFixed(1));
    var q3y = parseFloat(this.getBounds().getSouthWest().lat().toFixed(1));
    var q1x = parseFloat(this.getBounds().getNorthEast().lng().toFixed(1));
    var q1y = parseFloat(this.getBounds().getNorthEast().lat().toFixed(1));

    var needPan = false;
    if (!settings.repeat_map && nx !== 0) {
      if (q1x == 180 && q3x == -180) {
        nx = 0;
        needPan = true;
      }
      else if (q3x > q1x) {
        if (nx < 0) {
          nx = nx - 180 - q3x;
          needPan = true;
        }
        else {
          nx = nx - (180 + q1x);
          needPan = true;
        }

      }
    }

    // Becausee Gall Peter's projection is 180/90, we should use 90 as latitude range.
    if (ny !== 0) {
      if (q1y > 89 && q3y < - 89) {
        ny = 0;
        needPan = true;
      }
      else if (q1y > 89) {
        ny = ny + 89 - q1y;
        needPan = true;
      }
      else if (q3y < -89) {
        ny = ny + (- 89 - q3y);
        needPan = true;
      }
    }
    if (needPan) {
      this.setCenter(new google.maps.LatLng(ny, nx));
    }
  };

  /**
   * Add markers from the feed.
   */
  this._addMarkers = function(pins) {
    var self = this;

    if (settings['pin_popup_style'] == 'modal') {
      var infowindow = new ModalInfoBox({
        content: '',
        disableAutoPan: false,
        boxClass: 'infoWindow',
        overlayClickClose: true
      });
      google.maps.event.addListener(infowindow, 'domready', function(event) {
        Drupal.attachBehaviors(jQuery('.infoWindow'));
      });
    }
    else {
      var infowindow = new google.maps.InfoWindow({
         content: '',
         disableAutoPan: false,
         maxWidth: 0,
         zIndex: null
       });
    }

    for (i in pins) {

      pinopts = pins[i];
      pinopts.icon = pins[i].icon ? pins[i].icon : null,
      pinopts.map = this.map;
      pinopts.position = new google.maps.LatLng(pins[i].lat, pins[i].lng);

      var marker = new google.maps.Marker(pinopts);

      google.maps.event.addListener(marker, 'click', function() {
        infowindow.close();
        if (this.description) {
          infowindow.setContent(this.description);
          infowindow.open(this.map, this);
        }
        google.maps.event.trigger(this.map, 'gmap_image_field_marker_clicked', pins[i]);
      });
    }

    google.maps.event.addListener(this.map, 'click', function () {
      infowindow.close();
    });

  };

  /**
   * Build location picker
   */
  this.markThePlace = function (markThePlace_inputToPopulate, icon) {

    if (this._markThePlace_place) {
      this._markThePlace_place.setMap(null);
    }

    if (markThePlace_inputToPopulate && document.getElementById(markThePlace_inputToPopulate)) {
      var init_latlng = jQuery.map(document.getElementById(markThePlace_inputToPopulate).value.split(','), jQuery.trim);
      if (isNaN(parseInt(init_latlng[0] * 1))) {
        init_latlng[0] = 0;
      }
      if (isNaN(parseInt(init_latlng[1] * 1))) {
        init_latlng[1] = 0;
      }
      init_latlng = new google.maps.LatLng(init_latlng[0], init_latlng[1]);
    }
    else {
      init_latlng = new google.maps.LatLng(0, 0);
    }

    var icon = settings._pin_icons[icon];

    this._markThePlace_place = new google.maps.Marker({
      icon: icon ? icon.url : null,
      map: this.map,
      draggable: true,
      title: Drupal.t('Pin the node here'),
      animation: google.maps.Animation.DROP,
      position: init_latlng
    });

    google.maps.event.addListener(this._markThePlace_place, 'position_changed', function() {
      if (markThePlace_inputToPopulate) {
        var latlng = this.getPosition();
        var e = document.getElementById(markThePlace_inputToPopulate);
        if (e) {
          e.value = latlng.lat() + ', ' + latlng.lng();
        }
      }
    });
  };

  /**
   * ImgMapType class constructor
   */
  this.ImgMapType = function () {

    this._backgroundColor = (
        settings.custom_background
        ? (settings.background
            ? settings.background
            : settings.default_background)
        : settings.default_background);

    this.tileSize = new google.maps.Size(settings.tile_size, (settings.tile_size_h ? settings.tile_size_h : settings.tile_size));
    this.minZoom = settings.minzoomlevel;
    this.maxZoom = settings.maxzoomlevel;
    this.isPng = (settings.tile_format == 'png');

    if (settings.tile_size_h && settings.tile_size !== settings.tile_size_h) {
      this.projection = new GallPetersProjection(settings.tile_size, settings.tile_size_h);
    }

    /**
     * Get tile for specific coordinates and zoomlevel.
     *
     * @param coord
     * @param zoom
     * @param ownerDocument
     *
     * @return {HTMLElement}
     */
    this.getTile = function(coord, zoom, ownerDocument) {
      var tilesCount = Math.pow(2, zoom);
      var x = coord.x;
      var y = coord.y;

      // Recalculate coord.x if we have to repeat the map.
      if (settings.repeat_map) {
        x = Math.abs(coord.x);
        x = x % tilesCount;
        if (coord.x < 0 && x > 0) {
          x = tilesCount - x;
        }
      }

      if (x >= tilesCount || x < 0 || y >= tilesCount || y < 0) {
        // Empty quadrant.
        var div = ownerDocument.createElement('div');
        div.style.width = this.tileSize.width + 'px';
        div.style.height = this.tileSize.height + 'px';
        div.style.backgroundColor = this._backgroundColor;
        return div;
      }
      else {
        // Quadrant with tile.
        var img = ownerDocument.createElement('img');
        img.width = this.tileSize.width;
        img.height = this.tileSize.height;
        img.style.backgroundColor = this._backgroundColor;
        img.src = settings.tile_base + '/' + zoom + '/' + x + '_' + y + '.' + settings.tile_format;
        return img;
      }
    };
  };

  /**
   * Custom controls.
   */
  this._createControls = function() {

    var self = this;
    this.currentMapType = this.map.mapTypes.get(this.map.getMapTypeId());
    this.levelsCount = currentMapType.maxZoom - currentMapType.minZoom;

    this.bindZLevent = function(element, level) {
      google.maps.event.addDomListener(element, 'click', function() {
        self.map.setZoom(level);
      });
    };

    // PanTop button.
    var panButtonContainer = document.createElement('div');
    panButtonContainer.setAttribute('class', 'gmap-image-field-controlGroup-pan');

    var top = document.createElement('div');
    top.setAttribute('class', 'gmap-image-field-control gmap-image-field-control-panTop');
    google.maps.event.addDomListener(top, 'click', function () {
      self.map.panBy(0, -100);
    });
    panButtonContainer.appendChild(top);

    var bottom = document.createElement('div');
    bottom.setAttribute('class', 'gmap-image-field-control gmap-image-field-control-panBottom');
    google.maps.event.addDomListener(bottom, 'click', function () {
      self.map.panBy(0, 100);
    });
    panButtonContainer.appendChild(bottom);

    var left = document.createElement('div');
    left.setAttribute('class', 'gmap-image-field-control gmap-image-field-control-panLeft');
    google.maps.event.addDomListener(left, 'click', function () {
      self.map.panBy(-100, 0);
    });
    panButtonContainer.appendChild(left);

    var right = document.createElement('div');
    right.setAttribute('class', 'gmap-image-field-control gmap-image-field-control-panRight');
    google.maps.event.addDomListener(right, 'click', function () {
      self.map.panBy(100, 0);
    });
    panButtonContainer.appendChild(right);

    // Zoomlevel controls.
    var zoomControls = document.createElement('div');
    zoomControls.setAttribute('class', 'gmap-image-field-controlGroup-zoom');

    var zoomControlButtonPlus = document.createElement('div');
    zoomControlButtonPlus.setAttribute('class', 'gmap-image-field-control gmap-image-field-control-zoomControl gmap-image-field-control-zoomControl-plus');
    google.maps.event.addDomListener(zoomControlButtonPlus, 'click', function () {
      self.map.setZoom(self.map.getZoom() + 1);
    });

    var zoomControlButtonMinus = document.createElement('div');
    zoomControlButtonMinus.setAttribute('class', 'gmap-image-field-control gmap-image-field-control-zoomControl gmap-image-field-control-zoomControl-minus');
    google.maps.event.addDomListener(zoomControlButtonMinus, 'click', function () {
      self.map.setZoom(self.map.getZoom() - 1);
    });

    var zoomControlLevels = document.createElement('div');
    zoomControlLevels.setAttribute('class', 'gmap-image-field-controlGroup-zoomLevels');

    for (var i = 0; i <= this.levelsCount; i++) {
      var element = document.createElement('div');
      this.bindZLevent(element, levelsCount - i);
      zoomControlLevels.appendChild(element);
    }

    // Attach zoom changed event.
    google.maps.event.addListener(self.map, 'zoom_changed', function() {
      var m = this.mapTypes[this.getMapTypeId()];
      for (i in zoomControlLevels.childNodes) {
        var classes = 'gmap-image-field-control gmap-image-field-control-zoomLevel gmap-image-field-control-zoomLevel-' + (m.maxZoom - i);
        classes += (m.maxZoom - i == this.getZoom() ? ' gmap-image-field-control-zoomLevel-current' : '');
        zoomControlLevels.childNodes[i].className = classes;
      }
    });

    zoomControls.appendChild(zoomControlButtonPlus);
    zoomControls.appendChild(zoomControlLevels);
    zoomControls.appendChild(zoomControlButtonMinus);
    google.maps.event.trigger(self.map, 'zoom_changed');

    // Attach controllers to the map.
    this.map.controls[google.maps.ControlPosition.LEFT_CENTER].push(panButtonContainer);
    this.map.controls[google.maps.ControlPosition.LEFT_CENTER].push(zoomControls);
  };

  /**
   * Constructor logic starts here.
   */

  var center = jQuery.map(jQuery.map(settings.center.split(','), jQuery.trim), parseFloat);

  // Initialize the map.
  this.map = new google.maps.Map(container, {
    zoom: settings.zoom,
    center: new google.maps.LatLng((center[0] ? center[0] : 0), (center[1] ? center[1] : 0)),
    disableDefaultUI: settings.comlete_disable_controls ? true : settings.custom_controls,
    panControl: settings.comlete_disable_controls ? false : !settings.custom_controls,
    zoomControl: settings.comlete_disable_controls ? false : !settings.custom_controls,
    zoomControlOptions: {
      style: google.maps.ZoomControlStyle.LARGE
    },
    scaleControl: false,
    streetViewControl: false,
    overviewMapControl: (settings.comlete_disable_controls ? false : settings.map_overview),
    mapTypeControl: false,
    mapMaker: true
  });

  this.map.mapTypes.set(settings.id, (new this.ImgMapType()));
  this.map.setMapTypeId(settings.id);

  if (!settings.comlete_disable_controls && settings.custom_controls) {
    this._createControls();
  }

  // Lock bounds.
  if (settings.bounds_lock) {
    google.maps.event.addListener(this.map, 'center_changed', this._bndLockedRepan);
    google.maps.event.addListener(this.map, 'idle', this._bndLockedRepan);
  }

  if (settings.pins) {
    this._addMarkers(settings.pins);
  }

  return this;
}
