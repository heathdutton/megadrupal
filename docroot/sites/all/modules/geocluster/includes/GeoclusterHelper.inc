<?php

/**
 * @file
 * Geocluster extension for geofield and maps.
 */

class GeoclusterHelper {

  /**
   * Resolutions indexed by zoom levels.
   *
   * based on https://github.com/mapbox/clustr/blob/gh-pages/src/clustr.js#L4
   *
   * The resolutions are in meters / pixel, so the most common use is to divide
   * the distance between points by the resolution in order to determine the
   * number of pixels between the features.
   *
   * @return array An array of resolutions indexed by zoom levels.
   */
  static function resolutions() {
    // @todo: static, cache?
    $r = array();
    // Meters per pixel.
    // $maxResolution = 156543.03390625;
    $tile_size = 256;
    $maxResolution = GEOFIELD_KILOMETERS * 1000 / $tile_size;
    $maxResolution = 156.412 * 1000;
    for($zoom = 0; $zoom <= 30; ++$zoom) {
        $r[$zoom] = $maxResolution / pow(2, $zoom);
    }
    return $r;
  }

  /**
   * Dumb implementation to incorporate pixel variation with latitude
   * on the mercator projection.
   *
   * @todo: use a valid implementation instead of the current guessing.
   *
   * Current implementation is based on the observation:
   * lat = 0 => output is correct
   * lat = 48 => output is 223 pixels distance instead of 335 in reality.
   *
   * @param $lat
   * @return float the correction factor
   */
  static function pixel_correction($lat) {
    return 1 + (335.0 / 223.271875276 - 1) * ((float)(abs($lat)) / 47.9899);
  }

  /**
   * Calculate the distance between two given points in pixels.
   *
   * This depends on the resolution (zoom level) they are viewed in.
   *
   * @param $geometry
   * @param $otherGeometry
   * @param $resolution
   * @return float
   */
  static function distance_pixels($geometry, $otherGeometry, $resolution) {
    $distance = GeoclusterHelper::distance_haversine($geometry, $otherGeometry);
    $distance_pixels = $distance / $resolution * GeoclusterHelper::pixel_correction($geometry->getY());
    return $distance_pixels;
  }

  /**
   * Calculate the distance between two given points in meters.
   *
   * based on http://www.codecodex.com/wiki/Calculate_Distance_Between_Two_Points_on_a_Globe#PHP
   *
   * @static
   * @param $geometry
   * @param $otherGeometry
   * @return int
   */
  static function distance_haversine($geometry, $otherGeometry) {
    $long_1 = (float) $geometry->getX();
    $lat_1 = (float) $geometry->getY();
    $long_2 = (float) $otherGeometry->getX();
    $lat_2 = (float) $otherGeometry->getY();

    $earth_radius = GEOFIELD_KILOMETERS * 1000; // meters

    $dLat = deg2rad($lat_2 - $lat_1);
    $dLon = deg2rad($long_2 - $long_1);

    $a = sin($dLat/2) * sin($dLat/2) + cos(deg2rad($lat_1)) * cos(deg2rad($lat_2)) * sin($dLon/2) * sin($dLon/2);
    $c = 2 * asin(sqrt($a));
    $d = $earth_radius * $c;

    return $d;
  }

  /**
   * Convert from degrees to meters.
   *
   * based on
   * http://dev.openlayers.org/docs/files/OpenLayers/Layer/SphericalMercator-js.html
   * https://github.com/openlayers/openlayers/blob/master/lib/OpenLayers/Projection.js#L278
   *
   * @static
   * @param $lat
   * @param $lon
   * @return array
   */
  static function forwardMercator($lon, $lat) {
    $pole = 20037508.34;
    $x = $lon * $pole / 180;
    $y = log(tan((90 + $lat) * pi() / 360)) / pi() * $pole;
    return array('x' => $x, 'y' => $y);
  }

  /**
   * Convert from meters (Spherical Mercator) to degrees (EPSG:4326).
   *
   *  based on https://github.com/mapbox/clustr/blob/gh-pages/src/clustr.js
   *
   *  also see
   *  http://dev.openlayers.org/docs/files/OpenLayers/Layer/SphericalMercator-js.html
   *  https://github.com/openlayers/openlayers/blob/master/lib/OpenLayers/Projection.js#L278
   *
   * @static
   * @param $x
   * @param $y
   * @return array (lon, lat)
   */
  static function backwardMercator($x, $y) {
    $R2D = 180 / pi();
    $A = 6378137;
    return array(
      $x * $R2D / $A,
      ((pi() * 0.5) - 2.0 * atan(exp(-$y / $A))) * $R2D
    );
  }

  static function getCenter($geometries, $itemFactor = array()) {
    $lat = 0;
    $lon = 0;
    $len = count($geometries);
    $totalFactor = 0;
    for ($i = 0; $i < $len; $i++) {
      $geometry = $geometries[$i];
      $factor = (isset($itemFactor[$i]) ? $itemFactor[$i] : 1);
      $lat += $geometry->getY() * $factor;
      $lon += $geometry->getX() * $factor;
      $totalFactor += $factor;
    }
    $lat = $lat / $totalFactor;
    $lon = $lon / $totalFactor;
    $center = new Point($lon, $lat);
    return $center;
  }

  /*
  // other, simpler distance implementations that didn't really work out.

  function geocluster_distance_simple2($geometry, $otherGeometry) {
    $point = geocluster_forward_mercator($geometry->getX(), $geometry->getY());
    $long_1 = $point['x'];
    $lat_1 = $point['y'];

    $point = geocluster_forward_mercator($otherGeometry->getX(), $otherGeometry->getY());
    $long_2 = $point['x'];
    $lat_2 = $point['x'];

    return sqrt(
      pow($long_1 - $long_2, 2) +
        pow($lat_2 - $lat_2, 2)
    );
  }

  function geocluster_distance_simple($geometry, $otherGeometry) {
    $long_1 = $geometry->getX();
    $lat_1 = $geometry->getY();
    $long_2 = $otherGeometry->getX();
    $lat_2 = $otherGeometry->getY();

    return sqrt(
      pow($long_1 - $long_2, 2) +
      pow($lat_2 - $lat_2, 2)
    );
  }

  */

}
