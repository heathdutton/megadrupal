function insertAtCursor(myField, myValue) {
  //IE support
  if (document.selection) {
    myField.focus();
    sel = document.selection.createRange();
    sel.text = myValue;
  }

  //MOZILLA/NETSCAPE support
  else if (myField.selectionStart || myField.selectionStart == '0') {
    var startPos = myField.selectionStart;
    var endPos = myField.selectionEnd;
    myField.value = myField.value.substring(0, startPos)
                  + myValue
                  + myField.value.substring(endPos, myField.value.length);
  } else {
    myField.value += myValue;
  }
  return false;
}

(function ($) {

  // calling the function
  // insertAtCursor(document.formName.fieldName, ‘this value’);

  Drupal.contemplate = new Object();

  Drupal.behaviors.contemplate = {
    attach: function(context) {
      $("input[id*=enable]:not(.contemplate-preprocessed)").addClass('contemplate-preprocessed').change(function () {
        var target = $(this).attr('rel').replace(/.*toggletarget\|([-#a-z]*).*/, '$1');
        if ($(this).is(':checked')) {
          $(target).removeAttr('disabled');
          $(target).focus();
        }
        else {
          $(target).attr('disabled', true);
        }
        $(target + '-keys').css('opacity', $(this).is(':checked') ? 1 : .2);
      });
    }
  }

})(jQuery);
