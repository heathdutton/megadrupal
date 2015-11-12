<?php
// $Id: moon.inc,v 1.1 2011/02/18 10:11:53 nhwebworker Exp $

/**
 * @file
 * Calculate moon rise and set times.  Use low resolution algorithm outlined in
 * Montenbruck and Pflgler's Astronomy on the Personal Computer, 1989. In
 * essence, create a function that maps from time (x axis) to the sine of the
 * angle of the moon above the horizon (y axis) at the location coordinates. The
 * function crosses the x-axis at the time of rising and setting.
 */
function almanac_moontimes($year, $month, $day, $gmtoffset, $latitude, $longitude) {
  $hour = 0;
  $zone_ut_adder= $gmtoffset / 3600;
  $zone_adjust = $zone_ut_adder / 24;
  $adj_date = almanac_modjd($year, $month, $day, $hour) - $zone_adjust;
  $sine_lat = sin(deg2rad($latitude));
  $cos_lat = cos(deg2rad($latitude));
  $sine_h0 = sin(deg2rad(8 / 60));

  $xe = 0;
  $ye = 0;
  $z1 = 0;
  $z2 = 0;

  $utrise = 0;
  $utset = 0;
  $rise = 0;
  $sett = 0;
  $hour = 1;
  $zero2 = 0;
  $hour_zero = $hour - 1;

  // calculate sine of altitude at time 0
  $ym = sinalt($adj_date, $hour_zero, $longitude, $cos_lat, $sine_lat) - $sine_h0;
  ($ym > 0) ? $above = 1 : $above = 0;

  /*
   * main execution loop. calculate the sine of the angle of the object over 3 hour period.
   * Fit a simple parabola to the 3 height points, as a second order polynomial can be easily
   * calculated and will fit 3 points perfectly. If parabola has a root during the period,
   * then body has crossed the horizon and has either risen or set. Use the interpolated root value
   * as that time.
   */
  do {
    $next_hour = $hour + 1;
    $y0 = sinalt($adj_date, $hour, $longitude, $cos_lat, $sine_lat) - $sine_h0;
    $yp = sinalt($adj_date, $next_hour, $longitude, $cos_lat, $sine_lat) - $sine_h0;
    // y0 is difference between sine of elevation at current hour and at 0 hour, ym one hour behind, yp one hour ahead.
    $quadfit = quad($ym, $y0, $yp);
    $xe = $quadfit['xe'];
    $ye = $quadfit['ye'];
    $z1 = $quadfit['z1'];
    $z2 = $quadfit['z2'];
    $nz = $quadfit['nz'];
    switch ($nz) {
      case 0: // nothing  - no root during period, so go to next perod
        break;

      case 1:                      // simple rise / set event
      if ($ym < 0) {             // rise event
        $utrise = $hour + $z1;
        $rise = 1;
      }
      else {                     // set event
        $utset = $hour + $z1;
        $sett = 1;
      }
      break;

      case 2 :                     // rises and sets within interval
        if ($ye < 0) {             // minimum - so set then rise
          $utrise = $hour + $z2;
          $utset = $hour + $z1;
        }
        else {                     // maximum - so rise then set
          $utrise = $hour + $z1;
          $utset = $hour + $z2;}
          $rise = 1;
          $sett = 1;
          $zero2 = 1;
        }
        $ym = $yp;     // reuse the ordinate in the next interval
        $hour = $hour + 2;
  }
  while (($hour < 25) && ($rise * $sett <> 1));
  /* end main loop */

  $utrise = hours_minutes($utrise);
  $moonrise = gmmktime($utrise['hours'], $utrise['minutes'], 0, $month, $day, $year);
  $utset = hours_minutes($utset);
  $moonset = gmmktime($utset['hours'], $utset['minutes'], 0, $month, $day, $year);
  $moontimes = array('moonrise' => $moonrise + $gmtoffset, 'moonset' => $moonset + $gmtoffset);
  return $moontimes;
}

/**
 * returns sine of the altitude of the moon given time and geographic location
 */
function sinalt($mjd0, $hour, $glong, $cphi, $sphi) {
  $instant = $mjd0 + ($hour / 24) ;
  $t = ($instant - 51544.5) / 36525; //mjd of instant in julian centuries J2000.0
  $moon_pos = almanac_mooncalc($t);
  $tau = 15 * (lmst($instant, $glong) - $moon_pos['ra']);   //hour angle of object
  $sinalt = $sphi * sin(deg2rad($moon_pos['dec'])) + $cphi * cos(deg2rad($moon_pos['dec'])) * cos(deg2rad($tau));
  return $sinalt;
}

/**
 * Returns ra and dec of Moon at given time. Algorithm detailed in Montenbruck and Pflegler (1989)
 */
