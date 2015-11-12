/**
 * @file
 *    jQuery code to make the broadcast message send page more user-friendly.
 */
jQuery(document).ready(function() {

  // Show/hide the send now/later date entry when necessary.
  if (jQuery('input[name="send_when"]:checked').val() == 'now') {
    jQuery('#edit-date-wrapper').hide();
  }

  jQuery('input[name="send_when"]').click(function() {
    if (jQuery('input[name="send_when"]:checked').val() == 'now') {
      jQuery('#edit-date-wrapper').slideUp();
    }
    else {
      jQuery('#edit-date-wrapper').slideDown();
    }
  });

  // Show/hide the list or test sections depending on user selections.
  if (jQuery('input[name="recipient_type"]:checked').val() == 'list') {
    jQuery('#test-wrapper').hide();
  }

  jQuery('input[name="recipient_type"]').click(function() {
    if (jQuery('input[name="recipient_type"]:checked').val() == 'list') {
      jQuery('#test-wrapper').slideUp();
      jQuery('#list-wrapper').slideDown();

      // If it's a list, enable all buttons
      jQuery('input[name="send_when"]').each(function() {
        jQuery(this).removeAttr('disabled');
        jQuery(this).parent().slideDown();
      });

    }
    else {

      jQuery('#list-wrapper').slideUp();
      jQuery('#test-wrapper').slideDown();

      // If it's a test, disable the 'send later' button
      jQuery('input[name="send_when"]').each(function() {
        if (jQuery(this).val() !== 'now') {
          jQuery(this).attr('disabled', 'disabled');
          jQuery(this).parent().slideUp();
          jQuery(this).removeAttr('checked');
        }
        else {
          jQuery(this).attr('checked', 'checked');
          jQuery('#edit-date-wrapper').slideUp();
        }
      });

    }
  });

  /**
   * When an item is selected from 'all lists', select the corresponding
   * 'recently used' radio
   */
  jQuery('select[name="all_lists"]').change(function() {
    selectval = jQuery(this).val();

    in_top_lists = jQuery('input[name="recently_used_lists"][value="' + selectval + '"]').length > 0;
    if (in_top_lists) {
      jQuery('input[name="recently_used_lists"]').each(function() {
        if (jQuery(this).val() == selectval) {
          jQuery(this).attr('checked', 'checked');
          jQuery('#all-lists-wrapper').slideUp();
        }
        else {
          jQuery(this).removeAttr('checked');
        }
      });
    }
  });

  /**
   * When an item is selected from 'recently used' radios, select the
   * corresponding 'all lists' entry, if the select box is available.
   */
  jQuery('#all-lists-wrapper').hide();

  jQuery('input[name="recently_used_lists"]').click(function () {
    if (jQuery(this).val() == '-1') {
      jQuery('#all-lists-wrapper').slideDown();
    }
    else {
      jQuery('#all-lists-wrapper').slideUp();
    }
  });

});
