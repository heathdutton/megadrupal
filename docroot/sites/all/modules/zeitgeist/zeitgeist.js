/**
 * @file
 * Separator choice for inline displays on the settings form.
 *
 * This file is only included on the settings form, nowhere else, so we can use
 * comparatively simple selectors and be reasonably confident they will not be
 * matching anywhere else.
 *
 * @copyright (c) 2010-2013 Ouest Systemes Informatiques (OSInet)
 *
 * @license Licensed under the General Public License version 2 or later.
 */

/* global jQuery */

(function($) {
  "use strict";

/**
 * Fade a selection in or out, depending on the parameters
 *
 * $().toggle(visibility) behaves too erratically, hence the separate show/hide
 * calls.
 *
 * @param string selector
 *   A jQuery selector
 * @param boolean visible
 *   Set to true to fade in, to fase to fade out
 *
 * @return void
 */
function zeitgeistFade(selector, visible) {
  if (visible) {
    $(selector).show();
  }
  else {
    $(selector).hide();
  }
}

/**
 * Ready handler to apply visibility to the separator fields on
 * _zeitgeist_admin_settings_form.
 *
 * Hide the separator field initially unless the current display mode is set
 * to manual for the matching block (top/latest).
 *
 * @return void
 */
function zeitgeistOnSeparatorVisibility() {
  zeitgeistFade('.form-item-zeitgeist-top-sep',
    $("input:radio[name='zeitgeist_top_display']:checked").val() === 'implode');

  $("input:radio[name='zeitgeist_top_display']").click(function () {
    zeitgeistFade('.form-item-zeitgeist-top-sep',
      $("input:radio[name='zeitgeist_top_display']:checked").val() === 'implode');
  });

  zeitgeistFade('.form-item-zeitgeist-latest-sep',
    $("input:radio[name='zeitgeist_latest_display']:checked").val() === 'implode');

  $("input:radio[name='zeitgeist_latest_display']").click(function () {
    zeitgeistFade('.form-item-zeitgeist-latest-sep',
      $("input:radio[name='zeitgeist_latest_display']:checked").val() === 'implode');
  });
}

/**
 * Assign ready handlers
 */
$(function () {
  zeitgeistOnSeparatorVisibility();
});

}) (jQuery);
