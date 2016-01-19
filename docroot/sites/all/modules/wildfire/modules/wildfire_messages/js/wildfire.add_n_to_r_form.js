/**
 * JQuery code for adding a node to a repeater form
 */
jQuery(document).ready(function() {
    jQuery('#edit-broadcast').change(function() {
      wildfire_broadcast_get_repeaters(jQuery('#edit-broadcast').val());
    });
     
    /* Run once on document ready to ensure the initial state is correct */
    wildfire_broadcast_get_repeaters(jQuery('#edit-broadcast').val());
  }  
);

function wildfire_broadcast_get_repeaters(mid) {
  jQuery.post(
    Drupal.settings['base_path'] + 'admin/wildfire/broadcasts/get_repeaters/' + mid + '/' + jQuery('#edit-nid').val(),
    { mid: mid, nid: 0 },
    function(ret) {
      ret = jQuery.parseJSON(ret);

      /**
       * Disable and hide all repeater checkboxes
       */
      jQuery('div.form-checkboxes div.form-item').each(function() {
        jQuery(this).hide();
      });
      jQuery('div.form-checkboxes input').each(function() {
        jQuery(this).attr('disabled', 'disabled');
        jQuery(this).removeAttr('checked');
      });

      /**
       * Enable and show all relevant repeater checkboxes, and check
       * them if the node exists upon them.
       */
      jQuery.each(ret.repeaters, function(index, value) {
        jQuery('#edit-repeater-' + index).parent().show();
        jQuery('#edit-repeater-' + index).removeAttr('disabled');
        if (value['selected'] == 1)  {
          jQuery('#edit-repeater-' + index).attr('checked', 'checked');
        }
      });

    }
  );
}