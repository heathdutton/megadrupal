<?php

/**
 * @file
 * Contains \MpxViewsFieldAvailability.
 */

/**
 * Views field handler for if a video is unavailable, available, or expired.
 */
class MpxViewsFieldAvailability extends views_handler_field {

  /**
   * {@inheritdoc}
   */
  public function render($values) {
    $available_date = $values->{$this->field_alias};
    $expiration_date = $values->{$this->aliases['expiration_date']};
    switch (media_theplatform_mpx_get_video_availability($available_date, $expiration_date)) {
      case MPX_VIDEO_UNAVAILABLE:
        return t('Unavailable');

      case MPX_VIDEO_AVAILABLE:
        return t('Available');

      case MPX_VIDEO_EXPIRED:
        return t('Expired');
    }
  }

}
