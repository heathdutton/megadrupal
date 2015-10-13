(function ($, Drupal) {

  /**
   * The answer has been converted to upper case becaue the keys entered are
   * displayed as upper case. Hence to avoid case insensitive part, I have made
   * it to upper case.
   */
  var onKeyUp = function (e) {
    switch (e.keyCode) {
      case 9: // tab
      case 17: // ctrl
      case 18: // …
      case 91: // cmd
      case 16: // shift
        return true;
    }

    var id = this.id;
    var present_class = $('#' + id).attr('class').replace('form-text', '').trim();
    var field_answer = Drupal.settings.answer[present_class].toUpperCase();
    var input = $('#' + id).val();
    var reg = new RegExp('^' + input.toUpperCase() + '.*$', 'g');

    if (reg.test(field_answer)) {
      e.preventDefault();
    }
    else {
      $('#' + id).val(input.substr(0, input.length - 1));
    }
  };

  Drupal.behaviors.cloze = {
    attach: function (context) {
      $('.answering-form .cloze-question input', context).keyup(onKeyUp);
    }
  };

})(jQuery, Drupal);
