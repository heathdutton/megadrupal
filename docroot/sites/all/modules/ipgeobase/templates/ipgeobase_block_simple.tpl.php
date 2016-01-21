<?php
/**
 * @file
 * Simple block template for outputting info from ipgeobase.ru service.
 *
 * Variables:
 * - not_found - True if geolocation was not found
 * - city - City name
 * - ip - IP address
 * - latitude - Latitude
 * - longitude - Longitude
 * - region - Region name
 * - district - District name
 * - desc - Description of the IP provider
 */
?>
<?php if ($not_found): ?>
  <p><?php echo t('Unable to determine geolocation for your ip.'); ?></p>
<?php else: ?>
  <?php echo t('City') ?>: <?php echo $city; ?><br />
  <?php echo t('Lat') ?>: <?php echo $latitude; ?><br />
  <?php echo t('Lng') ?>: <?php echo $longitude; ?><br />
  <?php echo t('Region') ?>: <?php echo $region; ?><br />
  <?php echo t('District') ?>: <?php echo $district; ?><br />
<?php endif; ?>
<?php echo t('IP') ?>: <?php echo $ip; ?>