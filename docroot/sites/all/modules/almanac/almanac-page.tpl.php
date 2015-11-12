<?php
// $Id: almanac-page.tpl.php,v 1.1 2011/02/18 10:07:07 nhwebworker Exp $
/**
 * @file
 * Default theme implementation to display an almanac page.
 */
  extract($data);
  $settings = almanac_settings();
  $short_date = format_date($datetime->format('U'), $settings['datetime']['page'], '', $location['timezone']);
?>
<div id="almanac_page">
  <div class="location"><?php print t('!location, !date', array('!location' => check_plain($location['name']), '!date' => $short_date)) ?></div>
  <table>
    <tr>
      <th><?php print t('Sun'); ?></th>
      <th><?php print t('Rise') ?></th>
      <th><?php print t('Set') ?></th>
    </tr>
    <tr>
      <td><?php print t('Actual'); ?></td>
      <td><?php print $sunrise_formatted ?></td>
      <td><?php print $sunset_formatted ?></td>
    </tr>
    <tr>
      <td><?php print t('Civil Twilight') ?></td>
      <td><?php print $civil_twilight_begin_formatted ?></td>
      <td><?php print $civil_twilight_end_formatted ?></td>
    </tr>
    <tr>
      <td><?php print t('Nautical Twilight') ?></td>
      <td><?php print $nautical_twilight_begin_formatted ?></td>
      <td><?php print $nautical_twilight_end_formatted ?></td>
    </tr>
    <tr>
      <td><?php print t('Astronomical Twilight') ?></td>
      <td><?php print $astronomical_twilight_begin_formatted ?></td>
      <td><?php print $astronomical_twilight_end_formatted ?></td>
    </tr>
    <tr>
      <td colspan="3"><?php print t('Zenith: !zenith', array('!zenith' => $transit_formatted)) ?></td>
    </tr>
    <tr>
      <td colspan="3"><?php print t('Day length: !day_length',
          array('!day_length' => theme('almanac_time_period', array('seconds' => $day_length, 'qranularity' => $settings['datetime']['period_short'])))) ?></td>
    </tr>
    <tr>
      <td colspan="3"><?php print t('Visible Light: !visible_light',
          array('!visible_light' => theme('almanac_time_period', array('seconds' => $visible_light, 'qranularity' => $settings['datetime']['period_short'])))) ?></td>
    </tr>
    <tr>
      <td colspan="3"><?php
        print theme('almanac_day_length_diff_yesterday', array('diff' => $additional['yesterday']['day_length'] - $day_length));
      ?></td>
    </tr>
    <tr>
      <td colspan="3"><?php
        print theme('almanac_day_length_diff_tomorrow', array('diff' => $additional['tomorrow']['day_length'] - $day_length));
      ?></td>
    </tr>
  </table>
  <table>
    <tr>
      <th><?php print t('Moon'); ?></th>
      <th><?php print t('Rise') ?></th>
      <th><?php print t('Set') ?></th>
    </tr>
    <tr>
      <td>&nbsp;</td>
      <td><?php print $moonrise_formatted ?></td>
      <td><?php print $moonset_formatted ?></td>
    </tr>
    <tr>
      <td><?php print theme('almanac_moon_phase_sprite', array('phase' => $moonphase['phase'], 'latitude' => $location['lat'], 'attributes' => array('title' => $moonphase['name']))) ?></td>
      <td colspan="2"><?php print $moonphase['name'] . '<br/>' . t('%illumination of the Moon is Illuminated.', array('%illumination' => $moonphase['illumination'])) ?></td>
    </tr>
    <?php
      $names = array(
        'next_full' => t('Full Moon'),
        'next_new' => t('New Moon'),
        'next_first' => t('First Quarter Moon'),
        'next_last' => t('Third Quarter Moon'),
      );
      $phases = array(
        'next_full' => 0,
        'next_new' => 0.5,
        'next_first' => 0.75,
        'next_last' => 0.25,
      );
      foreach ($moonphase['dates'] as $k => $v) {
        $attrs = array('title' => $names[$k], 'description' => $v);
        print '<tr><td>'
          . theme('almanac_moon_phase_sprite', array('phase' => $phases[$k], 'latitude' => $location['lat'], 'attributes' => $attrs))
          . '</td><td colspan="2">'
          . t('%days days until the next !moon.', array('%days' => $moonphase[$k], '!moon' => drupal_strtolower($names[$k])))
          . '</td></tr>';
      }
    ?>
  </table>
</div>
