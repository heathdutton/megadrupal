/**
 * @file
 * Date choice (un)hiding on node form
 *
 * This file is only included on the node form, nowhere else, so we can use
 * comparatively simple selectors and be reasonably confident they will not be
 * matching anywhere else.
 *
 * @copyright (c) 2010 Ouest Systemes Informatiques (OSInet)
 *
 * @author Frederic G. MARAND <fgm@osinet.fr>
 *
 * @license General Public License version 2 or later
 *
 * Original code, not derived from the ed_classified module.
 */

/*jslint sloppy: true, white: true, onevar: true, undef: true, nomen: true, eqeqeq: true,
  plusplus: true, bitwise: true, regexp: true, newcap: true, immed: true,
  indent: 2 */
/*global Drupal,jQuery */

(function ($) {

  /**
   * Fade a selection in or out, depending on the parameters
   *
   * @param string selector
   *   A jQuery selector
   * @param boolean visible
   *   Set to true to fade in, to fase to fade out
   * @return void
   */
  function classifiedFade(selector, visible) {
    if (visible) {
      $(selector).fadeIn();
    }
    else {
      $(selector).fadeOut();
    }
  }

  /**
   * Limit data entry to the size defined in classified settings
   *
   * @return void
   */
  function classifiedBodyUpdateRemaining(bodySelector) {
    var max_length,   // maximum number of chars in node body
      oBody,          // jQuery object for the node body
      oCountSpan,     // jQuery object for the char count <span> element
      remaining;      // running count of remaining chars

    max_length = Drupal.settings.classified.max_length;

    oBody = $(bodySelector);
    oCountSpan = $('.classified-max-length>span');
    remaining = max_length - oBody.val().length;

    oCountSpan.text(remaining);

    if (remaining < max_length / 10) {
      oCountSpan.removeClass('error').addClass('warning');
      if (remaining <= 0) {
        oCountSpan.removeClass('warning').addClass('error');
        oBody.val(oBody.val().substr(0, max_length));
        oCountSpan.text(0);
      }
    }
    else {
      oCountSpan.removeClass('warning error');
    }
  }

  /**
   * Ready handler to apply visibility to the date field on classified_node_form.
   *
   * Hide the manual date selector initially unless the current date mode is set
   * to manual.
   *
   * @return void
   */
  function onReadyDateVisibility() {
    if ($('#edit-expire-mode-force').attr('checked') === false) {
      $('.form-item-expire-date').hide();
    }
    $("input:radio[name='expire_mode']").click(function () {
      classifiedFade('.form-item-expire-date',
        $("input:radio[name='expire_mode']:checked").val() === 'force');
    });
  }

  /**
   * Ready handler to apply body length limitation on classified_node_form.
   *
   * Also run it on first opening the form: the body length limit may have been
   * reduced since the node was created, so it may need to be trimmed.
   */
  function onReadyBodyLength(bodySelector) {
    var max_length = Drupal.settings.classified.max_length;
    if (max_length <= 0) {
      $('.classified-max-length').remove();
      return;
    }

    if (!$('.classified-max-length').length) {
      $(bodySelector).parent().append(Drupal.t('<p class="classified-max-length">Remaining characters: <span>@count</span>.</p>', {
        '@count' : max_length
      }));
    }
    classifiedBodyUpdateRemaining(bodySelector);
    $(bodySelector).keyup(function () {
      classifiedBodyUpdateRemaining(bodySelector);
    });
  }

  /**
   * Assign ready handlers
   */
  $(function () {
    onReadyDateVisibility();
    onReadyBodyLength('#edit-body-und-0-value');
  });

})(jQuery);
