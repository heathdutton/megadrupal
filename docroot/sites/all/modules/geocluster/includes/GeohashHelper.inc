<?php

define('GEOHASH_PRECISION', 12);

class GeohashHelper {

  static function getTopRightNeighbors($geohash) {
    // Return only top-right neighbors according to the structure of geohash.
    $neighbors = array();
    $top = GeoHashUtils::calcNeighbors($geohash, 'top');
    $neighbors[0] = GeoHashUtils::calcNeighbors($top, 'left');
    $neighbors[1] = $top;
    $neighbors[2] = GeoHashUtils::calcNeighbors($top, 'right');
    $neighbors[3] = GeoHashUtils::calcNeighbors($geohash, 'right');
    return $neighbors;
  }

  static function getNeighbors($geohash) {
    return GeoHashUtils::getNeighbors($geohash);
  }

  /**
   * Calculate geohash length for clustering by a specified distance in pixels.
   *
   * @static
   * @param $cluster_distance
   * @param $resolution
   * @return int
   */
  static function lengthFromDistance($cluster_distance, $resolution) {
    $cluster_distance_meters = $cluster_distance * $resolution;
    $x = $y = $cluster_distance_meters;
    list($width, $height) = GeoclusterHelper::backwardMercator($x, $y);

    $hashLen = GeohashHelper::lookupHashLenForWidthHeight($width, $height);
    return $hashLen + 1;
  }

  /**
   * Return a geohash length that has width & height >= specified arguments.
   *
   * based on solr2155.lucene.spatial.geohash.GeoHashUtils
   */
  static function lookupHashLenForWidthHeight($width, $height) {
    list ($hashLenToLatHeight, $hashLenToLonWidth) = GeohashHelper::getHashLenConversions();
    //loop through hash length arrays from beginning till we find one.
    for($len = 1; $len <= GEOHASH_PRECISION; $len++) {
      $latHeight = $hashLenToLatHeight[$len];
      $lonWidth = $hashLenToLonWidth[$len];
      if ($latHeight < $height || $lonWidth < $width) {
        // Previous length is big enough to encompass specified width & height.
        return $len-1;
      }
    }
    return GEOHASH_PRECISION;
  }

  /**
   * based on solr2155.lucene.spatial.geohash.GeoHashUtils
   */
  static function lookupDegreesSizeForHashLen($hashLen) {
    list ($hashLenToLatHeight, $hashLenToLonWidth) = GeohashHelper::getHashLenConversions();
    return array(
      $hashLenToLatHeight[$hashLen],
      $hashLenToLonWidth[$hashLen]
    );
  }

  /**
   * based on solr2155.lucene.spatial.geohash.GeoHashUtils
   *
   * See the table at http://en.wikipedia.org/wiki/Geohash
   */
  static function getHashLenConversions() {
    // @todo: static, cache?
    $hashLenToLatHeight = array(90*2);
    $hashLenToLonWidth = array(180*2);
    $even = FALSE;
    for ($i = 1; $i <= GEOHASH_PRECISION; $i++) {
      $hashLenToLatHeight[$i] = $hashLenToLatHeight[$i-1] / ($even ? 8 : 4);
      $hashLenToLonWidth[$i] = $hashLenToLonWidth[$i-1] / ($even ? 4 : 8);
      $even = !$even;
    }
    return array(
      $hashLenToLatHeight,
      $hashLenToLonWidth
    );
  }

}
