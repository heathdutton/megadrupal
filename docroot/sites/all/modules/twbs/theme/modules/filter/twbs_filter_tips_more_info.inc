<?php

/**
 * Returns HTML for a link to the more extensive filter tips.
 *
 * @ingroup themeable
 */
function twbs_filter_tips_more_info() {
  return '<p>' . l(t('More information about text formats'), 'filter/tips', array('attributes' => array('target' => '_blank'))) . '</p>';
}
