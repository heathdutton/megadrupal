(function ($) {
  function unitChanged () {
    var d = Drupal.settings.adManagerField.descriptions;
    var a = $(this).siblings('.description').children('.ad-manager-field');
    if (typeof d[$(this).val()] === 'undefined') {
      a.hide();
      return;
    }

    // Set the additional description text.
    a.html(d[$(this).val()]).show();
  }

  function appendAdditional () {
    var d = $(this).siblings('.description');
    var a = $('<div class="ad-manager-field"></div>')
      .hide();
    // If the description element is present.
    if (d.length) {
      a.addClass('ad-manager-field-margin');
      a.appendTo(d);
    }
    // Else create it.
    else {
      d = $('<div class="description"></div>');
      d.appendTo($(this).parent());
      a.appendTo(d);
    }
  }

  Drupal.behaviors.adManagerField = {
    attach: function (context, settings) {
      // Bind the unitChanged handler, set up the additional-description div,
      // and populate and additional description for the currently selected
      // item.
      var woo = $('.ad-manager-field', context)
        .once('adManagerField')
        .bind('change', unitChanged)
        .each(appendAdditional)
        .each(unitChanged);
    }
  };
})(jQuery);