function almanac_mooncalc($t) {
  $two_pi = 2 * M_PI;
  $arc = 206264.8062;
  $coseps = 0.91748;
  $sineps = 0.39778;
  $l0 = fpart(0.606433 + 1336.855225 * $t);       //mean long Moon in revs
  $l = $two_pi * fpart(0.374897 + 1325.55241 * $t);   //mean anomaly of Moon
  $ls = $two_pi * fpart(0.993133 + 99.997361 * $t);   //mean anomaly of Sun
  $d = $two_pi * fpart(0.827361 + 1236.853086 * $t);  //diff longitude sun and moon
  $f = $two_pi * fpart(0.259086 + 1342.227825 * $t);  //mean arg latitude
  /* longitude and latitude perturbation terms. */
  $dl = 22640 * sin($l) - 4586 * sin($l - 2 * $d) + 2370 * sin(2 * $d) + 769 * sin(2 * $l)
        - 668 * sin($ls) - 412 * sin(2 * $f) - 212 * sin(2 * $l - 2 * $d) - 206 * sin($l + $ls - 2 * $d)
        + 192 * sin($l + 2 * $d) - 165 * sin($ls - 2 * $d) - 125 * sin($d) - 110 * sin($l + $ls)
        + 148 * sin($l - $ls) - 55 * sin(2 * $f - 2 * $d);
  $s = $f + ($dl + 412 * sin(2 * $f) + 541 * sin($ls)) / $arc;
  $h = $f - 2 * $d;
  $n = -526 * sin($h) + 44 * sin($l + $h) - 31 * sin($h - $l) - 23 * sin($ls + $h)
       + 11 * sin($h - $ls) - 25 * sin($f - 2 * $l) + 21 * sin($f - $l);
  $lmoon = $two_pi * fpart($l0 + $dl / 1296000);   //latitude in rads
  $bmoon = (18520 * sin($s) + $n) / $arc;          //longitude in rads
  /* convert to equatorial coords */
  $cb = cos($bmoon);
  $x = $cb * cos($lmoon);
  $v = $cb * sin($lmoon);
  $w = sin($bmoon);
  $y = $coseps * $v - $sineps * $w;
  $z = $sineps * $v + $coseps * $w;
  $rho = sqrt(1 - $z * $z);
  $dec = (360 / $two_pi) * atan($z/$rho);
  $ra = (48 / $two_pi) * atan($y / ($x + $rho));
  if ( $ra < 0 ) {
    $ra = $ra + 24;
  }
  $moon = array('ra' => $ra, 'dec' => $dec);
  return $moon;
}

/* ---------------- other functions ---------------- */

// returns fractional part of a number
function fpart($xvalue) {
  $xfraction = abs($xvalue) - floor(abs($xvalue));
  return $xfraction;
}

/*
 * return modified julian date - number of days since 00:00h 17 Nov 1858
 * which is base date of modified julian calendar system
 */
function almanac_modjd($year, $month, $day, $hour) {
  $day1 = gregoriantojd($month, $day, $year);
  $day0 = gregoriantojd(11, 17, 1858);
  $modjd = $day1 - $day0 + $hour/24;
  return $modjd;
}

/**
 * returns the local siderial time for the modified julian date and longitude
 */
function lmst($mjd, $glong) {
  $mjd0 = intval($mjd);
  $ut = ($mjd - $mjd0) * 24;
  $t = ($mjd0 - 51544.5) / 36525;
  $gmst = 6.697374558 + 1.0027379093 * $ut;
  $gmst = $gmst + (8640184.812866 + (0.093104 - 0.0000062 * $t) * $t) * $t / 3600;
  $lmst = 24 * fpart(($gmst - $glong / 15) / 24);
  return $lmst;
}

/**
 * interpolate a parabola through three points and returns values of
 * coordinates of extreme value (xe, ye) if roots then return roots (z1, z2)
 * plus number of found roots ($nz)
 */
function quad($yminus, $y0, $yplus) {
  $nz = (int) 0;
  // determine coefficients a, b, c of y=a * x^2 + b * x +c
  // remember quadratic equation from school?
  $a = 0.5 * ($yminus + $yplus) - $y0;
  $b = 0.5 * ($yplus - $yminus);
  $c = $y0;
  // calculate extreme values
  $xe = (-1* $b) / (2 * $a);
  $ye = ($a * $xe + $b) * $xe + $c;
  // compute discriminant
  $dis = $b * $b - 4 * $a * $c;
  // if roots exist, compute
  if ( $dis >= 0 ) {
    $dx = 0.5 * sqrt($dis) / abs($a);
    $z1 = $xe - $dx; // root1
    $z2 = $xe + $dx; // root2
    (abs($z1) <= 1) ? ++$nz : $nz ;
    (abs($z2) <= 1) ? ++$nz : $nz ;
    ($z1 < -1) ? $z1 = $z2 : $z1 ;
  }
  $quad = array('xe' => $xe, 'ye' => $ye, 'z1' => $z1, 'z2' => $z2, 'nz' => $nz );
  return $quad;
}
