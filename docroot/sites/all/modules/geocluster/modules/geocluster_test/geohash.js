(function ($) {

  // geohash.js
// Geohash library for Javascript
// (c) 2008 David Troy
// Distributed under the MIT License

  BITS = [16, 8, 4, 2, 1];

  BASE32 = 											   "0123456789bcdefghjkmnpqrstuvwxyz";
  NEIGHBORS = { right  : { even :  "bc01fg45238967deuvhjyznpkmstqrwx" },
    left   : { even :  "238967debc01fg45kmstqrwxuvhjyznp" },
    top    : { even :  "p0r21436x8zb9dcf5h7kjnmqesgutwvy" },
    bottom : { even :  "14365h7k9dcfesgujnmqp0r2twvyx8zb" } };
  BORDERS   = { right  : { even : "bcfguvyz" },
    left   : { even : "0145hjnp" },
    top    : { even : "prxz" },
    bottom : { even : "028b" } };

  NEIGHBORS.bottom.odd = NEIGHBORS.left.even;
  NEIGHBORS.top.odd = NEIGHBORS.right.even;
  NEIGHBORS.left.odd = NEIGHBORS.bottom.even;
  NEIGHBORS.right.odd = NEIGHBORS.top.even;

  BORDERS.bottom.odd = BORDERS.left.even;
  BORDERS.top.odd = BORDERS.right.even;
  BORDERS.left.odd = BORDERS.bottom.even;
  BORDERS.right.odd = BORDERS.top.even;

  function refine_interval(interval, cd, mask) {
    if (cd&mask)
      interval[0] = (interval[0] + interval[1])/2;
    else
      interval[1] = (interval[0] + interval[1])/2;
  }

  function calculateAdjacent(srcHash, dir) {
    srcHash = srcHash.toLowerCase();
    var lastChr = srcHash.charAt(srcHash.length-1);
    var type = (srcHash.length % 2) ? 'odd' : 'even';
    var base = srcHash.substring(0,srcHash.length-1);
    if (BORDERS[dir][type].indexOf(lastChr)!=-1)
      base = calculateAdjacent(base, dir);
    return base + BASE32[NEIGHBORS[dir][type].indexOf(lastChr)];
  }

  function decodeGeoHash(geohash) {
    var is_even = 1;
    var lat = []; var lon = [];
    lat[0] = -90.0;  lat[1] = 90.0;
    lon[0] = -180.0; lon[1] = 180.0;
    lat_err = 90.0;  lon_err = 180.0;

    for (i=0; i<geohash.length; i++) {
      c = geohash[i];
      cd = BASE32.indexOf(c);
      for (j=0; j<5; j++) {
        mask = BITS[j];
        if (is_even) {
          lon_err /= 2;
          refine_interval(lon, cd, mask);
        } else {
          lat_err /= 2;
          refine_interval(lat, cd, mask);
        }
        is_even = !is_even;
      }
    }
    lat[2] = (lat[0] + lat[1])/2;
    lon[2] = (lon[0] + lon[1])/2;

    return { latitude: lat, longitude: lon};
  }

  function encodeGeoHash(latitude, longitude) {
    var is_even=1;
    var i=0;
    var lat = []; var lon = [];
    var bit=0;
    var ch=0;
    var precision = 12;
    geohash = "";

    lat[0] = -90.0;  lat[1] = 90.0;
    lon[0] = -180.0; lon[1] = 180.0;

    while (geohash.length < precision) {
      if (is_even) {
        mid = (lon[0] + lon[1]) / 2;
        if (longitude > mid) {
          ch |= BITS[bit];
          lon[0] = mid;
        } else
          lon[1] = mid;
      } else {
        mid = (lat[0] + lat[1]) / 2;
        if (latitude > mid) {
          ch |= BITS[bit];
          lat[0] = mid;
        } else
          lat[1] = mid;
      }

      is_even = !is_even;
      if (bit < 4)
        bit++;
      else {
        geohash += BASE32[ch];
        bit = 0;
        ch = 0;
      }
    }
    return geohash;
  }


    /**
   * based on https://github.com/jywarren/cartagen/blob/master/src/mapping/geohash.js
   * @namespace Contains methods and variables for spacially indexing features using geohashes.
   */
  Geohash = {

    /**
     * Generates a geohash.
     * @param {Number} lat    Latitude to hash
     * @param {Number} lon    Longitude to hash
     * @param {Number} length Length of hash
     * @return The generated geohash, truncated to the specified length
     * @type String
     */
    get_key: function(lat,lon,length) {
      if (!length) length = this.default_length
      if (length < 1) length = 1

      return encodeGeoHash(lat,lon).substring(0, length)
    },
    /**
     * Fetch features in a geohash
     * @param {Number} lat    Latitude of geohash
     * @param {Number} lon    Longitude of geohash
     * @param {Number} length Geohash length
     * @return Features in the same geohash as the specified location
     * @type Feature[]
     * @see Geohash.get_from_key
     * @see Geohash.get_upward
     */
    get: function(lat,lon,length) {
      if (!length) length = this.default_length

      var key = this.get_key(lat,lon,length)
      return this.hash.get(key)
    },

    /**
     * Gets the eights neighboring keys of the specified key, including diagonal neighbors.
     * @param {String} key Central geohash
     * @return Array of neighbors, starting from the key directly above the central key and
     *         proceeding clockwise.
     * @type String[]
     *
     */
    get_all_neighbor_keys: function(key) {
      var top = calculateAdjacent(key, 'top')
      var bottom = calculateAdjacent(key, 'bottom')
      var left = calculateAdjacent(key, 'left')
      var right = calculateAdjacent(key, 'right')
      var top_left = calculateAdjacent(top, 'left')
      var top_right = calculateAdjacent(top, 'right')
      var bottom_left = calculateAdjacent(bottom, 'left')
      var bottom_right = calculateAdjacent(bottom, 'right')
      return [top, top_right, right, bottom_right, bottom, bottom_left, left, top_left]
    },
    /**
     * Fetch adjacent geohashes
     * @param {String} key Central geohash
     * @return Array of neighbors
     * @type Feature[]
     */
    get_neighbors: function(key) {
      var neighbors = []

      this._dirs.each(function(dir) {
        var n_key = calculateAdjacent(key, dir)
        var n_array = this.get_from_key(n_key)
        if (n_array) neighbors = neighbors.concat(n_array)
      }, this)

      return neighbors
    },

    in_range: function(v,r1,r2) {
      return (v > Math.min(r1,r2) && v < Math.max(r1,r2));
    },


    /**
     *  Given a geohash key, recurses outwards to neighbors while still within the viewport
     *  @param {String}                   key  Central geohash
     *  @param {Hash (String -> Boolean)} keys Hash of keys and whether they have been included in
     *                                         search
     **/
    fill_bbox: function(key,keys, map_bbox) {
      // we may be able to improve efficiency by only checking certain directions
      neighbors = this.get_all_neighbor_keys(key);
      $.each(neighbors, function() {
        var k = this;
        if (!(k in keys)) {
          keys[k] = true;

          // if still inside viewport:
          var bbox = decodeGeoHash(k) //[lon1, lat2, lon2, lat1]
          if (Geohash.in_range(bbox.latitude[0],map_bbox[3],map_bbox[1]) &&
            Geohash.in_range(bbox.latitude[1],map_bbox[3],map_bbox[1]) &&
            Geohash.in_range(bbox.longitude[0],map_bbox[0],map_bbox[2]) &&
            Geohash.in_range(bbox.longitude[1],map_bbox[0],map_bbox[2])) {
            Geohash.fill_bbox(k,keys, map_bbox);
          }
        }
      }, this)
    },

    /**
     * Returns the bounding box of a geohash
     * @param {String} geohash Geohash to get bounding box of
     * @return Bounding box of geohash, in [lon_1, lat_2, lon_ 2, lat_1] format
     * @type Number[]
     */
    bbox: function(geohash) {
      var geo = decodeGeoHash(geohash)
      return [geo.longitude[0],geo.latitude[1],geo.longitude[1],geo.latitude[0],geohash]
    },
    /**
     * Draws the bounding box of a geohash
     * @param {String} key Geohash to draw bounding box of
     */
    draw_bbox: function(key) {
      var bbox = this.bbox(key)

      var line_width = 1/Map.zoom
      // line_width < 1
      $C.line_width(Math.max(line_width,1))
      $C.stroke_style(this.grid_color)

      var width = Projection.lon_to_x(bbox[2]) - Projection.lon_to_x(bbox[0])
      var height = Projection.lat_to_y(bbox[1]) - Projection.lat_to_y(bbox[3])

      $C.stroke_rect(Projection.lon_to_x(bbox[0]),
        Projection.lat_to_y(bbox[3]),
        width,
        height)
      $C.save()
      $C.translate(Projection.lon_to_x(bbox[0]),Projection.lat_to_y(bbox[3]))
      $C.fill_style(Object.value(this.fontBackground))
      var height = 16 / Map.zoom
      var width = $C.measure_text('Lucida Grande',
        height,
        key)
      var padding = 2
      // $C.fill_style('white')
      // $C.rect(-padding/2,
      // 		-(height + padding/2),
      // 		width + padding + 3/Map.zoom,
      //         height + padding - 3/Map.zoom)
      $C.draw_text('Lucida Grande',
        height,
        this.grid_color,
        3/Map.zoom,
        -3/Map.zoom,
        key)
      $C.restore()
    },
    draw_bboxes: function() {
      if (Geohash.grid) {
        this.keys.keys().each(function(key){
          Geohash.draw_bbox(key)
        })
      }
    },
    /**
     * Gets an appropriate key length for a ceratin size of feature
     * @param {Object} lat Width, in degrees of latitude, of feature
     * @param {Object} lon Height, in degrees of longitude, of feature
     * @return Appropriate length of key
     * @type Number
     */
    get_key_length: function(lat,lon) {
      if      (lon < 0.0000003357) lon_key = 12
      else if (lon < 0.000001341)  lon_key = 11
      else if (lon < 0.00001072)   lon_key = 10
      else if (lon < 0.00004291)   lon_key = 9
      else if (lon < 0.0003433)    lon_key = 8
      else if (lon < 0.001373)     lon_key = 7
      else if (lon < 0.01098)      lon_key = 6
      else if (lon < 0.04394)      lon_key = 5
      else if (lon < 0.3515)       lon_key = 4
      else if (lon < 1.406)        lon_key = 3
      else if (lon < 11.25)        lon_key = 2
      else if (lon < 45)           lon_key = 1
      else                         lon_key = 0 // eventually we can map the whole planet at once

      if      (lat < 0.0000001676) lat_key = 12
      else if (lat < 0.000001341)  lat_key = 11
      else if (lat < 0.000005364)  lat_key = 10
      else if (lat < 0.00004291)   lat_key = 9
      else if (lat < 0.0001716)    lat_key = 8
      else if (lat < 0.001373)     lat_key = 7
      else if (lat < 0.005493)     lat_key = 6
      else if (lat < 0.04394)      lat_key = 5
      else if (lat < 0.1757)       lat_key = 4
      else if (lat < 1.40625)      lat_key = 3
      else if (lat < 5.625)        lat_key = 2
      else if (lat < 45)           lat_key = 1
      else                         lat_key = 0 // eventually we can map the whole planet at once

      return Math.min(lat_key,lon_key)
    },
    /**
     * Generates Geohash.objects, populating it with the objects that
     * should be drawn this frame.
     * @return Geohash.objects, in reverse order
     * @type Feature[]
     * @see Geohash.objects
     */
    get_objects: function() {
      this.last_get_objects = [Map.x,Map.y,Map.zoom]
      this.objects = []

      // get geohash for each of the 4 corners,
      this.keys = new Hash

      this.key_length = this.get_key_length(0.0015/Map.zoom, 0.0015/Map.zoom)

      this.key = this.get_key(Map.lat, Map.lon, this.key_length)

      var bbox = decodeGeoHash(this.key) //[lon1, lat2, lon2, lat1]

      this.fill_bbox(this.key, this.keys)
      this.get_keys_upward(this.key)

      this.keys.keys().each(function(key, index) {
        this.get_keys_upward(key)
      }, this)

      //var quota = Geohash.feature_quota()


      // This should be re-added for 0.6 release

      // sort by key length

  //		var lengths = {}
  //		this.keys.keys().each(function(key) {
  //			if (!lengths[key.length]) lengths[key.length] = []
  //
  //			lengths[key.length].push(Geohash.get_from_key(key))
  //		})
  //
  //		for (i = 1; i <= this.key_length && quota > 0; ++i) {
  //			var features = lengths[i].flatten()
  //			if (quota >= features.length) {
  //				this.objects = this.objects.concat(features)
  //				quota -= features.length
  //			}
  //			else {
  //				j = 0
  //				while (quota > 0) {
  //					var o = lengths[i][j % (lengths[i].length)].shift()
  //					if (o) this.objects.push(o)
  //					++j
  //					--quota
  //				}
  //			}
  //		}
      var features;
      this.keys.keys().each(function(key) {
        features = this.get_from_key(key)
        this.object_hash.set(key, features)
        this.objects = features.concat(this.objects)
      }, this)

      this.sort_objects()

      //$l(this.objects.length)
      return this.objects
    },
    sort_objects: function() {
      this.objects.sort(Geometry.sort_by_area)
    },
    /**
     * Calculates the appropritate density of features based on the hardware' power (estimated by screen
     * resolution).
     * @return The density, in features per 1,000 square pixels.
     */
    feature_density: function() {
      return 2 * Viewport.power()
    },
    /**
     * Calculates the number of features that should be drawn.
     */
    feature_quota: function() {
      return ((Glop.width * Glop.height) * (Geohash.feature_density() / 1000)).round()
    },
    /**
     * Iterator for prototype.
     */
    _each: function(f) {
      this.hash.each(function(pair) {
        pair.value.each(function(val) { f(val) })
      })
    }
  }

})(jQuery);
