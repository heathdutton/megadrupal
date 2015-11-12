<?php
/**
 * @file
 * Template for theme('classified_expires', $node).
 *
 * Variables:
 *
 * - $expires
 *   The date of expiration, in default classified.module format
 * - $expires_raw
 *   The date of expiration, as a UNIX timestamp
 * - $remaining
 *   The time remaining until expiration, in days
 * - $remaining_ratio
 *   The percentile ratio of remaining time to expiration over ad lifetime since
 *   creation.
 * - $remaining_class
 *   A class defining if the ad is expired, or has already spent most of its
 *   alloted lifetime
 *   - 'classified-expires-expired'
 *   - 'classified-expires-soon'
 *   - 'classified-expires-later'
 *
 * @copyright (c) 2010 Ouest Systemes Informatiques (OSInet)
 *
 * @author Frederic G. MARAND <fgm@osinet.fr>
 *
 * @license General Public License version 2 or later
 *
 * Original code, not derived from the ed_classified module.
 */
?><div class="classified-expires <?php echo $remaining_class; ?>"><?php
  echo t('Time remaining: @ratio%<hr />@expires (@remaining)', array(
    '@expires'   => $expires,
    '@remaining' => $remaining,
    '@ratio'     => $remaining_ratio,
  ))?></div>
