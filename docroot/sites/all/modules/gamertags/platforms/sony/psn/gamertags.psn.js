(function ($) {

Drupal.behaviors.gamertagsPSNSelectList = {
  attach: function (context, settings) {
    $("input[name=gamertags_psn]", context).each(function() {
      Drupal.actNAOption(this.value);
      Drupal.togglePSNGamertags(this.value, true);
    }).keyup(function () {
      Drupal.togglePSNGamertags(this.value, false);
    });
  }
}

Drupal.togglePSNGamertags = function(text, justHide) {
  // What to do if textfield is empty.
  if (text == '') {
    // If this is the first time this form is displayed we just want to hide the selection box without any effects.
    if (justHide) {
      $('div.form-item-gamertags-psn-region').css('display', 'none');
    }
    else {
      $('div.form-item-gamertags-psn-region').hide('slow', function() {
        Drupal.actNAOption(text);
      });
    }
  }
  // Else, the text contains something so check to see if it's hidden - if so, SHOW it.
  else if ($('div.form-item-gamertags-psn-region').is(':hidden') == true) {
    Drupal.actNAOption(text);
    $('div.form-item-gamertags-psn-region').show('slow');
  }
}

Drupal.actNAOption = function(text) {
  // If the textfield IS empty then prepend N/A (and select it) if it doesn't already exist.
  if (text == '' && $("select[name=gamertags_psn_region]:eq(0)").val() != 'na') {
    $("select[name=gamertags_psn_region]").prepend('<option value="na">N/A</option>');
    $("select[name=gamertags_psn_region] option:eq(0)").attr("selected", "selected");
  }
  else if (text != '') {
    // Else remove it when the textfield does actually contain *something*.
    $("select[name=gamertags_psn_region] option[value='na']").remove();
  }
}

})(jQuery);
