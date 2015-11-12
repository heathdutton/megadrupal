function autotagging_js_suggest() {
  var oBody = jQuery('#edit-body-und-0-value');
  var sText = oBody.val();

  jQuery.ajax({
    'type': 'POST',
    'url': '/ajax/autotagging',
    'data': 'text=' + sText,
    'success': function(oData) {
      if (oData[1]) {
        aTerms = oData[1];
        var sOutput = '';
        jQuery('#autotagging_suggest').empty();
        for (var i in aTerms) {
          sOutput = '<span class="autotagging_term autotagging_term_pending"><a href="javascript:void(0);" onclick="autotagging_term_toggle(this);">+</a><span>' + aTerms[i] + '</span></span>';
          jQuery('#autotagging_suggest').append(sOutput);
        }
      }
    },
    'failure': function(sData) {
    }
  });
  return false;
}

function autotagging_term_toggle(oAnchor) {
  if (jQuery(oAnchor).text() == '+') {
    // add the term to terms list
    //
    var sTerms = jQuery("#edit-field-tags-und").val();
    var aTerms = new Array();
    if (jQuery.trim(sTerms) != '') {
      aTerms = sTerms.split(',');
    }

    aTerms[aTerms.length] = jQuery(oAnchor).parent().find('span').text();
    jQuery("#edit-field-tags-und").val(aTerms.join(', '));
    jQuery(oAnchor).text('-');
    jQuery(oAnchor).parent().addClass('autotagging_term_added').removeClass('autotagging_term_pending');
  }
  else {
    var aTerms = jQuery("#edit-field-tags-und").val().split(",");
    var sRemTerm = jQuery(oAnchor).parent().find('span').text();
    var aNewTerms = new Array();
    for (var i in aTerms) {
      var sTerm = jQuery.trim(aTerms[i]);
      if (sTerm != sRemTerm) {
        aNewTerms[aNewTerms.length] = sTerm;
      }
    }
    jQuery("#edit-field-tags-und").val(aNewTerms.join(", "));
    jQuery(oAnchor).text("+");
    jQuery(oAnchor).parent().addClass('autotagging_term_pending').removeClass('autotagging_term_added');
  }
}