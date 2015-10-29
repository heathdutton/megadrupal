(function ($) {

Drupal.behaviors.gamertagsXfireToggle = {
  attach: function (context, settings) {
  $("input[name=gamertags_xfire]", context).each(function() {
      Drupal.toggleXfireGamertags(this.value, true);
    }).keyup(function () {
      Drupal.toggleXfireGamertags(this.value, false);
    });
  }
}

Drupal.toggleXfireGamertags = function(text, justHide) {
  // What to do if textfield is empty.
  if (text == '') {
    // If this is the first time this form is displayed we just want to hide the selection box without any effects.
    if (justHide) {
      $('div#xfire-extras-wrapper').css('display', 'none');
    }
    else {
      $('div#xfire-extras-wrapper').hide('slow');
    }
  }
  // Else, the text contains something so check to see if it's hidden - if so, SHOW it.
  else if ($('div#xfire-extras-wrapper').is(':hidden') == true) {
    $('div#xfire-extras-wrapper').show('slow');
  }
}

})(jQuery);
