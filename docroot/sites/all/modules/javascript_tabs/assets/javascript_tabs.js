Drupal.behaviors.javascriptTabsSetup = {
  attach: function(context) {
    // first remove classes & add IDs so CSS doesn't get confused
    jQuery('div#javascript-tabs ul.tabs.primary').attr('id', 'javascript-tabs-primary');
    jQuery('div#javascript-tabs ul.tabs.secondary').attr('id', 'javascript-tabs-secondary');
    jQuery('div#javascript-tabs ul').removeClass('tabs primary secondary');

    // if there are secondary tabs then add them as a sub-menu
    if (jQuery('div#javascript-tabs ul#javascript-tabs-secondary').length) {
      jQuery('div#javascript-tabs ul#javascript-tabs-primary li.active').append(jQuery('div#javascript-tabs ul#javascript-tabs-secondary'));
    }

    // click / rollover code for displaying our menu
    jQuery('div#javascript-tabs div#javascript-tabs-rollover').click(function() {
      if (jQuery('div#javascript-tabs ul#javascript-tabs-primary').css('display') == 'none') {
        jQuery('div#javascript-tabs ul#javascript-tabs-primary').slideDown('fast');
      }
      else {
        jQuery('div#javascript-tabs ul#javascript-tabs-primary').slideUp('slow');
      }
    });
  }
};
